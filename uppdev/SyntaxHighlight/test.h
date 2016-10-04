#include <Core/Core.h>
#include <vector>

using namespace Upp;
using namespace std;

namespace Upp {
	namespace www {
		{
			{
			}
		}
	}
}

(())

namespace Alfa {
	namespace Beta {
		[] [[]]
		() () ((( ))) {
			(
			{					
	};
};

{
	{
	}
}

namespace Upp {
	template<> void Xmlize(XmlIO xml, vector<int>& data) {
		if(xml.IsStoring())
			for(int i = 0; i < (int)data.size(); i++)
				Xmlize(xml.Add("item"), data[i]);
		else {
			data.clear();
			for(int i = 0; i < xml->GetCount(); i++)
				if(xml->Node(i).IsTag("item")) {
					data.push_back(0);
					Xmlize(xml.At(i), data.back());
				}
		}
	}
};

CONSOLE_APP_MAIN
{
	vector<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	String s = StoreAsXML(x, "std-test");
	DUMP(s);
	vector<int> y;
	LoadFromXML(y, s);
	for(int i = 0; i < (int)y.size(); i++)
		DUMP(y[i]);
	
	"asdasdasd"\
	sdfsdf
	
	"\
abstract,\
alias,\
align,\
asm,\
assert,\
auto,\
body,\
bool,\
break"



}
