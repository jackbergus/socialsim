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
#include "graph/graph_t.h"
#include "graph/graphview.hxx"
#include "graph/JVM.h"

using namespace graphview;

class Vertex;
class Edge;
class GraphVertexIterator;
class GraphAdjIterator;
class AgentIterable;
class VertexIterable;

typedef boost::graph_traits<graph_t>::vertex_iterator GVIt;
typedef boost::graph_traits<graph_t>::adjacency_iterator GAIt;

class Graph {
	graph_t g;
	int v_id = 0;
	int e_id = 0;
	boost::minstd_rand gen;
	typedef boost::plod_iterator<boost::minstd_rand, graph_t> SFGen;
	void init_extern(bool withname);
#ifndef NOJAVA
	GraphView gv;
#endif
	double event_update_time = 400;
	
public: 
	

	Graph() 
#ifndef NOJAVA	
	: gv{JVM::getInstance()} 
#endif
	{} // No Implementation
	
	Graph(long unsigned int n_Vert, double a, double b) 
		: g{SFGen(gen, n_Vert, a, b), SFGen(), n_Vert}
#ifndef NOJAVA	
		  ,gv{JVM::getInstance()} 
#endif
	{
		init_extern(false);
	};
	
	Graph(std::string filename); //cpp


	Graph(const Graph& l) 
#ifndef NOJAVA	
	: gv{JVM::getInstance()} 
#endif
	{ //copy, the view too
		g = l.g;
		v_id = l.v_id;
		e_id = l.e_id;
	}
	
	~Graph();

	Vertex addVertex(std::string label,Agent* a);
	Vertex addVertex(std::string label);
	
	Edge addEdge(Vertex src, Vertex dst/*, std::string label*/);

	void write(std::ostream out_stream);//Writes in GraphML format
	void std_out_write();//Outputs in GraphML format
	
	void setName(std::string label, Vertex* v);
	void setName(std::string label, Edge* v);
	
	void remove(Vertex* v);
	void remove(Edge* v);
	
	graph_t get_gboost_graph();
	
	VertexIterable Vertices();
	int nVertices();
	AgentIterable Agents();
	GraphAdjIterator getAdjacency(Vertex v);
	GraphAdjIterator getAdjacency(Vertex* v);
	bool hasEdge(Vertex src, Vertex dst);
	
	void show(std::string title);
	void hide();
	void close();
	void viewBroadcast(int src, std::vector<int> dst, std::string message);
	void setEventUpdateTime(double t);
	double getEventUpdateTime();
	
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


class GraphAdjIterator {
	GAIt beg; 
	GAIt endalust;
public:
	GraphAdjIterator(GAIt fst, GAIt snd) : beg{fst}, endalust{snd}
	{};
	GAIt begin() const { return beg; }
	GAIt end() const { return endalust; }
	
};
