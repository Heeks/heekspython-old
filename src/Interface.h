// declares all the exported functions for HeeksPython

class Property;
class CHeeksCADInterface;

extern "C"{
#ifdef WIN32
#define HEEKSPYTHON_EXPORT __declspec( dllexport ) __cdecl
#else
#define HEEKSPYTHON_EXPORT
#endif

void HEEKSPYTHON_EXPORT OnStartUp(CHeeksCADInterface* h, const wxString& dll_path);
void HEEKSPYTHON_EXPORT OnNewOrOpen(int open, int res);
void HEEKSPYTHON_EXPORT GetOptions(void(*callbackfunc)(Property*));
void HEEKSPYTHON_EXPORT OnFrameDelete();
}
