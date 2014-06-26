/*
 * Vertex.h
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
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/random.hpp> 
#include <boost/random/mersenne_twister.hpp>
#include "graph/graph_t.h"
#include "generics.h"
class Message;
class AdjacentIterable;

/**
 * Vertex:
 * This class defines the abstraction of a graph vertex, with its properties
 * and the gboost identifier
 */
class Vertex {
	private:
		vertex_descriptor vd;
		Graph* g;
		data_structure<Message> incoming; 		/**< The list of the incoming messages */
	public:
		Vertex(vertex_descriptor desc,Graph* grap);
		Vertex(vertex_descriptor desc,Graph* grap, bool isstack);
		Vertex(vertex_descriptor desc,Graph* grap, bool isstack,int lim);
		
		void setName(std::string new_name);
		std::string getName();
		
		int getId();
		std::shared_ptr<Agent> getAgent();

		vertex_descriptor get_gboost_vertex();
		
		AdjacentIterable getAdjacency();
		int getDegree();
		Vertex* rndAdjacent(boost::variate_generator<boost::mt19937&, boost::uniform_real<> >& mersenne);
		
		void remove();
		Graph* getGraph();
		void send(Message msg);
		Message recv();
		int ssize();
};
