## Compiler flags
arch = amd64
JAVA_HOME=$(shell readlink -f /usr/bin/javac | sed "s:bin/javac::")
ROOTBIN = /usr/share/root/bin
INCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -I`root-config --incdir` 
LIBS = -ljvm -L$(JAVA_HOME)/jre/lib/$(arch) -L$(JAVA_HOME)/jre/lib/$(arch) -L$(JAVA_HOME)/jre/lib/$(arch)/client/ -L$(JAVA_HOME)/jre/lib/$(arch)/server/ 



CC_FLAGS := -std=c++11 -I../../src -I../../ext-libs 
CC_JSONO := ../../ext-libs/json/json.cpp
CC_DEBUG := -g -DDEBUG $(CC_FLAGS) -fPIC
GPP := g++ -std=c++11
OBJ := ../../src/graph/graphml.o
GRAPH := ../../src/graph/

SRC=$(wildcard *.cpp)
DST=$(SRC:.cpp=.o)

#compilazione dei file dia in pdf
%.o:		%.cpp
		$(GPP) -fPIC $(CC_DEBUG) $(INCLUDES) $(LIBS) -c $< -o $@

all:		$(DST)

clean:		
		rm -f *.o *~
	

