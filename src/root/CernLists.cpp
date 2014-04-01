/*
 * CernList.cpp
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

#include "CernLists.h"

TList* CernList::get() { return cl.get(); }
std::string CernList::GetName() { return self; }
int CernList::size() { return cl->GetSize(); }

CernHeader& operator<<( CernHeader &input, std::string rhs ) { 
	std::string val_key = rhs + "_" + input.self;
	input.cl->Add(new TNamed(rhs.c_str(),val_key.c_str()));
	return input;
}


CernRecord& operator<<( CernRecord  &input, int rhs ) { 
	std::string x = std::to_string(rhs);
	std::string val_key = x + "_" + input.self;
	input.cl->Add(new TNamed(x.c_str(),val_key.c_str()));
	return input;
}

CernRecord& operator<<( CernRecord  &input, double rhs ) { 
	std::string x = std::to_string(rhs);
	std::string val_key = x + "_" + input.self;
	input.cl->Add(new TNamed(x.c_str(),val_key.c_str()));
	return input;
}

