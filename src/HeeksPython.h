// HeeksCNC.h

// defines global variables and functions

#include "interface/HeeksCADInterface.h"

extern CHeeksCADInterface* heeksCAD;

class Property;
class CConsoleCanvas;

class CHeeksPythonApp{
public:
	CConsoleCanvas* m_console;
	wxString m_dll_path;

	CHeeksPythonApp();
	~CHeeksPythonApp();

	void OnStartUp(CHeeksCADInterface* h, const wxString& dll_path);
	void OnNewOrOpen(bool open, int res);
	void OnInitDLL();
	void OnDestroyDLL();
	void GetOptions(std::list<Property *> *list);
	void OnFrameDelete();
	wxString GetDllFolder();
};

class MyApp : public wxApp
{
 
 public:
 
   virtual bool OnInit(void);
   wxString GetResFolder();
 
 };
 
extern CHeeksPythonApp *theApp;

DECLARE_APP(MyApp)

