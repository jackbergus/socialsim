/*
 * CernOWFile.h
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
class CernIterator;
#include "CernLists.h"

#pragma once

/** CernOWFile:
 *  This class defines a write file that is already opened in writing session,
 *  in order to not to keep in memory all the data
 */
class CernOWFile {
	std::map<std::string,TObject*> loaded;
	TFile* f = (TFile*)nullptr; //No shared_ptr, it follows the old way. That's very sad
	TList* keys = (TList*)nullptr; //No shared_ptr, it follows the old way. That's very sad
	//TFile* f;
	bool toload;
	bool isloaded;
	std::string filename;
	int ins = -1;
	int ncols = -1;
	
	/** It checks wether the n° of columns are */
	void check();
	
	void doClose() ;
	
public:
	
	void reopenRead(std::string name);

	/** CernOWFile:
	 *  Loads a TFile from disk
	 *  @param name: name of the file to load
	 */
	CernOWFile(std::string name) : toload{true} {
		reopenRead(name);
	}
	
	//Creates a class without accessing a file
	CernOWFile() : toload{false}, isloaded{false} {};

	void reopenWrite(std::string name, CernHeader hd);
	
	
	/** CernOWFile:
	 *  It creates a new file with a given header 
	 *  @param name: name of the file
	 *  @param hd:   header of the table
	 */
	CernOWFile(std::string name, CernHeader hd) : toload{false} {
		reopenWrite(name,hd);
	}
	
		
	CernIterator begin () const;
	
		
	CernIterator end () const ;
	
	int nlines() ;
	
	int ncolumns();
	
	/** close:
	 *  Flushes the stream into an output file
	 */
	void close() ;
	
	/* This operator returns the added element inside the array */
	TObject* operator [](const std::string& key) ;
	
	/* Returns the list of all the objects that are contained inside a file */
	std::list<std::string> getKeys() ;
	
	friend CernOWFile  &operator<<(CernOWFile  &, CernRecord  );
	
	TMatrixD toDoubleMatrix(int linnum, int colnum) ;
	TMatrixD toDoubleMatrix() ;
};

