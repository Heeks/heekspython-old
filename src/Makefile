WXCFLAGS=$(shell wx-config --cflags)
WXLDFLAGS=$(shell wx-config --libs --gl-libs)
CC = g++
PREFIX=/usr/local
DATADIR=$(PREFIX)/share
HEEKSPYTHONDIR=$(DATADIR)/heekspython
mkdir_p= mkdir -p
PYTHONDIR = `python -c "from distutils.sysconfig import get_python_lib; print get_python_lib(plat_specific=1, standard_lib=0, prefix='$(PREFIX)')"`

# dan heeks uncomments the next line to make it work for him on his Inspiron 530
#WXCFLAGS=-I/usr/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/include/wx-2.8

#check the environment variable HEEKSCADPATH.  If it's not set, print a message and assume ../..
ifeq ($(strip $(HEEKSCADPATH)),)
  heekspathmessage = message
  HEEKSCADPATH = ../..
else
  heekspathmessage = 
endif


ifeq ($(strip $(PYTHONCFLAGS)),)
  PYTHONCFLAGS=$(shell python-config --includes)
endif

ifneq ($(strip $(CASROOT)),)
    CASINCPATH=$(CASROOT)
else
    CASINCPATH=/usr/include/opencascade
endif



CCFLAGS=-Wall -fPIC -g -I ./ $(WXCFLAGS) \
  -I$(HEEKSCADPATH)  -I/usr/include/GL -I$(PYTHONCFLAGS) -I$(CASROOT) \
  -D__WXGTK__ -DHEEKSCNC -DUNICODE -DWXUSINGDLL -DWXUSINGDLL

#WX_LIB_LETTER=d
WX_LIB_LETTER=u

ifeq ($(strip $(PYTHONLIBS)),)
  PYTHONLIBS=$(shell python-config --ldflags)
endif

OBJECTS=ConsoleCanvas.o HeeksPython.o PythonInterface.o ToolImage.o

SHARED_LIBRARY=../HeeksPython.so

$(SHARED_LIBRARY): $(heekspathmessage) $(OBJECTS)
	$(CC) -shared -fPIC -Wl,-soname,HeeksPython.so $(PYTHONLIBS) \
	  -Xlinker -export-dynamic -o $(SHARED_LIBRARY) $(OBJECTS) \
	  -lstdc++ -lGLU $(WXLDFLAGS)

#print a message if HEEKSCADPATH is not set
#use escape sequences for colors.  Z gets translated to \033
message:
	@echo Z[1mEnvironment variable HEEKSCADPATH not set. Assuming Z[32m../..Z[37mZ[0m | tr Z '\033'

ConsoleCanvas.o: $(HEEKSCADPATH)/interface/ToolImage.h		
	$(CC) -c  ConsoleCanvas.cpp $(CCFLAGS) -o $@
HeeksPython.o:  $(HEEKSCADPATH)/interface/ToolImage.h		
	$(CC) -c HeeksPython.cpp  $(CCFLAGS) -o $@
PythonInterface.o:  $(HEEKSCADPATH)/interface/ToolImage.h		
	$(CC) -c PythonInterface.cpp  $(CCFLAGS) -o $@

ToolImage.o: ${HEEKSCADPATH}/interface/ToolImage.cpp \
	${HEEKSCADPATH}/interface/ToolImage.h
	$(CC) -c ${HEEKSCADPATH}/interface/ToolImage.cpp ${CCFLAGS} -o $@

clean:
	-rm -f $(SHARED_LIBRARY) $(OBJECTS)

install:$(SHARED_LIBRARY)
	strip $^
	chmod 644 $^
	install $^ $(DESTDIR)$(PYTHONDIR)
	test -z "$(HEEKSPYTHONDIR)/heeksscripts" || $(mkdir_p) "$(HEEKSPYTHONDIR)/heeksscripts"
	cp ../autoexec.py "$(PYTHONDIR)"
	test -z "$(DESTDIR)$(HEEKSPYTHONDIR)/bitmaps" || $(mkdir_p) "$(DESTDIR)$(HEEKSPYTHONDIR)/bitmaps"
	cp ../bitmaps/*.png $(DESTDIR)$(HEEKSPYTHONDIR)/bitmaps/
	cp ../examples/*.py $(DESTDIR)$(HEEKSPYTHONDIR)/heeksscripts/	

uninstall:
	-rm -rf $(DESTDIR)$(HEEKSPYTHONDIR)
	-rm  $(PYTHONDIR)/autoexec.py
	-rm  $(DESTDIR)$(PYTHONDIR)/HeeksPython.so
