/*
 * CernIterator.h
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

/** CernIterator:
 * This is a common iterator for both read and written files
 */
class CernIterator {
	std::map<std::string,TObject*>::const_iterator ptr;
	TIter iterator;
	TKey *key;
	bool ismap;
	
public:
	CernIterator(TList* elem) : iterator{elem} {
		if (elem) 
			key = (TKey*)iterator();
		else
			key = (TKey*)nullptr;
		ismap = false;
	}
	
	CernIterator(std::map<std::string,TObject*>::const_iterator elem) 
		: iterator{(TList*)nullptr}, ptr{elem} {
		key = nullptr;
		ismap = true;
	}
	
	bool 			operator!=   (const CernIterator& other) const ;
	std::list<std::string>  operator*()  const;	
	const CernIterator& 	operator++() ;

};

