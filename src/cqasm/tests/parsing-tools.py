
import sys
import os

if len(sys.argv) != 2:
    print('invalid number of args')
    print()
    print('NOTE: this tool is unsupported and not at all portable.')
    print('If you want to use it, you\'ll probably have to edit its sources.')
    sys.exit(1)

from plumbum import local, FG

def diff():
    is_test_dir = False
    for t in ('ast', 'semantic'):
        if os.path.isfile(t + '.actual.txt') and os.path.isfile(t + '.golden.txt'):
            with open(t + '.actual.txt', 'r') as a:
                a = a.read()
            with open(t + '.golden.txt', 'r') as g:
                g = g.read()
            if a == g:
                print(t, 'is as expected')
            else:
                print(t, 'is NOT as expected, opening meld')
                local['meld'](t + '.actual.txt', t + '.golden.txt')
            is_test_dir = True
    if not is_test_dir:
        for sub in os.listdir():
            if os.path.isdir(sub):
                print('Entering %s...' % sub)
                with local.cwd(sub):
                    diff()
                print('Leaving %s...' % sub)

cmd = sys.argv[1]

if cmd == 'run':
    with local.cwd(os.path.dirname(__file__)):
        local['make']['--no-print-directory']['-C']['../cmake-build-debug']['test-parsing'] & FG
        output = local['../cmake-build-debug/tests/test-parsing'](retcode=None)
        print(output.split('[==========]')[2])
    cmd = 'diff'

if cmd == 'diff':
    diff()

elif cmd == 'gild':
    if os.path.isfile('ast.actual.txt'):
        print('copying ast result')
        local['cp']('-f', 'ast.actual.txt', 'ast.golden.txt')
        with open('ast.actual.txt', 'r') as f:
            ast_errors = next(f).startswith('ERROR')
        if ast_errors:
            print('parse failed, deleting semantic result')
            local['rm']('-f', 'semantic.golden.txt', 'semantic.actual.txt')
        elif os.path.isfile('semantic.actual.txt'):
            print('copying semantic result')
            local['cp']('-f', 'semantic.actual.txt', 'semantic.golden.txt')

else:
    print('unknown command')
