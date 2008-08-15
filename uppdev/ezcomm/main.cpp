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
 *	source code of EZ Comm Project Ultimate++ functionalities
 */

#include "main.h"

#include "time.h"

#define IMAGEFILE <ezcomm/ezcomm.iml>
#include <Draw/iml_source.h>

bool ezcommwin::HotKey(dword key)
{
	switch (key)
	{
		case K_ENTER:
		case K_CTRL_ENTER:
			Butt_out();
			return true;
	}
	return TopWindow::HotKey(key);
}

void ezcommwin::kill_all(void)
{
	std::list<ezproject::ezComm*>::iterator pos;

	for (pos = ezcomm.begin(); pos != ezcomm.end(); pos++)
	{
		(*pos)->disconnect();
		/* thread must release *pos */
	}
}

void ezcommwin::WClose()
{
	/* validate controls */
	port_rmt.Clear();
	port_lcl.Clear();

	/* resource release */
	kill_all();
	WSACleanup();

	/* go on termination */
	Close();
}

using namespace ezproject;

ezcommwin::ezcommwin()
{
	/* interface */
	CtrlLayout(*this, "EZ Comm");
	Icon(IMAGECLASS::Get("ico"), IMAGECLASS::Get("ico"));
	arr_r.SetImage(IMAGECLASS::Get("arrow_r"));
	arr_l.SetImage(IMAGECLASS::Get("arrow_l"));
	arr_o.SetImage(IMAGECLASS::Get("arrow_r"));
	arr_i.SetImage(IMAGECLASS::Get("arrow_l"));
	port_rmt.Min(1);
	port_rmt.Max(65535);
	port_lcl.Min(1);
	port_lcl.Max(65535);
	comm_type.Set(0, COMM_TYPE_UDP, "UDP");
	comm_type.Set(1, COMM_TYPE_TCP, "TCP");
	hist_addr_in.Hide();
	hist_addr_in.SetDropLines(HIST_SOCK_DROP_NUM);
	comm_type.Tip("select communication method");
	msg.Add("Have a nice day.");
	msg.Add("Bonjour.");
	msg.Add("Salut");
	msg.Add("hallo");
	msg.Add("ciao");
	msg.Add("hola");
	msg.Add("written by Allen");
	msg.Add("a sub-project of EZ Project");
	msg.Add("http://ezproject.sourceforge.net/");
	msg.Add("Windows or Linux, Text or Graphical.");
	msg.Add("Anti-Discrimination");
	msg.Add("cannot be used by Japanese who hold hostility against Chinese");
	msg.Add("cannot be used by those who have race discrimination");
	msg.Add("cannot be used by those who have gender discrimination");
	msg.Add("cannot be used by those who have sexual orientation discrimination");
	info.NoSb();
	disp(ezoi::OILVL_NA, "under GPL with\n a couple of exception rules");
	changes = 0;
	max_key = 0;

	ip_rmt.SetData("192.168.4.176");
	port_rmt.SetData(5070);
	port_lcl.SetData(5060);
	text_out.SetData("testing 123");

	/* callbacks */
	ip_rmt <<= THISBACK(Ip_rmt);
	port_rmt <<= THISBACK(Port_rmt);
	hist_text_in <<= THISBACK(Hist_text_in);
	port_lcl <<= THISBACK(Port_lcl);
	ip_lcl <<= THISBACK(Ip_lcl);
	hist_text_out.WhenAction = THISBACK(Hist_text_out);
	butt_clr <<= THISBACK(Butt_clr);
	butt_out <<= THISBACK(Butt_out);
	butt_in <<= THISBACK(Butt_in);
	WhenClose = THISBACK(WClose);

	/* styles */
	lowlight_style = EditField::StyleDefault();
	highlighted_ctrl = NULL;
	
	/* members */
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2,2), &wsaData);
}

void ezcommwin::Ip_rmt()
{
	/*if ((COMM_TYPE_TCP == comm_type) && !ezcomm.empty())
	{
		std::list<std::pair<std::string, int>*>::iterator pos;
		for (pos = ip_port_rmt.begin(); pos != ip_port_rmt.end(); pos++)
		{
			if ((*pos)->first == ip_rmt.GetData())
			     break;
		}
		if (ip_port_rmt.end() == pos)
		{
			disp(ezoi::OILVL_WARN, "select a connected client!");
			ip_rmt.Clear();
			highlight_ctrl(&ip_rmt);
			return;
		}
	}*/

	lowlight_ctrl(&ip_rmt);
	changes |= CHANGES_RMT;
}

