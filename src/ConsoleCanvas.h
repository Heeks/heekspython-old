// ConsoleCanvas.h

#pragma once

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

class CConsoleCanvas: public wxScrolledWindow
{
private:
    void Resize();
	bool Init_wxPython();
	wxWindow* DoPythonStuff(wxWindow* parent);
	

public:
    wxWindow *m_textCtrl;
	PyThreadState* m_mainTState;

    CConsoleCanvas(wxWindow* parent);
	virtual ~CConsoleCanvas(){}

	void Clear();
	void InitP();

    void OnSize(wxSizeEvent& event);

    DECLARE_NO_COPY_CLASS(CConsoleCanvas)
    DECLARE_EVENT_TABLE()
};

