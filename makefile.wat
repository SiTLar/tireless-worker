#WXDIR = C:/wxWidgets-2.8.12
WXDIR = /home/sitlar/wxMSW-2.8.12
!ifdef %WXDIR
WXDIR = $(%WXDIR)
!endif
EXTRALIB = ..
EXTRAINC = ..
SRC_DIR = src 
OBJS = &
	../wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
!ifeq DEBUG 1
!include config.wat
!else
!include config_rel.wat
!endif

!include setup.wat

MAIN_OBJECTS =  &
	$(OBJS)/thread.obj &
	$(OBJS)/main.obj &
	$(OBJS)/mycontrolls.obj &
	$(OBJS)/devabstruction.obj &
	$(OBJS)/frame.obj &
	$(OBJS)/config.obj &
	$(OBJS)/rxpack.obj &
	$(OBJS)/mathfunc.obj &
	$(OBJS)/build_config$(DEBUG_CFG).obj &
	$(OBJS)/handlerpanel.obj 


GENDLL_OBJECTS =  &
	$(OBJS)/devices.obj &
	$(OBJS)/logger.obj &
	$(OBJS)/com.obj &
#	$(OBJS)/devgpib.obj &
	$(OBJS)/devtcpip.obj &
	$(OBJS)/logplot.obj &
#	$(OBJS)/atexit.obj &
	$(OBJS)/logfile.obj

LOGPR_OBJECTS = &
	$(OBJS)/mathplot.obj &
	$(OBJS)/fxyseq.obj &
	$(OBJS)/dlgliminput_base.obj &
	$(OBJS)/logplotpr.obj 

EXTENDEDDLL_OBJECTS = &
	$(OBJS)/devvisa.obj &
	$(OBJS)/extendeddll.obj 

all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)
	-if not exist $(OBJS)/dll mkdir $(OBJS)/dll
	-if not exist $(OBJS)/exe mkdir $(OBJS)/exe

### Targets: ###
test : .SYMBOLIC
	echo $(%WXDIR)

all : .SYMBOLIC $(OBJS)/exe/tirelessworker.exe  $(OBJS)/exe/logplot.exe $(OBJS)/dll/generic.dll $(OBJS)/dll/extended.dll

