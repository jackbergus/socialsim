/*
 * CernFile.cpp
 * This file is part of socialsicm
 *
 * Copyright (C) 2014 - Giacomo Bergami
 *
 * socialsicm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * socialsicm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with socialsicm. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CernFile.h"

void CernFile::update() {
	if (!toload) {
		TFile f{filename.c_str(),"UPDATE"};
		for (auto x : loaded) {
			if (x.second->IsA() == TList::Class()) {
				f.WriteObject(x.second,x.first.c_str());//Writes correctly
			} else
				x.second->Write();
		}
		f.Close();
	}
}
	

TObject* CernFile::operator [](const std::string key) {
	return loaded[key];
}
	
std::map<std::string,TObject*>::iterator CernFile::begin() {
	return loaded.begin();
}
	
std::map<std::string,TObject*>::iterator CernFile::end() {
	return loaded.end();
}
	
/* Saves a given TObject to the map, in order to save the file */
CernFile  &operator<<( CernFile  &input, TObject* obj ) { 
	std::string k{obj->GetName()};
	input.loaded[k] = obj;
	return input;
}
	
/* Saves a given TObject to the map, in order to save the file */
CernFile  &operator<<( CernFile  &input, CernList obj ) { 
	std::string k{obj.GetName()};
	input.loaded[k] = (TObject*)obj.get();
	return input;
}
	
std::list<std::string> CernFile::getKeys() {
	std::list<std::string> elem;
	for (auto x: loaded) {
		elem.push_back(x.first);
	}
	return elem;
}
	
int CernFile::nlines() {
	return loaded.size();
}

