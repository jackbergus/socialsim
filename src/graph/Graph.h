/*
 * Graph.h
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
#include <array>
#include <boost/graph/iteration_macros.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp> 
#include "graph/graph_t.h"
#include "graph/graphview.hxx"
#include "graph/JVM.h"

using namespace graphview;

class Vertex;
class Edge;
class AdjacentIterable;
class AgentIterable;
class VertexIterable;
class EdgeIterable;

typedef boost::graph_traits<graph_t>::edge_iterator GEIt;
typedef boost::graph_traits<graph_t>::vertex_iterator GVIt;
typedef boost::graph_traits<graph_t>::adjacency_iterator GAIt;

class Graph {
	graph_t g;
	int v_id = 0;
	int e_id = 0;
	boost::minstd_rand gen;
	typedef boost::plod_iterator<boost::minstd_rand, graph_t> SFGen;
	void init_extern(bool withname);
	bool is_showed;
#ifndef NOJAVA
	GraphView gv;
#endif
	double event_update_time;
	bool structure_has_limit;
	bool structure_is_stack;
	int limit;
	
public: 
	
	bool isShowing();

	Graph() : structure_has_limit{false},  structure_is_stack{true}, limit{100}, is_showed{true}
#ifndef NOJAVA	
	, gv{JVM::getInstance()}
#endif
	{} // No Implementation
	
	Graph(long unsigned int n_Vert, double a, double b) 
		: structure_has_limit{false},  structure_is_stack{true}, limit{100}, is_showed{true}
#ifndef NOJAVA	
	, gv{JVM::getInstance()},
#endif 
	g{SFGen(gen, n_Vert, a, b), SFGen(), n_Vert}
	{
		init_extern(false);
	};
	
	Graph(std::string filename); //cpp
	Graph(std::string filename,bool haslim, bool isstack, int lim,bool doshow) : Graph(filename)
	 { structure_has_limit = haslim;  
	   structure_is_stack = isstack; 
	   limit = lim; 
	   is_showed = doshow;
	 };


	Graph(const Graph& l)
#ifndef NOJAVA	
	: gv{JVM::getInstance()}
#endif
	{ //copy, the view too
		g = l.g;
		v_id = l.v_id;
		e_id = l.e_id;
		structure_has_limit = l.structure_has_limit;
		structure_is_stack = l.structure_is_stack;
		limit = l.limit;
		is_showed = l.is_showed;
	}
	
	~Graph();

	Vertex* addVertex(std::string label, json::Object init);
	Vertex* addVertex(std::string label,Agent* a);
	Vertex* addVertex(std::string label);
	
	Edge* addEdge(Vertex src, Vertex dst);
	Edge* addEdge(Vertex* src, Vertex* dst);

	void write(std::ostream out_stream);//Writes in GraphML format
	void std_out_write();//Outputs in GraphML format
	
	void setName(std::string label, Vertex* v);
	void setName(std::string label, Edge* v);
	
	void remove(Vertex* v);
	void remove(Edge* v);
	
	graph_t get_gboost_graph();
	
	VertexIterable Vertices();
	EdgeIterable Edges();
	int nVertices();
	int nEdges();
	AgentIterable Agents();
	AdjacentIterable getAdjacency(Vertex v);
	AdjacentIterable getAdjacency(Vertex* v);
	bool hasEdge(Vertex src, Vertex dst);
	Edge* random_edge(boost::mt19937& gen);
	Vertex* random_vertex(boost::mt19937& gen);
	void stop(long int time);
	
	void show(std::string title);
	void hide();
	void close();
	int viewBroadcast(int src, std::vector<int> dst, std::string message);
	void setEventUpdateTime(double t);
	double getEventUpdateTime();
	void setVertexColor(Vertex* v, double val);
	void setVertexColor(Vertex* v, double val,bool forced);
	void setVertexColor(int v, double val);
	void setVertexColor(int v, double val,bool forced);
	
	//void getBetweennes();
	
	Vertex*  getTarget(edge_descriptor ed);
	Vertex*  getSource(edge_descriptor ed);
	Vertex*  getTarget(Edge* e);
	Vertex*  getSource(Edge* e);
	
	//Iterators
	std::shared_ptr<Agent> acast(vertex_descriptor vic); //casts a GraphVertexIterator into an Agent
	Vertex* vcast(vertex_descriptor vic);
	int vid(vertex_descriptor vic);

};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 *  Iteration over the vertex descriptors

class GraphVertexIterator {
	GVIt beg; 
	GVIt endalust;
public:
	GraphVertexIterator(GVIt fst, GVIt snd) : beg{fst}, endalust{snd}
	{};
	GVIt begin() const { return beg; }
	GVIt end() const { return endalust; }
	
};
 */
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class VxIter
{
    public:
    VxIter (GVIt begin, graph_t *g)
        : start( begin ) { 
        grafo = g;
    }

    bool operator!= (const VxIter& other) const {
        return start != other.start;
    }
 
    Vertex* operator* () const {
    	return (*grafo)[*start].self;
    }
 
    const VxIter& operator++ () {
        ++start; 
        return *this;
    }
 
    private:
    GVIt start;
    graph_t *grafo;
};
 /**
 *  Iteration over the vertices
 */
