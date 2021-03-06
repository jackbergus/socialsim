/*
 * Plan.h
 * This file is part of socialsim
 *
 * Copyright (C) 2013 - Giacomo Bergami
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

/**
 * This class implements the single plans, that have to be implemented inside
 * the reasoner
 **/

#pragma once

#include <string>
#include <memory>
#include <random>
#include "agent/Message.h"

class Plan {
public:
	virtual std::string getName() = 0;
	/**
	 * @arg state: the state of the agent (that will be updated)
	 * @arg msg:   the message that will change the agent's state
	 * @return: indicates at which level continue the evaluation
	 */
    virtual int process(json::Object* state, Message msg, double time) = 0;
    
    static double getOpinionType(json::Object* state, int type) {
    	return (*state)["opinion"][std::to_string(type)];
    }
    static void setOpinionType(json::Object* state, int type,double val) {
    	(*state)["opinion"][std::to_string(type)] = val;
    }
    
    static double getSenderReputation(json::Object* state, Message msg) {
    	if (!msg.getAgentSender())
    		return 0.5;
    	else
    		return (*state)[std::to_string(msg.getSender())];
    }
    static double getSourceReputation(json::Object* state, Message msg) {
    	if (!msg.getAgentSource())
    		return 0.5;
    	else
    		return (*state)[std::to_string(msg.getSource())];
    }
    static double getSenderReputation(json::Object* state, int sender) {
    	return (*state)[std::to_string(sender)];
    }
    
    static void setSenderReputation(json::Object* state, Message msg,double val) {
    	if (!msg.getAgentSender()) return;
    	(*state)[std::to_string(msg.getSender())] = val;
    }
    static void setSourceReputation(json::Object* state, Message msg,double val) {
    	if (!msg.getAgentSender()) return;
    	(*state)[std::to_string(msg.getSource())] = val;
    }
    static void setSenderReputation(json::Object* state, int sender, double val) {
    	(*state)[std::to_string(sender)] = val;
    }
    
    
    static double getPlagiability(json::Object* state) {
    	return (*state)["plagiability"];
    }
    static void doSend(json::Object* state, int type) {
    	(*state)["perform_broadcast"]=true;
    	(*state)["doshare"] = false;
    	(*state)["type_send"] = type;
    }
    static void doShare(json::Object* state, int type, double value) {
    	(*state)["perform_broadcast"]=true;
    	(*state)["doshare"] = true;
    	(*state)["shared"][std::to_string(type)] = value;
    	(*state)["type_send"] = type;
    }
    static void notSend(json::Object* state) {
    	(*state)["perform_broadcast"]=false;
    }
    
};


