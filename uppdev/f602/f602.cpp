#include <Core/Core.h>
#include <Web/Web.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	XmlNode n = ParseXML(LoadFile("e:/form.xml"));
//	DUMP(AsXML(n));
	const XmlNode& sig = n["dsig:Signature"];
	for(int i = 0; i < sig.GetCount(); i++) {
		if(sig[i].IsTag() && sig[i].GetTag() == "dsig:Object" && sig[i]["d:root"]["d:priloha_pdf"].GetCount()) {
			DUMP(AsXML(sig[i]["d:root"]["d:priloha_pdf"]["d:pocet_stran"]));
			DUMP(~sig[i]["d:root"]["d:priloha_pdf"]["d:pocet_stran"]);
			SaveFile("e:/test.pdf", Base64Decode(~sig[i]["d:root"]["d:priloha_pdf"]["d:base64data"]));
		}
	}
}
