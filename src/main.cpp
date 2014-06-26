
#include <thread>
#include <vector>
#include <iostream>
#include <cassert>
#include "agent_plan.h"
#include "event_distributions.h"
#include "event_machine.h"
#include "plugin-loader/EMachineLoader.h"

#include <TPluginManager.h>
#include <TRint.h> // Initialization 
#include <TROOT.h> // Global setup

/** probThread:
 * Executes a simulation task
 * @arg pos: if (pos==-1), then the program hasn't been created with the threading
 */ 
int probThread(int argc, char* argv[], int pos) {

	std::cout << "Loading the default Paths..." << std::endl << std::endl;
	
	//Using the default paths
	PathHandler *ph = PathHandler::initInstance("../plugins/emachine","../plugins/event","../plugins/event_distributions","../plugins/agent","../plugins/mentalplan","../plugins/agent_plan","../plugins/networks","../plugins/css");
	
	std::string content;
	if (argc<2)
		content = get_file_contents("configuration.json");
	else
		content = get_file_contents(argv[1]);
		
	json::Object j = json::Deserialize(content);
	
	//Changes the seed if used in not random mode 
	if (pos != -1) 
		j["seed"] = pos;
	
	json::Object conf = j["conf"];
	
	//If the project is in thread mode, I shall disable the graphical interface
	if (pos != -1) {
		conf["debug"] = 1;
	}
	
	std::string machname = conf["EventMachine"];
	
	std::cout << "Loading the EventMachine " << machname << std::endl ;
	std::shared_ptr<EMCreator> isingc =EMachineLoader::getInstance()->load(machname);
	EventMachine* ising = isingc->creator(j);// Create the ising model with the given parameters
	
	std::cout << "Starting" << std::endl << std::endl ;
	
	int event_update;
	if (pos != -1)
		event_update = 0;
	else 
		event_update = j["EventUpdate"];
		
	ising->setEventUpdateTime(event_update);
	
	std::string title = j["Title"];
	if (j.find("stopAt") == j.end()) {
		if (j.find("stopAtEvent") == j.end())
			//Starts event loop
			ising->start(title);	
		else {
			//Starts event loop, and stops after stop events
			int stop = j["stopAtEvent"];
			ising->start(title,(unsigned long int)stop); 
		}
	} else {
		double stop = j["stopAt"];
		//Starts event loop, and stops after stop time
		ising->start(title,stop);	
	}
	
	if (pos!=-1) {
		JVM::detach();
	}

}

void root_init(const char* library, const char*package) {
	std::string tmp{library};
	tmp = tmp + "()";
	gROOT->GetPluginManager()->AddHandler(library, "*",library,package,tmp.c_str()); 
}

int main(int argc, char* argv[]){

#ifndef NOJAVA
	JVM::getInstance(); //Initializes java in the main thread
#endif

	//root, such a @#!/-
	//Why do I have to do this, tell me, why... (http://youtu.be/HG7I4oniOyA)
	//XXX: gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo", "*","TStreamerInfo","RIO","TStreamerInfo()"); 
	//XXX: root_init("TStreamerInfo","RIO");
	//XXX: root_init("TKey","RIO");
	//XXX: root_init("TList","CORE");
	//XXX: root_init("TExMap","CONT"); //Bufferfile DirectoryFile
	//TODO: Activate Root preprocessing only in debug mode (1 simulation)

	//Straightforward: checks if the first argument is a number.
	if (argc<=2) {
		probThread(argc,argv,-1);
	} else {
		std::string num{argv[2]};
		std::string::size_type sz; 
		int nthread = std::stoi (num,&sz);
		if (nthread <= 1) {
			probThread(argc,argv,-1);
		} else {
			//Starts the threading simulation
			std::vector<std::thread> threads;

    			for(int i = 1; i <= nthread; ++i){
    				threads.push_back(std::thread(probThread,argc,argv,i-1));
    			}
    			
    			//Waits for each child to terminate
    			for(auto& thread : threads){
        			thread.join();
    			}
    			
		}
	}

	return 0;

}
