/*
 * NullPlan.cpp
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
 * along with socialsim; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */
 
 

#include "agent_plan.h"

#include <iostream>
#include <memory>

class NullPlan : public Plan {
		
	public:
		NullPlan() {}; //Costruttore base
		std::string getName() { return "NullPlan"; } //Nome del piano
		
		int process(json::Object* state, Message msg) { //Nome di *ogni* azione
			std::cout << "In NullPlan" << std::endl; 
			(*state)["perform_broadcast"]=true;
			return 50; //Salto al piano in posizione 50 dalla corrente (andando in avanti o indietro)
		};
};

MAKE_PLUGIN(NullPlan,Plan); //Creazione di un plugin
