/*
 * Message.h
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

#include <memory>
#include <iostream>

class Agent;

class Message {
	private:
		Agent* send;
		Agent* src;
		int t;
		double rel;
		double time;
		
	public:
		Message(Agent* sender, Agent* source, int type, double reliability,double send_time) : send{sender}, src{source}, t{type}, rel{reliability}, time{send_time} { 
#ifdef DEBUG
			if (!sender) 
				std::cout << "There is no sender!" << std::endl;
			if (!source) 
				std::cout << "There is no source!" << std::endl; 
#endif
		};
		Message(Agent* sender, int type, double reliability,double send_time) : send{sender}, src{sender}, t{type}, rel{reliability}, time{send_time} {
#ifdef DEBUG
		if (!sender) 
				std::cout << "There is no sender!" << std::endl;
#endif
		};
		Message() : src{nullptr}, send{nullptr}, t{0}, rel{0} {};
		Message(const Message& cp) {
			send = cp.send;
			src = cp.src;
			t = cp.t;
			rel = cp.rel;
			time = cp.time;
#ifdef DEBUG
			if (!send) 
				std::cout << "There is no sender!" << std::endl;
			if (!src) 
				std::cout << "There is no source!" << std::endl;
#endif
		}

		int getSender();
		Agent* getAgentSender() { return send; }
		Agent* getAgentSource() { return src; }
		int getSource();
		int getType();
		double getOpinion();
		double getTime();
};