void ezcommwin::Port_rmt()
{
	/*if ((COMM_TYPE_TCP == comm_type) && !ezcomm.empty())
	{
		std::list<std::pair<std::string, int>*>::iterator pos;
		for (pos = ip_port_rmt.begin(); pos != ip_port_rmt.end(); pos++)
		{
			if ((*pos)->second == port_rmt.GetData())
			     break;
		}
		if (ip_port_rmt.end() == pos)
		{
			disp(ezoi::OILVL_WARN, "select a connected client!");
			port_rmt.Clear();
			highlight_ctrl(&port_rmt);
			return;
		}
	}*/
	
	lowlight_ctrl(&port_rmt);
	changes |= CHANGES_RMT;
}

void ezcommwin::Port_lcl()
{
	lowlight_ctrl(&port_lcl);
	changes |= CHANGES_LCL;
}

void ezcommwin::Ip_lcl()
{
	changes |= CHANGES_LCL;
}

void ezcommwin::Hist_text_out()
{
	text_out.Set(hist_text_out.GetValue().ToString());
	text_out.SetFocus();
}

void ezcommwin::Hist_text_in()
{
	mutex_hist.Enter();
	int a= hist_text_in.GetIndex();
	int b=hist_text_in.GetKey(a);
	int c=sock_text_map[b];
	int indx = sock_text_map[static_cast<int>
		(hist_text_in.GetKey(hist_text_in.GetIndex()))];

	ip_src.SetData(ip_port_rmt.at(indx)->first.c_str());
	port_src.SetData(ip_port_rmt.at(indx)->second);
	text_in.Set(hist_text_in.GetValue(hist_text_in.GetIndex()).ToString());
	mutex_hist.Leave();
}

void ezcommwin::highlight_ctrl(EditField *ctrl)
{
	lowlight_ctrl(NULL);
	if (ctrl)
	{
		EditField::Style highlight_style = lowlight_style;
		highlight_style.paper = Red();
		ctrl->SetStyle(highlight_style);
		ctrl->SetFocus();
	}
	highlighted_ctrl = ctrl;
}

/*
parameter:	ctrl: NULL: reset highlighted one; non-NULL: reset the control
	if it is highlighted
*/
void ezcommwin::lowlight_ctrl(EditField *ctrl)
{
	if (highlighted_ctrl)
	{
		if (!ctrl || (ctrl == highlighted_ctrl))
		{
			highlighted_ctrl->SetStyle(lowlight_style);
			highlighted_ctrl = NULL;
			if (ctrl)
				Butt_clr();
		}
	}
}

void ezcommwin::disp(ezoi::eoilvl lvl, const char *cnt)
{
	const char * decorations = "";
	std::string lbl("[7@4A=");
	static time_t previous_time = 0;

	switch (lvl)
	{
		case ezoi::OILVL_FATAL:
			decorations = "*_";
			break;
		case ezoi::OILVL_WARN:
			decorations = "*d";
			break;
		case ezoi::OILVL_INFO:
			decorations = "";
			break;
		case ezoi::OILVL_NA:
			decorations = "/";
			break;
	}
	lbl.append(decorations);
	if (info.GetData() == cnt)
		lbl.append("o >");
	else
		lbl.append(" ");
	lbl.append(cnt);
	lbl.append("]");
	info.SetData(lbl.c_str());
	info.Info(cnt);

	/* user-friendly delay */
	time_t current_time = time(NULL);
	const time_t min_time = 1000;
	if (previous_time && current_time
		&& ((current_time - previous_time) < min_time / 1000))
		Sleep(min_time);
	previous_time = time(NULL);
}

bool ezcommwin::create_sockets(std::pair<ezproject::ezcommSocket *,
	ezproject::ezcommSocket *> &sockPair)
{
	String ip;
	int port;
	sockPair.first = new(ezcommSocket);

	ip = ip_lcl.GetData();
	if (port_lcl.GetLength())
		port = port_lcl.GetData();
	else
		port = 0;
	try
	{
		sockPair.first->set(ip, port);
	}
	catch (std::string err)
	{
		err.insert(0, "local address: ");
		disp(ezoi::OILVL_FATAL, err.c_str());
		delete sockPair.first;
		sockPair.first = NULL;
		return false;
	}

	if (!port_rmt.GetLength())
	{
		sockPair.second = NULL;
		return true;
	}

	sockPair.second = new(ezcommSocket);

	ip = ip_rmt.GetData();
	port = port_rmt.GetData();
	try
	{
		sockPair.second->set(ip, port);
	}
	catch (std::string err)
	{
		err.insert(0, "remote address: ");
		disp(ezoi::OILVL_FATAL, err.c_str());
		delete sockPair.first;
		sockPair.first = NULL;
		delete sockPair.second;
		sockPair.second = NULL;
		return false;
	}
	return true;
}

