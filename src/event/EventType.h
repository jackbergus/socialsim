/*
 * EventType.h
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

#include <memory>
#include <string>
#include "graph/Vertex.h"

enum EventEnumType {

	CREATE_NODE = 1,

	DELETE_NODE = 2,

	CREATE_ARCH = 3,
	
	SLEEP_NODE = 4,
	
	MESSAGE_SENT = 5,
	
	NO_MORE_EVENTS = 6,
	
	NO_EVENT = 0

};

static inline bool operator==(EventEnumType lhs, EventEnumType rhs)
{
    return static_cast<int>(lhs)==static_cast<int>(rhs);
};

static inline bool operator<(EventEnumType lhs, EventEnumType rhs)
{
    return static_cast<int>(lhs) < static_cast<int>(rhs);
};

#define CASE(X) if (s == #X ) return EventEnumType::X
#define ESAC(X) if (s == EventEnumType::X ) return #X

inline EventEnumType String_to_EventEnumType(std::string s) {
		CASE(CREATE_NODE);
	else 	CASE(DELETE_NODE);
	else 	CASE(CREATE_ARCH);
	else 	CASE(SLEEP_NODE);
	else 	CASE(MESSAGE_SENT);
	else    CASE(NO_MORE_EVENTS);
	else return EventEnumType::NO_EVENT;
}

inline std::string EventEnumType_to_String(EventEnumType s) {
		ESAC(CREATE_NODE);
	else 	ESAC(DELETE_NODE);
	else 	ESAC(CREATE_ARCH);
	else 	ESAC(SLEEP_NODE);
	else 	ESAC(MESSAGE_SENT);
	else    ESAC(NO_MORE_EVENTS);
	else return "NO_EVENT";
}
