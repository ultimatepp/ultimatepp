/***************************************************************************
                          main  -  description
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
 *	header of EZ Comm Project Ultimate++ functionalities
 */

#ifndef _ezcomm_ezcomm_h
#define _ezcomm_ezcomm_h

#include <string>
#include <utility>
#include <list>
#include <vector>
#include <map>

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ezcomm/ezcomm.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <ezcomm/ezcomm.iml>
#include <Draw/iml_header.h>

#include <ezcommon/ezcommon.h>
#include <ezcomm/ezudp.h>
#include <ezcomm/eztcp.h>

class ezcommwin : public WithezcommwinLayout<TopWindow> {
private:
	enum	// communication type, this must be syncronized with layout(comm_type) control
	{
		COMM_TYPE_UDP,
		COMM_TYPE_TCP
	};
	typedef unsigned char hist_drop_num_t;
	static const hist_drop_num_t HIST_SOCK_DROP_NUM = 8;
	static const hist_drop_num_t HIST_CONT_DROP_NUM = 20;
	EditField *highlighted_ctrl;
	EditField::Style lowlight_style;
	Array<String> msg;
	std::vector<std::pair<std::string, int>*> ip_port_rmt;
	typedef unsigned short indexes_t;	// no control/protection yet
	typedef std::map<indexes_t, indexes_t>::value_type map_t;
	std::map<indexes_t, indexes_t> sock_text_map;
	indexes_t max_key;
	typedef unsigned char changes_t;	// bitset
	changes_t changes;
	static const changes_t CHANGES_TYPE = 1;
	static const changes_t CHANGES_LCL = 2;
	static const changes_t CHANGES_RMT = 4;
	std::list<ezproject::ezComm*> ezcomm;
	std::vector<std::string> buf;

	/* interface */
	void highlight_ctrl(EditField *);
	void lowlight_ctrl(EditField *);
	void disp(ezproject::ezoi::eoilvl, const char *);
	virtual bool HotKey(dword key);
	void WClose();
	indexes_t add_peer(std::pair<ezproject::ezcommSocket *,
		ezproject::ezcommSocket *> &sockPair);
	indexes_t new_key(void);
	void delete_key(indexes_t);

	/* interface callbacks */
	void Ip_rmt();
	void Port_rmt();
	void Hist_text_in();
	void Port_lcl();
	void Ip_lcl();
	void Hist_text_out();
	void Butt_clr();
	void Butt_out();
	void Butt_in();
	
	/* threads and userclass callbacks */
	Thread thread_client;
	Thread thread_server;
	static Mutex mutex_hist;
	void func_client(void *ezcommPi, void *remote);
	void func_server(void *ezcommPi);
	void kill_all(void);
	bool create_sockets(std::pair<ezproject::ezcommSocket *, ezproject::ezcommSocket *>&);
	ezproject::ezComm *find_ezcomm(void);

public:
	void callbacks(enum ezproject::eaction, ezproject::ezComm *commPair,
		std::pair<ezproject::ezcommSocket *, ezproject::ezcommSocket *>, std::string info);
	typedef ezcommwin CLASSNAME;
	ezcommwin();
};
Mutex ezcommwin::mutex_hist;

#endif

