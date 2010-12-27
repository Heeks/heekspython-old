import sys
#comment out the next lineif you are using windows
sys.path.insert(0,'/usr/local/share/heekspython/heeksscripts')
import HeeksPython as cad

test_menu = cad.addmenu('test menu')
cad.add_menu_item(test_menu, 'item 1', 'import HeeksPython as cad; cad.addtext("1-")', 'test')
cad.add_menu_item(test_menu, 'Bolt Circle','import polar_array;frame_1 = polar_array.MyFrame(None, -1, "");frame_1.Show()', 'test')
cad.add_menu_item(test_menu, 'item 3', 'import HeeksPython as cad; cad.addtext("  -2")', 'test')

#uncomment the next section if you are using windows
'''
import wx

class CAMWindow(wx.ScrolledWindow):
    def __init__(self, parent):
        wx.ScrolledWindow.__init__(self, parent, name = 'CAM')
        tree = wx.TreeCtrl(self)
        tree.SetSize(wx.Size(200, 300))
        program = tree.AddRoot("Program")
        
        # Use some sizers to see layout options
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(tree, 1, wx.EXPAND)
        
        #Layout sizers
        self.SetSizer(self.sizer)
        self.SetAutoLayout(1)
        self.sizer.Fit(self)
        self.Show()

hwnd = cad.get_frame_hwnd()
frame = wx.Window_FromHWND(None, hwnd)
window = CAMWindow(frame)
cad.add_window(window.GetHandle())
'''


from PyQt4 import QtGui

app = QtGui.QApplication([])
widget = QtGui.QWidget()
widget.setWindowTitle('CAM2')
tree = QtGui.QTreeWidget(widget)
tree_item = QtGui.QTreeWidgetItem(tree)
tree_item.setText(0, "Program")
tree.addTopLevelItem(tree_item)
cad.add_window(widget.winId())
widget.show()

