/***************************************************************************
                          ezudp  -  description
                             -------------------
    begin                : Aug. 2, 2007
    copyright            : (C) 2007 by Allen
    email                : bon_ami_@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   Explicit Distribution Limitation                                      *
 *   This rule overrides others below.                                     *
 *   This program may not be modified or used by, or, if possible,         *
 *   redistributed to people described as below,                           *
 *   1.Japanese who hold hostility against Chinese.                        *
 *   2.or, those who discriminate against people based solely on race,     *
 *     gender or sexual orientation.                                       *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
 *	source code of EZ Comm Project UDP functionalities
 */

//#include <sstream>
//#include <ext/numeric>
#include "ezudp.h"
/*#include <stdio.h>
#include <stdlib.h>
#ifdef OLD_IOSTREAM
#include <iostream.h>
#else
#include <iostream>
#endif
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>*/

using namespace ezproject;

void ezUdp::relate(void) throw(std::string)
{
	if (socketStatus != SOCKET_CLOSED)
		return;
		/*throw std::string("socket not initialized!");	/* already bound? */

	socketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	socketStatus = SOCKET_OPENED;
	if (!localSocket)
	{
	/*	localSocket = new ezcommSocket;
		int port;

		do
		{
			localSocket->set("", port);
		} while (bind(socketID, localSocket->sock(), localSocket->size()) < 0);
		if (0)
	*/		
		closesocket(socketID);
		socketID = INVALID_SOCKET;
		throw std::string("no available socket!");
	}
	else //if (bind(socketID, localSocket->sock(), localSocket->size()) < 0)
	{
		closesocket(socketID);
		socketID = INVALID_SOCKET;
		std::string ms("failed to bind to ");
		//std::stringstream msg;
		std::string str("failed to bind to ");

//		str << "failed to bind to " << localSocket->ip() << ":"
//		/*	<< localSocket->port()*/;
	/*	std::string port_str;
		iota(str.begin(), str.end(), &port_str);
		str += port_str;*/
		/*msg += localSocket->ip();
		msg += ":";*/
		//str = msg.str();
		throw std::string(str);//std::string(msg.str());
	}
}

erttp ezUdp::establish(ezcommSocket* remote)
{
	bool sockExisted = false;

	try
	{
		relate();
	}
	catch (std::string err)
	{
		throw err;
	}
	remoteSocket = remote;
	if (socketStatus != SOCKET_CONNECTED)
	{
		socketStatus = SOCKET_CONNECTED;
	}
	else
		sockExisted = true;

	/* events */
	localEventDisconnect = WSACreateEvent();
	localEventTransmit = WSACreateEvent();
	localEventRead = WSACreateEvent();
	WSAEventSelect(socketID, localEventRead, FD_READ);

	const unsigned char offset = 0;
	const unsigned char quantity = 3;
	WSAEVENT evtArray[quantity];
	memset(evtArray, 0, sizeof(evtArray));
	evtArray[ACTION_RECEIVE - offset] = localEventRead;
	evtArray[ACTION_TRANSMIT - offset] = localEventTransmit;
	evtArray[ACTION_DISCONNECT - offset] = localEventDisconnect;

	DWORD evtIndex;
	ezcommSocket rcvSocket;
	char *buf = static_cast<char *>(malloc(buffSize));
	int bufLen;
	int socketLen = rcvSocket.size();

	/* ready */
	std::pair<ezcommSocket*, ezcommSocket*> sockPair(localSocket, remoteSocket);

	callbacks(cbHolder, ACTION_ESTABLISH, NULL, sockPair, "");

	while (INVALID_SOCKET != socketID)
	{
		evtIndex = WSAWaitForMultipleEvents(quantity, evtArray, FALSE,
			WSA_INFINITE, FALSE);
		if (WSA_WAIT_FAILED == evtIndex)
		{
			closesocket(socketID);
			throw WSAGetLastError();
		}
		evtIndex -= WSA_WAIT_EVENT_0;
		WSAResetEvent(evtArray[evtIndex]);
		switch (evtIndex)
		{
			case ACTION_RECEIVE - offset:
				bufLen = recvfrom(socketID, buf, buffSize - 1, 0,
					rcvSocket.sock(), &socketLen);
				if (bufLen >= 0)
				{
					buf[bufLen] = '\0';
					sockPair.second = &rcvSocket;
					callbacks(cbHolder, ACTION_RECEIVE, NULL, sockPair, buf);
				}
				break;
			case ACTION_TRANSMIT - offset:
				bufLen = sendto(socketID, localEventString.c_str(),
					localEventString.length(), 0,
					remoteSocket->sock(), remoteSocket->size());
				sockPair.second = remoteSocket;
				if (SOCKET_ERROR == bufLen)
				{
					closesocket(socketID);
					socketID = INVALID_SOCKET;
					throw WSAGetLastError();
				}
				callbacks(cbHolder, ACTION_TRANSMIT, NULL, sockPair, "");
				break;
			case ACTION_DISCONNECT - offset:
				if (SOCKET_ERROR == closesocket(socketID))
					throw WSAGetLastError();
				socketID = INVALID_SOCKET;
				break;
			default:
				break;
		}
	}
	
	free(buf);

	return RTTP_RGHT;
}

