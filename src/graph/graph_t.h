/*
 * graph_t.h
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

#include <iostream>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/graphml.hpp>

#include <boost/graph/plod_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include "graph/VProp.h"
#include "graph/EProp.h"

class Graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,VProp, EProp> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<graph_t>::edge_descriptor edge_descriptor;


