import os
from setuptools import setup
from shutil import copyfile
import subprocess
from sys import platform

rootDir = os.path.dirname(os.path.realpath(__file__))
buildDir = os.path.join(rootDir, "cbuild")
clibDir = buildDir
libQasmDir = os.path.join(rootDir, "libQasm")

if not os.path.exists(libQasmDir):
    os.makedirs(libQasmDir)

if not os.path.exists(buildDir):
    os.makedirs(buildDir)
os.chdir(buildDir)

if platform == "linux" or platform == "linux2":
    print('Detected Linux OS, installing libQasm...')
    cmd = 'cmake ../library/'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    cmd = 'make all'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    cmd = 'make test'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    clibname = "_libQasm.so"
else:
    print('Detected Windows, installing libQasm...')
    cmd = 'cmake -G "MinGW Makefiles" ../library/'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    cmd = 'mingw32-make'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    cmd = 'mingw32-make test'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    clibname = '_libQasm.pyd'
    

genclib = os.path.join(rootDir, "cbuild", clibname)
print('genclib {}'.format(genclib) )
clib = os.path.join(rootDir, "libQasm", clibname)
print('clib {}'.format(clib) )
copyfile(genclib, clib)

'''
Copy the __init__.py file into the libQasm directory
'''
initFile = os.path.join(rootDir, "library", "__init__.py")
initFile_target = os.path.join(rootDir, "libQasm", "__init__.py")
copyfile(initFile, initFile_target)

'''
Copy the generated libQasm python file to the libQasm directory 
'''
copyfile(os.path.join(clibDir, "libQasm.py"),
         os.path.join(rootDir, "libQasm", "libQasm.py"))

copyfile(os.path.join(clibDir, "libQasm.py"),
         os.path.join(rootDir, "libQasm", "libQasm.py"))

liblexgram = ''
if platform == 'win32':
    print('copying', os.path.join(clibDir, 'liblexgram.dll'), 'to', os.path.join(rootDir, 'libQasm', 'liblexgram.dll'))
    copyfile(os.path.join(clibDir, 'liblexgram.dll'),
             os.path.join(rootDir, 'libQasm', 'liblexgram.dll'))
    liblexgram = os.path.join(rootDir, 'libQasm', 'liblexgram.dll')
else:
    print('Not on Windows, skipping liblexgram.dll')

print('liblexgram', liblexgram)

os.chdir(rootDir)

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

setup(name='libQasm',
      version='0.0.1',
      description='libQasm Python Package',
      # long_description=read('README.md'),
      author='Kelvin Loh',
      author_email='kel85uk@gmail.com',
      url="www.github.com",
      packages=['libQasm'],
      include_package_data=True,
      package_data={'libQasm': [clib, liblexgram]},
      zip_safe=False)
