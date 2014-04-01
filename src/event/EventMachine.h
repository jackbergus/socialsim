/*
 * EventMachine.h
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

#include <list>
#include <memory>
#include <utility> 
#include <fstream>
class Clock;
class EventHandler;
class Event;
class Vertex;
class Edge;
class Graph;
#include "root/CernOWFile.h"
#include "root/CernLists.h"

#pragma once

// Ordinates the events by only sec value
class order_rising_sec
{
public:
  order_rising_sec() {}
  bool operator() (std::pair<double, std::shared_ptr<Event>>& lhs, std::pair<double, std::shared_ptr<Event>>&rhs) const
  {
    return lhs.first > rhs.first;
  }
};

class EventMachine {
	private:
		Clock c{};
		std::priority_queue<std::pair<double, std::shared_ptr<Event>>,std::vector<std::pair<double, std::shared_ptr<Event>>>,order_rising_sec> event_map;
		EventHandler ea;
		double stop_time;
		
		
		void InitHeader() {
			//Loads the header
			init_header();
			//Opens the file (overwrite) and stores the header
		}
		
		//void doDump() {
		//	log << dump();
		//}
	
	protected:
		Graph graph;
		std::string log_filename;
		
		
		double getSimTime() {
			return c.get();
		}
		
		std::shared_ptr<Event> getNextEvent() {
			if (event_map.size()>0) {
				std::pair<double, std::shared_ptr<Event>> start{event_map.top()};
				if (start.first >= 0) {
					std::cout << "Scheduling event at time " << start.first << " Old clock " << c.get() << std::endl;
					c.set(start.first);
					std::cout << "New Clock: " << c.get() << std::endl;
					event_map.pop();
					return start.second;
				}
			}
			//If it doesn't return, then an error occurred
			std::shared_ptr<Event> noe{new Event(EventEnumType::NO_MORE_EVENTS, 0, nullptr,nullptr)};
			event_map.pop();
			return noe;
		}
		
		void addEvent(EventEnumType t,Vertex* src, Vertex* dst) {
			std::shared_ptr<Event> k = ea.next(t,src,dst);
			k->setTime(k->getTime()+ c.get());
			std::pair<double, std::shared_ptr<Event>> p{k->getTime(),k};
			event_map.push(p);
		}
		
		void addEvent(EventEnumType t,Vertex* src, std::list<Vertex*> dst) {
			std::shared_ptr<Event> k = ea.next(t,nullptr,nullptr);
			double time = k->getTime() + c.get();
			std::cout << "Adding events that will be scheduled at time " << time <<  " Clock is " << c.get() << std::endl;
			for (Vertex* v: dst) {
				std::shared_ptr<Event> e{new Event(t,time,src,v)};
				std::pair<double, std::shared_ptr<Event>> p{time,e};
				event_map.push(p);
			}
		}
		
		virtual void processEvent(std::shared_ptr<Event> e) = 0;
		virtual void dump() = 0;
		virtual void init_header() = 0;
		virtual void close() = 0;

	
	public:
		EventMachine(json::Object obj, int seed, int vpos) 
			: ea{obj["event"],seed},
			  graph{ PathHandler::getInstance()->getNetwork(obj["graph"])}
			  
		{	
			std::string event = obj["event"];
			std::string grafo = obj["graph"];
			std::string sd = std::to_string(seed);
			std::string spos = std::to_string(vpos);
			log_filename = event + "_" + grafo + "_" + sd + "_" + spos + ".log";
			//log.open(filename); //starts the log
		}
		
		EventMachine(json::Object o) : EventMachine(o["object"],o["seed"],o["vertexpos"]) { };
		
		void start(std::string title) {
			start(title,0);
		}
		
		double setEventUpdateTime(double t) {
			double toret = graph.getEventUpdateTime();
			if (t>0)
				graph.setEventUpdateTime(t);
			return toret;
		}
		
		void stop() {
			stop_time = -1;
		}
		
		void start(std::string title,double time) {
			//TODO: computation inside a thread
			stop_time = time;
			bool setTime = (stop_time > 0 ? true : false);
			std::shared_ptr<Event> e = getNextEvent();
			graph.show(title); //Displays the network
			InitHeader();
			if (e->getType()==EventEnumType::NO_MORE_EVENTS) {
				close();
				return;
			}
			do {
				if ((setTime) && (e->getTime() > stop_time))
					break;
				processEvent(e); // Analyze the event. The Agent via Actions will update the Broadcast message
				std::cout << "TIME: " << c.get() << std::endl;
				dump();
				e = getNextEvent(); 
			} while (e->getType()!=EventEnumType::NO_MORE_EVENTS);
			close(); //stops the log
		}
		
};
