/*
 * Distribution.h
 * This file is part of socialsim
 *
 * Copyright (C) 2013 - Giacomo Bergami
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
 * along with socialsim; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#pragma once

#include <string>
#include <random>
#include <json/json.h>

#define DISTRCREATOR(under) 								\
class under##Creator : public DistrCreator { 				\
public:														\
	under##Creator() {};									\
	under* creator() { return new under(); };				\
	under* creator(double lam) { return new under(lam); };	\
}

struct Distribution {
    virtual double next() = 0;
};

struct DistrCreator {
	virtual Distribution* creator(json::Object o) = 0;
};



