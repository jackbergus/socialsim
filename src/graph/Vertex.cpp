/*
 * Vertex.cpp
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
#include "graph/Vertex.h"
#include "graph/Graph.h"
#include "agent/Agent.h"
#include "agent/Message.h"


Vertex::Vertex(VProp* prop, vertex_descriptor desc,Graph* grap) {
	vp = prop;
	vp->self = this;
	vd = desc;
	g = grap;
}

vertex_descriptor Vertex::get_gboost_vertex() {
	return vd;
}

void Vertex::setName(std::string new_name) {
	if ((!vp)||(!g)) return;
	
	g->setName(new_name,this);
}

int Vertex::getId() {
	if ((!vp)||(!g)) return -1;
	
	graph_t gr = g->get_gboost_graph();
	return (gr)[(vd)].id;
}

std::shared_ptr<Agent> Vertex::getAgent() {
	if ((!vp)||(!g)) { std::shared_ptr<Agent> n{nullptr}; return n; }
	
	graph_t gr = g->get_gboost_graph();
	return (gr)[(vd)].agent_ptr;
}

std::string Vertex::getName() {
	if ((!vp)||(!g)) { std::string e{}; return e; }
	
	graph_t gr = g->get_gboost_graph();
	return (gr)[(vd)].name;
}

void Vertex::remove() {
	if ((!vp)||(!g)) {
		return;
	}
	Graph* tmp = g;
	vp = nullptr;
	g = nullptr;
	tmp->remove(this);
}

Graph* Vertex::getGraph() {
	return g;
}

void Vertex::send(Message msg) {
	incoming.push(msg);///????
}

Message Vertex::recv() {
	if (incoming.empty()) {
		Message e{};
		return e;
	}
	Message cp = incoming.top();
#ifdef DEBUG
	std::cout << "Extracted message type: " << cp.getType() << std::endl;
#endif
	incoming.pop();
	return cp;
}

int Vertex::ssize() {
	return incoming.size();
}
