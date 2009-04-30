// HeeksCNC.cpp
/*
 * Copyright (c) 2009, Dan Heeks
 * This program is released under the BSD license. See the file COPYING for
 * details.
 */

#include "stdafx.h"

#include <wx/stdpaths.h>
#include <wx/dynlib.h>
#include <wx/aui/aui.h>
#include "../../interface/PropertyString.h"
#include "../../interface/Observer.h"
#include "../../interface/ToolImage.h"
#include "ConsoleCanvas.h"
#include "PythonConfig.h"

CHeeksCADInterface* heeksCAD = NULL;

CHeeksPythonApp *theApp;

CHeeksPythonApp::CHeeksPythonApp(){
	m_console = NULL;
}

CHeeksPythonApp::~CHeeksPythonApp(){
}

void CHeeksPythonApp::OnInitDLL()
{
}

void CHeeksPythonApp::OnDestroyDLL()
{
#if !defined WXUSINGDLL
	wxUninitialize();
#endif

	heeksCAD = NULL;
}


void OnConsole( wxCommandEvent& event )
{
	wxAuiManager* aui_manager = heeksCAD->GetAuiManager();
	wxAuiPaneInfo& pane_info = aui_manager->GetPane(theApp->m_console);
	if(pane_info.IsOk()){
		pane_info.Show(event.IsChecked());
		aui_manager->Update();
	}
}

void OnUpdateConsole( wxUpdateUIEvent& event )
{
	wxAuiManager* aui_manager = heeksCAD->GetAuiManager();
	event.Check(aui_manager->GetPane(theApp->m_console).IsShown());
}


void CHeeksPythonApp::OnStartUp(CHeeksCADInterface* h, const wxString& dll_path)
{
	m_dll_path = dll_path;
	heeksCAD = h;
#if !defined WXUSINGDLL
	wxInitialize();
#endif

	// add menus and toolbars
	wxFrame* frame = heeksCAD->GetMainFrame();
	wxAuiManager* aui_manager = heeksCAD->GetAuiManager();


	// add the console canvas
    m_console = new CConsoleCanvas(frame);
	m_console->InitP();
	aui_manager->AddPane(m_console, wxAuiPaneInfo().Name(_T("Console")).Caption(_T("Console")).Bottom().BestSize(wxSize(600, 200)));


	bool console_visible;
	PythonConfig config;

	config.Read(_T("ConsoleVisible"), &console_visible);

	aui_manager->GetPane(m_console).Show(console_visible);

	// add tick boxes for them all on the view menu
	wxMenu* view_menu = heeksCAD->GetWindowMenu();
	heeksCAD->AddMenuCheckItem(view_menu, _T("Console"), OnConsole, OnUpdateConsole);
	heeksCAD->RegisterHideableWindow(m_console);

	// add object reading functions
//?	heeksCAD->RegisterReadXMLfunction("Program", CProgram::ReadFromXMLElement);

	heeksCAD->SetDefaultLayout(wxString(_T("layout2|name=ToolBar;caption=General Tools;state=2108156;dir=1;layer=10;row=0;pos=0;prop=100000;bestw=279;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=GeomBar;caption=Geometry Tools;state=2108156;dir=1;layer=10;row=0;pos=290;prop=100000;bestw=248;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=SolidBar;caption=Solid Tools;state=2108156;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=341;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=970;floaty=297;floatw=296;floath=57|name=ViewingBar;caption=Viewing Tools;state=2108156;dir=1;layer=10;row=1;pos=352;prop=100000;bestw=248;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=TransformBar;caption=Transformation Tools;state=2108156;dir=1;layer=10;row=1;pos=611;prop=100000;bestw=217;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Graphics;caption=Graphics;state=768;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=800;besth=600;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Objects;caption=Objects;state=2099196;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=300;besth=400;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Options;caption=Options;state=2099196;dir=4;layer=1;row=0;pos=1;prop=100000;bestw=300;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Input;caption=Input;state=2099196;dir=4;layer=1;row=0;pos=2;prop=100000;bestw=300;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Properties;caption=Properties;state=2099196;dir=4;layer=1;row=0;pos=3;prop=100000;bestw=300;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=MachiningBar;caption=Machining tools;state=2108156;dir=1;layer=10;row=0;pos=549;prop=100000;bestw=279;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Program;caption=Program;state=2099196;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=600;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Output;caption=Output;state=2099196;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=600;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=504|dock_size(4,1,0)=234|dock_size(1,10,0)=33|dock_size(1,10,1)=33|dock_size(3,0,0)=219|")));
}

void CHeeksPythonApp::OnNewOrOpen(bool open)
{
	
}

void CHeeksPythonApp::GetOptions(std::list<Property *> *list){
	
}

void CHeeksPythonApp::OnFrameDelete()
{
	wxAuiManager* aui_manager = heeksCAD->GetAuiManager();
	PythonConfig config;
	config.Write(_T("ConsoleVisible"), aui_manager->GetPane(m_console).IsShown());

}

wxString CHeeksPythonApp::GetDllFolder()
{
	return m_dll_path;
}

class MyApp : public wxApp
{
 
 public:
 
   virtual bool OnInit(void);
 
 };
 
 bool MyApp::OnInit(void)
 
 {
 
   return true;
 
 }

 DECLARE_APP(MyApp)
 
 IMPLEMENT_APP(MyApp)
 
