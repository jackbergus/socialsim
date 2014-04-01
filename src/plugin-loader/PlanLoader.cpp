/*
 * PlanLoader.cpp
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
#include "agent/Agent.h"
#include "agent/Message.h"
#include "agent/Plan.h"
#include "plugin-loader/PlanLoader.h"

PlanLoader* PlanLoader::ptr = nullptr;
	
std::shared_ptr<Plan> PlanLoader::load(std::string file) {
	return l.load(pa->getPlan(file));
}
