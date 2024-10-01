# A basic mkdocstrings handler for {fmt}.
# Copyright (c) 2012 - present, Victor Zverovich
# https://github.com/fmtlib/fmt/blob/master/LICENSE

import os
import xml.etree.ElementTree as ElementTree

from pathlib import Path
from subprocess import CalledProcessError, PIPE, Popen, STDOUT
from typing import Any, List, Mapping, Optional

from mkdocstrings.handlers.base import BaseHandler


class Definition:
    """A definition extracted by Doxygen."""

    def __init__(
            self,
            name: str,
            kind: Optional[str] = None,
            node: Optional[ElementTree.Element] = None,
            is_member: bool = False
    ):
        self.name = name
        self.kind = kind if kind is not None else node.get('kind')
        self.id = name if not is_member else None
        self.desc = None
        self.is_static = False
        self.members = None
        self.params = None
        self.type = None


# A map from Doxygen to HTML tags.
tag_map = {
    'bold': 'b',
    'computeroutput': 'code',
    'emphasis': 'em',
    'itemizedlist': 'ul',
    'listitem': 'li',
    'para': 'p',
    'programlisting': 'pre',
    'verbatim': 'pre',
}

# A map from Doxygen tags to text.
tag_text_map = {
    'codeline': '',
    'highlight': '',
    'sp': ' ',
}


def escape_html(s: str) -> str:
    return s.replace("<", "&lt;")


def doxyxml2html(nodes: List[ElementTree.Element]):
    out = ''
    for n in nodes:
        tag = tag_map.get(n.tag)
        if not tag:
            out += tag_text_map[n.tag]
        out += '<' + tag + '>' if tag else ''
        out += '<code class="language-cpp">' if tag == 'pre' else ''
        if n.text:
            out += escape_html(n.text)
        out += doxyxml2html(list(n))
        out += '</code>' if tag == 'pre' else ''
        out += '</' + tag + '>' if tag else ''
        if n.tail:
            out += n.tail
    return out


def get_description(node: ElementTree.Element) -> List[ElementTree.Element]:
    return node.findall('briefdescription/para') + \
        node.findall('detaileddescription/para')


def normalize_type(t: str) -> str:
    normalized_type = t.replace('< ', '<').replace(' >', '>')
    return normalized_type.replace(' &', '&').replace(' *', '*')


def convert_type(t: ElementTree.Element) -> Optional[str]:
    if t is None:
        return None
    result = t.text if t.text else ''
    for ref in t:
        result += ref.text
        if ref.tail:
            result += ref.tail
    result += t.tail.strip()
    result = normalize_type(result)
    return normalize_type(result)


def convert_params(func: ElementTree.Element) -> List[Definition]:
    params = []
    for p in func.findall('param'):
        d = Definition(p.find('declname').text, 'param')
        d.type = convert_type(p.find('type'))
        params.append(d)
    return params


def render_param(param: Definition) -> str:
    return param.type + (f'&nbsp;{param.name}' if len(param.name) > 0 else '')


def render_decl(d: Definition) -> str:
    text = ''
    if d.id is not None:
        text += f'<a id="{d.id}">\n'
    text += '<pre><code class="language-cpp decl">'

    text += '<div>'
    end = ';'
    if d.is_static:
        text += 'static '
    if d.kind == 'function' or d.kind == 'variable':
        text += escape_html(d.type) + ' ' if len(d.type) > 0 else ''
    else:
        text += d.kind + ' '
    text += d.name

    if d.params is not None:
        params = ', '.join([
            (p.type + ' ' if p.type else '') + p.name for p in d.params])
        text += '(' + escape_html(params) + ')'

    text += end
    text += '</div>'
    text += '</code></pre>\n'
    if d.id is not None:
        text += f'</a>\n'
    return text


