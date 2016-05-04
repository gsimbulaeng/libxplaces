/*
 * xpEventListenerImpl.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpEventListenerImpl.h"
#include "xpNetwork.h"
#include "xpToolSet.h"
#include "xpAliveAction.h"
#include "xplaces.h"


xpEventListenerImpl::xpEventListenerImpl(bool hotSwapMode) {
	this->hotSwapMode = hotSwapMode;
	setSenderIpPort(xpNetwork::getInstance()->getIpPort());
	setSenderSession(xpToolSet::randomString(64, true, true, true));
//	sessionsMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&sessionsMutex, NULL);
	fprintf(stderr, "New listener, session key: %s\n", getSenderSession().c_str());

	if(this->hotSwapMode) {
		pthread_create(new pthread_t(), NULL, (void *(*)(void*)) &checkDevices, (void *) this);
	}
}

xpEventListenerImpl::~xpEventListenerImpl() {
}

bool xpEventListenerImpl::newEvent(xpEvent *event)
{
	if(event->getType() & XP_ALIVE) {
		xpDeviceProxy * device = (xpDeviceProxy *) xpNetwork::getInstance()->lookupDeviceNoBlocking(event->getSenderName());
		xpAliveAction * action = new xpAliveAction();
		action->setSenderIpPort(getSenderIpPort());
		action->setSenderSession(getSenderSession());
		device->sendAction(action);
	}

	eventReceived(event);

	return true;
}

void xpEventListenerImpl::addSessionToList(std::string newSession) {
	pthread_mutex_lock(&sessionsMutex);
	addSessionToListNoBlocking(newSession);
	pthread_mutex_unlock(&sessionsMutex);
}

void xpEventListenerImpl::addSessionToListNoBlocking(std::string newSession) {
	sessionsList.push_back(newSession);
	sessionsVector.push_back(newSession);
}


void xpEventListenerImpl::removeSessionFromList(std::string session) {
	pthread_mutex_lock(&sessionsMutex);

	removeSessionFromListNoBlocking(session);

	pthread_mutex_unlock(&sessionsMutex);
}


void xpEventListenerImpl::removeSessionFromListNoBlocking(std::string session) {
	sessionsList.remove(session);

	for (unsigned int i = sessionsVector.size() - 1; i >= 0; i--) {
		if (sessionsVector[i].compare(session) == 0) {
			sessionsVector.erase(sessionsVector.begin() + (i));
			break;
		}
	}
}



bool xpEventListenerImpl::checkSessionExists(std::string sessionToCheck) {

	pthread_mutex_lock(&sessionsMutex);
	std::list<std::string>::iterator iter1 = sessionsList.begin();
	while( iter1 != sessionsList.end() ) {
		if ((*iter1).compare(sessionToCheck) == 0) {
			pthread_mutex_unlock(&sessionsMutex);
			return true;
		}
		++iter1;
	}
	pthread_mutex_unlock(&sessionsMutex);
	return false;
}

void xpEventListenerImpl::checkDevices(void * reference) {

	xpEventListenerImpl * listener = (xpEventListenerImpl *) reference;

	unsigned int count;

	while(true) {
		pthread_mutex_lock(&(listener->sessionsMutex));
		if(listener->getSessionListSize() > 0) {

			for (count = 0; count < listener->sessionsVector.size(); count++ ) {
				xpAnnounceItem * deadNode = xpNetwork::getInstance()->getDeadNode(listener->sessionsVector[count]);

				if(deadNode != NULL) {
					fprintf(stderr, "Ghost Node! Looking for a new session for %s...\n", deadNode->getDescriptor()->getName().c_str());

					xpDeviceProxy * app = xpNetwork::getInstance()->lookupDevice(deadNode->getDescriptor()->getName());

					if(app != NULL) {
						listener->removeSessionFromListNoBlocking(listener->sessionsVector[count]);

						listener->addSessionToListNoBlocking(app->getDescriptor()->getSenderSession());
						listener->setRecipientIpPort(app->getDescriptor()->getSenderIpPort());
						listener->setRecipientSession(app->getDescriptor()->getSenderSession());

						if(xpNetwork::getInstance()->getDeliveryService()->addEventListener(*listener, listener->getEventTypeMask()) == 0) {
							fprintf(stderr, "Connection re-estabished, listener session added! %s\n", listener->getSenderSession().c_str());
							xpNetwork::getInstance()->addToListenerList(listener);
						}
					}
				}
				else {
				}
			}
		}
		pthread_mutex_unlock(&(listener->sessionsMutex));

#ifdef WIN32
		Sleep(xpNetwork::getInstance()->announceInterval *1000);
#else
		sleep(xpNetwork::getInstance()->getAnnounceInterval());
#endif
	}
}
