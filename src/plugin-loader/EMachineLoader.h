/*
 * EMachineLoader.h
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

class EventMachine;

struct EMCreator {	//Il plugin è il costruttore della distribuzione esponenziale, accettante un oggetto json
	virtual EventMachine* creator(json::Object o) = 0;
};


/**
 * PlanLoader:
 * This is a singleton that loads all the Plans, in order to allocate the 
 * resources only once.
 */
class EMachineLoader {

private:
	Loader<EMCreator> l;
	PathHandler *pa;

	EMachineLoader() { pa = PathHandler::getInstance(); }
	
	static EMachineLoader* ptr;
	
public:
	static EMachineLoader* getInstance() {
		if (!ptr) {
			ptr = new EMachineLoader();
		}
		return ptr;
	}
	
	std::shared_ptr<EMCreator> load(std::string file);


};