erttp ezUdp::serve(void) throw(std::string)
{
	try
	{
		relate();
	}
	catch (std::string err)
	{
		throw err;
	}

	socketStatus = SOCKET_LISTENING;

	/* events */
	localEventDisconnect = WSACreateEvent();
	localEventRead = WSACreateEvent();
	WSAEventSelect(socketID, localEventRead, FD_READ);

	const unsigned char offset = 0;
	unsigned char quantity = 2;
	WSAEVENT evtArray[2];
	memset(evtArray, 0, sizeof(evtArray));
	evtArray[ACTION_RECEIVE - offset] = localEventRead;
	evtArray[ACTION_DISCONNECT - offset] = localEventDisconnect;

	DWORD evtIndex;
	ezcommSocket rcvSocket;
	char *buf = static_cast<char *>(malloc(buffSize));
	int bufLen;
	int socketLen = rcvSocket.size();

	/* ready */
	std::pair<ezcommSocket*, ezcommSocket*> sockPair(localSocket, remoteSocket);

	callbacks(cbHolder, ACTION_SERVE, NULL, sockPair, "");

	while (INVALID_SOCKET != socketID)
	{
		evtIndex = WSAWaitForMultipleEvents(quantity, evtArray, FALSE,
			WSA_INFINITE, FALSE);
		if (WSA_WAIT_FAILED == evtIndex)
		{
			closesocket(socketID);
			throw WSAGetLastError();
		}
		evtIndex -= WSA_WAIT_EVENT_0;
		WSAResetEvent(evtArray[evtIndex]);
		switch (evtIndex)
		{
			case ACTION_RECEIVE - offset:
				bufLen = recvfrom(socketID, buf, buffSize - 1, 0,
					rcvSocket.sock(), &socketLen);
				if (bufLen >= 0)
				{
					buf[bufLen] = '\0';
					sockPair.second = &rcvSocket;
					
					ezcommSocket *sockrmt = new ezcommSocket(*(sockPair.second));
					ezcommSocket *socklcl = new ezcommSocket(*(sockPair.first));
					ezUdp *ezcommNew = new ezUdp(callbacks, cbHolder, socketID,
						socklcl, sockrmt);
					WSACloseEvent(localEventRead);
					localEventRead = WSA_INVALID_EVENT;
					quantity--;
					callbacks(cbHolder, ACTION_ESTABLISH, ezcommNew, sockPair, buf);
				}
				break;
			case ACTION_DISCONNECT - offset:
				if (quantity > 1)
				{
					if (SOCKET_ERROR == closesocket(socketID))
						throw WSAGetLastError();
				}
				socketID = INVALID_SOCKET;
				break;
			default:
				break;
		}
	}

	free(buf);

	return RTTP_RGHT;
}
