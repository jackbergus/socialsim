CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.plugin)
CC_FLAGS := -std=c++11 -I../../src -I../../ext-libs 
CC_JSONO := ../../ext-libs/json/json.cpp
CC_DEBUG := -DDEBUG $(CC_FLAGS)
OBJ := ../../ext-libs/boost/graphml.o
GRAPH := ../../src/graph/

all:
	echo "nothing to compile"

atest_graph:	graph
	g++ -std=c++11 $(CC_DEBUG) -c ../../src/plugin-loader/PathHandler.cpp	 -o phan.o
	g++ -std=c++11 $(CC_DEBUG) -c ../../src/agent/Message.cpp	 	 -o mesg.o
	g++ -std=c++11 $(CC_DEBUG) -c ../../src/agent/WholeMentalPlan.cpp	 -o wmpl.o
	g++ -std=c++11 $(CC_DEBUG) -c ../../src/agent/Agent.cpp	 		 -o agen.o
	g++ -std=c++11 $(CC_DEBUG) -c ../../src/agent/Actions.cpp	 	 -o acti.o
	g++ -std=c++11 $(CC_DEBUG) -c ../../src/plugin-loader/PlanLoader.cpp	 -o ploa.o		
	g++ -std=c++11 $(CC_DEBUG) -c test_graphnet.cxx
	g++ -std=c++11 $(CC_DEBUG) -ldl json.o phan.o ploa.o mesg.o wmpl.o agen.o Vertex.o Edge.o Graph.o $(OBJ) test_graphnet.o acti.o -o atest_graph 

debug:	atest_graph

clean:
	rm -f *.o
	rm -f *.plugin
	rm -rf atest_*
	

graph:	extlibs
	g++ $(CC_DEBUG) -I../ -c $(GRAPH)Vertex.cpp 
	g++ $(CC_DEBUG) -I../ -c $(GRAPH)Edge.cpp
	g++ $(CC_DEBUG) -I../ -c $(GRAPH)Graph.cpp

extlibs:
	g++ -std=c++11 $(CC_DEBUG) -c ../../ext-libs/json/json.cpp 		 -o json.o

	
