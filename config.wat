# =========================================================================
#     This configuration file was generated by
#     Bakefile 0.2.9 (http://www.bakefile.org)
#     Beware that all changes made to this file will be overwritten next
#     time you run Bakefile!
# =========================================================================


# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------



# C compiler 
CC = wcc386

# C++ compiler 
CXX = wpp386

# Standard flags for CC 
CFLAGS = 

# Standard flags for C++ 
CXXFLAGS = 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS = 

# Standard linker flags 
LDFLAGS = 

# The C preprocessor 
CPP = $(CC) -p

# What type of library to build? [0,1]
#   1 - DLL
SHARED = 1

# Build wxUniversal instead of native port? [0,1]
#   1 - Universal
WXUNIV = 0

# Compile Unicode build of wxWidgets? [0,1]
#   1 - Unicode
UNICODE = 1

# Type of compiled binaries [debug,release]
# BUILD = release
BUILD = debug

# Should debugging info be included in the executables? The default value
# "default" means that debug info will be included if BUILD=debug
# and not included if BUILD=release. [0,1,default]
DEBUG_INFO = default 

# Should __WXDEBUG__ be defined? The default value "default" means that it will
# be defined if BUILD=debug and not defined if BUILD=release. [0,1,default]
DEBUG_FLAG =default 

# Multiple libraries or single huge monolithic one? [0,1]
#   0 - Multilib
#   1 - Monolithic
MONOLITHIC = 0

# Build GUI libraries? [0,1]
#   0 - Base
#   1 - GUI
USE_GUI = 1

# Build wxHTML library (USE_GUI must be 1)? [0,1]
USE_HTML = 1

# Build multimedia library (USE_GUI must be 1)? [0,1]
USE_MEDIA = 1

# Build wxXRC library (USE_GUI must be 1)? [0,1]
USE_XRC = 1

# Build wxAUI library (USE_GUI must be 1)? [0,1]
USE_AUI = 1

# Build wxRichTextCtrl library (USE_GUI must be 1)? [0,1]
USE_RICHTEXT = 1

# Build OpenGL canvas library (USE_GUI must be 1)? [0,1]
USE_OPENGL = 0

# Build ODBC database classes (USE_GUI must be 1)? [0,1]
USE_ODBC = 0

# Build quality assurance classes library (USE_GUI must be 1)? [0,1]
USE_QA = 0

# Enable exceptions in compiled code. [0,1]
USE_EXCEPTIONS = 1

# Enable run-time type information (RTTI) in compiled code. [0,1]
USE_RTTI = 1

# Enable threading in compiled code. [0,1]
USE_THREADS = 1

# Enable wxCairoContext for platforms other than Linux/GTK. [0,1]
USE_CAIRO = 0

# Link with gdiplus.lib? (Needed for wxGraphicsContext, will also set wxUSE_GRAPHICS_CONTEXT) [0,1]
USE_GDIPLUS = 0

# Is this official build by wxWidgets developers? [0,1]
OFFICIAL_BUILD = 0

# Use this to name your customized DLLs differently 
VENDOR = custom

#  
WX_FLAVOUR = 

#  
WX_LIB_FLAVOUR = 

# Name of your custom configuration. This affects directory
# where object files are stored as well as the location of
# compiled .lib files and setup.h under the lib/ toplevel directory. 
CFG = 

# Compiler flags needed to compile test suite in tests directory. If you want
# to run the tests, set it so that the compiler can find CppUnit headers. 
CPPUNIT_CFLAGS = 

# Linker flags needed to link test suite in tests directory. If you want
# to run the tests, include CppUnit library here. 
CPPUNIT_LIBS = 

# Version of C runtime library to use. You can change this to
# static if SHARED=0, but it is highly recommended to not do
# it if SHARED=1 unless you know what you are doing. [dynamic,static]
RUNTIME_LIBS = dynamic
USE_DYNLIB_CLASS = 1

