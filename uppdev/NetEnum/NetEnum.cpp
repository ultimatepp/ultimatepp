#include <Core/Core.h>
#include <winnetwk.h>

using namespace Upp;

void EnumNetwork(const Array<NetNode>& nn)
{
	LOGBEGIN();
	for(int i = 0; i < nn.GetCount(); i++) {
		const NetNode& m = nn[i];
		LOG(m.GetName() << ' ' << m.GetPath());
		EnumNetwork(m.Enum());
	}
	LOGEND();
}

CONSOLE_APP_MAIN
{
	EnumNetwork(NetNode::EnumRoot());
}
