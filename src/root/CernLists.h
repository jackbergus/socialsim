/*
 * CernList.h
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

#include "Cern.h"

#pragma once

/** CernList:
 *  This is an adapter to the TList, in order to ease the writing of the 
 *  List inside the Cern File
 */
class CernList {
protected:
	std::string self;
	std::shared_ptr<TList> cl;
	
public:
	CernList(std::string name) : self{name} , cl{new TList()} {};
	CernList(std::string name,std::list<std::string> ls) : CernList(name) {
		for (std::string x : ls) {
			std::string val_key = x + "_" + name;
			cl->Add(new TNamed(x.c_str(),val_key.c_str()));
		}
		cl->SetName(name.c_str());
	}
	CernList(const CernList& cp) {
		self = cp.self;
		cl = cp.cl;
	}
	/*CernList(CernList&& cp) 
		: self{std::move(cp.self)}, cl{std::move(cp.cl)} {
	}*/
	CernList() : self{""}, cl{nullptr} {};
	TList* get();
	std::string GetName();
	int size();
};

class CernHeader: public CernList {
public:
	CernHeader(std::string name) : CernList(name) {};
	CernHeader() : CernHeader("header") {};
	CernHeader(std::string name,std::list<std::string> ls) : CernList(name, ls) {};
	friend CernHeader &operator<<( CernHeader &, std::string  ) ;
};

class CernRecord: public CernList {
public:
	CernRecord(std::string name) : CernList(name) {};
	CernRecord(std::string name,std::list<double> ls) : CernRecord(name) {
		for (double e : ls) {
			std::string x = std::to_string(e);
			std::string val_key = x + "_" + name;
			cl->Add(new TNamed(x.c_str(),val_key.c_str()));
		}
		cl->SetName(name.c_str());
	}
	CernRecord() : CernRecord("") {};
	friend CernRecord &operator<<( CernRecord  &, int ) ;
	friend CernRecord &operator<<( CernRecord  &, double ) ;
};
