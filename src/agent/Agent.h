/*
 * Agent.h
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



#pragma once
#include <json/json.h>
#include "WholeMentalPlan.h"
#include "graph/Vertex.h"

class Actions;

/** 
 * The user of the social network
 */
class Agent {

	private:
		
		WholeMentalPlan reasoner;	/**< the inside reasoner of the subject */
		json::Object    state;		/**< the graph inner state */
		int 		aid;		/**< the id defining the vertex position of the agent */
		int 		class_id;	/**< the id defining the initial state of the agent */
		Actions         perform;	/**< the collection of actions to be performed */
	
	public:
		
		Agent(int id, std::string mentalplan_file, std::string state_file, int clsid) 
			: reasoner{mentalplan_file}, aid{id}, class_id{clsid}
		{
			std::string content = get_file_contents(state_file.c_str());
			json::Object state2 = json::Deserialize(content);
		}
		
		Agent(int id, std::string mentalplan_file, int clsid) 
			: reasoner{mentalplan_file}, aid{id}, class_id{clsid}
		{
			std::string empty = "{}";
			state = json::Deserialize(empty);
		}
		
		Agent(int id, json::Object init) 
			: reasoner{init["mentalplan"]}, state{init["state"]}, class_id{init["aid"]}, aid{id} {

		}
		
		Agent(json::Object init,Vertex* ptr) 
			: reasoner{init["mentalplan"]}, 
			  state{init["state"]}, 
			  class_id{init["aid"]},
			  aid{ptr->getId()}
		{
			Actions ac{ptr,this}; //To state that here, you have to create the object first
			perform = ac;
		}
		
		json::Object* getState();
		int getId();
		int getAgentClassId();
		
		// Returns true if it sent the broadcast message;
		bool percept(double time);
		bool reason(double time);

};
