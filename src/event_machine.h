/*
 * event_machine.h
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
#include <memory>
#include <queue> 
#include <utility>   // std::pair
#include <random>
#include <queue>     // std::priority_queue
#include <unordered_map>
#include <unistd.h>
#include <functional> // std::great
#include <json/json.h>

//The root library wrapper
#include "root/CernOWFile.h"
#include "root/CernLists.h"

#include "graph/graph_t.h"
#include "event/Clock.h"
#include "event/EventType.h"
#include "event/Distribution.h"
#include "graph/Graph.h"
#include "event/Event.h"
#include "agent/Actions.h"
#include "graph/Edge.h"
#include "plugin-loader/PathHandler.h"
#include "agent/WholeMentalPlan.h"
#include "agent/Agent.h"
#include "graph/Vertex.h"
#include "agent/Agent.h"
#include "agent/Message.h"
#include "event/EventHandler.h"
#include "event/EventMachine.h"
#include "plugin-loader/EMachineLoader.h"



