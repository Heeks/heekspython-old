// HeeksPython.cpp

#include "stdafx.h"

#ifdef WIN32
#include "windows.h"
#endif

#include "PythonInterface.h"
#include "Interface.h"
#include "interface/HeeksCADInterface.h"
#include "interface/HeeksObj.h"
#include <set>

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

extern CHeeksCADInterface *heeksCAD;
extern CHeeksPythonApp *theApp;

class Property;

void OnStartUp(CHeeksCADInterface* h, const wxString& dll_path)
{
	theApp = new CHeeksPythonApp();
	theApp->OnStartUp(h, dll_path);
}

void OnNewOrOpen(int open)
{
	theApp->OnNewOrOpen(open != 0);
}

void GetOptions(void(*callbackfunc)(Property*))
{
	std::list<Property*> list;
	theApp->GetOptions(&list);
	for(std::list<Property*>::iterator It = list.begin(); It != list.end(); It++){
		Property* p = *It;
		(*callbackfunc)(p);
	}
}

void OnFrameDelete()
{
	theApp->OnFrameDelete();
}


static PyObject* NewLine(PyObject* self, PyObject* args)
{
	double s[3]={0,0,0};
	double e[3]={0,0,0};
	if (!PyArg_ParseTuple(args, "dddd", &s[0],&s[1],&e[0],&e[1])) return NULL;

	heeksCAD->GetMainObject()->Add(heeksCAD->NewLine(s,e),NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* NewLine3d(PyObject* self, PyObject* args)
{
	double s[3]={0,0,0};
	double e[3]={0,0,0};
	if (!PyArg_ParseTuple(args, "dddddd", &s[0],&s[1],&s[2],&e[0],&e[1],&e[2])) return NULL;

	heeksCAD->GetMainObject()->Add(heeksCAD->NewLine(s,e),NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* NewCircle(PyObject* self, PyObject* args)
{
	double c[3]={0,0,0};
	double r=0;
	if (!PyArg_ParseTuple(args, "ddd", &c[0],&c[1],&r)) return NULL;

	heeksCAD->GetMainObject()->Add(heeksCAD->NewCircle(c,r),NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyMethodDef HeeksPythonMethods[] = {
	{"line", NewLine, METH_VARARGS , "line(start_x, start_y, end_x, end_y)"},
	{"line3d", NewLine3d, METH_VARARGS , "line3d(start_x, start_y, start_z, end_x, end_y, end_z)"},
	{"circle", NewCircle, METH_VARARGS , "circle(centre_x, centre_y, radius)"},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initHeeksPython(void)
{
	Py_InitModule("HeeksPython", HeeksPythonMethods);
}
