// PythonConfig.h
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>

class PythonConfig: public wxConfig
{
public:
	PythonConfig():wxConfig(_T("HeeksPython")){}
	~PythonConfig(){}
};