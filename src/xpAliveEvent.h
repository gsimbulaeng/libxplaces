/*
 * xpAliveEvent.h
 *
 *  Created on: 10 Jan 2011
 *      Author: xplaces
 */

#ifndef XPALIVEEVENT_H_
#define XPALIVEEVENT_H_

#include "xpEvent.h"

//!  xpAliveEvent is a properties list that brings informations concerning alive nodes.
/*!
  An xpAliveEvent is sent to an xpListenerImpl. The class inherits the base skeleton from xpEvent.
*/
class xpAliveEvent : public xpEvent {
public:
	/*! xpAliveEvent()
	    \brief Constructor for the xpAliveEvent class.
		\details Base constructor for the xpAliveEvent class, the action will be instanced with type action only.
		\return An empty xpAliveEvent object
	*/
	xpAliveEvent();
	virtual ~xpAliveEvent();

	/*! \fn long int getTimestamp()
	    \brief It returns the timestam.
		\details It extracts the timestamp from the properties list.
		\return The xpAliveAction timestamp as long int.
	*/
	long int getTimestamp() {
		return getLongIntProperty("timestamp");
	}

};

#endif /* XPALIVEEVENT_H_ */
