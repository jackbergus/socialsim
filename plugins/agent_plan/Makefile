CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.plugin)
CC_FLAGS := -g -std=c++11 -shared -fPIC -Wl,--export-dynamic -I../../src -I../../ext-libs

%.plugin:	%.cpp
	g++ $(CC_FLAGS) -o $@ $<
   
all:	$(OBJ_FILES)
	
clean:
	rm -f *.o
	rm -f *.plugin
	rm -f out
	
out:	all
	g++ -std=c++11 -g -c ../../ext-libs/json/json.cpp 		 -o json.o
	g++ -std=c++11 -g -c ../../src/plugin-loader/PathHandler.cpp	 -o phan.o -I../../src/
	g++ -std=c++11 -g -c ../../src/plugin-loader/PlanLoader.cpp	 -o ploa.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c ../../src/agent/Message.cpp	 	 -o mesg.o -I../../ext-libs -I../../src/
	g++ -std=c++11 -g -c test_dumb.cxx				 -I../../ext-libs -I../../src
	g++ -std=c++11 -g -ldl 	-o out json.o phan.o ploa.o mesg.o test_dumb.o

debug: 	out
