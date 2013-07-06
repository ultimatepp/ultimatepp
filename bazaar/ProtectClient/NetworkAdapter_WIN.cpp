#include "NetworkAdapter.h"

#ifdef PLATFORM_WIN32

#include <winsock2.h>
#include <iphlpapi.h>

// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

Vector <NetworkAdapter> GetNetworkAdaptersInfo(void)
{
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG family = AF_UNSPEC;
	DWORD flags = GAA_FLAG_INCLUDE_PREFIX;
	ULONG outBufLen = 0;
	Buffer<BYTE> pBuffer;
	Vector <NetworkAdapter> ret;

	switch(GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen))
	{
		case ERROR_BUFFER_OVERFLOW:
			pBuffer.Alloc(outBufLen);
			pAddresses = (PIP_ADAPTER_ADDRESSES)~pBuffer;
			break;
		case ERROR_NO_DATA:
		case ERROR_INVALID_FUNCTION:
		default:
			return ret;
	}

	if (NO_ERROR != GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen))
		return ret;

	for (PIP_ADAPTER_ADDRESSES pAdd = pAddresses; pAdd; pAdd = pAdd->Next)
	{
		NetworkAdapter &adapter = ret.Add();
		int len = min((DWORD)6, pAdd->PhysicalAddressLength);
		if (len > 0)
			adapter.mac = ToUpper(HexString(pAdd->PhysicalAddress, len, 1, ':'));
		adapter.description = TrimBoth(WString(pAdd->Description).ToString());
		adapter.fullname = TrimBoth(WString(pAdd->FriendlyName).ToString());
		switch (pAdd->IfType)
		{
			case IF_TYPE_ETHERNET_CSMACD:
				adapter.type = "ETHERNET";
				break;
			case IF_TYPE_ISO88025_TOKENRING:
				adapter.type = "TOKENRING";
				break;
			case IF_TYPE_PPP:
				adapter.type = "MODEM";
				break;
			case IF_TYPE_SOFTWARE_LOOPBACK:
				adapter.type = "SOFTWARE_LOOPBACK";
				break;
			case IF_TYPE_ATM:
				adapter.type = "ATM";
				break;
			case IF_TYPE_IEEE80211:
				adapter.type = "IEEE80211";
				break;
			case IF_TYPE_TUNNEL:
				adapter.type = "TUNNEL";
				break;
			case IF_TYPE_IEEE1394:
				adapter.type = "IEEE1394";
				break;
			default:
				adapter.type = "OTHER";
		}
		if (adapter.type == "ETHERNET" && ToLower(adapter.description).Find("wireless") >= 0)
			adapter.type = "IEEE80211";
	}
	return ret;
}

#endif
