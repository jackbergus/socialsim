
## Compiler flags
arch = amd64
ROOTBIN = /usr/share/root/bin
INCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux  -I`root-config --incdir` 
LIBS = `root-config --libs` -Wl,-R`root-config --libs` 


CC_FLAGS := -std=c++11 -I../../src -I../../ext-libs 
CC_JSONO := ../../ext-libs/json/json.cpp
CC_DEBUG := -DDEBUG $(CC_FLAGS) -g  -fPIC
GPP := g++ -std=c++11
OBJ := ../../src/graph/graphml.o
GRAPH := ../../src/graph/

SRC=$(wildcard *.cpp)
DST=$(SRC:.cpp=.o)

#compilazione dei file dia in pdf
%.o:		%.cpp
		$(GPP) -fPIC $(CC_DEBUG) $(INCLUDES) $(LIBS) -c $< -o $@

all:		$(DST)

debug:		all
		$(GPP) -fPIC $(CC_DEBUG) $(INCLUDES) $(LIBS) -c root.cxx
		$(GPP) $(CC_DEBUG) $(INCLUDES) $(LIBS) -Wl,-R`/usr/share/root/bin/root-config --libdir` *.o -o root

clean:		
		rm -f *.o *~
	

