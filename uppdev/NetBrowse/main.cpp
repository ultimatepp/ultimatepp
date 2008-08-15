#include <Core/Core.h>
#include <winnetwk.h>

#pragma comment (lib, "mpr")

using namespace Upp;

class NetworkInfo {
	struct Node : NETRESOURCE {
		bool        loaded;
		int         parent;
	
		void Copy();
		
		Node();
		~Node();
	};
	Array<Node>  net;

public:
	String      GetName(int node);
	String      GetPath(int node);
	int         GetParent(int node)          { return net[node].parent; }
	void        Connect(int node);

	Vector<int> Enum(int parent_node = 0);
	
	NetworkInfo();
};

static void sCopy(LPSTR& x)
{
	LPSTR s = x;
	if(s) {
		int n = strlen(s) + 1;
		x = new char[n];
		memcpy(x, s, n);
	}
}

void NetworkInfo::Node::Copy()
{
	sCopy(lpProvider);
	sCopy(lpRemoteName);
	sCopy(lpLocalName);
	sCopy(lpComment);
}

NetworkInfo::Node::Node()
{
	lpProvider = NULL;
	lpRemoteName = NULL;
	lpLocalName = NULL;
	lpComment = NULL;
}

NetworkInfo::Node::~Node()
{
	delete[] lpProvider;
	delete[] lpRemoteName;
	delete[] lpLocalName;
	delete[] lpComment;
}

NetworkInfo::NetworkInfo()
{
	Node& n = net.Add();
	memset(&n, 0, sizeof(Node));
	n.parent = -1;
}

String DosInitCaps(const char *name)
{
	for(const char *s = name; *s; s++)
		if(IsLetter(*s) && IsLower(*s))
			return name;
	return InitCaps(name);
}

String NetworkInfo::GetName(int node)
{
	Node& n = net[node];
	String name;
	const char *s = n.lpRemoteName;
	if(s) {
		if(s[0] == '\\' && s[1] == '\\')
			name = DosInitCaps(GetFileName(s));
		else
			name = s;
	}
	if(n.lpComment && *n.lpComment)
		if(name.GetCount())
			return String().Cat() << n.lpComment << " (" << name << ")";
		else
			return n.lpComment;
	return name;
}

String NetworkInfo::GetPath(int node)
{
	if(node == 0 || (net[node].dwUsage & RESOURCEUSAGE_CONTAINER))
		return Null;
	return net[node].lpRemoteName;
}

void NetworkInfo::Connect(int node)
{
}

Vector<int> NetworkInfo::Enum(int node)
{
	Node& pn = net[node];
	HANDLE hEnum;
	if(!pn.loaded) {
		Connect(node);
		if(::WNetOpenEnum(node ? RESOURCE_GLOBALNET : RESOURCE_CONTEXT,
		                  RESOURCETYPE_DISK, 0, node ? &pn : NULL, &hEnum) == 0) {
			DWORD cEntries = -1, cbBuffer = 0x4000;
			Buffer<NETRESOURCE> lpnr(cbBuffer);
			while(::WNetEnumResource(hEnum, &cEntries, lpnr, &cbBuffer) == 0) {
				for(int i = 0; i < (int)cEntries; i++) {
					Node& n = net.Add();
					*(NETRESOURCE *)&n = lpnr[i];
					n.Copy();
					n.parent = node;
					n.loaded = !(n.dwUsage & RESOURCEUSAGE_CONTAINER);
				}
			}
			::WNetCloseEnum(hEnum);
		}
	}
	Vector<int> r;
	for(int i = 1; i < net.GetCount(); i++)
		if(net[i].parent == node)
			r.Add(i);
	return r;
}

NetworkInfo nwk;

void EnumNetworkInfo(int node)
{
	Vector<int> n = nwk.Enum(node);
	for(int i = 0; i < n.GetCount(); i++) {
		DUMP(nwk.GetName(n[i]));
		DUMP(nwk.GetPath(n[i]));
		EnumNetworkInfo(n[i]);
	}
}

CONSOLE_APP_MAIN
{
	EnumNetworkInfo(0);
}
