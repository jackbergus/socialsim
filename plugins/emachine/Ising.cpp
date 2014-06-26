/*
 * Ising.cpp
 * This file is part of socialsim
 *
 * Copyright (C) 2014 - Giacomo Bergami
 *
 * socialsim is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * socialsim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with socialsim. If not, see <http://www.gnu.org/licenses/>.
 */

#include "event_machine.h"
extern "C" {
	#include <unistd.h>
	#include <stdlib.h> 
}

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/random/uniform_int.hpp>
#include <sstream>

double s2d (std::string string_h) {
  double d;
  std::istringstream totalSString( string_h );
  totalSString >> d;
  std::cout << "EE" << std::endl;
  return d;
}

class Ising : public EventMachine {
	int vPos;
	protected:
	//XXX: CernOWFile log;
	std::string OPINION{"opinion"};
	std::string ARGUM{"0"};
	std::ofstream log2;
	//std::ofstream log3;
	boost::mt19937 mersenne;
	boost::mt19937 mersenne1;
	boost::uniform_real<> dist{0,1};
	std::list<int> noaddNodes;
	boost::variate_generator<boost::mt19937&, boost::uniform_real<> > dice;
	boost::variate_generator<boost::mt19937&, boost::uniform_real<> > dice1;
	bool same_event;
	int current_node = 02;
	boost::mt19937 mersenne2;
	boost::normal_distribution<> nd;
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > ndist;
		
		void dump() {
			//CernRecord cr{"riga"};
			//The time variable
			log2 << getSimTime() ;
			//log3 << getSimTime();
			
			//The agent's opinion over the topic. In this case we have only one subject
			for (Agent* a : graph.Agents()) {
				log2 << " " << Plan::getOpinionType(a->getState(),0);
			} 
			
			/*for (Agent* a : graph.Agents()) {
				for (Agent* b : graph.Agents()) {
					log3 << " " << Plan::getSenderReputation(a->getState(),b->getId());
				} 
			} */
			
			log2 << " " << current_node;
			log2 << std::endl;
			//log3 << std::endl;
			//Saves row to file
			
		}
		
		
		void close() { 
			std::cout << "CLOSING FILE" << std::endl;
			log2.close(); 
			//log3.close();
		}
		
		
		void init_header() {
			//CernHeader ch;
			//The time variable
			//ch << "time ";
			//The agent's opinion over the topic. In this case we have only one subject
			std::string column;
			log2 << "time";
			//log3 << "time";
			for (Agent* a : graph.Agents()) {
				column = " O(" + std::to_string(a->getId()) + ")";
				log2 << column;
			} 

			for (Agent* a : graph.Agents()) {
				for (Agent* b : graph.Agents()) {
					column = " R(" + std::to_string(a->getId()) + "," + std::to_string(b->getId()) + ")";
					//log3 << column;
				} 
			} 
			//std::cout << "CH SIZE: " << ch.size() << std::endl;
			//log2.reopenWrite(log_filename,ch);
			log2 << " Uid";
			log2 << std::endl;
			//log3 << std::endl;
			
		}
		
		int processEvent(std::shared_ptr<Event> e) {
			bool did_broadcast; 
			if (e->getType() == EventEnumType::NO_EVENT) {
				std::cout << "No event for #" << e->getDestin()->getId() << std::endl;
				did_broadcast = e->getDestin()->getAgent()->reason(ndist());
			} else if (e->getType() == EventEnumType::MESSAGE_SENT) {
				std::cout << "Message Sent from #" << e->getDestin()->getId() << std::endl;
				did_broadcast = e->getDestin()->getAgent()->percept(ndist());
			} 
			if (did_broadcast) {	
				std::list<Vertex*> vl;
				current_node = e->getDestin()->getId();
				
				for (Vertex* v: e->getDestin()->getAdjacency()) {
					std::cout << "Message Sent from #" << e->getDestin()->getId() << " to #" << v->getId() << std::endl;
					vl.push_back(v);
				}
				
				if (same_event)
					addEventIncr(EventEnumType::MESSAGE_SENT,e->getDestin(),vl);
				else 
					addEventIncrDiff(EventEnumType::MESSAGE_SENT,e->getDestin(),vl);
			} else
				std::cout << "Sending no message" << std::endl;
			return 1;
		}

	public: Ising(json::Object conf, int seed, int vertexpos) 
		: EventMachine(conf,seed,vertexpos,true,true,100),
		   vPos{vertexpos},
		   log2{"OpinionOpinionSeed"+std::to_string(((int)conf["opinionSeed"]))+log_filename},
		   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		   //first generator ~ opinion
		   mersenne{(unsigned int)((int)conf["opinionSeed"])},
		   dice{mersenne, dist},
		   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		   //second generator ~ other
		   mersenne1{0},
		   dice1{mersenne1,dist},
		   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		   //normal distribution generator 
		   // a. getting generator (+10 jump)
		   mersenne2{0},
		   // b. getting normal distribution with sigma as parameter
		   nd{0.0,s2d((std::string)conf["SIGMA"])},
		   // c. obtaining the desired distribution
		   ndist{mersenne2,nd} 
		   
