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

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

class Clock;
class EventHandler;
class Event;
class Vertex;
class Edge;
class Graph;

#include "root/CernOWFile.h"
#include "root/CernLists.h"
extern "C" {
	#include <sys/time.h>
}

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
		bool debug;
		
		bool setTime = false;
		double stop_time = -1;
		
		bool setEvents = false;
		unsigned long int stop_events = -1;
		unsigned long int nevents = 0;
		
		
		
		void InitHeader() {
			//Loads the header
			init_header();
			//Opens the file (overwrite) and stores the header
		}
		
		//void doDump() {
		//	log << dump();
		//}
		
		
		void addEvent(EventEnumType t,Vertex* src, Vertex* dst,double timer) {
			std::shared_ptr<Event> sp{new Event(t, timer, src, dst)};
			std::pair<double, std::shared_ptr<Event>> p{timer,sp};
#ifdef DEBUG
			coutTime();
			std::cout << "[PRIVATE] Adding event(s?) " << EventEnumType_to_String(t) << " that will be scheduled at time " << timer <<  "."<< std::endl;
#endif
			event_map.push(p);
		}

		std::shared_ptr<Event> getNextEvent() {
			if (event_map.size()>0) {
				std::pair<double, std::shared_ptr<Event>> start{event_map.top()};
				if (start.first >= 0) {
#ifdef DEBUG
					coutTime();
					std::cout << "Scheduling event at time " << start.first << " Old clock " << c.get() << std::endl;
#endif
					c.set(start.first);
#ifdef DEBUG
					std::cout << "New Clock: " << c.get() << std::endl;
#endif
					event_map.pop();
					std::cout << EventEnumType_to_String(start.second->getType()) << std::endl;
					nevents++; // new event scheduled
					return start.second;
				} else {
#ifdef DEBUG
					std::cerr << "WARNING: " << EventEnumType_to_String(start.second->getType()) << " generated a non positive time ~ quitting" << std::endl;
#endif
				}
			}
			//If it doesn't return, then an error occurred
			std::shared_ptr<Event> noe{new Event(EventEnumType::NO_MORE_EVENTS, 0, nullptr,nullptr)};
#ifdef DEBUG
			std::cerr << "WARNING: NO EVENT RETURNING AS MAP IS EMPTY" << std::endl;
#endif
			return noe;
		}
		
		void addEventAsk(EventEnumType t,Vertex* src, Vertex* dst,bool doIncr) {
			std::shared_ptr<Event> k = ea.next(t,src,dst);
			double time;
			if (doIncr)
				time = k->getTime()+ c.get();
			else
				time = k->getTime();
#ifdef DEBUG
			coutTime();
			std::cout << "Adding event " << EventEnumType_to_String(t) << " that will be scheduled at time " << time << std::endl;
#endif
			k->setTime(time);
			std::pair<double, std::shared_ptr<Event>> p{k->getTime(),k};
			event_map.push(p);
		}
		
		void addEventAsk(EventEnumType t,Vertex* src, std::list<Vertex*> dst,bool doIncr) {
			double time;
			if (doIncr)
				time = ea.next(t,src,nullptr)->getTime() + c.get();
			else 
				time = ea.next(t,src,nullptr)->getTime();
			for (Vertex* v: dst) {
				addEvent(t,src,v,time);
#ifdef DEBUG
				std::cout << "Add(s!)" << std::endl;
#endif
			}
		}
		
		
	
	protected:
		Graph graph;
		std::string log_filename;
		
		void msleep (unsigned int ms) {
			int microsecs;
			struct timeval tv;
			microsecs = ms * 1000;
			tv.tv_sec  = microsecs / 1000000;
			tv.tv_usec = microsecs % 1000000;
			select (0, NULL, NULL, NULL, &tv);  
		}
		
		double getSimTime() {
			return c.get();
		}
		
		void coutTime() {
			std::cout << "\x1B[32m" << "[Clock:" << getSimTime() << "]\x1B[0m";
		}

		void addEventIncr(EventEnumType t,Vertex* src, Vertex* dst) {
			addEventAsk(t,src,dst,true);
		}
		
		void addEventAt(EventEnumType t,Vertex* src, Vertex* dst) {
			addEventAsk(t,src,dst,false);
		}
		
		// +Adds all the dst events at a same time
		void addEventIncr(EventEnumType t,Vertex* src, std::list<Vertex*> dst) {
			addEventAsk(t,src,dst,true);
		}
		
		// +Adds all the dst events at a same time
		void addEventAt(EventEnumType t,Vertex* src, std::list<Vertex*> dst) {
			addEventAsk(t,src,dst,false);
		}
		
		// +Adds all the dst events at different times
		void addEventIncrDiff(EventEnumType t,Vertex* src, std::list<Vertex*> dst) {
			for (Vertex* d : dst) {
				addEventIncr(t,src,d);
			}
		}
		
		// +Adds all the dst events at different time
		void addEventAtDiff(EventEnumType t,Vertex* src, std::list<Vertex*> dst) {
			for (Vertex* d : dst) {
				addEventAt(t,src,d);
			}
		}
		
		virtual int processEvent(std::shared_ptr<Event> e) = 0;
		virtual void dump() = 0;
		virtual void init_header() = 0;
		virtual void close() = 0;

		bool doStop(std::shared_ptr<Event> e) {
			std::cout << nevents << " " << stop_events << std::endl;
			return	(((setTime) && (e->getTime() > stop_time))||((setEvents) && (nevents > stop_events)));
		}
	
	public:
		EventMachine(json::Object obj, int seed, int vpos,bool haslim, bool isstack, int lim) 
			: ea{obj["event"],seed},
			  graph{ ((obj.find("graph")!=obj.end()) ? PathHandler::getInstance()->getNetwork(obj["graph"]) : ""), haslim, isstack, lim , ((obj.find("debug") == obj.end()) ? false : ((int)obj["debug"] ? true : false)) }
			  
		{	
			std::string event = obj["event"];
			std::string grafo = obj["graph"];
			std::string sd = std::to_string(seed);
			std::string spos = std::to_string(vpos);
			c.set(ea.start(((double)graph.nVertices())));
#ifdef DEBUG
			coutTime();
			std::cout << " Start time has been setted" << std::endl;
#endif
			log_filename = event + "_" + grafo + "_" + sd + "_" + spos + ".log";
			debug = ((obj.find("debug") == obj.end()) ? false : ((int)obj["debug"] ? true : false));
			//log.open(filename); //starts the log
		}
		

		
		EventMachine(json::Object o) : EventMachine(o["object"],o["seed"],o["vertexpos"],false,true,100) { };
		
		
		void start(std::string title) {
			if (!debug)
				//Displays the network
				graph.show(title); 
			else 	
				//Be sure to not display a thing
				graph.hide(); 
			//Starts the logging with the header
			InitHeader();
			
			//Generating the first event
			std::shared_ptr<Event> e = getNextEvent();
			if (e->getType()==EventEnumType::NO_MORE_EVENTS) {
				close();
				return;
			}
			
			int hasnext = 1;
			
			do {
				if (doStop(e)) break;
				
				// Analyze the event. The Agent via Actions will 
				// update the Broadcast message. The return value
				// means if the em instance had stopped the simulation 
				// or not
				hasnext = processEvent(e); 
#ifdef DEBUG				
				coutTime(); std::cout  << std::endl;
#endif				
				//Call the dump of the current event
				dump();
				
				//Handle the next event
				e = getNextEvent(); 
			} while ((e->getType()!=EventEnumType::NO_MORE_EVENTS) && (hasnext));
			close(); //stops the log
		}
		
		
		double setEventUpdateTime(double t) {
			double toret = graph.getEventUpdateTime();
			graph.setEventUpdateTime(t);
			return toret;
		}
		
		double getEventUpdateTime() { return graph.getEventUpdateTime(); }
		
		void stop() {
			stop_time = -1;
			stop_events = -1;
			setTime = false;
			setEvents = false;
		}
		
		void start(std::string title, unsigned long int halt_events) {
			stop_events 	= halt_events;
			setEvents 	= (stop_events > 0 ? true : false);
			start(title);
		}
		
		void start(std::string title,double halt_time) {
			stop_time 	= halt_time;
			setTime 	= (stop_time > 0 ? true : false);
			start(title);
		}
		
};
