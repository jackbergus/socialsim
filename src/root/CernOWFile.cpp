/*
 * CernOWFile.cpp
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
#include "CernOWFile.h"
#include "CernIterator.h"
#include <iostream>

void CernOWFile::doClose() {
	if ((f) && isloaded) {
		f->Close();
		delete f;
	}
}

void CernOWFile::reopenRead(std::string name) {
	doClose();
	toload = true;
	f = new TFile(name.c_str());
	keys = f->GetListOfKeys();
}

void CernOWFile::reopenWrite(std::string name, CernHeader hd) {
	doClose();
	toload = false;
	std::string tmp{name};
	remove(name.c_str());
	f = new TFile(name.c_str(),"UPDATE");
	//filename = tmp;
	f->WriteObject(hd.get(),"header");
#ifdef DEBUG
	std::cout << "Header size is " << hd.size() << std::endl;
#endif
	ncols = hd.size();
	ins = 0;
}

void CernOWFile::check() {
	if (ins==-1) {
		bool first = true;
		int i, j;
		i = 0;
		for (std::list<std::string> x : *this) {
			if (first) {
				first = false;
			} else {
				j = 0;
				for (std::string e: x) {
					j++;
				}
				if (ncols==-1)
					ncols = j;
				else
					assert(ncols==j);
				i++;
			}
		}
		ins = i;
	}
}
		
CernIterator CernOWFile::begin () const {
	if (toload)
		return CernIterator(keys);
	else
		return CernIterator(loaded.begin());
}
	
		
CernIterator CernOWFile::end () const {
	if (toload)
		return CernIterator((TList*)nullptr);
	else
		return CernIterator(loaded.end());
}
	
int CernOWFile::nlines() {
	check();
	return ins;
}
	
int CernOWFile::ncolumns() {
	check();
	return ncols;
}
	
	/** flush:
	 *  Flushes the stream into an output file
	 */
void CernOWFile::close() {
	f->Close();
	f = nullptr;
}
	
	/* This operator returns the added element inside the array */
TObject* CernOWFile::operator [](const std::string& key) {
	if (!toload)
		return loaded[key];
	else {
		assert(keys != nullptr);
		return keys->FindObject(key.c_str());
	}
}

	
	/* Returns the list of all the objects that are contained inside a file */
std::list<std::string> CernOWFile::getKeys() {
	std::list<std::string> elem;
	if (toload) {
		assert(keys != nullptr);
		TIter nextkey{keys};
		TKey* key;
		while (key = (TKey*)nextkey()) {
			std::string k{key->GetName()};
			elem.push_back(k);
		}
	}
	return elem;
}
	
CernOWFile  &operator<<(CernOWFile  &input, CernRecord obj ) { 
#ifdef DEBUG
	std::cout << obj.size() << " vs. " << input.ncols << std::endl;
#endif
	assert(obj.size() == input.ncols); // the object size has to be equal to the number of columns 
	input.f->WriteObject(obj.get(),std::to_string(input.ins).c_str()); //The object name is the row number
	input.ins++;
	return input;
}
	
TMatrixD CernOWFile::toDoubleMatrix(int linnum, int colnum) {
	TMatrixD a{linnum,colnum};
	bool first = true;
	int i, j;
	i = 0;
	for (std::list<std::string> x : *this) {
		if (first) {
			//TODO: add arguments
			first = false;
		} else {
			j = 0;
			for (std::string e: x) {
				a(i,j) = std::stod(e);
				j++;
			}
			i++;
		}
	}
	return a;
}

TMatrixD CernOWFile::toDoubleMatrix() {
	return toDoubleMatrix(nlines(),ncolumns());
}

