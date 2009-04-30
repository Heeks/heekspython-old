// ConsoleCanvas.cpp

#include "stdafx.h"
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
#include "ConsoleCanvas.h"

#include <fstream>

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#include <wx/wxPython/wxPython.h>
#define _DEBUG
#else
#include <Python.h>

// Import Python and wxPython headers
#include <wx/wxPython/wxPython.h>
#endif


BEGIN_EVENT_TABLE(CConsoleCanvas, wxScrolledWindow)
    EVT_SIZE(CConsoleCanvas::OnSize)
END_EVENT_TABLE()

CConsoleCanvas::CConsoleCanvas(wxWindow* parent)
        : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
	m_textCtrl=0;
//	DoPythonStuff(this);
//	m_textCtrl = new wxTextCtrl( this, 100, _T(""),	wxPoint(180,170), wxSize(200,70), wxTE_MULTILINE | wxTE_DONTWRAP);

//	Resize();
}

void CConsoleCanvas::InitP()
{
    if ( !Init_wxPython() )
        return;  

	m_textCtrl = DoPythonStuff(this);
	Resize();
}

bool CConsoleCanvas::Init_wxPython()
{
    // Initialize Python
    Py_Initialize();
    PyEval_InitThreads();

    // Load the wxPython core API.  Imports the wx._core_ module and sets a
    // local pointer to a function table located there.  The pointer is used
    // internally by the rest of the API functions.
    if ( ! wxPyCoreAPI_IMPORT() ) {
        wxLogError(wxT("***** Error importing the wxPython API! *****"));
        PyErr_Print();
        Py_Finalize();
        return false;
    }        
    
    // Save the current Python thread state and release the
    // Global Interpreter Lock.
    m_mainTState = wxPyBeginAllowThreads(); 

    return true;
}

char* python_code2 = "\
import sys\n\
import wx\n\
import wx.py\n\
sys.path.append('.')\n\
output = wx.PyOnDemandOutputWindow()\n\
sys.stdin = sys.stderr = output\n\
app = wx.App()\n\
def makeWindow(parent,style=wx.TE_MULTILINE | wx.TE_DONTWRAP):\n\
    return wx.py.shell.Shell(parent)\n\
";

PyObject *_pPyModule;
PyObject *_pPyDictionary;
PyObject *_pPyGetValFunc;
PyObject *_pPyobStringIO;

int PythonErrorMessage(){
// Py_Initialize should have been done by now.....
PyObject *modStringIO = NULL;
PyObject *obFuncStringIO = NULL;

// Import cStringIO module
modStringIO = PyImport_ImportModule("cStringIO");
if(PyErr_Occurred() || modStringIO == NULL){
printf("pyParserEvaluator::Init::PyImport cStringIO failed:");
PyErr_Print();
goto PY_INIT_ERR;
}
// get StringIO constructor
obFuncStringIO = PyObject_GetAttrString(modStringIO, "StringIO");
if(PyErr_Occurred() || obFuncStringIO == NULL){
printf("pyParserEvaluator::Init: cant find cStringIO.StringIO:");
PyErr_Print();
goto PY_INIT_ERR;
}
// Construct cStringIO object
_pPyobStringIO = PyObject_CallObject(obFuncStringIO, NULL);
if(PyErr_Occurred() || _pPyobStringIO==NULL){
printf("pyParserEvaluator::Init: cStringIO.StringIO() failed:");
PyErr_Print();
goto PY_INIT_ERR;
}
// get getvalue() method in StringIO instance
_pPyGetValFunc = PyObject_GetAttrString(_pPyobStringIO,
"getvalue");
if(PyErr_Occurred() || _pPyGetValFunc==NULL){
printf("pyParserEvaluator::Init: cant find getvalue function:");
PyErr_Print();
goto PY_INIT_ERR;
}
// try assigning this object to sys.stderr
int ret = PySys_SetObject("stderr", _pPyobStringIO);
if(ret != 0){
printf("failed to assign _pPyobStringIO to stderr\n");
goto PY_INIT_ERR;
}
return ret;

PY_INIT_ERR:
Py_XDECREF(modStringIO);
Py_XDECREF(obFuncStringIO);
Py_XDECREF(_pPyobStringIO);
Py_XDECREF(_pPyGetValFunc);
return ret;
}

