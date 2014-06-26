/*
 * WholeMentalPlan.cpp
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

 
#include "agent/WholeMentalPlan.h"


void WholeMentalPlan::init_plans(json::Object o) {
	PathHandler *pa = PathHandler::getInstance();
	for (auto it = o.begin() ; it != o.end(); ++it) {
		if (it->second.GetType() == json::ValueType::IntVal) {
			int val = it->second;
#ifdef DEBUG
			std::cerr << "Loading " << pa->getPlan(it->first) << std::endl;
#endif 
			
			plan_map[val] = std::unique_ptr<Plan>(PlanLoader::getInstance()->raw_load(it->first));
		}
	}
}
	
Plan* WholeMentalPlan::set_first_plan() {
	current_ptr = plan_map.begin();
	if (current_ptr == plan_map.end()) {
		current_pos = -1;
		current_plan = nullptr;
	} else {
		current_pos  = current_ptr->first;
		current_plan = current_ptr->second.get();
	}
	return current_plan;
}
	
int WholeMentalPlan::forward_to(int pos) {
	if (pos<=0) {
		set_first_plan();
		return 0;
	} else if (pos < current_pos) {
		return backward_to(pos);
	} else {
		for ( ; current_ptr != plan_map.end(); ++current_ptr) 
			if (current_ptr->first >= pos) {
				current_pos = current_ptr->first;
				current_plan = current_ptr->second.get();
				return current_pos;
			}
		return INT_MAX;
	}
}
	
int WholeMentalPlan::backward_to(int pos) {
	if (pos<=0) {
		set_first_plan();
		return 0;
	} else if (pos > current_pos) {
		return forward_to(pos);
	} else {
		for ( ; current_ptr != plan_map.begin(); --current_ptr) 
			if (current_ptr->first <= pos) {
				current_pos = current_ptr->first;
				current_plan = current_ptr->second.get();
				return current_pos;
			}
		return 0;
	}
}

	
int WholeMentalPlan::reason(json::Object* state, Message msg,double time) {
	
	//Optimization: if no plan is there, I return immediately (I don't reason)
	if (plan_map.size()==0) return INT_MAX;
	
	set_first_plan();

	while (current_plan != nullptr) {
		int next = current_plan->process(state,msg,time);
		//int next = 500;
		
		//Optimization: if the plan has only one plan...
		if (plan_map.size()==1) {
			//...If the next pace is greater than the current pos, then it has to return
			if (next>current_pos) return INT_MAX;
		}
		if (next > current_pos) {
			next = forward_to(next);
			if (next == INT_MAX)
				return INT_MAX;
		} else if (next < current_pos) {
			next = backward_to(next);
			if (next == INT_MAX);
				return INT_MAX;
		}
	}
	return INT_MAX;
}


