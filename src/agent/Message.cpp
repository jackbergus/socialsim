/*
 * Message.cpp
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

 

#include "Message.h"
#include "agent/Actions.h"
#include "graph/Vertex.h"
#include "agent/Agent.h"

int Message::getSender() {return send->getId();} ;

int Message::getSource() {return src->getId();} ;

int Message::getType() {return t;};

double Message::getOpinion() { return rel; };

double Message::getTime() { return time; };
