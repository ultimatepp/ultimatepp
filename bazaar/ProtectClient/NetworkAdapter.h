#ifndef _NetworkAdapter_h_
#define _NetworkAdapter_h_

#include <Core/Core.h>

using namespace Upp;

struct NetworkAdapter : Moveable<NetworkAdapter>
{
	String description;
	String fullname;
	String mac;
	String type;
};

Vector<NetworkAdapter> GetNetworkAdaptersInfo(void);

#endif
