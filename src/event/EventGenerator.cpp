/*
 * EventGenerator.cpp
 * This file is part of socialsim
 *
 * Copyright (C) 2013 - Giacomo Bergami
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
 * along with socialsim; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */
 
 
#include "json/json.h"
#include "graph/Graph.h"
#include "graph/Vertex.h"
#include "agent/Actions.h"
#include "agent/Agent.h"
#include "event/Distribution.h"
#include "event/EventType.h"
#include "event/Event.h"
#include "event/EventGenerator.h"

std::shared_ptr<Event> EventGenerator::next(Vertex* src, Vertex* dst) {
	double time = d->next();
	std::shared_ptr<Event> toret{new Event(et,time,src,dst)};
	return toret;
}

