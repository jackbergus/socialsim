#include <iostream>
#include "agent_plan.h"
#include "event_distributions.h"
#include "event_machine.h"
#include "plugin-loader/EMachineLoader.h"

int main(int argc, char* argv[]){
	std::cout << "Loading the default Paths..." << std::endl << std::endl;
	
	//Using the default paths
	PathHandler *ph = PathHandler::initInstance("../plugins/emachine","../plugins/event","../plugins/event_distributions","../plugins/agent","../plugins/mentalplan","../plugins/agent_plan","../plugins/networks","../plugins/css");
	
	std::cout << "Loading the EventMachine" << std::endl << std::endl ;
	std::shared_ptr<EMCreator> isingc =EMachineLoader::getInstance()->load("Ising.plugin");
	
	json::Object j;
	json::Object eaO;
	eaO["event"] = "Ising.json"; 		// The configuration of the simulation
	eaO["graph"] = "lattice_5x6.xml"; 	// The network to use (and also the agents) 
	j["seed"] = 1; 				// EXAMPLE
	j["vertexpos"] = 12; 			// The node that starts the overall computation
	j["conf"] = eaO;			// Setting the configuration
	
	EventMachine* ising = isingc->creator(j);// Create the ising model with the given parameters
	std::cout << "Starting" << std::endl << std::endl ;
	
	ising->setEventUpdateTime(400); //Sets the update time of the events, in milliseconds
	ising->start("Modello di Ising",30);//Starts event loop, and stops after 30s
	//ising->start("Modello di Ising");//Starts event loop
	
}
