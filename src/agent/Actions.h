/*
 * Actions.h
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
class Vertex;
class Agent;
class Message;
class Graph;

/**
 * This class defines the actions that a single agent could perform over the network
 */
class Actions {
	Vertex* ptr;
	Agent* ag;
	
	public:
		Actions(Vertex* p, Agent* a) : ptr{p}, ag{a} {};
		Actions(const Actions& act) : ptr{act.ptr}, ag{act.ag} {};
		Actions() {};
		
		/** 
		 * Sends the message to all his neighbourhood (broadcast) 
		 * @param type the subject of the message
		 * @param reliability the reliability on topic @a type of the sender
		 */
		void act(int type, double reliability);
		
		/**
		 * Sends the message to all his neighbourhood
		 * (Returns the first message in stack)
		 */
		Message percept();

};
