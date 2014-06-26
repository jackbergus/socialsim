## MEMENTO: the flag  disables the using of the jvm
## Compiler flags
arch = amd64
JAVA_HOME=$(shell readlink -f /usr/bin/javac | sed "s:bin/javac::")
ROOTBIN = /usr/share/root/bin
INCLUDES = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -I`root-config --incdir`
## Avoiding Export LD_LIBRARY_PATH
LIBS = -ljvm -L$(JAVA_HOME)/jre/lib/$(arch) -Wl,-R$(JAVA_HOME)/jre/lib/$(arch) -L$(JAVA_HOME)/jre/lib/$(arch) -L$(JAVA_HOME)/jre/lib/$(arch)/client/ -L$(JAVA_HOME)/jre/lib/$(arch)/server/ -Wl,-R$(JAVA_HOME)/jre/lib/$(arch)/server/ -L/usr/local/lib/ -Wl,-R/usr/local/lib/ `root-config --libs` -Wl,-R`root-config --libdir`


CC_FLAGS := -std=c++11 -I. -I../ext-libs 
CC_JSONO := ../ext-libs/json/json.cpp
CC_DEBUG := $(CC_FLAGS) -g  -flto
GPP := g++ -std=c++11

SRC=$(wildcard *.cpp)
DST=$(SRC:.cpp=.o)

#compilazione dei file dia in pdf
%.o:		%.cpp
		$(GPP) -flto $(CC_DEBUG) $(INCLUDES) $(LIBS) -c $< -o $@

all:	subfolder $(DST)
	$(GPP)  $(CC_DEBUG) -c $(CC_JSONO)
	$(GPP) -lpthread -Wl,-Bstatic json.o plugin-loader/*.o  main.o  agent/*.o event/*.o graph/*.o -Wl,-Bdynamic  -o socialsim $(INCLUDES) $(LIBS)  -ldl -Wl,-Bstatic -lboost_graph -Wl,-Bdynamic #root/*.o
	#echo ""
	#echo "==========================================================================="
	#echo "Dont forget to set library path before running the example:"
	#echo "# export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(JAVA_HOME)/jre/lib/i386:$(JAVA_HOME)/jre/lib/i386/client/"
	#echo "or"
	#echo "# export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(JAVA_HOME)/jre/lib/amd64:$(JAVA_HOME)/jre/lib/amd64/server/"

	
subfolder: 
	cd graph && make
	cd agent && make 
	cd event && make
	cd plugin-loader && make
	#cd root && make
	
	
clean:
	cd agent && make clean
	cd plugin-loader && make clean
	cd graph && make clean
	cd event && make clean
	#cd root && make clean
	rm -rf main.o graphview socialsim
