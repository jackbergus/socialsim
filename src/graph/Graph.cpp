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

bool Graph::isShowing() {
 return (is_showed && (event_update_time>0)); 
}

void Graph::init_extern(bool withname) {
	v_id = 0;
	//int v_size = num_vertices(g);
	e_id = 0;
	PathHandler *ph = PathHandler::getInstance();//NOTE: Singleton. No Delete needed
	
	BGL_FORALL_VERTICES( v, g, graph_t )    {
		g[v].id = v_id++;
#ifndef NOJAVA
		int err = gv.addNode(g[v].id);
		if (err==-1) {
			std::cout << "Error while adding " << g[v].id << " node" << std::endl;
			exit(1);
		} 
#ifdef DEBUG
		else std::cout << g[v].id << std::endl;
#endif
#endif

		if (withname) {
			//init agents
			std::string agent_json_file = ph->getAgent(g[v].name);
			std::string content = get_file_contents(agent_json_file.c_str());
			json::Object obj = json::Deserialize(content);
			//for (int i=0;i<v_size; i++) {
			//	obj["state"][std::to_string(i)] = 0.0;
			//}
			if (!structure_has_limit)
				g[v].self = new Vertex(v,this,structure_is_stack);
			else
				g[v].self = new Vertex(v,this,structure_is_stack,limit);
			//g[v].self = new Vertex(v,this); // XXX
			std::shared_ptr<Agent> self_ag{new Agent(obj,g[v].self)};
			g[v].agent_ptr = self_ag; //Passing the reference
			g[v].loaded = true;
			
		}
		
		
	}
	
	BGL_FORALL_EDGES( e, g, graph_t )    {
		g[e].id = e_id++;
#ifndef NOJAVA
		int err =  gv.addEdge(g[e.m_source].id,g[e.m_target].id); //TODO: id is int
		if (err==-1) {
			std::cout << "Error while adding an edge" << std::endl;
			exit(1);
		}
#endif
		if (withname) {
			std::shared_ptr<Edge> foo{new Edge(&g[e],e,this)};
			g[e].self = std::move(foo);
		}
	}
}

Graph::Graph(std::string filename) : 
#ifndef NOJAVA	
	gv{JVM::getInstance()},
#endif
 structure_has_limit{false}, structure_is_stack{true}, limit{100}
{
	if (filename.length()==0) return;
	boost::dynamic_properties dp;
	dp.property("vertex_name",get(&VProp::name,g));
	dp.property("vertex_cmdb_id",get(&VProp::id,g));
	dp.property("edge_id",get(&EProp::id,g));
	std::ifstream fin{filename.c_str()};
	read_graphml(fin, g, dp);
	
	init_extern(true);
	
}

Vertex* Graph::addVertex(std::string label, json::Object init) {
	VProp vp;
	vp.name = label;
	vp.id = v_id++;
	vp.loaded = false;
	int test = boost::num_vertices(g);
	vertex_descriptor v0 = boost::add_vertex(g);
	assert(test < boost::num_vertices(g));
	test = boost::num_vertices(g);
	//std::cout << "NUMVERT2: " << test << std::endl;
	g[v0] = vp;
	Vertex *v;
	if (!structure_has_limit)
		v = new Vertex(v0,this,structure_is_stack);
	else
		v = new Vertex(v0,this,structure_is_stack,limit);
	std::shared_ptr<Agent> apt{new Agent(init,v)};
	g[v0].agent_ptr = std::move(apt);
	g[v0].self = v; //updates vp pointer to vertex
#ifndef NOJAVA
	if (isShowing()) {
		int err = gv.addNode(vp.id);
		if (err==-1) {
			std::cout << "Error while adding a node" << std::endl;
			exit(1);
		}
	}
#endif
	return v;
}

Vertex* Graph::addVertex(std::string label, Agent* a) {
	VProp vp;
	vp.name = label;
	vp.id = v_id++;
	vp.loaded = false;
	int test = boost::num_vertices(g);
	vertex_descriptor v0 = boost::add_vertex(g);
	assert(test < boost::num_vertices(g));
	g[v0] = vp;
	Vertex *v;
	if (!structure_has_limit)
		v = new Vertex(v0,this,structure_is_stack);
	else
		v = new Vertex(v0,this,structure_is_stack,limit);
	if ((a!=nullptr)&&(a!=NULL)) {
		std::shared_ptr<Agent> apt{a};
		vp.agent_ptr = std::move(apt);
		g[v0].self = v;
	}
	//updates vp pointer to vertex
#ifndef NOJAVA
	if (isShowing()) {
		int err = gv.addNode(vp.id);
		if (err==-1) {
			std::cout << "Error while adding a node" << std::endl;
			exit(1);
		}
	}
#endif
	return v;
}

Vertex* Graph::addVertex(std::string label) {
	return addVertex(label,nullptr);
}

