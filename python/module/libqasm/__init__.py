# Author Imran Ashraf, Jeroen van Straten

import os
from sys import version_info


# Before we can import the dynamic modules, we have to set the linker search path appropriately.
ld_lib_path = os.environ.get('LD_LIBRARY_PATH', '')
if ld_lib_path:
    ld_lib_path += ':'
os.environ['LD_LIBRARY_PATH'] = ld_lib_path + os.path.dirname(__file__)
del ld_lib_path, os

# We only support Python 3
if version_info[0] != 3:
    raise EnvironmentError(
        "sys.version_info refers does not refer to a version of Python 3. "
        "This is not permitted. "
        "sys.version_info = {}".format(version_info))
del version_info

# Import the SWIG-generated module.
from .libqasm import *

# __all__ = [ init, schedule, compile ]
