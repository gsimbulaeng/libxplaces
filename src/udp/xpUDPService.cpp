/*
 * xpUDPService.cpp
 *
 *  Created on: Jul 6, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpUDPService.h"
#include "../xpNetwork.h"
#include <iostream>


#define BUFLEN 512

using namespace std;

xpUDPService::xpUDPService(std::string ip_port) {


	xpAddress myAddress(ip_port, "");

	sockaddr_in address = myAddress.toSocketAddress();

	counter = 0;
	lastCheck = time(NULL);


	fprintf(stdout,"xpNetwork runs on %s\n", xpNetwork::getInstance()->getIpPort().c_str());
	fprintf(stdout,"UDP Server runs on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

#ifdef WIN32

	WSADATA WinsockData;
	fprintf(stdout,"Starting up WinSock Service\n");
	if (WSAStartup(MAKEWORD(2, 2), &WinsockData) != 0) {
		        std::cout << "Failed to find Winsock 2.2!" << std::endl;
		        return;
		    }
	this->ReceiveSock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, 0);
	if (ReceiveSock == INVALID_SOCKET) {
		std::cout << "Failed to get a socket Receiver. Error " << WSAGetLastError() <<std::endl;
		return ;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(ReceiveSock, (SOCKADDR *) &address, sizeof(address));

#else
	if ((socket_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))== -1) {
		 perror("error while opening socket");
		 exit(1);
	}
	if (bind(socket_id, (struct sockaddr *) &address, sizeof(address)) == -1) {
			 perror("error while binding");
			 exit(1);
		}
#endif

	 pthread_t tid;
	 pthread_create(&tid, NULL, (void*(*)(void*))handle_requests, (void*)this);
}

xpUDPService::~xpUDPService() {
	// TODO Auto-generated destructor stub
}

void *xpUDPService::handle_requests(void *p) {


	xpUDPService *service = (xpUDPService*) p;

	while(true) {
		sockaddr remote;
		socklen_t address_length = sizeof(remote);
		char datagram[4096];

#ifdef WIN32

		int recBytes;
		if(recBytes = recvfrom(service->ReceiveSock, datagram, 4096, 0, (SOCKADDR *)&remote,
			&address_length)!= 0){
#else
		if (recvfrom(service->socket_id, datagram, 4096, 0, &remote, &address_length) != -1) {
#endif
			xpPropertyList pl(datagram);

			xpNetwork::getInstance()->dispatcher(pl);
		}
	}

	return NULL;
}

int xpUDPService::announce(xpMessage device){
	device.setMessageType(XP_ANNOUNCE);
	xpAddress address(device.getRecipientIpPort(), device.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = device.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}

int xpUDPService::action(xpMessage action){
	action.setMessageType(XP_ACTION);
	xpAddress address(action.getRecipientIpPort(), action.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = action.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}

int xpUDPService::event(xpMessage event){
	event.setMessageType(XP_EVENT);
	xpAddress address(event.getRecipientIpPort(), event.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = event.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}

int xpUDPService::addEventListener(xpMessage device, long type){
	device.setMessageType(XP_ADD_LISTENER);
	xpAddress address(device.getRecipientIpPort(), device.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = device.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}

int xpUDPService::removeEventListener(xpMessage device){
	device.setMessageType(XP_REMOVE_LISTENER);
	xpAddress address(device.getRecipientIpPort(), device.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = device.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}


int xpUDPService::sendDatagram(struct sockaddr_in destination, const char* datagram) {
	int slen = sizeof(destination);

	uint32_t length = *((uint32_t*)datagram);

	length = xpToolSet::toLocalEndianess(length);

#ifdef WIN32
	SOCKET SendSock;
	WSADATA WinsockData;
	if (WSAStartup(MAKEWORD(2, 2), &WinsockData) != 0) {
		        std::cout << "Failed to find Winsock 2.2!" << std::endl;
		        return -1;
		    }
	SendSock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
	if (SendSock == INVALID_SOCKET) {
		std::cout << "Failed to get a socket Sender. Error " << WSAGetLastError() <<std::endl;
		return -1;
	}

	if (sendto(SendSock, datagram, length, 0, (struct sockaddr *) &destination, slen) == -1) {
			fprintf(stdout, "Error while sending datagram\n");
			return -2;
		}

	closesocket(SendSock);

#else
	int s = sizeof(destination);
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {

		fprintf(stderr, "Error while opening socket\n");
		return -1;
	}

	if (sendto(s, datagram, length, 0, (struct sockaddr *) &destination, slen) == -1) {
		fprintf(stderr, "Error while sending datagram\n");
		return -2;
	}

	close(s);
#endif
	return 0;
}

void xpUDPService::checkCounter() {
	counter++;
	double diff = difftime(time(NULL), lastCheck) ;
	if (diff > 2.0) {
		fprintf(stderr, "Events forwarded/sec %f \n", counter/diff);

		lastCheck = time(NULL);
		counter = 0;
	}
}


