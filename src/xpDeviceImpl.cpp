/*
 * xpDevice.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpDeviceImpl.h"
#include "xpEventListenerProxy.h"
#include <stdio.h>
#include "xpAddress.h"
#include "xpAliveEvent.h"
#include "xpNetwork.h"

xpDeviceImpl::xpDeviceImpl(xpDescriptor * deviceRef, bool checkListenerMode) : xpDevice(deviceRef) {
	this->checkListenerMode = checkListenerMode;

	pthread_mutex_init(&listenersMutex, NULL);
//	listenersMutex = PTHREAD_MUTEX_INITIALIZER;

	if(checkListenerMode)
		pthread_create(new pthread_t(), NULL, (void *(*)(void*)) &checkListeners, (void *) this);

}

xpDeviceImpl::~xpDeviceImpl() {
}

void xpDeviceImpl::checkListeners(void * reference) {
	xpDeviceImpl * device = (xpDeviceImpl *) reference;

	while(true) {
		xpAliveEvent * event = new xpAliveEvent();

		event->setSenderIpPort(device->getDescriptor()->getSenderIpPort());
		event->setSenderSession(device->getDescriptor()->getSenderSession());
		event->setSenderType(device->getDescriptor()->getType());
		event->setSenderName(device->getDescriptor()->getName());

		pthread_mutex_lock(&(device->listenersMutex));

		for (unsigned int i = 0; i < device->eventListeners.size(); i++) {
			xpEventListenerProxy *listener = (xpEventListenerProxy*)device->eventListeners[i];

			if ( difftime(time(NULL), listener->getTimestamp()) > xpNetwork::getInstance()->getAnnounceInterval() * 2) {
				fprintf(stderr, "\nRemoving the following dead listener:\n   %s\n", listener->getSenderSession().c_str());
				device->eventListeners.erase(device->eventListeners.begin() + (i));
			} else {
				event->setRecipientIpPort(listener->getSenderIpPort());
				event->setRecipientSession(listener->getSenderSession());
//				fprintf(stderr, "Sending alive event to %s...\n", listener->getSenderSession().c_str());
				listener->newEvent(event);
			}
		}
		pthread_mutex_unlock(&(device->listenersMutex));

#ifdef WIN32
		Sleep(xpNetwork::getInstance()->announceInterval * 1000);
#else
		sleep((int)(xpNetwork::getInstance()->getAnnounceInterval()));
#endif
	}

}

bool xpDeviceImpl::sendAction(xpAction *action) {

	if(action->getType() & XP_ALIVE) {
		pthread_mutex_lock(&listenersMutex);
		if ((int) eventListeners.size() > 0) {
			std::vector<xpEventListener*>::iterator it;
			for ( it=eventListeners.begin() ; it != eventListeners.end(); it++ ) {
				xpEventListenerProxy *listener = (xpEventListenerProxy*)*it;
				if (listener->getSenderSession().compare(action->getSenderSession()) == 0) {
					listener->setTimestamp();
					break;
				}
			}
		}
		pthread_mutex_unlock(&listenersMutex);
	}

	actionReceived(action);

	return true;
}

void xpDeviceImpl::notifyListeners(xpEvent *event) {

	event->setSenderIpPort(getDescriptor()->getSenderIpPort());
	event->setSenderSession(getDescriptor()->getSenderSession());
	event->setSenderType(getDescriptor()->getType());
	event->setSenderName(getDescriptor()->getName());

	pthread_mutex_lock(&listenersMutex);
	if ((int) eventListeners.size() > 0) {
//		fprintf(stderr, "Notify to %d listeners\n", (int)eventListeners.size());

		std::vector<xpEventListener*>::iterator it;

		for ( it=eventListeners.begin() ; it != eventListeners.end(); it++ ) {
			xpEventListenerProxy *listener = (xpEventListenerProxy*)*it;
//			fprintf(stderr, "Notify event type %ld \n", event->getType());
			if (event->getType() & listener->getEventTypeMask()) {
				event->setRecipientIpPort(listener->getSenderIpPort());
				event->setRecipientSession(listener->getSenderSession());
				listener->newEvent(event);
//				fprintf(stderr, "Notify event to %s %s\n", listener->getSenderIpPort().c_str(), listener->getSenderSession().c_str());
			}

		}
	}
	pthread_mutex_unlock(&listenersMutex);

	//TODO Device Impl: Se il programmatore finale crea un oggetto event, deve avere il controllo su esso, per cui non è possibile invocare la distruzione di event in libreria
	//delete event;

}

bool xpDeviceImpl::addEventListener(xpEventListener *event_listener, long int event_mask) {
	pthread_mutex_lock(&listenersMutex);
	event_listener->setTimestamp();
	eventListeners.push_back(event_listener);
	fprintf(stderr, "\n%s added the following listener:\n\tip:port %s\n\tsession %s\n\tevent type(s) %ld\n\n", getDescriptor()->getName().c_str(), event_listener->getSenderIpPort().c_str(), event_listener->getSenderSession().c_str(), event_listener->getEventTypeMask());
	pthread_mutex_unlock(&listenersMutex);
	return true;
}

bool xpDeviceImpl::removeEventListener(xpEventListener *eventListener) {

	pthread_mutex_lock(&listenersMutex);
	for (unsigned int i = eventListeners.size() - 1; i >= 0; i--) {
		if (eventListeners[i]->getSenderSession().compare(eventListener->getSenderSession()) == 0) {
			eventListeners.erase(eventListeners.begin() + (i));
			fprintf(stderr, "\n123Listener removed! %s\n", eventListener->getSenderSession().c_str());
			pthread_mutex_unlock(&listenersMutex);
			return true;
		}
	}
	pthread_mutex_unlock(&listenersMutex);
	return false;
}
