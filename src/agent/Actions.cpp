/*
 * Actions.cpp
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

#include <stack>
#include "agent/Actions.h"
#include "agent/Agent.h"
#include "agent/Message.h"
#include "graph/Graph.h"
#include "graph/Vertex.h"
#include "graph/Edge.h"
#include "plugin-loader/PathHandler.h"
#include "agent/WholeMentalPlan.h"
 
extern "C" {
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
}

 
int Actions::act(int type, double reliability, double time,Agent* source) {
#ifdef DEBUG
	std::cout << "TYPE " << type << std::endl;
#endif
	if (!source) source = ag;
	
	Message msg{ag,source,type,reliability,time};
	
	if (!ptr) {
#ifdef DEBUG
		std::cerr << "Error: Action ptr is null: not sending" << std::endl;
#endif
		return -1;
	}
	
	if (!ptr->getGraph()) {
#ifdef DEBUG
		std::cerr << "Error: ptr-->Graph ptr is null: not sending" << std::endl;
#endif
		return -1;
	}
	
	////////////////////////////////////////////////////////////////////////
	//                       Visualizes the broadcast                     //
	////////////////////////////////////////////////////////////////////////
	//TODO:deactivate
	
	struct rusage r_start, r_end;
	memset((void*)&r_start,0,sizeof(struct rusage));
	memset((void*)&r_end,0,sizeof(struct rusage));
	getrusage(RUSAGE_SELF, &r_start);

#ifndef NOJAVA	
	if (ptr->getGraph()->isShowing()) {
		std::vector<int> adj{};
		for (Vertex* v: ptr->getAdjacency()) {
			adj.push_back(v->getId());
		}
		std::string messageToView{""};
		messageToView = "<Type=" + std::to_string(type) + ", opinion=" + std::to_string(reliability) + ">";

		int err = ptr->getGraph()->viewBroadcast(ag->getId(),adj,messageToView);
		if (!err) {
			std::cout << "Error visualizing the broadcast" << std::endl;
			exit(1);
		}
	}
#endif
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	
	int count = 0;

	for (Vertex* v: ptr->getAdjacency()) {
#ifdef DEBUG
		std::cout << "Adding element to node " << ptr->getGraph()->vid(v) << std::endl;
#endif
		//Sending the message to all the neighbours
		v->send(msg);
		count++;
#ifdef DEBUG
		std::cout << "its list message size is:" << v->ssize() << std::endl;
#endif
	}
	getrusage(RUSAGE_SELF, &r_end);
	double uSec = (r_end.ru_utime.tv_sec - r_start.ru_utime.tv_sec)*1000000.0;
	uSec += (r_end.ru_utime.tv_usec - r_start.ru_utime.tv_usec);
	if (uSec)
	std::cout << "(Send) CPU-uSec:" << uSec << std::endl;
	return (count*2);
}
		
Message Actions::percept() {
	Message nomsg{};
	if (!ptr) {
#ifdef DEBUG
		std::cerr << "Error: Action ptr is null" << std::endl;
#endif
		return nomsg;
	}
	return (ptr->recv());
}

