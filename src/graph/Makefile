## Compiler flags
arch = amd64
JAVA_HOME=$(shell readlink -f /usr/bin/javac | sed "s:bin/javac::")
INCLUDES =  -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux 
LIBS = -ljvm -L$(JAVA_HOME)/jre/lib/$(arch) -L$(JAVA_HOME)/jre/lib/$(arch) -L$(JAVA_HOME)/jre/lib/$(arch)/client/ -L$(JAVA_HOME)/jre/lib/$(arch)/server/ 

## Parser flags
OBJECT_NAME = GraphView
PACKAGE_NAME = graphview
GIWS_DESC_FILE = $(OBJECT_NAME).giws.xml
GIWS_OUT_FILES = $(addprefix $(PACKAGE_NAME),.cpp .hxx)

## TODO: 
CC_FLAGS := -std=c++11 -I../../src -I../../ext-libs 
CC_JSONO := ../../ext-libs/json/json.cpp
CC_DEBUG := $(CC_FLAGS) -g  -fPIC -flto
GPP := g++ -std=c++11
OBJ := ../../ext-libs/boost/graphml.o
GRAPH := ../../src/graph/

SRC=$(wildcard *.cpp) graphview.cpp
DST=$(SRC:.cpp=.o)

#compilazione dei file dia in pdf
%.o:		%.cpp
		@echo "    CC $<"
		@$(GPP) -fPIC -flto $(CC_DEBUG) $(INCLUDES) $(LIBS) -c $< -o $@
		

all:		java $(DST) 
	

test: $(GIWS_OUT_FILES)
	$(GPP) $(INCLUDES) $(LIBS) $(GIWS_OUT_FILES) test.cpp -o test
	@echo ""
	@echo "==========================================================================="
	@echo "Dont forget to set library path before running the example:"
	@echo "# export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(JAVA_HOME)/jre/lib/i386:$(JAVA_HOME)/jre/lib/i386/client/"
	@echo "or"
	@echo "# export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(JAVA_HOME)/jre/lib/amd64:$(JAVA_HOME)/jre/lib/amd64/server/"
	@echo "==========================================================================="

java: $(GIWS_OUT_FILES)
	mkdir -p ../$(PACKAGE_NAME)
	cp $(PACKAGE_NAME)/*.class ../$(PACKAGE_NAME)
	cp ../../ext-libs/graphstream/*.jar ../$(PACKAGE_NAME)
	

$(GIWS_OUT_FILES): $(PACKAGE_NAME)/$(GIWS_DESC_FILE).class
	python ../../ext-libs/javaparser/giws/giws -f GraphView.giws.xml -p
	javac -cp ../../ext-libs/graphstream/gs-algo-1.0.jar:../../ext-libs/graphstream/gs-core-1.2.jar:../../ext-libs/graphstream/gs-ui-1.2.jar  graphview/*.java
	

$(PACKAGE_NAME)/$(GIWS_DESC_FILE).class:
	java -jar ../../ext-libs/javaparser/JavaToXML.jar graphview/GraphView.java

clean:
	rm -rf $(GIWS_DESC_FILE) test graphview/*.class ../graphview/  *.o *~

	

