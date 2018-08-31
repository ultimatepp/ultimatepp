#include "NetworkAdapter.h"

#ifdef PLATFORM_POSIX

#include <sys/ioctl.h>
#include <net/if.h>

NAMESPACE_UPP

Vector<NetworkAdapter> GetNetworkAdaptersInfo(void)
{
	Vector<NetworkAdapter> res;

	// this is used to avoid adding duplicate entries
	// with same MAC and differing only by adapter family
	// (posix may return same interface twice for IPV4 and IPV6)
	Index<String>macs;
	
	// Get a socket handle
	int sck = socket(PF_INET, SOCK_DGRAM, 0);
	if(sck < 0)
		return res;
	
	// get a list of all available interfaces
	// as buffer size is unknown, we check results and retry as needed
	int bufSize = 4096;
	struct ifconf ifc =	{ 0	};
	caddr_t buf;
	for(int iTry = 0; iTry < 4; iTry++)
	{
		buf = (caddr_t)new byte[bufSize];
		ifc.ifc_len = bufSize;
		ifc.ifc_buf = buf;

		// get interfaces list, 
		if (ioctl(sck, SIOCGIFCONF, &ifc) < 0)
		{
			// failed, free buffer and leave
			delete[] buf;
			close(sck);
			Cerr() << "Failed to get interfaces list\n";
			return res;
		}
		
		// check if buffer size was big enough
		if(ifc.ifc_len != bufSize)
			break;

		// not big enough, double size and retry
		delete[] buf;
		bufSize *= 2;
		if(iTry >= 3)
		{
			Cerr() << "Max buffer size too small\n";
			close(sck);
			return res;
		}
	}
		
	// iterate through returned data fill result vector
	int nIfaces = ifc.ifc_len / sizeof(struct ifreq);
	struct ifreq *iface = ifc.ifc_req;
	for(int iIface = 0; iIface < nIfaces; iIface++)
	{
		// platform dependent code to get MAC address
		String MAC;
		
// LINUX
#ifdef SIOCGIFHWADDR
		if(ioctl(sck, SIOCGIFHWADDR, iface) < 0)
		{
			// couldn't get MAC, just leave it empty
			continue;
		}
		MAC = Format("%02x:%02x:%02x:%02x:%02x:%02x",
			(byte)iface->ifr_hwaddr.sa_data[0],
			(byte)iface->ifr_hwaddr.sa_data[1],
			(byte)iface->ifr_hwaddr.sa_data[2],
			(byte)iface->ifr_hwaddr.sa_data[3],
			(byte)iface->ifr_hwaddr.sa_data[4],
			(byte)iface->ifr_hwaddr.sa_data[5]
		);

// SOLARIS AND SysVR4 systems
#elif SIOCGENADDR
		if (ioctl(sck, SIOCGENADDR, iface) < 0)
		{
			// couldn't get MAC, just leave it empty
			continue;
		}
		MAC = Format("%02x:%02x:%02x:%02x:%02x:%02x",
			(byte)iface->ifr_enaddr[0],
			(byte)iface->ifr_enaddr[1],
			(byte)iface->ifr_enaddr[2],
			(byte)iface->ifr_enaddr[3],
			(byte)iface->ifr_enaddr[4],
			(byte)iface->ifr_enaddr[5]
		);

// Mac OSX and other BSD
#elif __MACH__ || __NetBSD__ || __OpenBSD__ || __FreeBSD__

		int mib[6];
		mib[0] = CTL_NET;
		mib[1] = AF_ROUTE;
		mib[2] = 0;
		mib[3] = AF_LINK;
		mib[4] = NET_RT_IFLIST;
		mib[5] = if_nametoindex(iface->ifr_name);
		if (mib[5] == 0)
			// nameless interface ? skip
			continue;

		int len = 0;
		if (sysctl(mib, 6, NULL, (size_t*)&len, NULL, 0) != 0)
		{
			// sysctl error, just leave MAC empty
			continue;
		}

		char *macbuf = new char[len];
		if (!macbuf)
		{
			// can't allocat buffer, skip this MAC
			continue;
		}
		if (sysctl(mib, 6, macbuf, (size_t*)&len, NULL, 0) != 0)
		{
			delete[] macbuf;
			continue;
		}

		struct if_msghdr *ifm = (struct if_msghdr *)macbuf;
		struct sockaddr_dl *sdl = (struct sockaddr_dl *)(ifm + 1);
		byte ptr[] = (byte **)LLADDR(sdl);

		MAC = Format("%02x:%02x:%02x:%02x:%02x:%02x",
			ptr[0],
			ptr[1],
			ptr[2],
			ptr[3],
			ptr[4],
			ptr[5]
		);

		delete[] macbuf;
#else
#error OS Distribution Not Recognized
#endif

		// remove MAC addresses of 6 nulls...
		// loopback interface, for example, don't have a MAC
		if(MAC == "00:00:00:00:00:00")
			MAC.Clear();

		// check out if we've already stored this MAC
		// in case, we just skip the interface
		if(!MAC.IsEmpty() && macs.Find(MAC) >= 0)
			continue;
		macs.Add(MAC);

		NetworkAdapter &adapter = res.Add();
		
		// set MAC in result
		adapter.mac = MAC;
		
		// get interface name
		adapter.fullname = iface->ifr_name;
		
		// set interface type from name
		if(adapter.fullname.StartsWith("eth"))
			adapter.type = "ETHERNET";
		else if(adapter.fullname.StartsWith("lo"))
			adapter.type = "SOFTWARE_LOOPBACK";
		else if(adapter.fullname.StartsWith("ppp"))
			adapter.type = "MODEM";		
		else if(adapter.fullname.StartsWith("hci"))
			adapter.type = "BLUETOOTH";
		else if(adapter.fullname.StartsWith("tr"))
			adapter.type = "TOKENRING";
		else if(
					adapter.fullname.StartsWith("vbox") ||
					adapter.fullname.StartsWith("wifi") ||
					adapter.fullname.StartsWith("ath")
		)
			adapter.type = "VIRTUALBOX";
		else if(adapter.fullname.StartsWith("wlan"))
			adapter.type = "IEEE80211";
		else			
			adapter.type = "OTHER";
		
/*
		// get interface family and set description with it
		// (may be of some use...)
		adapter.description = "Family: ";
		int family = iface->ifr_addr.sa_family;
		switch(family)
		{
			case AF_PACKET:
				adapter.description += "AF_PACKET";
				break;
			case AF_INET :
				adapter.description += "AF_INET";
				break;
			case AF_INET6 :
				adapter.description += "AF_INET6";
				break;
			default:
				adapter.description += "UNKNOWN";
		}
*/
		// description as name... we dont' have any better choice
		adapter.description = adapter.fullname;
		
		iface++;
	}
	delete[] buf;
	close(sck);

	return res;
}

END_UPP_NAMESPACE

#endif
