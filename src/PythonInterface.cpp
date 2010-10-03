// HeeksPython.cpp
#include <Python.h>
#include "stdafx.h"

#ifdef WIN32
#include "windows.h"
#endif

#include "PythonInterface.h"
#include "Interface.h"
#include "interface/HeeksCADInterface.h"
#include "interface/HeeksObj.h"
#include "ConsoleCanvas.h"
//#include "src/PointDrawing.h"
#include <set>



#include <wx/wxPython/wxPython.h>

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
	lastobj = heeksCAD->Cut(list);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
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

static PyObject* Fillet(PyObject* self, PyObject* args)
{ //the lines definitely need to be trimmed first
	HeeksObj *obj;
	int pyobj = 0;
	double p[3];
	double r;

    if (!PyArg_ParseTuple( args, "idddd", &pyobj,&p[0], &p[1], &p[2], &r)) return NULL;
	obj = (HeeksObj*)heeksCAD->GetIDObject(pyobj>>16,pyobj&0xFFFF);
	heeksCAD->FilletSketchAtPoint(obj,p,r);
	heeksCAD->GetMainObject()->Add(lastobj,NULL);
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
	{"setcolor",SetColor, METH_VARARGS, "setcolor(int r, int b, int g)"},
	{"fillet" ,Fillet, METH_VARARGS, "fillet(obj,point,radius)"},
	{"coordinate" ,NewCoordinateSystem, METH_VARARGS, "coordinate(position,x_vec,y_vec)"},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initHeeksPython(void)
{
	Py_InitModule("HeeksPython", HeeksPythonMethods);
}