char* pErrorString = NULL;

int _getPythonErrorMessage()
{
// call getvalue() method in StringIO instance
int ret = 0;
PyObject *obResult=NULL;
char *sresult = NULL;
obResult = PyObject_CallObject(_pPyGetValFunc, NULL);
if(PyErr_Occurred() || obResult==NULL){
printf("getvalue() failed\n");
ret = -1;
goto CLEAN_AND_RETURN;
}
// did getvalue return a string?
if(!PyString_Check(obResult)){
printf("getvalue() did not return error string\n");
ret = -1;
goto CLEAN_AND_RETURN;
}
// retrieve error message string from this object
if(NULL != (sresult = PyString_AsString(obResult))){
pErrorString = strdup(sresult);
} else {
ret = -1;
goto CLEAN_AND_RETURN;
}
return(ret);

CLEAN_AND_RETURN:
Py_XDECREF(obResult);
return(ret);
}

wxWindow* CConsoleCanvas::DoPythonStuff(wxWindow* parent)
{
    // More complex embedded situations will require passing C++ objects to
    // Python and/or returning objects from Python to be used in C++.  This
    // sample shows one way to do it.  NOTE: The above code could just have
    // easily come from a file, or the whole thing could be in the Python
    // module that is imported and manipulated directly in this C++ code.  See
    // the Python API for more details.

    wxWindow* window = NULL;
    PyObject* result;

    // As always, first grab the GIL
    wxPyBlock_t blocked = wxPyBeginBlockThreads();

    // Now make a dictionary to serve as the global namespace when the code is
    // executed.  Put a reference to the builtins module in it.  (Yes, the
    // names are supposed to be different, I don't know why...)
    PyObject* globals = PyDict_New();
    PyObject* builtins = PyImport_ImportModule("__builtin__");
    PyDict_SetItemString(globals, "__builtins__", builtins);
    Py_DECREF(builtins);

    // Execute the code to make the makeWindow function
    result = PyRun_String(python_code2, Py_file_input, globals, globals);
    // Was there an exception?
    if (! result) {
        PyErr_Print();
        wxPyEndBlockThreads(blocked);
        return NULL;
    }
    Py_DECREF(result);

    // Now there should be an object named 'makeWindow' in the dictionary that
    // we can grab a pointer to:
    PyObject* func = PyDict_GetItemString(globals, "makeWindow");
    wxASSERT(PyCallable_Check(func));

	PythonErrorMessage();

    // Now build an argument tuple and call the Python function.  Notice the
    // use of another wxPython API to take a wxWindows object and build a
    // wxPython object that wraps it.
    PyObject* arg = wxPyMake_wxObject(parent, false);
    wxASSERT(arg != NULL);
    PyObject* tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(tuple, 0, arg);
    result = PyEval_CallObject(func, tuple);

    // Was there an exception?
    if (! result)
	{
		PyErr_Print();
		_getPythonErrorMessage();
		std::ofstream ofs(_T("c:\\pyerr.txt"));
		ofs<<pErrorString;
	}
    else {
        // Otherwise, get the returned window out of Python-land and
        // into C++-ville...
        bool success = wxPyConvertSwigPtr(result, (void**)&window, _T("wxWindow"));
        wxASSERT_MSG(success, _T("Returned object was not a wxWindow!"));
        Py_DECREF(result);
    }

    // Release the python objects we still have
    Py_DECREF(globals);
    Py_DECREF(tuple);

    // Finally, after all Python stuff is done, release the GIL
    wxPyEndBlockThreads(blocked);

    return window;
}




void CConsoleCanvas::OnSize(wxSizeEvent& event)
{
    Resize();

    event.Skip();
}

void CConsoleCanvas::Resize()
{
	wxSize size = GetClientSize();
	if(m_textCtrl)
		m_textCtrl->SetSize(0, 0, size.x, size.y);
}

void CConsoleCanvas::Clear()
{
//	m_textCtrl->Clear();
}

