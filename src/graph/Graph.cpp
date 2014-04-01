/*
 * Graph.cpp
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

#include <memory>
#include "agent/Actions.h"
#include "graph/Graph.h"
#include "graph/Vertex.h"
#include "graph/Edge.h"
#include "plugin-loader/PathHandler.h"
#include "agent/WholeMentalPlan.h"
#include "agent/Agent.h"
#include "agent/Actions.h"

Graph::~Graph() {
	BGL_FORALL_VERTICES( v, g, graph_t )    {
		if (g[v].loaded) //Removes the allocated vertices
			delete g[v].self;
	}
}

void Graph::init_extern(bool withname) {
	v_id = 0;
	//int v_size = num_vertices(g);
	e_id = 0;
	PathHandler *ph = PathHandler::getInstance();//NOTE: Singleton. No Delete needed
	
	BGL_FORALL_VERTICES( v, g, graph_t )    {
		g[v].id = v_id++;
#ifndef NOJAVA
		gv.addNode(g[v].id);
#endif

		if (withname) {
			//init agents
			std::string agent_json_file = ph->getAgent(g[v].name);
			std::string content = get_file_contents(agent_json_file.c_str());
			json::Object obj = json::Deserialize(content);
			//for (int i=0;i<v_size; i++) {
			//	obj["state"][std::to_string(i)] = 0.0;
			//}
			g[v].self = new Vertex(&g[v],v,this); // XXX
			std::shared_ptr<Agent> self_ag{new Agent(obj,g[v].self)};
			g[v].agent_ptr = self_ag; //Passing the reference
			g[v].loaded = true;
			
		}
		
		
	}
	
	BGL_FORALL_EDGES( e, g, graph_t )    {
		g[e].id = e_id++;
#ifndef NOJAVA
		gv.addEdge(g[e.m_source].id,g[e.m_target].id); //TODO: id is int
#endif
	}
}

Graph::Graph(std::string filename) 
#ifndef NOJAVA
: gv{JVM::getInstance()} 
#endif
{
	boost::dynamic_properties dp;
	dp.property("vertex_name",get(&VProp::name,g));
	dp.property("vertex_cmdb_id",get(&VProp::id,g));
	dp.property("edge_id",get(&EProp::id,g));
	std::ifstream fin{filename.c_str()};
	read_graphml(fin, g, dp);
	
	init_extern(true);
	
}

Vertex Graph::addVertex(std::string label, Agent* a) {
	VProp vp;
	vp.name = label;
	vp.id = v_id++;
	vp.loaded = false;
	if ((a!=nullptr)&&(a!=NULL)) {
		std::shared_ptr<Agent> apt{a};
		vp.agent_ptr = std::move(apt);
	}
	vertex_descriptor v0 = boost::add_vertex(g);
	g[v0] = vp;
	Vertex v{&vp,v0,this}; //updates vp pointer to vertex
	return v;
}

Vertex Graph::addVertex(std::string label) {
	return addVertex(label,nullptr);
}

Edge Graph::addEdge(Vertex src, Vertex dst/*, std::string label*/) {
	EProp ep;
	ep.id = e_id++;
	/* ep.name = label; */
	edge_descriptor e0 = boost::add_edge(src.get_gboost_vertex(),dst.get_gboost_vertex(),ep,g).first;
	Edge e{&ep,e0,this};
	return e;
}


void Graph::write(std::ostream out_stream) {
	boost::dynamic_properties dp;
	dp.property("vertex_name",get(&VProp::name,g));
	dp.property("vertex_cmdb_id",get(&VProp::id,g));
	dp.property("edge_id",get(&EProp::id,g));
	write_graphml(out_stream, g, dp);
}
	
void Graph::std_out_write() {
	boost::dynamic_properties dp;
	dp.property("vertex_name",get(&VProp::name,g));
	dp.property("vertex_cmdb_id",get(&VProp::id,g));
	dp.property("edge_id",get(&EProp::id,g));
	write_graphml(std::cout, g, dp);
}
	
void Graph::setName(std::string label, Vertex* v) {
	g[v->get_gboost_vertex()].name = label;
}

	
void Graph::remove(Vertex* v) {
	if (!v) return;
	boost::remove_vertex(v->get_gboost_vertex(),g);
	v->remove();
	//only if the objecy was dynamically allocated
	if (g[v->get_gboost_vertex()].loaded) {
		delete v;
	}
}	
	
void Graph::remove(Edge* v) {
	if (!v) return;
	boost::remove_edge(v->get_gboost_edge(),g);
	v->remove();
}

/*GraphVertexIterator Graph::Vertices() {
	std::pair<GVIt,GVIt> vxit = vertices(g);
	GraphVertexIterator gvit{vxit.first,vxit.second};
	return gvit;
}*/

int Graph::nVertices() {
	return num_vertices(g);
}

AgentIterable Graph::Agents() {
	std::pair<GVIt,GVIt> vxit = vertices(g);
	AgentIterable gvit{vxit.first,vxit.second,&g};
	return gvit;
}

VertexIterable Graph::Vertices() {
	std::pair<GVIt,GVIt> vxit = vertices(g);
	VertexIterable gvit{vxit.first,vxit.second,&g};
	return gvit;
}

GraphAdjIterator Graph::getAdjacency(Vertex v) {
	std::pair<GAIt,GAIt> vxit = adjacent_vertices(v.get_gboost_vertex(), g);
	GraphAdjIterator gvit{vxit.first,vxit.second};
	return gvit;
}

GraphAdjIterator Graph::getAdjacency(Vertex* v) {
	std::pair<GAIt,GAIt> vxit = adjacent_vertices(v->get_gboost_vertex(), g);
	GraphAdjIterator gvit{vxit.first,vxit.second};
	return gvit;
}

bool Graph::hasEdge(Vertex src, Vertex dst) {
	for (auto i : getAdjacency(src)) {
		if ((i)==dst.get_gboost_vertex())
			return true;
	}
	return false;
}

void Graph::show(std::string title) {
#ifndef NOJAVA
	gv.show(title.c_str());
#endif
}

void Graph::close() {
#ifndef NOJAVA
	gv.close();
#endif
}

void Graph::hide() {
#ifndef NOJAVA
	gv.hide();
#endif
}

void Graph::setEventUpdateTime(double t) {
	if (t>0) event_update_time = t;
}

double Graph::getEventUpdateTime() {
	return event_update_time;
}

void Graph::viewBroadcast(int src, std::vector<int> dst, std::string message) {
	std::cout << event_update_time << std::endl;
#ifndef NOJAVA
	gv.sendBroadcast_with_stop(src,dst.data(),dst.size(),event_update_time,message.c_str());
#endif
}

std::shared_ptr<Agent> Graph::acast(vertex_descriptor vic) {
	return g[vic].agent_ptr;
}

Vertex* Graph::vcast(vertex_descriptor vic) {
	return g[vic].self;
}

int Graph::vid(vertex_descriptor vic) {
	return g[vic].id;
}
	
graph_t Graph::get_gboost_graph() {
	return g;
}

