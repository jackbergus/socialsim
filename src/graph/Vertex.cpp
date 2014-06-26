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

//uses the data structure as a stack
Vertex::Vertex(vertex_descriptor desc,Graph* grap) : incoming{true} {
	vd = desc;
	g = grap;
}

Vertex::Vertex(vertex_descriptor desc,Graph* grap, bool isstack) : incoming{isstack} {
	vd = desc;
	g = grap;
}

Vertex::Vertex(vertex_descriptor desc,Graph* grap, bool isstack, int lim) : incoming{isstack,lim} {
	vd = desc;
	g = grap;
}

vertex_descriptor Vertex::get_gboost_vertex() {
	return vd;
}

void Vertex::setName(std::string new_name) {
	if ((!g)) return;
	
	g->setName(new_name,this);
}

int Vertex::getId() {
	if ((!g)) return -1;
	
	graph_t gr = g->get_gboost_graph();
	return (gr)[(vd)].id;
}

std::shared_ptr<Agent> Vertex::getAgent() {
	if ((!g)) { std::shared_ptr<Agent> n{nullptr}; return n; }
	
	graph_t gr = g->get_gboost_graph();
	return (gr)[(vd)].agent_ptr;
}

std::string Vertex::getName() {
	if ((!g)) { std::string e{}; return e; }
	
	graph_t gr = g->get_gboost_graph();
	return (gr)[(vd)].name;
}

AdjacentIterable Vertex::getAdjacency() {
	return g->getAdjacency(this);
}

int Vertex::getDegree() {
	int i = 0;
	for (auto x:getAdjacency()) {
		i++;
	}
	return i;
}

Vertex* Vertex::rndAdjacent(boost::variate_generator<boost::mt19937&, boost::uniform_real<> >& dice) {
	std::vector<Vertex*> lAdj;
	for (Vertex *v: getAdjacency())
		lAdj.push_back(v);
	int pos = ceil(dice()*(lAdj.size()))-1;
#ifdef DEBUG
	std::cout << "POS RAND: " << pos << " of " << lAdj.size() << std::endl;
#endif
	return lAdj[pos];
}

void Vertex::remove() {
	if ((!g)) {
		return;
	}
	Graph* tmp = g;
	g = nullptr;
	tmp->remove(this);
}

Graph* Vertex::getGraph() {
	return g;
}

void Vertex::send(Message msg) {
	incoming.put(msg);///????
}

Message Vertex::recv() {
	if (incoming.isEmpty()) {
		Message e{};
		return e;
	}
	Message cp = incoming.get();
#ifdef DEBUG
	std::cout << "Extracted message type: " << cp.getType() << std::endl;
#endif
	return cp;
}

int Vertex::ssize() {
	return incoming.getSize();
}