const char *translate_err(int code)
{
		switch (code)
		{
			case WSANOTINITIALISED:
				return "A successful WSAStartup call must occur before using this function.";
			case WSAENETDOWN:
				return "The network subsystem has failed.";
			case WSAEINPROGRESS:
				return "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
			case WSA_NOT_ENOUGH_MEMORY:
				return "Not enough free memory was available to complete the operation.";
			case WSA_INVALID_HANDLE:
				return "One or more of the values in the lphEvents array is not a valid event object handle.";
			case WSA_INVALID_PARAMETER:
				return "The cEvents parameter does not contain a valid handle count.";
			case WSAEACCES:
				return "The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST parameter to allow the use of the broadcast address.";
			case WSAEINVAL:
				return "An unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.";
			case WSAEINTR:
				return "A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.";
			case WSAEFAULT:
				return "The buf or to parameters are not part of the user address space, or the tolen parameter is too small.";
			case WSAENETRESET:
				return "The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.";
			case WSAENOBUFS:
				return "No buffer space is available.";
			case WSAENOTCONN:
				return "The socket is not connected (connection-oriented sockets only.";
			case WSAENOTSOCK:
				return "The descriptor is not a socket.";
			case WSAEOPNOTSUPP:
				return "MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.";
			case WSAESHUTDOWN:
				return "The socket has been shut down; it is not possible to sendto on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.";
			case WSAEWOULDBLOCK:
				return "The socket is marked as nonblocking and the requested operation would block.";
			case WSAEMSGSIZE:
				return "The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.";
			case WSAEHOSTUNREACH:
				return "The remote host cannot be reached from this host at this time.";
			case WSAECONNABORTED:
				return "The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.";
			case WSAECONNRESET:
				return "The virtual circuit was reset by the remote side executing a hard or abortive close. For UPD sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a Port Unreachable ICMP packet. The application should close the socket as it is no longer usable.";
			case WSAEADDRNOTAVAIL:
				return "The remote address is not a valid address, for example, ADDR_ANY.";
			case WSAEAFNOSUPPORT:
				return "Addresses in the specified family cannot be used with this socket.";
			case WSAEDESTADDRREQ:
				return "A destination address is required.";
			case WSAENETUNREACH:
				return "The network cannot be reached from this host at this time.";
			case WSAETIMEDOUT:
				return "The connection has been dropped, because of a network failure or because the system on the other end went down without notice.";
			default:
				return "some code unknown...";
		}
}

void ezcommwin::func_client(void *ezcommPi, void *remoteI)
{
	ezComm *ezcommP = static_cast<ezComm *>(ezcommPi);
	ezcommSocket *remote = static_cast<ezcommSocket *>(remoteI);
	std::string ip = remote->ip();
	int port = remote->port();

	try
	{
		switch (ezcommP->establish(remote))
		{
			case RTTP_RGHT:
				disp(ezoi::OILVL_INFO, "socket closed");
				break;
			default:
				disp(ezoi::OILVL_FATAL, "error returned when establishing");
		}
	}
	catch (std::string err)
	{
		disp(ezoi::OILVL_FATAL, err.c_str());
	}
	catch (int code)
	{
		disp(ezoi::OILVL_FATAL, translate_err(code));
	}

	ezcomm.remove(ezcommP);
	delete ezcommP;

	/* remove peer socket item from list */
	String addr = ip;
	String port_str = ConvertInt().Format(port);
	int index;

	addr += ':';
	addr += port_str;
	
	for (index = 0; index < hist_addr_in.GetCount(); index++)
		if (hist_addr_in.GetValue(index) == addr)
		{
			hist_addr_in.Remove(index);
			const char * const disp_str = "a peer disconnected";
			hist_addr_in.Add(disp_str);
			index = hist_addr_in.FindKey(disp_str);
			hist_addr_in.SetIndex(index);
			hist_addr_in.Refresh();
			hist_addr_in.Remove(index);
			break;
		}
	if (hist_addr_in.GetCount() < 1)
		butt_out.Disable();

	if (ezcomm.empty())
	{
		buf.clear();
		ip_lcl.Enable();
		port_lcl.Enable();
		butt_out.Enable();
		if (COMM_TYPE_TCP == comm_type)
		{
			ip_rmt.Show();
			port_rmt.Show();
			hist_addr_in.Hide();
		}
		butt_in.SetLabel("listen");
		butt_in.Enable();
		comm_type.Enable();
		disp(ezoi::OILVL_INFO, "ready.");
	}
}

