/*
 * Edge.cpp
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

#include "graph/Edge.h"
#include "graph/Graph.h"
#include "agent/Message.h"
#include "agent/Actions.h"

Edge::Edge(EProp* prop, edge_descriptor desc,Graph* grap) {
	vp = prop;
	vp->self = this;
	vd = desc;
	g = grap;
}

edge_descriptor Edge::get_gboost_edge() {
	return vd;
}

/*
void Edge::setName(std::string new_name) {
	if ((!vp)||(!g)) return;
	
	g->setName(new_name,this);
}

std::string Edge::getName() {
	if ((!vp)||(!g)) { std::string e{}; return e; }
	
	graph_t gr = g->get_gboost_graph();
	return (gr)[(vd)].name;
}
*/

void Edge::remove() {
	if ((!vp)||(!g)) {
		return;
	}
	Graph* tmp = g;
	vp = nullptr;
	g = nullptr;
	tmp->remove(this);
}
