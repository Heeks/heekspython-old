import HeeksPython as cad
test_menu = cad.addmenu('test menu')
cad.add_menu_item(test_menu, 'item 1', 'import HeeksPython as cad; cad.addtext("item 1")', 'test')
cad.add_menu_item(test_menu, 'item 2', 'import platform\nif platform.system() == "Windows":\n execfile("test2.py")\nelse:\n execfile("linuxtest.py")', 'test')
cad.add_menu_item(test_menu, 'item 2', 'import HeeksPython as cad; cad.addtext("item 1")', 'test')

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

