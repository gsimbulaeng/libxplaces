/*
 * xpAliveAction.h
 *
 *  Created on: 10 Jan 2011
 *      Author: xplaces
 */

#ifndef XPALIVEACTION_H_
#define XPALIVEACTION_H_

#include "xpAction.h"

//!  xpAliveAction is a properties list that brings informations concerning alive nodes.
/*!
  An xpAliveAction is sent through an xpDeviceProxy, to an xpDeviceImpl. The class inherits the base skeleton from xpAction.
*/
class xpAliveAction : public xpAction {
public:
	/*! xpAliveAction()
	    \brief Constructor for the xpAliveAction class.
		\details Base constructor for the xpAliveAction class, the action will be instanced with type action only.
		\return An empty xpAliveAction object
	*/
	xpAliveAction();
	virtual ~xpAliveAction();

	/*! \fn long int getTimestamp()
	    \brief It returns the timestam.
		\details It extracts the timestamp from the properties list.
		\return The xpAliveAction timestamp as long int.
	*/
	long int getTimestamp() {
		return getLongIntProperty("timestamp");
	}

};

#endif /* XPALIVEACTION_H_ */
