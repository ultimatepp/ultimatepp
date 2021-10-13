#include "Core.h"

#ifdef PLATFORM_WIN32

namespace Upp {

NetNode::NetNode()
{
	memset(&net, 0, sizeof(NETRESOURCE));
}

NetNode& NetNode::operator=(const NetNode& s)
{
	net = s.net;
	local = clone(s.local);
	remote = clone(s.remote);
	comment = clone(s.comment);
	provider = clone(s.provider);
	name = s.name;
	path = s.path;
	SetPtrs();
	return *this;
}

String DosInitCaps(const char *name)
{
	for(const char *s = name; *s; s++)
		if(IsLetter(*s) && IsLower(*s))
			return name;
	return InitCaps(name);
}

void NetNode::SetPtrs()
{
	auto SetPtr = [](Vector<char16>& s, char16 *& ptr) { if(ptr) ptr = s; };
	SetPtr(local, net.lpLocalName);
	SetPtr(remote, net.lpRemoteName);
	SetPtr(comment, net.lpComment);
	SetPtr(provider, net.lpProvider);
}

/*
void NetNode::Serialize(Stream& s)
{
	s % net.dwScope % net.dwType % net.dwDisplayType % net.dwUsage;
	s % local % remote % comment % provider % name % path;
	SetPtrs();
}
*/

Array<NetNode> NetNode::Enum() const
{
	HANDLE hEnum;
	if(::WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_DISK, 0, (NETRESOURCE *)&net, &hEnum))
		return Array<NetNode>();
	return Enum0(hEnum);
}

Array<NetNode> NetNode::EnumRoot()
{
	HANDLE hEnum;
	if(::WNetOpenEnum(RESOURCE_CONTEXT, RESOURCETYPE_DISK, 0, NULL, &hEnum))
		return Array<NetNode>();
	return Enum0(hEnum);
}

Array<NetNode> NetNode::EnumRemembered()
{
	HANDLE hEnum;
	if(::WNetOpenEnum(RESOURCE_REMEMBERED, RESOURCETYPE_DISK, 0, NULL, &hEnum))
		return Array<NetNode>();
	return Enum0(hEnum);
}

Array<NetNode> NetNode::Enum0(HANDLE hEnum)
{
	Array<NetNode> r;
	DWORD cEntries = (DWORD)-1, cbBuffer = 0x4000;
	Buffer<NETRESOURCEW> lpnr(cbBuffer);
	while(::WNetEnumResourceW(hEnum, &cEntries, lpnr, &cbBuffer) == 0) {
		for(int i = 0; i < (int)cEntries; i++) {
			NETRESOURCEW& sn = lpnr[i];
			const WCHAR *s = sn.lpRemoteName;
			NetNode& nn = r.Add();
			NETRESOURCEW& n = nn.net;
			n = sn;
			auto get = [](const char16 *s) { Vector<char16> x; while(s && *s) x.Add(*s++); x.Add(0); return x; };
			nn.local = get(n.lpLocalName);
			nn.remote = get(n.lpRemoteName);
			nn.comment = get(n.lpComment);
			nn.provider = get(n.lpProvider);
			nn.SetPtrs();
			if(s) {
				if(s[0] == '\\' && s[1] == '\\')
					nn.name = DosInitCaps(GetFileName(FromSystemCharsetW(s)));
				else
					nn.name = FromSystemCharsetW(s);
			}
			if(n.lpComment && *n.lpComment) {
				if(nn.name.GetCount())
					nn.name = String().Cat() << FromSystemCharsetW(n.lpComment)
					                         << " (" << nn.name << ")";
				else
					nn.name = FromSystemCharsetW(n.lpComment);
			}
			if(!(n.dwUsage & RESOURCEUSAGE_CONTAINER))
				nn.path = FromSystemCharsetW(n.lpRemoteName);
		}
	}
	::WNetCloseEnum(hEnum);
	return r;
}

int NetNode::GetDisplayType() const
{
	switch(net.dwDisplayType) {
	case RESOURCEDISPLAYTYPE_NETWORK:
		return NETWORK;
	case RESOURCEDISPLAYTYPE_DOMAIN:
	case RESOURCEDISPLAYTYPE_GROUP:
	case RESOURCEDISPLAYTYPE_TREE:
	case /*RESOURCEDISPLAYTYPE_NDSCONTAINER*/0x0000000B:
		return GROUP;
	case RESOURCEDISPLAYTYPE_SERVER:
		return SERVER;
	case RESOURCEDISPLAYTYPE_SHARE:
	case RESOURCEDISPLAYTYPE_SHAREADMIN:
	case RESOURCEDISPLAYTYPE_DIRECTORY:
		return SHARE;
	}
	return UNKNOWN;
}

}

#endif
