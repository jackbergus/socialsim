/*
 * PathHandler.h
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

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
}
#include <string>

#ifdef WIN32
    #define SEPARATOR '\\'
#else
    #define SEPARATOR '/'
#endif


inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

class PathHandler {
	private:
		std::string event_machine_folder;
		std::string event_folder;
		std::string distribution_folder;
		std::string agent_init_state_folder;
		std::string agent_mentalplan_folder;
		std::string plan_plugin_folder;
		std::string netowrk_distribution_folder;
		std::string view_css_folder;
		
		PathHandler(std::string e_machine,
				    std::string event,
				    std::string distr,
				    std::string ag_state, 
					std::string ag_plan,
					std::string plan,
					std::string net,
					std::string css) { event_machine_folder = e_machine;
							   event_folder = event;
					                   distribution_folder = distr;
					                   agent_init_state_folder = ag_state;
							   agent_mentalplan_folder = ag_plan;
					                   plan_plugin_folder = plan;
					                   netowrk_distribution_folder = net;
					                   view_css_folder = css; };
		
		static PathHandler* ptr;
		static bool is_created;
		
		std::string concat_path_file(std::string folder, std::string file_name);
	
	public:
		static PathHandler* initInstance(std::string e_machine,
					std::string event,
					std::string distr,
					std::string ag_state, 
					std::string ag_plan,
					std::string plan,
					std::string net,
					std::string css) {
			if (!is_created) {
				ptr = new PathHandler(e_machine,event,distr,ag_state,ag_plan,plan,net,css);
				is_created = true;
			}
			return ptr;
		}
		
		static PathHandler* getInstance() { return ptr; }

		/*PathHandler(const PathHandler& a) {
			agent_init_state_folder = a.agent_init_state_folder;	
			agent_mentalplan_folder = a.agent_mentalplan_folder;
			plan_plugin_folder = a.plan_plugin_folder;	
			netowrk_distribution_folder = a.netowrk_distribution_folder;
			view_css_folder = a.view_css_folder;
		};*/
		
		std::string getEventMachine(std::string em_file);
		std::string getEvent(std::string ev_file);
		std::string getDistribution(std::string distr_file);
		std::string getAgent(std::string agent_file);
		std::string getMentalPlan(std::string plan_file);
		std::string getPlan(std::string plan_file);
		std::string getNetwork(std::string net_file);
		std::string getViewStyle(std::string css_file);

};

