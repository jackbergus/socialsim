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

extern "C" {
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
}

json::Object* Agent::getState() { return &state; }
		
int Agent::getId() { return aid; }

int Agent::getAgentClassId() { return class_id; }

bool Agent::percept(double time) {
	//gets a copy of a message
	// this method is "safe", since it'll be called only when a message is sheduled,
	//
	Message cpy = perform.percept(); 
	bool toret = false;
	//Ignore the output
	
	struct rusage r_start, r_end, a_start, a_end;
	memset((void*)&r_start,0,sizeof(struct rusage));
	memset((void*)&r_end,0,sizeof(struct rusage));
	memset((void*)&a_start,0,sizeof(struct rusage));
	memset((void*)&a_end,0,sizeof(struct rusage));
	
	getrusage(RUSAGE_SELF, &r_start);
	reasoner.reason(&state, cpy, time);
	getrusage(RUSAGE_SELF, &a_start);
	
	     //necessary typecast
	     
	if (((bool)state["perform_broadcast"])) {
		int type = state["type_send"];
		getrusage(RUSAGE_SELF, &a_end);
		if ((bool)state["doshare"])
			perform.act(type,state["shared"][std::to_string(type)],time,cpy.getAgentSender());
		else 
			perform.act(type,state["opinion"][std::to_string(type)],time,nullptr);
		getrusage(RUSAGE_SELF, &r_end);
		toret = true;
	}
	getrusage(RUSAGE_SELF, &r_end);
	if (!toret)
	getrusage(RUSAGE_SELF, &a_end);
	
	double uSec = (r_end.ru_utime.tv_sec - r_start.ru_utime.tv_sec)*1000000.0;
	uSec += (r_end.ru_utime.tv_usec - r_start.ru_utime.tv_usec);
	if (uSec!=0)
	std::cout << "(Percept) CPU-uSec:" << uSec << std::endl;
	uSec = (a_end.ru_utime.tv_sec - a_start.ru_utime.tv_sec)*1000000.0;
	uSec += (a_end.ru_utime.tv_usec - a_start.ru_utime.tv_usec);
	if (uSec!=0)
	std::cout << "(Nothing in Percept: " << toret << ") CPU-uSec:" << uSec << std::endl;

	
	return toret;
}

bool Agent::reason(double time) {
	Message cpy{}; //Starts with subject 0 as default
	
	struct rusage r_start, r_end, a_start, a_end;
	memset((void*)&r_start,0,sizeof(struct rusage));
	memset((void*)&r_end,0,sizeof(struct rusage));
	
	getrusage(RUSAGE_SELF, &r_start);
	reasoner.reason(&state, cpy,time);
	
	bool toret = false;
	     //necessary typecast
	if (((bool)state["perform_broadcast"])) {
		int type = state["type_send"];
		perform.act(type,state["opinion"][std::to_string(type)],time,nullptr);
		toret= true;
	}
	
	getrusage(RUSAGE_SELF, &r_end);
	
	double uSec = (r_end.ru_utime.tv_sec - r_start.ru_utime.tv_sec)*1000000.0;
	uSec += (r_end.ru_utime.tv_usec - r_start.ru_utime.tv_usec);
	if (uSec!=0)
	std::cout << "(Reason) CPU-uSec:" << uSec << std::endl;
	
	return toret;
}