Edge* Graph::addEdge(Vertex src, Vertex dst/*, std::string label*/) {
	addEdge(&src,&dst);
}

Edge* Graph::addEdge(Vertex* src, Vertex* dst/*, std::string label*/) {
	EProp ep;
	ep.id = e_id++;
	edge_descriptor e0 = boost::add_edge(src->get_gboost_vertex(),dst->get_gboost_vertex(),ep,g).first;
	std::shared_ptr<Edge> foo{new Edge(&g[e0],e0,this)};
	g[e0].self = std::move(foo);
#ifndef NOJAVA
	if (isShowing()) {
		int err =  gv.addEdge(src->getId(),dst->getId()); //TODO: id is int
		if (err==-1) {
			std::cout << "Error while adding an edge" << std::endl;
			exit(1);
		}
	}
#endif
	return g[e0].self.get();
}

#if 0
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
#endif
	
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

int Graph::nEdges() {
	return num_edges(g);
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

EdgeIterable Graph::Edges() {
	std::pair<GEIt,GEIt> vxit = edges(g);
	EdgeIterable gvit{vxit.first,vxit.second,&g};
	return gvit;
}

AdjacentIterable Graph::getAdjacency(Vertex v) {
	std::pair<GAIt,GAIt> vxit = adjacent_vertices(v.get_gboost_vertex(), g);
	AdjacentIterable gvit{vxit.first,vxit.second,&g};
	return gvit;
}

AdjacentIterable Graph::getAdjacency(Vertex* v) {
	std::pair<GAIt,GAIt> vxit = adjacent_vertices(v->get_gboost_vertex(), g);
	AdjacentIterable gvit{vxit.first,vxit.second,&g};
	return gvit;
}

bool Graph::hasEdge(Vertex src, Vertex dst) {
	for (Vertex* i : getAdjacency(src)) {
		if ((i->get_gboost_vertex())==dst.get_gboost_vertex())
			return true;
	}
	return false;
}

Edge* Graph::random_edge(boost::mt19937& gen) {
	if (nEdges()>0) {
		edge_descriptor ed = boost::random_edge(g,gen);
		return g[ed].self.get();	
	} else {
		return nullptr;
	}
}

Vertex* Graph::random_vertex(boost::mt19937& gen) {
	if (nVertices()>0) {
		vertex_descriptor ed = boost::random_vertex(g,gen);
		return g[ed].self;	
	} else {
		return nullptr;
	}
}

void Graph::show(std::string title) {
#ifndef NOJAVA
	is_showed = true;
	gv.show(title.c_str());
#endif
}

void Graph::close() {
#ifndef NOJAVA
	is_showed = false;
	gv.close();
#endif
}

void Graph::hide() {
#ifndef NOJAVA
	is_showed = false;
	gv.hide();
#endif
}

void Graph::setEventUpdateTime(double t) {
	event_update_time = t;
}

double Graph::getEventUpdateTime() {
	return event_update_time;
}

void Graph::setVertexColor(Vertex* v, double val) {
	if ((isShowing())&&(v)) {
		if (!gv.setNodeColor(v->getId(), val)) {
			std::cerr << "Error while setting the color" << std::endl;
			exit(1);
		}
	}
}

void Graph::setVertexColor(Vertex* v, double val,bool forced) {
	if ((isShowing()||forced)&&(v)) {
		if (!gv.setNodeColor(v->getId(), val)) {
			std::cerr << "Error while setting the color" << std::endl;
			exit(1);
		}
	}
}

void Graph::setVertexColor(int v, double val) {

	if ((isShowing())&&(!gv.setNodeColor(v, val))) {
		std::cerr << "Error while setting the color" << std::endl;
		exit(1);
	}
}

/*
void Graph::getBetweennes() {
	boost::shared_array_property_map<double, boost::property_map<graph_t, int>::const_type>
  centrality_map(num_vertices(g), get(VProp::id, g));
}
*/

void Graph::setVertexColor(int v, double val,bool forced) {

	if ((isShowing()||forced)&&(!gv.setNodeColor(v, val))) {
		std::cerr << "Error while setting the color" << std::endl;
		exit(1);
	}
}

int Graph::viewBroadcast(int src, std::vector<int> dst, std::string message) {
#ifndef NOJAVA
	if (is_showed)
	return gv.sendBroadcast_with_stop(src,dst.data(),dst.size(),event_update_time,message.c_str());
	else return 1;
#endif
}

void Graph::stop(long int time) {
#ifndef NOJAVA
	gv.stop(time);
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

Vertex* Graph::getTarget(edge_descriptor ed) {
	return g[(ed.m_target)].self;
}

Vertex* Graph::getSource(edge_descriptor ed) {
	return g[(ed.m_source)].self;
}

Vertex* Graph::getTarget(Edge* e) {
	return getTarget(e->get_gboost_edge());
}

Vertex* Graph::getSource(Edge* e) {
	return getSource(e->get_gboost_edge());
}


