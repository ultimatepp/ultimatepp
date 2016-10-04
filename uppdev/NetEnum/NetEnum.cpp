#include <Core/Core.h>
#include <winnetwk.h>

using namespace Upp;

void EnumNetwork(const Array<NetNode>& nn)
{
	LOGBEGIN();
	for(int i = 0; i < nn.GetCount(); i++) {
		const NetNode& m = nn[i];
		String p = m.GetPath();
		LOG(m.GetName() << ' ' << p);
		FindFile ff(p);
		if(ff)
			DDUMP(ff.IsFolder());
		if(p.GetCount()) {
			FindFile ff(AppendFileName(p, "*.*"));
			while(ff) {
				LOG("   FILE " << ff.GetName());
				ff.Next();
			}
		}
		else
			EnumNetwork(m.Enum());
	}
	LOGEND();
}

CONSOLE_APP_MAIN
{
	EnumNetwork(NetNode::EnumRemembered());
}