void ezcommwin::func_server(void *ezcommPi)
{
	ezComm *ezcommP = static_cast<ezComm *>(ezcommPi);
	try
	{
		switch (ezcommP->serve())
		{
			case RTTP_RGHT:
				disp(ezoi::OILVL_INFO, "server closed");
				break;
			default:
				disp(ezoi::OILVL_FATAL, "error returned when establishing");
		}
	}
	catch (std::string err)
	{
		disp(ezoi::OILVL_FATAL, err.c_str());
	}
	catch (int code)
	{
		disp(ezoi::OILVL_FATAL, translate_err(code));
	}

	ezcomm.remove(ezcommP);
	delete ezcommP;

	if (ezcomm.empty())
	{
		buf.clear();
		ip_lcl.Enable();
		port_lcl.Enable();
		butt_out.Enable();
		if (COMM_TYPE_TCP == comm_type)
		{
			ip_rmt.Show();
			port_rmt.Show();
			hist_addr_in.Hide();
		}
		butt_in.SetLabel("listen");
		butt_in.Enable();
		comm_type.Enable();
		disp(ezoi::OILVL_INFO, "ready.");
	}
}

void cb_exec(void * holder, enum eaction action, ezComm *ezcommNew,
	std::pair<ezcommSocket *, ezcommSocket *> sockPair, std::string info)
{
	ezcommwin *ez = static_cast<ezcommwin *>(holder);
	ez->callbacks(action, ezcommNew, sockPair, info);
}

/*
return value:	0: exhausted
*/
ezcommwin::indexes_t ezcommwin::new_key(void)
{
/*	if (sock_text_map.empty())
		return max_key = 1;*/
	indexes_t key = max_key + 1;
	if (key)	//valid
		return max_key = key;
	for (key = 1; key != 0; key++)
	{
		if (sock_text_map.end() == sock_text_map.find(key))
			return key;
	}
	return 0;
}

void ezcommwin::delete_key(ezcommwin::indexes_t key)
{
	if (key == max_key)
		max_key--;	//more can be done
}

ezcommwin::indexes_t ezcommwin::add_peer(std::pair<ezcommSocket *,
	ezcommSocket *> &sockPair)
{
	std::string ip = sockPair.second->ip();
	String addr = ip;
	int port = sockPair.second->port();
	//ConvertInt ci;
	String port_str = ConvertInt().Format(port);
	int peer_index = -1;
	indexes_t key = new_key();
	std::vector<std::pair<std::string, int>*>::iterator it;

	if (0 == key)
		disp(ezoi::OILVL_WARN, "Too many peers in history!");
	ip_src.SetData(addr);
	addr += ':';
	port_src.SetData(sockPair.second->port());
	addr += port_str;
	switch (comm_type)
	{
		case COMM_TYPE_UDP:
			for (it = ip_port_rmt.begin(); it != ip_port_rmt.end(); it++)
			{
				if (((*it)->first == ip) && ((*it)->second == port))
					break;
				peer_index++;
			}
			if (key)
				sock_text_map.insert(map_t(key, ++peer_index));
			if (ip_port_rmt.end() == it)
				peer_index = -1;
			break;
		case COMM_TYPE_TCP:
			for (; (peer_index + 1) < hist_addr_in.GetCount();)
			{
				if (0 == strcmp(hist_addr_in.GetValue(peer_index
					+ 1).ToString(), addr))
					break;
				peer_index++;
			}
			if ((hist_addr_in.GetCount() == peer_index + 1))
			{	//TCP server must guarantee peer quantity is in boundary
				if (key)
				{
					hist_addr_in.Add(key, addr);
					if (-1 == peer_index)
						hist_addr_in.SetIndex(0);
					sock_text_map.insert(map_t(key,
						static_cast<int>(ip_port_rmt.size())));
				}
				peer_index = -1;
			}
			else
			{
				/* relate new key to old ip_port_rmt's index */
				sock_text_map.insert(map_t(key,
					sock_text_map[static_cast<int>
						(hist_addr_in.GetKey(++peer_index))]));
				hist_addr_in.SetIndex(peer_index);
			}
			break;
	}
	if (peer_index < 0)
	{
		std::pair<std::string, int> *vl
			= new std::pair<std::string, int>(ip, port);
		ip_port_rmt.push_back(vl);
	}
	ip_rmt.Disable();
	addr = ip_rmt.GetData();
	ip_rmt.SetData(String(sockPair.second->ip()));
	ip_rmt.AddHistory(HIST_SOCK_DROP_NUM);
	if (addr.GetLength() > 0)
		ip_rmt.SetData(addr);
	ip_rmt.Enable();
	port_rmt.Disable();
	port = port_rmt.GetData();
	port_rmt.SetData(sockPair.second->port());
	port_rmt.AddHistory(HIST_SOCK_DROP_NUM);
	if (port > 0)
		port_rmt.SetData(port);
	port_rmt.Enable();
	return key;
}

