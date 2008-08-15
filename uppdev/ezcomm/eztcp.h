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

#ifndef EZTCP_H
#define EZTCP_H

#include "ezcomm.h"

namespace ezproject {

class ezTcp : public ezComm
{
public:
	ezTcp(ezCallbacks callback, void *cbh, ezcommSocket* local = NULL)
		: ezComm(callback, cbh, local) {};
	erttp establish(ezcommSocket* remote = NULL);
	erttp serve() throw(std::string);

private:
	ezTcp(ezCallbacks callback, void *cbh, socketIDs socket,
		ezcommSocket* local, ezcommSocket* remote)
		: ezComm(callback, cbh, socket, local, remote)
		{ socketStatus = SOCKET_CONNECTED; };
	void relate() throw(std::string);
	ezTcp &operator=(const ezTcp &sth);	//we just don't need this
	ezTcp(const ezTcp &sth);	//we just don't need this
};


}	/*	namespace ezproject	*/

#endif
