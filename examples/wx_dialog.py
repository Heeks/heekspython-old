import wx
mw = wx.GetTopLevelWindows()[0]
mw.SetStatusText("hello!")
menu = mw.GetMenuBar()
mymenu = wx.wxMenu()
mymenu.Append(wx.wxID_ABOUT,"My &Menu","A sample menu item")
menu.Append(mymenu,"Test")
dia = wx.Dialog(None,-1)
pan = wx.Panel(dia,-1)
box = wx.BoxSizer(wx.VERTICAL)
box.Add(wx.StaticText(pan,-1,"Hello World!"))
box.Add(wx.Button(pan,-1,"Press me!"))
pan.SetSizer(box)
dia.Show()