class VertexIterable
{
    public:
    VertexIterable (GVIt fst, GVIt snd ,graph_t *g) 
    : f{fst}, s{snd} {
    	grafo = g;
    }

    VxIter begin () const {
        return VxIter(f,grafo);
    }
 
    VxIter end () const {
        return VxIter(s,grafo);
    }

 
    private:
   	GVIt f,s;
   	graph_t *grafo;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class AgIter
{
    public:
    AgIter (GVIt begin, graph_t *g)
        : start( begin ) { 
        grafo = g;
    }

    bool operator!= (const AgIter& other) const {
        return start != other.start;
    }
 
    Agent* operator* () const {
    	return (*grafo)[*start].agent_ptr.get();
    }
 
    const AgIter& operator++ () {
        ++start; 
        return *this;
    }
 
    private:
    GVIt start;
    graph_t *grafo;
};
 /**
 *  Iteration over the agents
 */
class AgentIterable
{
    public:
    AgentIterable (GVIt fst, GVIt snd ,graph_t *g) 
    : f{fst}, s{snd} {
    	grafo = g;
    }

    AgIter begin () const {
        return AgIter(f,grafo);
    }
 
    AgIter end () const {
        return AgIter(s,grafo);
    }

 
    private:
   	GVIt f,s;
   	graph_t *grafo;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/* 
class GraphAdjIterator {
	GAIt beg; 
	GAIt endalust;
public:
	GraphAdjIterator(GAIt fst, GAIt snd) : beg{fst}, endalust{snd}
	{};
	GAIt begin() const { return beg; }
	GAIt end() const { return endalust; }
	
};
*/

class AdIter
{
    public:
    AdIter (GAIt begin, graph_t *g)
        : start( begin ) { 
        grafo = g;
    }

    bool operator!= (const AdIter& other) const {
        return start != other.start;
    }
 
    Vertex* operator* () const {
    	return (*grafo)[*start].self;
    }
 
    const AdIter& operator++ () {
        ++start; 
        return *this;
    }
 
    private:
    GAIt start;
    graph_t *grafo;
};
 /**
 *  Iteration over the adjacents
 */
class AdjacentIterable
{
    public:
    AdjacentIterable (GAIt fst, GAIt snd ,graph_t *g) 
    : f{fst}, s{snd} {
    	grafo = g;
    }

    AdIter begin () const {
        return AdIter(f,grafo);
    }
 
    AdIter end () const {
        return AdIter(s,grafo);
    }

 
    private:
   	GAIt f,s;
   	graph_t *grafo;
};

class EdIter
{
    public:
    EdIter (GEIt begin, graph_t *g)
        : start( begin ) { 
        grafo = g;
    }

    bool operator!= (const EdIter& other) const {
        return start != other.start;
    }
 
    Edge* operator* () const {
    	return (*grafo)[*start].self.get();
    }
 
    const EdIter& operator++ () {
        ++start; 
        return *this;
    }
 
    private:
    GEIt start;
    graph_t *grafo;
};
 /**
 *  Iteration over the edges
 */
class EdgeIterable
{
    public:
    EdgeIterable (GEIt fst, GEIt snd ,graph_t *g) 
    : f{fst}, s{snd} {
    	grafo = g;
    }

    EdIter begin () const {
        return EdIter(f,grafo);
    }
 
    EdIter end () const {
        return EdIter(s,grafo);
    }

 
    private:
   	GEIt f,s;
   	graph_t *grafo;
};
