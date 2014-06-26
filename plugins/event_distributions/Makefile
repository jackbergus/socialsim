## Compiler flags
arch = amd64

ROOTBIN = /usr/share/root/bin
JAVA_HOME=$(shell readlink -f /usr/bin/javac | sed "s:bin/javac::")
INCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -I`root-config --incdir` 
LIBS = -ljvm -L$(JAVA_HOME)/jre/lib/$(arch) -L$(JAVA_HOME)/jre/lib/$(arch) -L$(JAVA_HOME)/jre/lib/$(arch)/client/ -L$(JAVA_HOME)/jre/lib/$(arch)/server/ `root-config --libs` -Wl,-R`root-config --libdir`  -L/usr/local/lib/ -Wl,-R/usr/local/lib/ 

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.plugin)
CC_FLAGS := -g -std=c++11 -shared -fPIC -flto -Wl,--export-dynamic -I../../src -I../../ext-libs 
CC_JSONO := ../../ext-libs/json/json.cpp
CC_DEBUG := -g 


%.plugin:	%.cpp	
	@echo "    CC $<"
	@g++ $(CC_FLAGS) $(INCLUDES) $(LIBS) -o $@ $< $(CC_JSONO) 
   
all:	$(OBJ_FILES)
	
clean:
	rm -f *.o
	rm -f *.plugin
	rm -f out

out:	all
	g++ -std=c++11 -g -c ../../ext-libs/json/json.cpp 		 -o json.o
	g++ -std=c++11 -g -c ../../src/plugin-loader/PathHandler.cpp	 -o phan.o -I../../src/
	g++ -std=c++11 -g -c ../../src/plugin-loader/DistrLoader.cpp	 -o dloa.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c test_exp.cxx				 -I../../ext-libs -I../../src
	g++ -std=c++11 -g -ldl 	-o out json.o phan.o dloa.o test_exp.o

debug:	out
