CXX      = $(COMPILER)
DEB      = $(DEBUG)
INPUT_OS = $(OS)

ifeq ($(shell uname),SunOS)
   EXTLIBS = -lm     # -ldl -lsocket -lnsl -lm -lc
   OS = solaris
endif
ifeq ($(shell uname),Linux)
   EXTLIBS = -lm     # -ldl -lnsl -lm -lc
   OS = linux
endif
# if we have Windows then create .exe
ifeq ($(INPUT_OS),win)
   OS = exe
endif

FLAGS = # -DSTUDENT_VERSION   # FOR STUDENT VERSION !!!

# ---------set the compiler and the flags ----------------
#-std=c++0x   -std=c++11 -m64 
CFLAGS = -std=c++11 -msse3  
ifeq ($(DEB),yes)
   CFLAGS += -O0 -g  -D_GLIBCXX_DEBUG
else
   CFLAGS += -O3 
endif

CLINKS = -static-libstdc++  
CFLAGS += $(FLAGS)
# if compiler is not given than use G++ 
CXXFLAGS = $(CFLAGS)
ifeq ($(CXX),)
    CXX = g++
endif