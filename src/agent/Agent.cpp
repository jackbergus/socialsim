/*
 * Agent.cpp
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

 

#include "agent/Actions.h"
#include "agent/Agent.h"

json::Object* Agent::getState() { return &state; }
		
int Agent::getId() { return aid; }

int Agent::getAgentClassId() { return class_id; }

bool Agent::percept() {
	//gets a copy of a message
	// this method is "safe", since it'll be called only when a message is sheduled,
	//
	Message cpy = perform.percept(); 
	//Ignore the output
	reasoner.reason(&state, cpy);
	     //necessary typecast
	if (((bool)state["perform_broadcast"]) == true) {
		int type = state["type_send"];
		perform.act(type,state["opinion"][std::to_string(type)]);
		return true;
	}
	return false;
}

bool Agent::reason() {
	Message cpy{}; //Starts with subject 0 as default
	reasoner.reason(&state, cpy);
	     //necessary typecast
	if (((bool)state["perform_broadcast"]) == true) {
		int type = state["type_send"];
		perform.act(type,state["opinion"][std::to_string(type)]);
		return true;
	}
	return false;
}
