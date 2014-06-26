/*
 * WholeMentalPlan.h
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


#pragma once

#include <fstream>
#include <string>
#include <cerrno>
#include <climits>

#include <json/json.h>
#include <loaderPlugin/Loader.h>

#include "agent/Plan.h"
#include "plugin-loader/PathHandler.h"
#include "plugin-loader/PlanLoader.h"

//FORWARDS
#include "agent/Message.h"

//
#include "../generics.h"

// class Message;  in Plan.h

struct WholeMentalPlan {

	typedef std::map<int,std::unique_ptr<Plan>>::iterator Iterator;

	std::map<int,std::unique_ptr<Plan>> plan_map;
	
	int 	 current_pos;
	Plan*    current_plan;
	Iterator current_ptr;
	
	void 	 init_plans(json::Object o);
	Plan* 	 set_first_plan();
	int 	 forward_to(int pos);
	int 	 backward_to(int pos);

	
	public:
		WholeMentalPlan(std::string file) {
			std::string fullpath = PathHandler::getInstance()->getMentalPlan(file);
			std::string content = get_file_contents(fullpath.c_str());
#ifdef DEBUG
			std::cout << "WholeMentalPlan " << " File:" << file << " Content:" << content << "\n";
#endif
			json::Object o = json::Deserialize(content);
			init_plans(o); /**< Initializes the plans */
		}
		
		/** reason:
		* @param state: the agent state
		* @param msg:   the message that starts the reasoning process
		*/
		int reason(json::Object* state, Message msg, double time);
};