void ezcommwin::callbacks(enum eaction action, ezComm *ezcommNew,
	std::pair<ezcommSocket *, ezcommSocket *> sockPair, std::string info)
{
	int peer_index = -1, port, loop;
	std::list<std::pair<std::string, int>*>::iterator pos;
	indexes_t key;

	switch (action)
	{
		case ACTION_SERVE:
			disp(ezoi::OILVL_INFO, "listening...");
			butt_in.Enable();
			break;
		case ACTION_ESTABLISH:
			if (ezcommNew)
			{
				ezcommSocket *sockNew = new ezcommSocket(*(sockPair.second));
				ezcomm.push_back(ezcommNew);
				thread_client.Run(callback2(this, &ezcommwin::func_client,
					dynamic_cast<void *>(ezcommNew),
					static_cast<void *>(sockNew)));
				if (0 == info.length())
				{
					add_peer(sockPair);
					break;
				}
				/*to display incoming message, fall through */
			}
			else
			{
				if (0 == info.length())
				{
					if (!buf.empty())
					{
						disp(ezoi::OILVL_INFO, "bound and trying sending...");
						std::vector<std::string>::iterator pos = buf.begin();
						ezcomm.front()->transmit(*pos);
						buf.erase(pos);
					}
					else
					{
						disp(ezoi::OILVL_INFO, "connected");
						butt_out.Enable();
						butt_in.Enable();
					}
				}
				else
				{
					disp(ezoi::OILVL_WARN, info.c_str());
					butt_in.Enable();
					butt_out.Enable();
				}
				break;
			}
		case ACTION_RECEIVE:
			/* peer record */
			mutex_hist.Enter();
			key = add_peer(sockPair);

			/* content record */
			text_in.Set(info);
			hist_text_in.Add(key, String(info));

			disp(ezoi::OILVL_INFO, "sth. received");
			mutex_hist.Leave();
			break;
		case ACTION_TRANSMIT:
			if (0 != info.length())
				disp(ezoi::OILVL_WARN, info.c_str());
			else
				disp(ezoi::OILVL_INFO, "sent as commanded");
			butt_in.Enable();
			butt_out.Enable();
			break;
		case ACTION_DISCONNECT:
			break;
/*		case ACTION_NEGLECT:
			break;*/
		case ACTION_MAX:
			disp(ezoi::OILVL_WARN, "invalid action in callback!");
			break;
	}
}

ezComm *ezcommwin::find_ezcomm(void)
{
	ezComm *ret = NULL;
	std::list<ezproject::ezComm*>::iterator pos;
	std::pair<std::string, int> *vl;

	mutex_hist.Enter();
	vl = ip_port_rmt.at(sock_text_map[static_cast<int>
		(hist_addr_in.GetKey(hist_addr_in.GetIndex()))]);
	for (pos = ezcomm.begin(); pos != ezcomm.end(); pos++)
	{
		if (((*pos)->ip() == vl->first)
			&& ((*pos)->port() == vl->second))
		{
			ret = *pos;
			break;
		}
	}
	mutex_hist.Leave();
	return ret;
}

