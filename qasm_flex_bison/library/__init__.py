# Author Imran Ashraf

# The import syntax changes slightly between python 2 and 3, so we
# need to detect which version is being used:
from sys import version_info
if version_info[0] == 3:
    PY3 = True
# elif version_info[0] == 2:
#     PY3 = False
else:
    raise EnvironmentError("sys.version_info refers to a version of "
        "Python is not 3. This is not permitted. "
        "sys.version_info = {}".format(version_info))

from .libQasm import libQasm
