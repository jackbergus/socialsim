/*
 * EventHandler.cpp
 * This file is part of socialsim
 *
 * Copyright (C) 2013 - Giacomo Bergami
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
 * along with socialsim; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */
 
 

#include <iostream>
#include <json/json.h>
#include "graph/Graph.h"
#include "graph/Vertex.h"
#include "agent/Actions.h"
#include "agent/Agent.h"
#include "event/Distribution.h"
#include "event/Event.h"
#include "event/EventGenerator.h"
#include "event/EventHandler.h"
#include "plugin-loader/DistrLoader.h"

/// PRIVATE

void	EventHandler::newEventGen(EventEnumType t, std::shared_ptr<Distribution> d) {
	std::shared_ptr<EventGenerator> eg{new EventGenerator(t,d)};
	m_map[t] = eg;
}

/// PUBLIC

EventHandler::EventHandler(std::string file, int seed) {
	DistrLoader *dl = DistrLoader::getInstance();
	PathHandler *pa = PathHandler::getInstance();
	std::string path = pa->getEvent(file);
#ifdef DEBUG
	std::cout << "FOUND: " << path << " for file " << file << std::endl;
#endif
	std::string cont = get_file_contents(path.c_str());
	json::Object o = json::Deserialize(cont);
		
	for (auto it : o) {
		//FIRST:  Name of the event
		//SECOND: distribution
		if (it.second.GetType() == json::ValueType::ObjectVal) {
			json::Object obj = it.second; //It contains the name and the arguments
			obj["seed"] = seed; //Sets the same seed to all the elements
			std::string file = obj["name"]; //name of the distribution
			std::shared_ptr<DistrCreator> build = dl->load(file);
			std::shared_ptr<Distribution> distr{build->creator(obj)};
			newEventGen(String_to_EventEnumType(it.first),distr);
		} else {  std::cout << "End" << std::endl; }
	}
};

std::shared_ptr<Event> EventHandler::next(EventEnumType t,Vertex* src, Vertex* dst){
	std::map<EventEnumType,std::shared_ptr<EventGenerator>>::iterator it = m_map.find(t);
	if (it == m_map.end()) {
		std::shared_ptr<Event> err{nullptr};
		return err;
	}
	std::shared_ptr<EventGenerator> distr = it->second;
	return distr->next(src,dst); 
}

/*

#include <iostream>
#include <memory>
#include "Exp.h"
#include "EventHandler.h"

int main(void) {
	EventHandler ea{};
	std::shared_ptr<Distribution> e1{new Exp(1)};
	std::shared_ptr<Distribution> e2{new Exp(1)};
	ea.newEventGen(EventEnumType::CREATE_NODE,e1);
	ea.newEventGen(EventEnumType::DELETE_NODE,e2);
	int i;
	for (i=0; i<5; i++) {
		std::cout << ea.next(EventEnumType::CREATE_NODE)->getTime() << std::endl;
	}
	for (i=0; i<5; i++) {
		std::cout << ea.next(EventEnumType::DELETE_NODE)->getTime() << std::endl;
	}
}

USAGE:

g++ -std=c++11 -c Exp.cpp
g++ -std=c++11 -c Event.cpp
g++ -std=c++11 -c EventGenerator.cpp
g++ -std=c++11 -c EventHandler.cpp
g++ -std=c++11 -c main.cpp
g++ -std=c++11 -o test Exp.o Event.o EventGenerator.o EventHandler.o main.o

*/