void ezcommwin::Butt_out()
{
	enum {
		BUTT_OUT_RESULT_FAILURE,
		BUTT_OUT_RESULT_SENT_NEWLY,
		BUTT_OUT_RESULT_SENT_OLD
	} result = BUTT_OUT_RESULT_FAILURE;

	if (ezcomm.empty())
	{
		if ((0 == ip_rmt.GetLength()) || (0 == port_rmt.GetLength()))
		{
			disp(ezoi::OILVL_FATAL, "Input destination's IP & port!");
			if (0 == ip_rmt.GetLength())
				highlight_ctrl(&ip_rmt);
			else
				highlight_ctrl(&port_rmt);
		}
		else
		{
			std::pair<ezproject::ezcommSocket *,
				ezproject::ezcommSocket *> sockPair;

			butt_out.Disable();
			butt_in.Disable();
			comm_type.Disable();

			disp(ezoi::OILVL_INFO, "connecting...");
			if (create_sockets(sockPair))
			{
				ezComm *ezcommNew = NULL;

				try
				{
					switch (comm_type)
					{
						case COMM_TYPE_UDP:
							ezcommNew = new ezUdp(cb_exec, this,
								sockPair.first);
							break;
						case COMM_TYPE_TCP:
							ezcommNew = new ezTcp(cb_exec, this,
								sockPair.first);
							break;
					}
					if (ezcommNew)
					{
						ezcomm.push_back(ezcommNew);
						if ((COMM_TYPE_UDP == comm_type)
							|| text_out.GetLength())
							buf.insert(buf.begin(), text_out.Get());
						mutex_hist.Enter();
						thread_client.Run(callback2(this,
							&ezcommwin::func_client,
							dynamic_cast<void *>(ezcommNew),
							static_cast<void *>(sockPair.second)));
		
						butt_in.SetLabel("stop listening");
						ip_lcl.Disable();
						ip_lcl.AddHistory(HIST_SOCK_DROP_NUM);
						port_lcl.Disable();
						port_lcl.AddHistory(HIST_SOCK_DROP_NUM);
						result  = BUTT_OUT_RESULT_SENT_NEWLY;
					}
				}
				catch (std::string err)
				{
					disp(ezoi::OILVL_FATAL, err.c_str());
					delete ezcommNew;
					ezcommNew = NULL;
				}
				if (result != BUTT_OUT_RESULT_FAILURE)
				{
					const char *ip_p = ip_rmt.GetText().ToString();
					int port_conv = port_rmt.GetData();
					/*std::string ip_str = ip_p;*/
					std::pair<std::string, int> *vl
						= new std::pair<std::string, int>(ip_p, port_conv);
					std::vector<std::pair<std::string, int>*>::iterator it;

					for (it = ip_port_rmt.begin(); it != ip_port_rmt.end();
					     it++)
					     delete *it;
					ip_port_rmt.clear();
					sock_text_map.clear();
					hist_text_in.Clear();
					text_in.Clear();
					if (COMM_TYPE_TCP == comm_type)
					{
						String addr = ip_p;
						//hist_addr_in.Clear();
						addr += ':';
						addr += port_rmt.GetText().ToString();
						hist_addr_in.Add(static_cast<int>(ip_port_rmt.size()),
							addr);
						hist_addr_in.SetIndex(0);
						ip_rmt.Hide();
						port_rmt.Hide();
						hist_addr_in.Show();
					}
					ip_port_rmt.push_back(vl);
				}
				else
				{
					delete sockPair.first;
					delete sockPair.second;
					comm_type.Enable();
				}
			}
		}
	}
	else
	{
		switch (comm_type)
		{
			case COMM_TYPE_UDP:
				if ((0 == ip_rmt.GetLength()) || (0 == port_rmt.GetLength()))
				{
					disp(ezoi::OILVL_FATAL, "Input destination's IP & port!");
					if (0 == ip_rmt.GetLength())
						highlight_ctrl(&ip_rmt);
					else
						highlight_ctrl(&port_rmt);
				}
				else
					result = BUTT_OUT_RESULT_SENT_OLD;
				break;
			case COMM_TYPE_TCP:
				if (hist_addr_in.GetValue().ToString().IsEmpty())
					//never reaches here
					disp(ezoi::OILVL_FATAL, "Select destination!");
				else
					result = BUTT_OUT_RESULT_SENT_OLD;
		}
		if (result != BUTT_OUT_RESULT_FAILURE)
		{
			result = BUTT_OUT_RESULT_FAILURE;
			disp(ezoi::OILVL_INFO, "sending...");
			if ((COMM_TYPE_UDP == comm_type) && (changes & CHANGES_RMT))
			{
				String ip;
				int port;
				ezcommSocket *remote = NULL;
	
				remote = new(ezcommSocket);
				ip = ip_rmt.GetData();
				port = port_rmt.GetData();
				try
				{
					remote->set(ip, port);
					ezcomm.front()->transmit(text_out.Get(), remote);
					result  = BUTT_OUT_RESULT_SENT_OLD;
				}
				catch (std::string err)
				{
					disp(ezoi::OILVL_FATAL, err.c_str());
					delete remote;
					remote = NULL;
				}
			}
			else
			{
				ezComm *ezp = NULL;
	
				switch (comm_type)
				{
					case COMM_TYPE_UDP:
						ezp = ezcomm.front();
						break;
					case COMM_TYPE_TCP:
						ezp = find_ezcomm();
				}
				if (ezp)
				{
					ezp->transmit(text_out.Get());
					result  = BUTT_OUT_RESULT_SENT_OLD;
				}
				else
					disp(ezoi::OILVL_FATAL, "No destination detected");
			}
		}
	}

	changes = 0;
	switch (result)
	{
		case BUTT_OUT_RESULT_SENT_NEWLY:
		case BUTT_OUT_RESULT_SENT_OLD:
			ip_rmt.AddHistory(HIST_SOCK_DROP_NUM);
			port_rmt.AddHistory(HIST_SOCK_DROP_NUM);
			if (text_out.GetLength())
			{
				int text_index;

				text_index = hist_text_out.Find(text_out.Get());
				if (text_index < 0)
					hist_text_out.Add(text_out.Get());
				else
					hist_text_out.SetIndex(text_index);
				text_out.SetCursor(0);
			}
			if (BUTT_OUT_RESULT_SENT_NEWLY == result)
				mutex_hist.Leave();
			break;
		case BUTT_OUT_RESULT_FAILURE:
			butt_out.Enable();
			butt_in.Enable();
	}
}

