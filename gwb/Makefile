#
# makefile 
#

TARGET = gwb
SRC_DIR=.
LIB_DIR=.

#
# Determine the platform (Linux, Mac OS X, Windows+Cygwin etc.)
#
# On Linux,    this should be 'Linux'
# On Mac OS X, this should be 'Darwin'
#

OSNAME := $(shell uname -s)
MACHINE := $(shell uname -m)

ifeq ($(OSNAME),Linux)
  ifeq ($(MACHINE),x86_64)
    PLATFORM := LINUX64
  else
    PLATFORM := LINUX
  endif
else 
  ifeq ($(OSNAME),Darwin)
    PLATFORM := MAC_OS_X
  else
    PLATFORM := OTHER
  endif
endif

#-----------------------------------------------------------------------------
# Choose a compiler & its options
#--------------------------------------------------------------------------
g++ = g++
CXX = g++

OPTS = -pipe -g -MMD

ifeq ($(PLATFORM),MAC_OS_X)
 OPTS += -DMACOS -Wreturn-type -frounding-math
endif

OPTS += -frounding-math

#--------------------------------------------------------------------
# Xlib and OpenGL
#--------------------------------------------------------------------

ifeq ($(PLATFORM),MAC_OS_X)
   GL_INCL = -I/opt/local/include
   GL_LIB = -L/opt/local/lib  -lglui -framework OpenGL -framework GLUT -framework Foundation
else
   GL_LIB = -lglui -lGL -lGLU -lglut
endif

#--------------------------------------------------------------------
# MPFR
#--------------------------------------------------------------------

ifeq ($(PLATFORM),MAC_OS_X)
	MPFR_DIR = /opt/local
	MPFR_INCL= -I${MPFR_DIR}/include
	MPFR_LIB = -L${MPFR_DIR}/lib -lmpfr -lgmp
else 
	MPFR_LIB = -lmpfr -lgmp
endif


#--------------------------------------------------------------------
INCLUDE = -I$(SRC_DIR) $(GL_INCL) $(GLEW_INCL) $(MPFR_INCL)
LIB = $(GL_LIB) $(GLEW_LIB) $(MPFR_LIB)

#--------------------------------------------------------------------

CFLAGS     = $(OPTS) $(INCLUDE)
CXXFLAGS   = $(CFLAGS)

#--------------------------------------------------------------------
SRCS=$(wildcard $(addsuffix /*.cpp,$(SRC_DIR) GL))
OBJS=${SRCS:.cpp=.o}
DFILES=${SRCS:.cpp=.d}

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) ${CXXFLAGS} -o $@ $(OBJS) $(LIB) 

clean:
	-rm -f $(OBJS) $(TARGET) $(DFILES) Dependencies

#--------------------------------------------------------------------
.SUFFIXES: .cpp

.cpp.o: 
	${CXX} ${CXXFLAGS} -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d

Dependencies:
	touch Dependencies

-include Dependencies