class CppHandler(BaseHandler):
    def __init__(self, **kwargs: Any) -> None:
        super().__init__(handler='cpp', **kwargs)

        headers = [
            'cqasm_python.hpp'
        ]

        # Run doxygen.
        cmd = ['doxygen', '-']
        scripts_dir = Path(__file__).parents[3]
        top_dir = os.path.dirname(scripts_dir)
        include_dir = os.path.join(top_dir, 'include', 'libqasm', 'v3x')
        self._ns2doxyxml = {}
        build_dir = os.path.join(top_dir, 'build', 'Release', 'doc', 'cpp')
        os.makedirs(build_dir, exist_ok=True)
        self._doxyxml_dir = os.path.join(build_dir, 'doxyxml')
        p = Popen(cmd, stdin=PIPE, stdout=PIPE, stderr=STDOUT)
        _, _ = p.communicate(input=r'''
            PROJECT_NAME      = libqasm
            GENERATE_XML      = YES
            GENERATE_LATEX    = NO
            GENERATE_HTML     = NO
            INPUT             = {0}
            XML_OUTPUT        = {1}
            QUIET             = YES
            AUTOLINK_SUPPORT  = NO
            MACRO_EXPANSION   = YES
            PREDEFINED        = _WIN32=1 \
                                __linux__=1 \
            '''
            .format(' '.join([os.path.join(include_dir, h) for h in headers]), self._doxyxml_dir)
            .encode('utf-8')
        )
        if p.returncode != 0:
            raise CalledProcessError(p.returncode, cmd)

        # Merge all file-level XMLs into one to simplify search.
        self._file_doxyxml = None
        for h in headers:
            filename = h.replace(".hpp", "_8hpp.xml")
            with open(os.path.join(self._doxyxml_dir, filename)) as f:
                doxyxml = ElementTree.parse(f)
                if self._file_doxyxml is None:
                    self._file_doxyxml = doxyxml
                    continue
                root = self._file_doxyxml.getroot()
                for node in doxyxml.getroot():
                    root.append(node)

    def collect_compound(self) -> Definition:
        """Collect a compound definition such as a struct."""
        path = os.path.join(self._doxyxml_dir, 'classV3xAnalyzer.xml')
        with open(path) as f:
            xml = ElementTree.parse(f)
            node = xml.find('compounddef')
            d = Definition('V3xAnalyzer', node=node)
            d.desc = get_description(node)
            d.members = []
            for m in \
                    node.findall('sectiondef[@kind="public-attrib"]/memberdef') + \
                    node.findall('sectiondef[@kind="public-static-func"]/memberdef') + \
                    node.findall('sectiondef[@kind="public-func"]/memberdef'):
                name = m.find('name').text
                # Doxygen incorrectly classifies members of private unnamed unions as
                # public members of the containing class.
                if name.endswith('_'):
                    continue
                desc = get_description(m)
                is_static = m.get('static') == 'yes'
                if len(desc) == 0:
                    continue
                kind = m.get('kind')
                member = Definition(name if name else '', kind=kind, is_member=True)
                t = m.find('type')
                member.type = convert_type(t)
                if kind == 'function':
                    member.params = convert_params(m)
                member.desc = desc
                member.is_static = is_static
                d.members.append(member)
            return d

    def collect(self, _identifier: str, _config: Mapping[str, Any]) -> Definition:
        return self.collect_compound()

    def render(self, d: Definition, config: dict) -> str:
        if d.id is not None:
            self.do_heading('', 0, id=d.id)
        text = '<div class="docblock">\n'
        text += render_decl(d)
        text += '<div class="docblock-desc">\n'
        text += doxyxml2html(d.desc)
        if d.members is not None:
            for m in d.members:
                text += self.render(m, config)
        text += '</div>\n'
        text += '</div>\n'
        return text


def get_handler(
    theme: str,
    custom_templates: Optional[str] = None,
    **_config: Any
) -> CppHandler:
    """Return an instance of `CppHandler`.

    Arguments:
        theme: The theme to use when rendering contents.
        custom_templates: Directory containing custom templates.
        **_config: Configuration passed to the handler.
    """
    return CppHandler(theme=theme, custom_templates=custom_templates)