void ezcommwin::Butt_in()
{
	if (!ezcomm.empty())
	{
		butt_in.Disable();
		butt_out.Disable();
		disp(ezoi::OILVL_INFO, "disconnecting...");
		kill_all();
		ip_rmt.Show();
		port_rmt.Show();
		hist_addr_in.Clear();
		hist_addr_in.Hide();
	}
	else
	{
		if (0 == port_lcl.GetLength())
		{
			disp(ezoi::OILVL_FATAL, "Input local port!");
			highlight_ctrl(&port_lcl);
			butt_in.Enable();
		}
		else
		{
			std::pair<ezproject::ezcommSocket *,
				ezproject::ezcommSocket *> sockPair;
			butt_in.Disable();
			if (COMM_TYPE_TCP == comm_type)
				butt_out.Disable();
			comm_type.Disable();
			if (!create_sockets(sockPair))
			{
				butt_in.Enable();
				butt_out.Enable();
				comm_type.Enable();
			}
			else
			{
				ezComm *ezcommNew = NULL;

				try
				{
					switch (comm_type)
					{
						case COMM_TYPE_UDP:
							ezcommNew = new ezUdp(cb_exec, this,
								sockPair.first);
							break;
						case COMM_TYPE_TCP:
							ezcommNew = new ezTcp(cb_exec, this,
								sockPair.first);
							break;
					}
					if (ezcommNew)
					{
						ezcomm.push_back(ezcommNew);
						thread_server.Run(callback1(this,
							&ezcommwin::func_server,
							dynamic_cast<void *>(ezcommNew)));
						butt_in.SetLabel("stop listening");
						if (COMM_TYPE_TCP == comm_type)
						{
							ip_rmt.Hide();
							port_rmt.Hide();
							hist_addr_in.Show();
						}
						disp(ezoi::OILVL_INFO, "listening");
					}
				}
				catch (std::string err)
				{
					disp(ezoi::OILVL_FATAL, err.c_str());
					delete ezcommNew;
					ezcommNew = NULL;
				}
				if (!ezcommNew)
				{
					delete sockPair.first;
					butt_in.Enable();
					butt_out.Enable();
					comm_type.Enable();
				}
			}
		}
	}
}

void ezcommwin::Butt_clr()
{
	disp(ezoi::OILVL_NA, msg.At(rand() % msg.GetCount()));
}

GUI_APP_MAIN
{
	ezcommwin ezcw;

	ezcw.Run();
}

