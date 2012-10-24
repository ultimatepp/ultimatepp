#include <Core/Core.h>
#include <vector>

namespace Upp {

template<> void Xmlize(XmlIO& xml, std::vector<int>& data) {
	if(xml.IsStoring())
		for(int i = 0; i < (int)data.size(); i++) {
			XmlIO xio = xml.Add("item");
			Xmlize(xio, data[i]);
		}
	else {
		data.clear();
		for(int i = 0; i < xml->GetCount(); i++)
			if(xml->Node(i).IsTag("item")) {
				data.push_back(0);
				XmlIO xio = xml.At(i);
				Xmlize(xio, data.back());
			}
	}
}

};

using namespace Upp;
using namespace std;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
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
}
