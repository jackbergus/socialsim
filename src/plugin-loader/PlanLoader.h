/*
 * PlanLoader.h
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
#include <loaderPlugin/Loader.h>

class Plan;

/**
 * PlanLoader:
 * This is a singleton that loads all the Plans, in order to allocate the 
 * resources only once.
 */
class PlanLoader {

private:
	Loader<Plan> l;
	PathHandler *pa;

	PlanLoader() { pa = PathHandler::getInstance(); }
	
	static PlanLoader* ptr;
	
public:
	static PlanLoader* getInstance() {
		if (!ptr) {
			ptr = new PlanLoader();
		}
		return ptr;
	}
	
	std::shared_ptr<Plan> load(std::string file);


};


