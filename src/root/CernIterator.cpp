/*
 * CernIterator.cpp
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

#include "CernIterator.h"
	
bool CernIterator::operator!= (const CernIterator& other) const {
	if (ismap == other.ismap) {
		if (ismap)
			return ptr != other.ptr;
		else
			return key != other.key;
	}
	return false;
}
	
std::list<std::string> CernIterator::operator* () const {
	TList* toret;
	if (ismap)
		toret = (TList*)ptr->second;
	else
    		toret = (TList*)key->ReadObj();
    	return cern::toStdList(toret);
}
	
const CernIterator& CernIterator::operator++ () {
	if (ismap)
		ptr++;
	else
		key = (TKey*)iterator();
	return *this;
}

