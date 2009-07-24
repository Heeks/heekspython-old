#######################################################3
to use my handy dandy polar_array.py wxdialogue:

import sys
sys.path.insert(0,'/home/dan/HeeksCAD/heekspython/data')
import polar_array
frame_1 = polar_array.MyFrame(None, -1, "")
frame_1.Show()



#################################################
to make it easier to call the frame- make this function:

def array():
    frame_1 = polar_array.MyFrame(None, -1, "")
    frame_1.Show()

then just issue this command in the console:
array()