clean : .SYMBOLIC 
	-if exist $(OBJS)/*.obj del $(OBJS)/*.obj
	-if exist $(OBJS)/*.res del $(OBJS)/*.res
	-if exist $(OBJS)/*.lbc del $(OBJS)/*.lbc
	-if exist $(OBJS)/*.ilk del $(OBJS)/*.ilk
	-if exist $(OBJS)/*.pch del $(OBJS)/*.pch
	-if exist $(OBJS)/*.exe del $(OBJS)/*.exe
	-if exist $(OBJS)/dll/*.dll del $(OBJS)/dll/*.dll

$(OBJS)/exe/logplot.exe :  $(LOGPR_OBJECTS)
	@%create $(OBJS)/logpr.lbc
	@%append $(OBJS)/logpr.lbc option quiet
	@%append $(OBJS)/logpr.lbc name $^@
	@%append $(OBJS)/logpr.lbc option caseexact
	@%append $(OBJS)/logpr.lbc libpath $(EXTRALIB)/ 
	@%append $(OBJS)/logpr.lbc  $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16' $(____CAIRO_LIBDIR_FILENAMES_p) $(LDFLAGS)
	@for %i in ($(LOGPR_OBJECTS)) do @%append $(OBJS)/logpr.lbc file %i
	@for %i in (  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p) $(__WXLIB_ADV_p) $(__WXLIB_XML_p) $(__WXLIB_MONO_p)   $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE) $(__WXLIB_NET_p) $(__GDIPLUS_LIB_p) $(__CAIRO_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib wxcode_$(PORTNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)_propgrid.lib wx$(PORTNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)_wxFlatNotebook.lib ) do @%append $(OBJS)/logpr.lbc library %i
	@for %i in () do @%append $(OBJS)/logpr.lbc option stack=%i
	wlink @$(OBJS)/logpr.lbc

$(OBJS)/exe/tirelessworker.exe :  $(MAIN_OBJECTS) $(OBJS)/main.res
	-if not exist $(OBJS)/exe mkdir $(OBJS)/exe
	@%create $(OBJS)/main.lbc
	@%append $(OBJS)/main.lbc option quiet
	@%append $(OBJS)/main.lbc name $^@
	@%append $(OBJS)/main.lbc option caseexact
	@%append $(OBJS)/main.lbc libpath $(EXTRALIB)/ 
	@%append $(OBJS)/main.lbc  $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16' $(____CAIRO_LIBDIR_FILENAMES_p) $(LDFLAGS)
	@for %i in ($(MAIN_OBJECTS)) do @%append $(OBJS)/main.lbc file %i
	@for %i in (  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p) $(__WXLIB_ADV_p) $(__WXLIB_XML_p) $(__WXLIB_MONO_p)   $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(__WXLIB_NET_p) $(EXTRALIBS_FOR_BASE)  $(__GDIPLUS_LIB_p) $(__CAIRO_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib $(EXTRALIB)/regina.lib $(EXTRALIB)/rexx.lib wxcode_$(PORTNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)_propgrid.lib wx$(PORTNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)_wxFlatNotebook.lib ) do @%append $(OBJS)/main.lbc library %i
	@%append $(OBJS)/main.lbc option resource=$(OBJS)/main.res
	@for %i in () do @%append $(OBJS)/main.lbc option stack=%i
	wlink @$(OBJS)/main.lbc


$(OBJS)/dll/generic.dll :  $(GENDLL_OBJECTS)
	-if not exist $(OBJS)/dll mkdir $(OBJS)/dll
	@%create $(OBJS)/gendll.lbc
	@%append $(OBJS)/gendll.lbc option quiet
	@%append $(OBJS)/gendll.lbc name $^@
#	@%append $(OBJS)/gendll.lbc alias _atexit='_stdcall_atexit@4'
	@%append $(OBJS)/gendll.lbc option caseexact
	@%append $(OBJS)/gendll.lbc  $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) $(LDFLAGS)
	@for %i in ($(GENDLL_OBJECTS)) do @%append $(OBJS)/gendll.lbc file %i
	@for %i in (  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p) $(__WXLIB_ADV_p) $(__WXLIB_XML_p) $(__WXLIB_MONO_p)   $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE) $(__WXLIB_NET_p) $(__GDIPLUS_LIB_p) $(__CAIRO_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib  ) do @%append $(OBJS)/gendll.lbc library %i
	@%append $(OBJS)/gendll.lbc
	@%append $(OBJS)/gendll.lbc system nt_dll
	@%append $(OBJS)/gendll.lbc EXPORT dynLoad =dynLoad_
	wlink @$(OBJS)/gendll.lbc

$(OBJS)/dll/extended.dll :  $(EXTENDEDDLL_OBJECTS)
	-if not exist $(OBJS)/dll mkdir $(OBJS)/dll
	@%create $(OBJS)/extdll.lbc
	@%append $(OBJS)/extdll.lbc option quiet
	@%append $(OBJS)/extdll.lbc name $^@
	@%append $(OBJS)/extdll.lbc option caseexact
	@%append $(OBJS)/extdll.lbc  $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) $(____CAIRO_LIBDIR_FILENAMES) $(LDFLAGS)
	@for %i in ($(EXTENDEDDLL_OBJECTS)) do @%append $(OBJS)/extdll.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(EXTRALIB)/Lusbapi.lib $(EXTRALIB)/visa32.lib) do @%append $(OBJS)/extdll.lbc library %i
	@%append $(OBJS)/extdll.lbc
	@%append $(OBJS)/extdll.lbc system nt_dll
	@%append $(OBJS)/extdll.lbc EXPORT dynLoad =dynLoad_
	wlink @$(OBJS)/extdll.lbc

#.EXTENSIONS: .exe .nlm .dsk .lan .exp .lib .obj &
#                  .i .asm .c .cpp .cxx .cc .for .pas .cob &
#                  .h .hpp .hxx .hh .fi .mif .inc



cpp_dir = $(SRC_DIR)
h_dir = $(SRC_DIR)
hpp_dir = $(SRC_DIR)
obj_dir = $(OBJS)

.cpp : $(SRC_DIR)
.c : $(SRC_DIR)

.cpp.obj: $< .AUTODEPEND
	$(CXX) -bt=nt -zq -fo=$^@ $(MAIN_CXXFLAGS) -DUNICODE $(CPPFLAGS) $(CXXFLAGS) $(__RTTIFLAG_7) $(__EXCEPTIONSFLAG_8) $<
	
.c.obj:  $< .AUTODEPEND
	$(CC) -bt=nt -zq -fo=$^@ $(MAIN_CXXFLAGS) $<
	

$(OBJS)/main.res :  .AUTODEPEND $(SRC_DIR)/thread.rc
	wrc -q -ad -bt=nt -r -fo=$^@    -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) -i=$(WXDIR)/include $(____CAIRO_INCLUDEDIR_FILENAMES) -i=. $(__DLLFLAG_p) -i=$(WXDIR)/samples -dNOPCH $<
