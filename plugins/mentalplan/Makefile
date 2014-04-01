CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.plugin)
CC_FLAGS := -g -std=c++11 -shared -fPIC -I../../src -I../../ext-libs
CC_JSONO := ../../ext-libs/json/json.cpp
OBJ := ../../ext-libs/boost/graphml.o

all:
	echo "nothing to compile"

out:	
	g++ -std=c++11 -g -c ../../ext-libs/json/json.cpp 		 -o json.o
	g++ -std=c++11 -g -c ../../src/plugin-loader/PathHandler.cpp	 -o phan.o -I../../src/
	g++ -std=c++11 -g -c ../../src/plugin-loader/PlanLoader.cpp	 -o ploa.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c ../../src/graph/Graph.cpp	 	 	 -o grap.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c ../../src/graph/Vertex.cpp	 	 	 -o vert.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c ../../src/graph/Edge.cpp	 	 	 -o edge.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c ../../src/agent/Agent.cpp	 	 	 -o agen.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c ../../src/agent/Actions.cpp	 	 	 -o acti.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c ../../src/agent/Message.cpp	 	 -o mesg.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c ../../src/agent/WholeMentalPlan.cpp	 -o wmpl.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c test_plan.cxx				 -I../../ext-libs -I../../src
	g++ -std=c++11 -g -ldl 	-o out $(OBJ) json.o phan.o ploa.o mesg.o wmpl.o grap.o edge.o vert.o agen.o acti.o test_plan.o 

debug:	out

clean:
	rm -f *.o
	rm -f *.plugin
	rm -f out

