/*
 * CernFile.h
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
#include <string>

#pragma once

/** CernFile:
 *  This is an abstraction over the CernFile, since it allows to ease the 
 *  writing of lists inside a file, and some other default structures.
 *  The elements are added by name, and duplicated keys are overwrited
 */
class CernFile {
	std::map<std::string,TObject*> loaded;
	bool toload;
	std::string filename;
	
public:

	CernFile(std::string name,bool load) : toload{load} {
		if (toload) {
			TFile f{name.c_str()};
			TIter nextkey(f.GetListOfKeys());
			TKey *key;
			while (key = (TKey*)nextkey()) {
				std::string k{key->GetName()};
				loaded[k] = key->ReadObj();
			}
		} else {
			filename = name;
			remove(name.c_str());
		}
	}
	
	/** update:
	 *  Flushes the stream into an output file
	 */
	void update();
	
	/* This operator returns the added element inside the array */
	TObject* operator [](const std::string key) ;
	
	std::map<std::string,TObject*>::iterator begin();
	
	std::map<std::string,TObject*>::iterator end() ;
	
	/* Saves a given TObject to the map, in order to save the file */
	friend CernFile  &operator<<( CernFile  &, TObject*  ) ;
	
	/* Saves a given TObject to the map, in order to save the file */
	friend CernFile  &operator<<( CernFile  &, CernList ) ;
	/* Returns the list of all the objects that are contained inside a file */
	std::list<std::string> getKeys();
	
	int nlines();

};
