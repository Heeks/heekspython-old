// HeeksPython.cpp
#include "stdafx.h"

#ifdef WIN32
#include "windows.h"
#endif

#include "PythonInterface.h"
#include "Interface.h"
#include "interface/HeeksCADInterface.h"
#include "interface/HeeksObj.h"
#include "interface/ToolImage.h"
#include "ConsoleCanvas.h"
#include "PythonConfig.h"
#include "interface/ObjList.h"


//#include "src/PointDrawing.h"
#include <set>

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#include <wx/wxPython/wxPython.h>
#define _DEBUG
#else
#include <Python.h>
#include <wx/wxPython/wxPython.h>
#endif



extern CHeeksCADInterface *heeksCAD;
extern CHeeksPythonApp *theApp;
extern wxWindow* m_window;


class Property;

void OnStartUp(CHeeksCADInterface* h, const wxString& dll_path)
{

	theApp = new CHeeksPythonApp();
	theApp->OnStartUp(h, dll_path);
}

void OnNewOrOpen(int open, int res)
{
	theApp->OnNewOrOpen(open != 0, res);
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

HeeksObj* lastobj;

static PyObject* NewPoint(PyObject* self, PyObject* args)
{
//from PointDrawing.cpp:
//temp_object = new HPoint(end.m_point, &wxGetApp().current_color);
//if(temp_object)temp_object_in_list.push_back(temp_object);

	const double p[3]={0,0,0};
	if (!PyArg_ParseTuple(args, "ddd", &p[0],&p[1],&p[2])) return NULL;

	lastobj = heeksCAD->NewPoint(p);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}




static PyObject* NewLine(PyObject* self, PyObject* args)
{
	double s[3]={0,0,0};
	double e[3]={0,0,0};
	if (!PyArg_ParseTuple(args, "dddd", &s[0],&s[1],&e[0],&e[1])) return NULL;

	lastobj = heeksCAD->NewLine(s,e);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
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

	lastobj = heeksCAD->NewLine(s,e);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
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

	lastobj = heeksCAD->NewCircle(c,r);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* NewArc(PyObject* self, PyObject* args)
{
	double c[3];
	double u[3];
	double r,s,e;

    if (!PyArg_ParseTuple( args, "ddddddddd", &c[0],&c[1],&c[2],&r,&s,&e,&u[0],&u[1],&u[2])) return NULL;

	lastobj = heeksCAD->NewArc(c,u,r,s,e);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

//added by DF- arc2 function using start point, end point,center point, uv
//NewArc(const double* s, const double* e, const double* c, const double* up)
static PyObject* NewArc2(PyObject* self, PyObject* args)
{
	double c[3];
	double u[3];
	double s[3];
    double e[3];

    if (!PyArg_ParseTuple( args, "dddddddddddd",&s[0],&s[1],&s[2],&e[0],&e[1],&e[2],&c[0],&c[1],&c[2],&u[0],&u[1],&u[2])) return NULL;

	lastobj = heeksCAD->NewArc(s,e,c,u);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}



static PyObject* NewCuboid(PyObject* self, PyObject* args)
{
	double c[3]={0,0,0};
	double x=0;
	double y=0;
	double z=0;
	if (!PyArg_ParseTuple(args, "dddddd", &c[0],&c[1],&c[2],&x,&y,&z)) return NULL;
	lastobj = heeksCAD->NewCuboid(c,x,y,z);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}


static PyObject* NewCylinder(PyObject* self, PyObject* args)
{
	double c[3]={0,0,0};
	double r=0;
	double h=0;
	if (!PyArg_ParseTuple(args, "ddddd", &c[0],&c[1],&c[2],&r,&h)) return NULL;
	lastobj = heeksCAD->NewCylinder(c,r,h);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* NewCylinderEx(PyObject* self, PyObject* args)
{
	double pos[3]={0,0,0};
	double dir[3]={0,0,0};
	double r=0;
	double h=0;
	if (!PyArg_ParseTuple(args, "dddddddd", &pos[0],&pos[1],&pos[2],&dir[0],&dir[1],&dir[2],&r,&h)) return NULL;
	lastobj = heeksCAD->NewCylinderEx(pos,dir,r,h);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* NewCone(PyObject* self, PyObject* args)
{
	double c[3]={0,0,0};
	double r1=0;
	double r2=0;
	double h=0;
	if (!PyArg_ParseTuple(args, "dddddd", &c[0],&c[1],&c[2],&r1,&r2,&h)) return NULL;
	lastobj = heeksCAD->NewCone(c,r1,r2,h);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* NewSphere(PyObject* self, PyObject* args)
{
	double c[3]={0,0,0};
	double r=0;
	if (!PyArg_ParseTuple(args, "dddd", &c[0],&c[1],&c[2],&r)) return NULL;
	lastobj = heeksCAD->NewSphere(c,r);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}






static PyObject* NewGroup(PyObject* self, PyObject* args)
{
	lastobj = heeksCAD->NewGroup();
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* Reorder(PyObject* self, PyObject* args)
{
	int oid;
	HeeksObj* obj;

    if (!PyArg_ParseTuple( args, "i", &oid)) return NULL;

    // Convert the PyCObject to a void pointer:
	obj = (HeeksObj*)heeksCAD->GetIDObject(oid>>16,oid&0xFFFF);
	heeksCAD->ReOrderSketch(obj,SketchOrderTypeCloseCW);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* ViewExtents(PyObject* self, PyObject* args)
{

	heeksCAD->ViewExtents(false);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* XYZview(PyObject* self, PyObject* args)
{

	heeksCAD->XYZView(true);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}


static PyObject* NewSketch(PyObject* self, PyObject* args)
{
	lastobj = heeksCAD->NewSketch();
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* WxHandle(PyObject* self, PyObject* args)
{

	PyObject *pValue = wxPyMake_wxObject(m_window, false);
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* GetLastObj(PyObject* self, PyObject* args)
{
	//return PyInt_FromLong(lastobj->m_id | lastobj->GetIDGroupType()<<16);
	return PyInt_FromLong(lastobj->m_id | lastobj->GetIDGroupType()<<16);
}

static PyObject* Rotate(PyObject* self, PyObject* args)
{
	HeeksObj *obj;
	int pyobj = 0;
	double p[3];
	double u[3];
	double r;

    if (!PyArg_ParseTuple( args, "iddddddd", &pyobj,&p[0],&p[1],&p[2],&u[0],&u[1],&u[2],&r)) return NULL;

    // Convert the PyCObject to a void pointer:
	obj = (HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF);
	heeksCAD->RotateObject(obj,p,u,r);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}


static PyObject* Extrude(PyObject* self, PyObject* args)
{
	HeeksObj *obj;
	int pyobj = 0;
	double h;

    if (!PyArg_ParseTuple( args, "id", &pyobj,&h)) return NULL;

    // Convert the PyCObject to a void pointer:
	obj = (HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF);
	lastobj = heeksCAD->ExtrudeSketch(obj,h,true);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}


static PyObject* Revolve(PyObject* self, PyObject* args)
{
	HeeksObj *obj;
	int pyobj = 0;
	double a;

    if (!PyArg_ParseTuple( args, "id", &pyobj,&a)) return NULL;

    // Convert the PyCObject to a void pointer:
	obj = (HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF);
	lastobj = heeksCAD->RevolveSketch(obj,a,true);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}



static PyObject* Translate(PyObject* self, PyObject* args)
{
	HeeksObj *obj;
	int pyobj = 0;
	double p[3];

    if (!PyArg_ParseTuple( args, "iddd", &pyobj,&p[0],&p[1],&p[2])) return NULL;

    // Convert the PyCObject to a void pointer:
    obj = (HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF);
	heeksCAD->TranslateObject(obj,p);
	heeksCAD->Repaint();
	

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* Scale(PyObject* self, PyObject* args)
{
	HeeksObj *obj;
	int pyobj = 0;
	double p[3];
	double scale = 1.0;

    if (!PyArg_ParseTuple( args, "idddd", &pyobj,&p[0],&p[1],&p[2],&scale)) return NULL;

    // Convert the PyCObject to a void pointer:
    	obj = (HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF);
	heeksCAD->ScaleObject(obj,p,scale);
	heeksCAD->Repaint();
	

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* Add(PyObject* self, PyObject* args)
{
	HeeksObj *group, *obj;
	int pygroup,pyobj;

    if (!PyArg_ParseTuple( args, "ii", &pygroup, &pyobj)) return NULL;

    // Convert the PyCObject to a void pointer:
    obj = (HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF);
	group = (HeeksObj*)heeksCAD->GetIDObject(pygroup>>16,pygroup&0xFFFF);
	obj->Owner()->Remove(obj);
	group->Add(obj,NULL);
	heeksCAD->Repaint();
	

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* Fuse(PyObject* self, PyObject* args)
{
	int pyobj1,pyobj2;

    if (!PyArg_ParseTuple( args, "ii", &pyobj1, &pyobj2)) return NULL;

    // Convert the PyCObject to a void pointer:
	std::list<HeeksObj*> list;
	list.push_back((HeeksObj*)heeksCAD->GetIDObject(pyobj1>>16,pyobj1&0xFFFF));
	list.push_back((HeeksObj*)heeksCAD->GetIDObject(pyobj2>>16,pyobj2&0xFFFF));
	lastobj = heeksCAD->Fuse(list);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();
	

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}


static PyObject* Common(PyObject* self, PyObject* args)
{
	int pyobj1,pyobj2;

    if (!PyArg_ParseTuple( args, "ii", &pyobj1, &pyobj2)) return NULL;

    // Convert the PyCObject to a void pointer:
	std::list<HeeksObj*> list;
	list.push_back((HeeksObj*)heeksCAD->GetIDObject(pyobj1>>16,pyobj1&0xFFFF));
	list.push_back((HeeksObj*)heeksCAD->GetIDObject(pyobj2>>16,pyobj2&0xFFFF));
	lastobj = heeksCAD->Common(list);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();
	

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}



static PyObject* LineArc2Wire(PyObject* self, PyObject* args)
{
	int pyobj;

    if (!PyArg_ParseTuple( args, "i", &pyobj)) return NULL;

    // Convert the PyCObject to a void pointer:
	std::list<HeeksObj*> list;
	list.push_back((HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF));
	
	lastobj = heeksCAD->LineArcsToWire(list);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();
	

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}


static PyObject* Cut(PyObject* self, PyObject* args)
{
	int pyobj1,pyobj2;

    if (!PyArg_ParseTuple( args, "ii", &pyobj1, &pyobj2)) return NULL;

    // Convert the PyCObject to a void pointer:
	std::list<HeeksObj*> list;
	list.push_back((HeeksObj*)heeksCAD->GetIDObject(pyobj1>>16,pyobj1&0xFFFF));
	list.push_back((HeeksObj*)heeksCAD->GetIDObject(pyobj2>>16,pyobj2&0xFFFF));
	//lastobj = heeksCAD->Cut(list);
	//heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Cut(list);
	heeksCAD->Repaint();
	

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* Pipe(PyObject* self, PyObject* args)
{
	int pyobj1,pyobj2;
	HeeksObj *wire,*profile;

    if (!PyArg_ParseTuple( args, "ii", &pyobj1, &pyobj2)) return NULL;

    // Convert the PyCObject to a void pointer:
	wire = (HeeksObj*)heeksCAD->GetIDObject(pyobj1>>16,pyobj1&0xFFFF);
	profile = (HeeksObj*)heeksCAD->GetIDObject(pyobj2>>16,pyobj2&0xFFFF);
	lastobj = heeksCAD->MakePipe(wire,profile);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();
	

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}


static PyObject* SetColor(PyObject* self, PyObject* args)
{
	int r,g,b;

    if (!PyArg_ParseTuple( args, "iii", &r,&b,&g)) return NULL;

	heeksCAD->SetColor(r,b,g);

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* Fillet2d(PyObject* self, PyObject* args)
{ //the lines definitely need to be trimmed first
	HeeksObj *obj;
	int pyobj = 0;
	double p[3];
	double r;

    if (!PyArg_ParseTuple( args, "idddd", &pyobj,&p[0], &p[1], &p[2], &r)) return NULL;
	obj = (HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF);
	heeksCAD->FilletSketchAtPoint(obj,p,r);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;

}

static PyObject* NewCoordinateSystem(PyObject* self, PyObject* args)
{
	double o[3]={0,0,0};
	double x[3]={1, 0, 0};
	double y[3]={0, 1, 0};
	if (!PyArg_ParseTuple(args, "ddddddddd",  &o[0],&o[1],&o[2],&x[0],&x[1],&x[2],&y[0],&y[1],&y[2])) return NULL;
	lastobj = heeksCAD->NewCoordinateSystem(o,x,y);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	heeksCAD->Repaint();

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}


static PyObject* PickPoint(PyObject* self, PyObject* args)
{	//get point in space, so to speak- not necessarily on an actual object
	//will return a point that is clicked in the graphicscanvas
	//the little spinner wheel will rotate until this is completed
	double pt1[3]={0,0,0};
	double px = 0;double py=0;double pz=0;
	heeksCAD->PickPosition(_("Pick something, please!"),pt1);
	
	px= pt1[0];py= pt1[1]; pz= pt1[2];
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	PyObject *pTuple = PyTuple_New(3);
	{
		PyObject *pValue = PyFloat_FromDouble(px);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 0, pValue);
	}
	{
		PyObject *pValue = PyFloat_FromDouble(py);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 1, pValue);
	}
	{
		PyObject *pValue = PyFloat_FromDouble(pz);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 2, pValue);
	}

	Py_INCREF(pTuple);
	return pTuple;
}



static PyObject* GetClickedPos(PyObject* self, PyObject* args)
{
	//will return last double clicked point in graphicscanvas
	//need to double click on a point object
	double pt1[3]={0,0,0};
	double px = 0;double py=0;double pz=0;
	heeksCAD->GetLastClickPosition(pt1);
	
	px= pt1[0];py= pt1[1]; pz= pt1[2];
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	PyObject *pTuple = PyTuple_New(3);
	{
		PyObject *pValue = PyFloat_FromDouble(px);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 0, pValue);
	}
	{
		PyObject *pValue = PyFloat_FromDouble(py);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 1, pValue);
	}
	{
		PyObject *pValue = PyFloat_FromDouble(pz);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 2, pValue);
	}

	Py_INCREF(pTuple);
	return pTuple;
}

static PyObject* GetPoint3d(PyObject* self, PyObject* args)
{
	//will return coordinates x,y,z of actual point object in graphicscanvas
	//need to pick point before calling function
	HeeksObj *obj;
	double pt1[3]={0,0,0};
	double px = 0;double py=0;double pz=0;
	obj= heeksCAD->GetFirstMarkedObject();
	if (!obj) return NULL;
	heeksCAD->VertexGetPoint(obj,pt1);
	px= pt1[0];py= pt1[1]; pz= pt1[2];
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
	PyObject *pTuple = PyTuple_New(3);
	{
		PyObject *pValue = PyFloat_FromDouble(px);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 0, pValue);
	}
	{
		PyObject *pValue = PyFloat_FromDouble(py);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 1, pValue);
	}
	{
		PyObject *pValue = PyFloat_FromDouble(pz);
		if (!pValue){
			Py_DECREF(pTuple);return NULL;
		}
		PyTuple_SetItem(pTuple, 2, pValue);
	}

	Py_INCREF(pTuple);
	return pTuple;
}


inline wxString _U(const char String[] = "")
{
    return wxString(String, wxConvUTF8);
}


static PyObject* NewText(PyObject* self, PyObject* args)
{	

	const char *text; //the text that gets input in python

	if (!PyArg_ParseTuple(args, "s",  &text)) return NULL; 
	
	heeksCAD->AddText(_U(text));

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;

}


static PyObject* AddMenu(PyObject* self, PyObject* args)
{	
	const char  *menu_name;
	if (!PyArg_ParseTuple(args, "s",  &menu_name)) return NULL;	

	wxFrame* frame = heeksCAD->GetMainFrame();
	wxMenu *newMenu = new wxMenu;
	frame->GetMenuBar()->Append(newMenu,  _U(menu_name));

	//return PyInt_FromSize_t((unsigned int)newMenu);
	return PyInt_FromSize_t((size_t)newMenu);//size_t instead of unsigned int for 64 bit gcc
}

static PyObject* GetFrameHwnd(PyObject* self, PyObject* args)
{	
	wxFrame* frame = heeksCAD->GetMainFrame();
	#if defined(__WXMSW__)
		return PyInt_FromSize_t((unsigned int)(frame->GetHWND()));
	#else
		return PyInt_FromSize_t((size_t)(frame->GetHandle()));//size_t instead of unsigned int for 64 bit gcc
	#endif
}

std::map<int, wxString> menu_item_map;

void OnMenuItem(wxCommandEvent &event)
{
	std::map<int, wxString>::iterator FindIt = menu_item_map.find(event.GetId());
	if(FindIt != menu_item_map.end())
	{
		// As always, first grab the GIL
		wxPyBlock_t blocked = wxPyBeginBlockThreads();

		// Now make a dictionary to serve as the global namespace when the code is
		// executed.  Put a reference to the builtins module in it.  (Yes, the
		// names are supposed to be different, I don't know why...)
		PyObject* globals = PyDict_New();
		PyObject* builtins = PyImport_ImportModule("__builtin__");
		PyDict_SetItemString(globals, "__builtins__", builtins);
		Py_DECREF(builtins);

		// Execute the python code
		std::string _str((const char *) FindIt->second.mb_str(wxConvUTF8));
		PyObject* result = PyRun_String(_str.c_str(), Py_file_input, globals, globals);

		// Release the python objects we still have
		if (result)Py_DECREF(result);
		else PyErr_Print();
		Py_DECREF(globals);

		// Finally, after all Python stuff is done, release the GIL
		wxPyEndBlockThreads(blocked);
	}
}

static PyObject* AddMenuItem(PyObject* self, PyObject* args)
{	
	long int_menu;
	const char *title;
	const char *python_script;
	const char *bitmap_path;
	if (!PyArg_ParseTuple(args, "lsss",  &int_menu, &title, &python_script, &bitmap_path)) return NULL;	

	wxMenu *menu = (wxMenu*)int_menu;

	int id = heeksCAD->AddMenuItem(menu, wxString(_U(title)), ToolImage(_U(bitmap_path)), OnMenuItem, NULL);

	menu_item_map.insert(std::make_pair(id, wxString(_U(python_script))));

	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}



static std::list<wxWindow*> new_windows;

std::map<int, wxWindow*> window_map;

void OnWindow( wxCommandEvent& event )
{
	std::map<int, wxWindow*>::iterator FindIt = window_map.find(event.GetId());
	if(FindIt != window_map.end())
	{
		wxWindow* window = FindIt->second;
		wxAuiManager* aui_manager = heeksCAD->GetAuiManager();
		wxAuiPaneInfo& pane_info = aui_manager->GetPane(window);
		if(pane_info.IsOk()){
			pane_info.Show(event.IsChecked());
			aui_manager->Update();
		}
	}
}

void OnUpdateWindow( wxUpdateUIEvent& event )
{
	std::map<int, wxWindow*>::iterator FindIt = window_map.find(event.GetId());
	if(FindIt != window_map.end())
	{
		wxWindow* window = FindIt->second;
		wxAuiManager* aui_manager = heeksCAD->GetAuiManager();
		event.Check(aui_manager->GetPane(window).IsShown());
	}
}

static PyObject* AddWindow(PyObject* self, PyObject* args)
{	
	long int_window;
	if (!PyArg_ParseTuple(args, "l",  &int_window)) return NULL;	

	wxFrame* frame = heeksCAD->GetMainFrame();
	wxAuiManager* aui_manager = heeksCAD->GetAuiManager();

	wxWindow * new_window = new wxWindow();
	#if defined(__WXMSW__)
		new_window->SetHWND((WXHWND)int_window);
		new_window->AdoptAttributesFromHWND();
	#endif
	new_window->Reparent(frame);

	wxString label = new_window->GetLabel();

	new_windows.push_back(new_window);

	aui_manager->AddPane(new_window, wxAuiPaneInfo().Name(label).Caption(label).Bottom().BestSize(wxSize(600, 200)));

	bool window_visible;
	wxString config_name = label + wxString(_T("Visible"));
	PythonConfig config;

	config.Read(config_name, &window_visible);

	aui_manager->GetPane(new_window).Show(window_visible);

	wxMenu* view_menu = heeksCAD->GetWindowMenu();
	int id = heeksCAD->AddMenuItem(view_menu, label, wxBitmap(), OnWindow, OnUpdateWindow,0,true);
	heeksCAD->RegisterHideableWindow(new_window);
	window_map.insert(std::make_pair(id, new_window));

	return PyInt_FromLong(new_window->GetId());
}

static PyObject* DXFImport(PyObject* self, PyObject* args)
{	

	const char  *filepath;
	if (!PyArg_ParseTuple(args, "s",  &filepath)) return NULL;	

	heeksCAD->OpendxfFile(_U(filepath));	
	
	heeksCAD->Repaint();	
	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
}

static PyObject* Redraw(PyObject* self, PyObject* args)
{
	
	
	heeksCAD->Repaint();
	PyObject *pValue = Py_None;
	Py_INCREF(pValue);
	return pValue;
	

}


static PyObject* GetProfile(PyObject* self, PyObject* args)
{
	//I am trying to return a single sketch 
	//this doesn't work yet
	//it just returns and int of '1' every time
	
	PyObject *resultobj = 0;

	int result;

	result = heeksCAD->PickObjects(_("Select a Sketch"), MARKING_FILTER_SKETCH,true);
	resultobj = PyLong_FromLong(result);
	
	PyObject *pValue = resultobj;
	Py_INCREF(pValue);
	return pValue;

}


static PyMethodDef HeeksPythonMethods[] = {
	{"sketch", NewSketch, METH_VARARGS , "sketch()"},
	{"wxhandle", WxHandle, METH_VARARGS , "wxhandle()"},
	{"extrude", Extrude, METH_VARARGS , "extrude(sketch,height)"},
	{"revolve", Revolve, METH_VARARGS , "revolve(sketch,angle)"},
	{"reorder", Reorder, METH_VARARGS , "reorder(sketch)"},
	{"point", NewPoint, METH_VARARGS , "point(x,y,z)"},
	{"linearc2wire", LineArc2Wire, METH_VARARGS , "linearc2wire(linearc)"},
	{"pipe", Pipe, METH_VARARGS , "pipe(wire,sketch)"},
	{"arc", NewArc, METH_VARARGS, "arc(cx,cy,cz,radius,start_a,end_a,ux,uy,uz)"},
	{"arc2", NewArc2, METH_VARARGS, "arc(sx,sy,sz,ex,ey,ez,cx,cy,cz,ux,uy,uz)"},
	{"line", NewLine, METH_VARARGS , "line(start_x, start_y, end_x, end_y)"},
	{"line3d", NewLine3d, METH_VARARGS , "line3d(start_x, start_y, start_z, end_x, end_y, end_z)"},
	{"circle", NewCircle, METH_VARARGS , "circle(centre_x, centre_y, radius)"},
	{"cuboid", NewCuboid, METH_VARARGS , "cuboid(centre_x, centre_y, centre_z, length, width, height)"},
	{"cylinder", NewCylinder, METH_VARARGS , "cylinder(centre_x, centre_y, centre_z, radius, height)"},
        {"directedcylinder", NewCylinderEx, METH_VARARGS , "directedcylinder(centre_x, centre_y, centre_z, dir_x, dir_y, dir_z, radius, height)"},
	{"cone", NewCone, METH_VARARGS , "cylinder(centre_x, centre_y, centre_z, radius1, radius2, height)"},
    	{"sphere",NewSphere,METH_VARARGS ,"sphere(centre_x, centre_y, centre_z, radius)"},
	{"group", NewGroup, METH_VARARGS , "group()"},
	{"add", Add, METH_VARARGS, "add(group,obj)"},
	{"fuse",Fuse, METH_VARARGS, "fuse(obj1,obj2)"},
	{"common",Common, METH_VARARGS, "common(obj1,obj2)"},
	{"cut",Cut, METH_VARARGS, "cut(obj1,obj2)"},
	{"getlastobj", GetLastObj, METH_VARARGS , "getlastobj()"},
	{"rotate",Rotate, METH_VARARGS , "rotate(object,p_x,p_y,p_z,u_x,u_y,u_z,r)"},
	{"translate",Translate, METH_VARARGS , "translate(object,p_x,p_y,p_z)"},
	{"scale",Scale, METH_VARARGS , "scale(object,p_x,p_y,p_z,scale)"},
	{"setcolor",SetColor, METH_VARARGS, "setcolor(int r, int b, int g)"},
	{"fillet2d" ,Fillet2d, METH_VARARGS, "fillet2d(obj,point,radius)"},
	{"coordinate" ,NewCoordinateSystem, METH_VARARGS, "coordinate(position,x_vec,y_vec)"},	
	{"pickpoint" , PickPoint, METH_VARARGS, "pickpoint()"},	
	{"lastclicked" , GetClickedPos, METH_VARARGS, "lastclicked()"},	
	{"view_extents" , ViewExtents, METH_VARARGS, "view_extents()"},
	{"ve" , ViewExtents, METH_VARARGS, "ve()"},
	{"xyzview" , XYZview, METH_VARARGS, "xyzview()"},
	{"getpoint" , GetPoint3d, METH_VARARGS, "getpoint()"},	
	{"addtext", NewText, METH_VARARGS , "addtext('string')"},
	{"importdxf", DXFImport, METH_VARARGS , "importdxf('/filepath/filename.dxf')"},	
	{"addmenu", AddMenu, METH_VARARGS , "menu = addmenu('string')"},
	{"add_menu_item", AddMenuItem, METH_VARARGS , "add_menu_item(menu, 'string', 'python_script')"},
	{"add_window", AddWindow, METH_VARARGS , "add_window(hwnd)"},
	{"get_frame_hwnd", GetFrameHwnd, METH_VARARGS , "hwnd = get_frame_hwnd()"},
	{"redraw" , Redraw, METH_VARARGS, "redraw()"},
	{"getsketch" , GetProfile, METH_VARARGS, "getsketch()"},	
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initHeeksPython(void)
{
	Py_InitModule("HeeksPython", HeeksPythonMethods);
}
