/*
 * xpAliveEvent.cpp
 *
 *  Created on: 10 Jan 2011
 *      Author: xplaces
 */

#include "xpAliveEvent.h"
#include "xplaces.h"
#include <ctime>

xpAliveEvent::xpAliveEvent() : xpEvent() {
	setType(XP_ALIVE);
	setProperty("timestamp", time(NULL));
}

xpAliveEvent::~xpAliveEvent() {
}