		   {
			
			if (conf.find("opinionSeed")==conf.end()) {
				std::cerr << "No 'opinionSeed' found" << std::endl;
				exit(1);
			}
			
			if (conf.find("sameEvent")==conf.end()) {
				std::cerr << "No 'opinionSeed' found" << std::endl;
				exit(1);
			} else 
				same_event = (((int)conf["sameEvent"]) == 1 ? true : false);
			std::cout << "sameEvent: " << same_event << std::endl;
			
			//Using the sigma for the agent error factor
			
			if (conf.find("SIGMA")==conf.end()) {
				std::cerr << "No sigma found" << std::endl;
				exit(1);
			} /*else 
				//The standard function doesn't seem to work
				SIGMA_VAL = s2d((std::string)conf["SIGMA"]);
			std::cout << "SIGMA: " << (double)SIGMA_VAL << std::endl;*/
			
			std::string setting;
			if (conf.find("setting")==conf.end()) {
				std::cerr << "No setting found" << std::endl;
				exit(1);
			} else 
				setting = (std::string)conf["setting"];
			std::cout << "SETTING: " << setting << std::endl;
			
			//Starting simulation
			for (Vertex* vx: graph.Vertices()) {
				if (vx->getId()==vertexpos) {
					//Phony self-message for a start
					addEventIncr(EventEnumType::NO_EVENT,vx,vx);
					break;
				}
			}
			
			//2000...
			if (setting.compare("1")==0) {
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
				
					//Initializing the reputation values
					for (int i=0; i<graph.nVertices(); i++) {
						(*obj)[std::to_string(i)] = (int)1;
					}
				
					(*obj)["plagiability"] = 1;
				
					//Different opinions
					(*obj)[OPINION][ARGUM] = (int)(a->getId() == vertexpos ? 1 : 0);
				
				}
			} else if (setting.compare("2")==0) {
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
				
					//Initializing the reputation values
					for (int i=0; i<graph.nVertices(); i++) {
						(*obj)[std::to_string(i)] = (double)0.5;
					}
				
					(*obj)["plagiability"] = 1;
				
					//Different opinions
					(*obj)[OPINION][ARGUM] = (int)(a->getId() == vertexpos ? 1 : 0);
				
				}
			} else if (setting.compare("3")==0) {
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
				
					//Initializing the reputation values
					for (int i=0; i<graph.nVertices(); i++) {
						(*obj)[std::to_string(i)] = (double)1;
					}
				
					(*obj)["plagiability"] = 1;
				
					//Different opinions
					(*obj)[OPINION][ARGUM] = (double)(a->getId() == vertexpos ? 1 : 0.5);
				
				}
			//5000...
			} else if (setting.compare("4")==0) {
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
				
					//Initializing the reputation values
					for (int i=0; i<graph.nVertices(); i++) {
						(*obj)[std::to_string(i)] = 1;
					}
				
					(*obj)["plagiability"] = 1;
				
					//Different opinions
					(*obj)[OPINION][ARGUM] = (double)dice();
				
				}
			} else if (setting.compare("5")==0) {
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
				
					//Initializing the reputation values
					for (int i=0; i<graph.nVertices(); i++) {
						(*obj)[std::to_string(i)] = 1;
					}
				
					//Different opinions
					(*obj)[OPINION][ARGUM] = (double)dice1();
				
				}
				
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
					(*obj)["plagiability"] = (double)dice();
				}
			} else if (setting.compare("6")==0) {
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
				
					//Initializing the reputation values
					for (int i=0; i<graph.nVertices(); i++) {
						(*obj)[std::to_string(i)] = (double)dice();
					}
				}
				
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
					//Different opinions
					(*obj)[OPINION][ARGUM] = (double)dice1();
				}
				
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
					(*obj)["plagiability"] = (double)dice1();
				}

			} else if (setting.compare("7")==0) {
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
				
					//Initializing the reputation values
					for (int i=0; i<graph.nVertices(); i++) {
						(*obj)[std::to_string(i)] = (int)1;
					}
				
					(*obj)["plagiability"] = (double)dice();
				
					//Different opinions
					(*obj)[OPINION][ARGUM] = (int)(a->getId() == vertexpos ? 1 : 0);
				
				}
				
			} else if (setting.compare("8")==0) {
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
				
					//Initializing the reputation values
					for (int i=0; i<graph.nVertices(); i++) {
						(*obj)[std::to_string(i)] = (double)dice();
					}
				
					//Different opinions
					(*obj)[OPINION][ARGUM] = (double)dice1();
				
				}
				
				for (Agent* a : graph.Agents()) {
					json::Object *obj = a->getState();
					(*obj)["plagiability"] = 1;
				}
				
			} else {
				std::cerr << "No 'setting' found" << std::endl;
				exit(1);
			}

			
			
			
		}
		
		Ising(json::Object c) : Ising(c["conf"], c["seed"], c["vertexpos"]) {};
};

class ICreator : public EMCreator {
	public:
		ICreator() {};
		Ising* creator(json::Object o) {	//Accesso ad un oggetto json come parametro
			return new Ising(o);	//Inizializzazione dell'oggetto
		}
};

MAKE_PLUGIN(ICreator,EMCreator);
