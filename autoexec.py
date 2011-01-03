import platform
import sys
if platform.system() == "Windows":
	sys.path.insert(0,'C:\\Users\\Dan\\HeeksCNC')
else:
	sys.path.insert(0,'/usr/local/share/heekspython/heeksscripts')

import HeeksCNC
HeeksCNC.start()
