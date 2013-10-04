#---------------------------------------------------------------------
# Detect system type using uname command.
#
# On Linux,    uname -s returns 'Linux'
# On Linux 64, uname -m additionally returns x86_64
# On Mac OS X, uname -s returns 'Darwin'
#
# See discussion at http://osdir.com/ml/gnu.make.bugs/2002-09/msg00003.html
# Conclusion: instead of using OSTYPE (env var defined in bash), manually
# set OSTYPE := $(shell uname -msr)
# In order to avoid confusion, I'm going to just use OSNAME := $(shell uname -s).
#---------------------------------------------------------------------

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

#---------------------------------------------------------------------
# Choose a compiler & its options
#---------------------------------------------------------------------
g++ = g++
CXX = g++

ifeq ($(CXX),icpc)
  # If the compiler is ICPC, use some extra flags
  OPTS = -pipe -O3 -MMD -ip -vec-report0 -diag-disable cpu-dispatch
else 
  OPTS = -pipe -O3 -MMD 
endif

ifeq ($(PLATFORM),MAC_OS_X)
 OPTS += -DMACOS
endif

ifneq ($(ENABLE_LOGGING),0)
	OPTS += -DENABLE_LOGGING=1
endif


#-------------------------------------------------------------------
# Base parameters for INCLUDE and LIB.
#
# The next sections after this one add items to INCLUDE and LIB using
# the += operator. 
#-------------------------------------------------------------------

INCLUDE = -I.
LIB = $(LIBS)

#--------------------------------------------------------------------

DIRS= . mathtool modelgraph
SRCS=$(wildcard $(addsuffix /*.cpp,$(DIRS)))
OBJS=${SRCS:.cpp=.o}


#--------------------------------------------------------------------
# Set CFLAGS
#--------------------------------------------------------------------
INCLUDE    =   $(addprefix -I,$(DIRS))
#LIB        = -framework OpenGL -framework GLUT -framework Foundation 
CFLAGS     = $(OPTS) $(INCLUDE) 
CXXFLAGS   = $(CFLAGS)

all: obj2gwb

#--------------------------------------------------------------------
obj2gwb: $(OBJS) 
	${CXX} ${CXXFLAGS} -o $@ $(OBJS) $(LIB)

clean:
	rm -f *.o $(OBJS) Dependencies obj2gwb

#--------------------------------------------------------------------
# Build Rules
#--------------------------------------------------------------------

.SUFFIXES: .cpp

.cpp.o: 
	${CXX} ${CXXFLAGS} -c $< -o $@
	cat $*.d >> Dependencies
	-rm -f $*.d

Dependencies:
	touch Dependencies

-include Dependencies

