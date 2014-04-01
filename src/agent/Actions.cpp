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
 
void Actions::act(int type, double reliability) {
	std::cout << "TYPE " << type << std::endl;
	Message msg{ag,type,reliability};
	
	if (!ptr) {
#ifdef DEBUG
		std::cerr << "Error: Action ptr is null: not sending" << std::endl;
#endif
		return;
	}
	
	if (!ptr->getGraph()) {
#ifdef DEBUG
		std::cerr << "Error: ptr-->Graph ptr is null: not sending" << std::endl;
#endif
		return;
	}
	
	////////////////////////////////////////////////////////////////////////
	//                       Visualizes the broadcast                     //
	////////////////////////////////////////////////////////////////////////
	std::vector<int> adj{};
	for (auto v: ptr->getGraph()->getAdjacency(*ptr)) {
		adj.push_back(ptr->getGraph()->vid(v));
	}
	std::string messageToView{""};
	messageToView = "<Type=" + std::to_string(type) + ", opinion=" + std::to_string(reliability) + ">";
	ptr->getGraph()->viewBroadcast(ag->getId(),adj,messageToView);
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	
	
	for (auto v: ptr->getGraph()->getAdjacency(*ptr)) {
#ifdef DEBUG
		std::cout << "Adding element to node " << ptr->getGraph()->vid(v) << std::endl;
#endif
		//Sending the message to all the neighbours
		(ptr->getGraph()->get_gboost_graph())[v].self->send(msg);
#ifdef DEBUG
		std::cout << "its list message size is:" << (ptr->getGraph()->get_gboost_graph())[v].self->ssize() << std::endl;
#endif
	}
}
		
Message Actions::percept() {
	Message nomsg{};
	if (!ptr) {
#ifdef DEBUG
		std::cerr << "Error: Action ptr is null" << std::endl;
#endif
		return nomsg;
	}
	
	if (!ptr->getGraph()) {
#ifdef DEBUG
		std::cerr << "Error: ptr-->Graph ptr is null" << std::endl;
#endif
	}
	
	return (ptr->getGraph()->get_gboost_graph())[ptr->get_gboost_vertex()].self->recv();
}

