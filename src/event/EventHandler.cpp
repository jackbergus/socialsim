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
		} else {  
#ifdef DEBUG
			std::cout << "End" << std::endl;
#endif
		}
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

double EventHandler::start(double var) {
	std::map<EventEnumType,std::shared_ptr<EventGenerator>>::iterator it = m_map.find(EventEnumType::NO_MORE_EVENTS);
	if (it == m_map.end()) 
		return 0;
	else
		return it->second->start(var);
}

