/*
 * DistrLoader.h
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
#include "plugin-loader/PathHandler.h"
#include <loaderPlugin/Loader.h>

class DistrCreator;

/**
 * PlanLoader:
 * This is a singleton that loads all the Plans, in order to allocate the 
 * resources only once.
 */
class DistrLoader {

private:
	Loader<DistrCreator> l;
	PathHandler *pa;

	DistrLoader() { pa = PathHandler::getInstance(); }
	
	static DistrLoader* ptr;
	
public:
	static DistrLoader* getInstance() {
		if (!ptr) {
			ptr = new DistrLoader();
		}
		return ptr;
	}
	
	std::shared_ptr<DistrCreator> load(std::string file);

};

