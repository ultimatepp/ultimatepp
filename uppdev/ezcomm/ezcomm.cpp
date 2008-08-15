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
//  *	source code of EZComm Project common functionalities
//  */
// 

#include "ezcomm.h"

using namespace ezproject;

/*	socket abstraction	*/
void ezcommSocket::set(std::string ip, int port)
{
	if (port < 1)
		throw std::string("invalid port!");

	if (ip.size())
	{
		unsigned long ip_val = inet_addr(ip.c_str());
		
		if (INADDR_NONE == ip_val)
			throw std::string("invalid IP!");

		sin.sin_addr.s_addr = ip_val;
	}
	else
		sin.sin_addr.s_addr = INADDR_ANY;

	sin.sin_port = htons(port);
}

std::string ezcommSocket::ip() const
{
	if (INADDR_ANY == sin.sin_addr.s_addr)
		return "*";

	struct in_addr ad;
	
	ad.s_addr = sin.sin_addr.s_addr;
	return inet_ntoa(ad);
}

int ezcommSocket::port() const
{
	return ntohs(sin.sin_port);
}


/*	communication abstraction */

std::string ezComm::ip() const
{
	if (remoteSocket)
		return remoteSocket->ip();
	else
		return "";
}

int ezComm::port() const
{
	if (remoteSocket)
		return remoteSocket->port();
	else
		return 0;
}

//##ModelId=46A08411038A

//##ModelId=46A0843D0290

//##ModelId=46A084620186
void ezComm::disconnect()
{
	WSASetEvent(localEventDisconnect);
}

//##ModelId=46A084900290
/*void ezComm::neglect()
{
	WSASetEvent(localEventNeglect);
}*/

//##ModelId=46A0849D00DA
void ezComm::transmit(std::string buf, ezcommSocket* remote)
{
	if (remote)
	{
		if (remoteSocket)
			delete remoteSocket;
		remoteSocket = remote;
	}
	localEventString = buf;
	WSASetEvent(localEventTransmit);
}

//##ModelId=474D1D4D0251

//##ModelId=474D20BA0148

