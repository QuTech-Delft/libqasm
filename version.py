import os
import re


def get_version(verbose=False):
    """Extract version information from source code"""

    root_dir = os.getcwd()  # root of the repository
    inc_dir = os.path.join(root_dir, "include", "libqasm")  # C++ include directory
    matcher = re.compile('static const char\* version\{ "(.*)" \}')
    version = None
    with open(os.path.join(inc_dir, "versioning.hpp"), "r") as f:
        for ln in f:
            m = matcher.match(ln)
            if m:
                version = m.group(1)
                break

    if verbose:
        print("get_version: %s" % version)

    return version
