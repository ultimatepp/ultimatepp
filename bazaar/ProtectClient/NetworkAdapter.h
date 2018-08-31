#ifndef _NetworkAdapter_h_
#define _NetworkAdapter_h_

#include <Core/Core.h>

NAMESPACE_UPP

struct NetworkAdapter : Moveable<NetworkAdapter>
{
	String description;
	String fullname;
	String mac;
	String type;
};

Vector<NetworkAdapter> GetNetworkAdaptersInfo(void);

END_UPP_NAMESPACE

#endif
