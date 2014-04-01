/*
 * Cern.h
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

#include <list>	   //Collection of elements
#include <map>
#include <memory>
#include <iostream>
#include <cassert>
#include <cstdio>

#include <Rtypes.h> //Int_t
#include <TFile.h> //Uses the standard File
#include <TMath.h> //Math elements
#include <TMatrixD.h> //Matrix
#include <TKey.h>
#include <TString.h>

//
#include <TRandom.h>
#include <TH1.h>
//

/* g++ -std=c++11 -I`/usr/share/root/bin/root-config --incdir` root.cpp `/usr/share/root/bin/root-config --libs` -Wl,-R`/usr/share/root/bin/root-config --libdir` -g */ 

#pragma once

namespace cern {

static inline TString toTString(std::string str) {
	TString ts{str.c_str()};
	return ts;
}

static inline std::string toString(TString* str) {
	std::string s{str->Data()};
	return s;
}

static inline std::shared_ptr<TList>  toList(std::list<std::string> ls) {
	std::shared_ptr<TList> tl{new TList()};
	for (std::string x : ls) {
		tl->Add((TObject*)new TString(x.c_str()));
	}
	return tl;
}

static inline std::list<std::string> toStdList(std::shared_ptr<TList> ls) {
	TIter next{(TCollection*)ls.get()};
	TNamed *key;
	std::list<std::string> toret;
	while (key = (TNamed*)next()) {
		std::string elem{key->GetName()};
		toret.push_back(elem);
	}
	return toret;
}

static inline std::list<std::string> toStdList(TList* ls) {
	TIter next{ls};
	TNamed *key;
	std::list<std::string> toret;
	while (key = (TNamed*)next()) {
		std::string elem{key->GetName()};
		toret.push_back(elem);
	}
	return toret;
}

}
