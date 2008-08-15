/***************************************************************************
                          eztcp  -  description
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
 *	header of EZ Comm Project TCP functionalities
 */

#include <iostream>
#include <sstream>
#include "eztcp.h"
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

void ezTcp::relate(void)  throw(std::string)
{
	if (socketStatus != SOCKET_CLOSED)
		return;
		/*throw std::string("socket not initialized!");	/* already bound? */

	socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
	else if (bind(socketID, localSocket->sock(), localSocket->size()) < 0)
	{
		closesocket(socketID);
		socketID = INVALID_SOCKET;
		std::ostringstream msg;
		std::string str("failed to bind to ");

		msg << "failed to bind to " << 88//localSocket->ip() << ":"
		;//	<< localSocket->port();
	/*	std::string port_str;
		iota(str.begin(), str.end(), &port_str);
		str += port_str;*/
		//str = msg.str();
		throw str;
	}
}

erttp ezTcp::establish(ezcommSocket* remote)
{
	bool sockExisted = false;

	remoteSocket = remote;
	if (!remote)
		throw std::string("No remote socket to connect to!");

	if (socketStatus != SOCKET_CONNECTED)
	{
		try
		{
			relate();
		}
		catch (std::string err)
		{
			throw err;
		}
	
		if (connect(socketID, remoteSocket->sock(), remoteSocket->size()) < 0)
		{
			closesocket(socketID);
			socketID = INVALID_SOCKET;
			throw std::string("failed to connect!");
		}

		socketStatus = SOCKET_CONNECTED;
	}
	else
		sockExisted = true;

	/* events */
//	localEventDisconnected = WSACreateEvent();
	//WSAEventSelect(socketID, localEventDisconnected, );
	localEventDisconnect = WSACreateEvent();
	localEventTransmit = WSACreateEvent();
	localEventRead = WSACreateEvent();
	WSAEventSelect(socketID, localEventRead, FD_READ | FD_CLOSE);

	const unsigned char quantity = 3;
	WSAEVENT evtArray[quantity];
	WSANETWORKEVENTS evts;

	memset(evtArray, 0, sizeof(evtArray));
	evtArray[ACTION_RECEIVE] = localEventRead;
	evtArray[ACTION_TRANSMIT] = localEventTransmit;
	evtArray[ACTION_DISCONNECT] = localEventDisconnect;
//	evtArray[ACTION_DISCONNECTED] = localEventDisconnected;

	DWORD evtIndex;
	char *buf = static_cast<char *>(malloc(buffSize));
	int bufLen;

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
			case ACTION_TRANSMIT:
				bufLen = send(socketID, localEventString.c_str(),
					localEventString.length(), 0);
				if (SOCKET_ERROR == bufLen)
				{
					closesocket(socketID);
					socketID = INVALID_SOCKET;
					throw WSAGetLastError();
				}
				callbacks(cbHolder, ACTION_TRANSMIT, NULL, sockPair, "");
				break;
			case ACTION_RECEIVE:
				if (SOCKET_ERROR == WSAEnumNetworkEvents(socketID, localEventRead, &evts))
				{
					closesocket(socketID);
					socketID = INVALID_SOCKET;
					throw WSAGetLastError();
				}
				if (FD_READ == evts.lNetworkEvents)
				{
					bufLen = recv(socketID, buf, buffSize - 1, 0);
					if (bufLen >= 0)
					{
						buf[bufLen] = '\0';
						callbacks(cbHolder, ACTION_RECEIVE, NULL, sockPair, buf);
					}
					break;
				}
				else if (FD_CLOSE != evts.lNetworkEvents)
					break;
				/* CLOSE, fall through */
			//case ACTION_DISCONNECTED:
			case ACTION_DISCONNECT:
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

erttp ezTcp::serve(void) throw(std::string)
{
	try
	{
		relate();
	}
	catch (std::string err)
	{
		throw err;
	}
	
	if (SOCKET_ERROR == listen(socketID, maxConn = 5))
		throw WSAGetLastError();

	socketStatus = SOCKET_LISTENING;

	/* events */
//	localEventDisconnected = WSACreateEvent();
	//WSAEventSelect(socketID, localEventDisconnected, FD_CLOSE);
	localEventDisconnect = WSACreateEvent();
	localEventRead = WSACreateEvent();
	WSAEventSelect(socketID, localEventRead, FD_ACCEPT | FD_CLOSE);

	const unsigned char offset = 0;
	const unsigned char quantity = 2;
	WSAEVENT evtArray[quantity];
	WSANETWORKEVENTS evts;

	memset(evtArray, 0, sizeof(evtArray));
	evtArray[ACTION_RECEIVE - offset] = localEventRead;
	evtArray[ACTION_DISCONNECT - offset] = localEventDisconnect;
//	evtArray[ACTION_DISCONNECTED - offset] = localEventDisconnected;

	DWORD evtIndex;
	socketIDs socketNew;
	ezcommSocket rcvSocket;
	std::string buf;
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
				if (SOCKET_ERROR == WSAEnumNetworkEvents(socketID, localEventRead, &evts))
				{
					closesocket(socketID);
					socketID = INVALID_SOCKET;
					throw WSAGetLastError();
				}
				if (FD_ACCEPT == evts.lNetworkEvents)
				{
					socketNew = accept(socketID, rcvSocket.sock(), &socketLen);
					if (INVALID_SOCKET == socketNew)
						throw WSAGetLastError();
					else
					{
						sockPair.second = &rcvSocket;	//local socket is unknown now
						
						ezcommSocket *sockrmt = new ezcommSocket(*(sockPair.second));
						ezcommSocket *socklcl = new ezcommSocket(*(sockPair.first));
						ezTcp *ezcommNew = new ezTcp(callbacks, cbHolder, socketNew,
							socklcl, sockrmt);
						/*WSACloseEvent(localEventRead);
						localEventRead = WSA_INVALID_EVENT;
						quantity--;*/
						callbacks(cbHolder, ACTION_ESTABLISH, ezcommNew, sockPair, buf);
					}
					break;
				}
				else if (FD_CLOSE != evts.lNetworkEvents)
					break;
				/* CLOSE, fall through */
			//case ACTION_DISCONNECTED - offset:
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

	return RTTP_RGHT;
}
