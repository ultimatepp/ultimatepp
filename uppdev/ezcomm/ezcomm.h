// /***************************************************************************
//                           ezcomm  -  description
//                              -------------------
//     begin                : Dec. 11, 2007
//     copyright            : (C) 2007 by Allen
//     email                : bon_ami_@hotmail.com
//  ***************************************************************************/
// 
// /***************************************************************************
//  *                                                                         *
//  *   Explicit Distribution Limitation                                      *
//  *   This rule overrides others below.                                     *
//  *   This program may not be modified or used by, or, if possible,         *
//  *   redistributed to people described as below,                           *
//  *   1.Japanese who hold hostility against Chinese.                        *
//  *   2.or, those who discriminate against people based solely on race,     *
//  *     gender or sexual orientation.                                       *
//  *                                                                         *
//  ***************************************************************************/
// /***************************************************************************
//  *                                                                         *
//  *   This program is free software; you can redistribute it and/or modify  *
//  *   it under the terms of the GNU General Public License as published by  *
//  *   the Free Software Foundation; either version 2 of the License, or     *
//  *   (at your option) any later version.                                   *
//  *                                                                         *
//  ***************************************************************************/
// /*
//  *	header of EZComm Project common functionalities
//  */
// 

#ifndef EZCOMM_H_HEADER_INCLUDED_B8A19115
#define EZCOMM_H_HEADER_INCLUDED_B8A19115

#include <string>

#ifdef ULTIMATEPP
#include "winsock2.h"
#include <ezcommon/ezcommon.h>

typedef SOCKET socketIDs;
typedef WSAEVENT localEvents;
#else
#include <sys/socket.h>
#include <ezproject/ezcommon.h>

typedef int socketIDs;
#endif

namespace ezproject {

class ezcommSocket
{
public:
	ezcommSocket() { memset(&sin, 0, sizeof(sin)); sin.sin_family = AF_INET; };
//	ezcommSocket(const ezcommSocket &dup) { memcpy(&sin, dup.sock(), sizeof(sin)); };
	static int size() { return sizeof(struct sockaddr_in); };
	void set(std::string ip, int port);
	std::string ip() const;
	int port() const;
	struct sockaddr *sock()
		{ return reinterpret_cast<struct sockaddr *>(&sin); };

private:
	struct sockaddr_in sin;
};

class ezComm;
enum eaction
{
//	ACTION_DISCONNECTED,
	ACTION_DISCONNECT,
	ACTION_RECEIVE,
	ACTION_TRANSMIT,
	ACTION_ESTABLISH,
	ACTION_SERVE,
	ACTION_MAX
};
typedef void (*ezCallbacks)(void * holder, enum eaction, ezComm *ezcommNew,
	std::pair<ezcommSocket *, ezcommSocket *>, std::string buf);


//##ModelId=46A05E3A0213
class ezComm
{
public:
	std::string ip() const;
	int port() const;

	// return value shows whether it fails or disconnected naturally.
	// this operation blocks to receive network data
	//##ModelId=46A08411038A
	virtual erttp establish(ezcommSocket* remote = NULL) = 0;

	// this blocks procedure to accept incoming connections
	//##ModelId=46A0843D0290
	virtual erttp serve() throw(std::string) = 0;

	//##ModelId=46A084620186
	void disconnect();
#if 0
	//##ModelId=46A084900290
	void neglect();
#endif
	//##ModelId=46A0849D00DA
	void transmit(std::string buf, ezcommSocket* remote = NULL);

	//##ModelId=474D1D4D0251
	ezComm(ezCallbacks callback, void * cbh, ezcommSocket* local = NULL)
		: callbacks(callback)
		{ cbHolder = cbh; localSocket = local; remoteSocket = NULL;
			socketStatus = SOCKET_CLOSED; buffSize = 256; maxConn = 1;
				/*localEventAccept = */localEventRead = localEventDisconnect
					= localEventTransmit = WSA_INVALID_EVENT; };

	//##ModelId=475E4617038A
	~ezComm() { delete localSocket; delete remoteSocket;
		if (socketID != INVALID_SOCKET) closesocket(socketID);
		if (WSA_INVALID_EVENT != localEventDisconnect)
		{ WSACloseEvent(localEventDisconnect); localEventDisconnect = WSA_INVALID_EVENT; }
		if (WSA_INVALID_EVENT != localEventTransmit)
		{ WSACloseEvent(localEventTransmit); localEventTransmit = WSA_INVALID_EVENT; }
		if (WSA_INVALID_EVENT != localEventRead)
		{ WSACloseEvent(localEventRead); localEventRead = WSA_INVALID_EVENT; }
/*		if (WSA_INVALID_EVENT != localEventDisconnected)
		{ WSACloseEvent(localEventDisconnected); localEventDisconnected = WSA_INVALID_EVENT; }*/};

protected:
	//##ModelId=474D20BA0148
	ezComm(ezCallbacks callback, void *cbh, socketIDs socket,
		ezcommSocket* local, ezcommSocket* remote)
		: callbacks(callback), socketID(socket)
		{ cbHolder = cbh; localSocket = local; remoteSocket = remote;
			socketStatus = SOCKET_CLOSED; buffSize = 256; maxConn = 1;
			/*localEventAccept = */localEventRead = localEventDisconnect
				= localEventTransmit = WSA_INVALID_EVENT; };

	//##ModelId=46A06F54036B
	ezCallbacks callbacks;
	void *cbHolder;
	//##ModelId=46A0785602CE
	//##ModelId=474CFF0C03A9
	//##ModelId=46A078E400FA
	ezcommSocket* localSocket;
	//##ModelId=46A07C400261
	ezcommSocket* remoteSocket;
	//##ModelId=474D15CD033C
	socketIDs socketID;
	//##ModelId=475E461702AF
	enum
	{
		SOCKET_CLOSED,
		SOCKET_OPENED,
		SOCKET_CONNECTED,
		SOCKET_LISTENING
	} socketStatus;
	//##ModelId=474D1C5400AB
	size_t buffSize;
	unsigned char maxConn;
	//localEvents localEventAccept;
	localEvents localEventRead;		//for recv(), recvfrom(), and accept()
	//##ModelId=475E057B006D
	localEvents localEventDisconnect;
	//##ModelId=475E061E0186
//	localEvents localEventDisconnected;
	//##ModelId=475E062702EE
	localEvents localEventTransmit;
	//##ModelId=475E0640004E
	std::string localEventString;

private:
	//##ModelId=475E46180000
	ezComm &operator=(const ezComm &sth);	//we just don't need this
	//##ModelId=475E46180177
	ezComm(const ezComm &sth);	//we just don't need this
};


}	/*	namespace ezproject	*/


#endif /* EZCOMM_H_HEADER_INCLUDED_B8A19115 */
