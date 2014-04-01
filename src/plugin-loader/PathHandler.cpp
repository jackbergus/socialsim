/*
 * PathHandler.cpp
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


#include <string>
#include <iostream>
#include "plugin-loader/PathHandler.h"

bool PathHandler::is_created = false;

PathHandler* PathHandler::ptr = nullptr;


std::string PathHandler::concat_path_file(std::string folder, std::string file_name) {
#ifdef DEBUG
	std::cout << folder + SEPARATOR + file_name + "\n";
#endif
	if (folder.back()!=SEPARATOR) 
		folder = folder + SEPARATOR;
	std::string path = folder + file_name;
	if (!file_exists(path)) {
		path.clear();
	}
	return path;
}

std::string PathHandler::getEventMachine(std::string ev_file) {
	return concat_path_file(event_machine_folder, ev_file);
}

std::string PathHandler::getEvent(std::string ev_file) {
	return concat_path_file(event_folder, ev_file);
}
	
std::string PathHandler::getDistribution(std::string distr_file) {
	return concat_path_file(distribution_folder, distr_file);
}
		
std::string PathHandler::getAgent(std::string agent_file) {
	return concat_path_file(agent_init_state_folder,agent_file);
}
		
std::string PathHandler::getMentalPlan(std::string plan_file) {
	return concat_path_file(agent_mentalplan_folder,plan_file);
}
		
std::string PathHandler::getPlan(std::string plan_file) {
	return concat_path_file(plan_plugin_folder,plan_file);
}
		
std::string PathHandler::getNetwork(std::string net_file){
	return concat_path_file(netowrk_distribution_folder,net_file);
}
		
std::string PathHandler::getViewStyle(std::string css_file){
	return concat_path_file(view_css_folder,css_file);
}
