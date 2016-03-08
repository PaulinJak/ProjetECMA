# ---------------------------------------------------------------------
#      Local configuration
# ---------------------------------------------------------------------

# don't forget to run
# $ use diam
# $ usediam cplex
# in your shell before using this makefile.

## Cheated Makefile

## Putting OS related tag
UNAME := $(shell uname)

ILOG_ROOT=/opt/ibm/ILOG/CPLEX_Studio126
ILOG_SYSTEM=x86-64_linux
ILOG_LIBFORMAT=static_pic

BOOST_DIR=/usr/local/include
BOOST_LIB_DIR=/usr/lib/

CPLEX_DIR            = $(ILOG_ROOT)/cplex
CONCERT_DIR          = $(ILOG_ROOT)/concert
CPLEX_LIB_DIR        = $(CPLEX_DIR)/lib/$(ILOG_SYSTEM)/$(ILOG_LIBFORMAT)
CONCERT_LIB_DIR      = $(CONCERT_DIR)/lib/$(ILOG_SYSTEM)/$(ILOG_LIBFORMAT)
CPLEX_INCLUDE_DIR    = $(CPLEX_DIR)/include
CONCERT_INCLUDE_DIR  = $(CONCERT_DIR)/include


#  Add -DNDEBUG to those if you need it.

CPLEX_CXX_CFLAGS     = -I$(CPLEX_INCLUDE_DIR) -I$(CONCERT_INCLUDE_DIR) \
                       -m64 -fPIC -fno-strict-aliasing \
                       -fexceptions -O -DIL_STD 

CPLEX_C_CFLAGS       = -I$(CPLEX_INCLUDE_DIR) \
                       -m64 -fPIC -fno-strict-aliasing 

CPLEX_CXX_LDDIRS     = -L$(CPLEX_LIB_DIR) -L$(CONCERT_LIB_DIR)
CPLEX_CXX_LDFLAGS    = -lconcert -lilocplex -lcplex -lm
CPLEX_C_LDDIRS       = -L$(CPLEX_LIB_DIR)
CPLEX_C_LDFLAGS      = -lcplex -lm

ifeq ($(UNAME), Linux)
CPLEX_C_LDFLAGS   += -pthread
CPLEX_CXX_LDFLAGS += -pthread
endif

BOOST_FLAG=-I$(BOOST_DIR) 
BOOST_CXX_LDDIRS= -L$(BOOST_LIB_DIR)
# ---------------------------------------------------------------------


CXX_OBJ  := $(patsubst %.cpp, %.o, $(CXX_SRC))
OBJ      := $(CXX_OBJ) #$(C_OBJ) 


CCXX     := g++ -c #compile and assemble but do not link
#CXXFLAGS stores compiling options for C++ 
CXXFLAGS += -Wall $(CPLEX_CXX_CFLAGS) $(BOOST_FLAG) -g -std=c++11 #-DNVERBOSE
LDFLAGS  += $(CPLEX_CXX_LDFLAGS) -g

ifeq ($(UNAME), Darwin)
CXXFLAGS += -stdlib=libstdc++ -std=c++14
LDFLAGS  += -stdlib=libstdc++
endif

LDDIRS   += $(CPLEX_CXX_LDDIRS)
LDDIRS 	+=$(BOOST_CXX_LDDIRS)
LINK     := g++

#$@ is the name of the file to be made.
#$? is the names of the changed dependents.
#$< the name of the related file that caused the action.
#$* the prefix shared by target and dependent files.


all : ECMA


ECMA: main.o  modele_Cplex.o  options.o recuit.o voisinage.o genetique.o glouton.o
	$(LINK) $(LDDIRS) -o ECMA main.o modele_Cplex.o recuit.o voisinage.o genetique.o glouton.o $(LDFLAGS) -lm $(CXXFLAGS) 

options.o: src/options.hpp
	$(CCXX) $(LDDIRS)  src/options.cpp -o  options.o $(LDFLAGS) -lm $(CXXFLAGS) 

main.o : src/main.cpp src/modele_Cplex.hpp src/instance_Cplex.hpp src/recuit.hpp src/genetique.hpp
	$(CCXX) $(LDDIRS) src/main.cpp -o main.o  $(LDFLAGS) -lm $(CXXFLAGS) 

modele_Cplex.o: src/modele_Cplex.cpp  src/modele_Cplex.hpp src/instance_Cplex.hpp  #src/options.hpp
	$(CCXX) $(LDDIRS)  src/modele_Cplex.cpp -o modele_Cplex.o $(LDFLAGS) -lm $(CXXFLAGS) 

recuit.o : src/recuit.cpp src/recuit.hpp src/voisinage.h
	$(CCXX) $(LDDIRS) -o recuit.o src/recuit.cpp $(LDFLAGS) -lm $(CXXFLAGS)

voisinage.o : src/voisinage.cpp src/voisinage.h
	$(CCXX) $(LDDIRS) -o voisinage.o src/voisinage.cpp $(LDFLAGS) -lm $(CXXFLAGS)

genetique.o :  src/genetique.cpp src/genetique.hpp
	$(CCXX) $(LDDIRS) -o genetique.o src/genetique.cpp $(LDFLAGS) -lm $(CXXFLAGS)

genetique.o :  src/glouton.cpp src/glouton.hpp
	$(CCXX) $(LDDIRS) -o glouton.o src/glouton.cpp $(LDFLAGS) -lm $(CXXFLAGS)



clean:
	rm -f *.o
