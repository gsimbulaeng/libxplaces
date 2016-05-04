/*
 * xpAliveAction.cpp
 *
 *  Created on: 10 Jan 2011
 *      Author: xplaces
 */

#include "xpAliveAction.h"
#include "xplaces.h"
#include <ctime>

xpAliveAction::xpAliveAction() : xpAction() {
	setType(XP_ALIVE);
	setProperty("timestamp", time(NULL));
}

xpAliveAction::~xpAliveAction() {
}
