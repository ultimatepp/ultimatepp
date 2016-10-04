#include <CtrlLib/CtrlLib.h>

#define TOPICFILE <Latinsert/app.tpp/all.i>
#include <Core/topic_group.h>

GUI_APP_MAIN
{
	String l[26];
	for(int i = 128; i < 2048; i++) {
		int a = ToUpper(ToAscii(i));
		if(a >= 'A' && a <= 'Z') {
			a -= 'A';
			l[a] << "[^" << AsString(i) << "^ " << ToUtf8(i) << "] ";
		}
	}
	String r = "[R4{_} ";
	for(int i = 0; i < 26; i++) {
		if(l[i].GetCount()) {
			DUMP(l[i]);
			r << l[i] << "&";
		}
	}
	TopWindow win;
	PromptOK(GetTopic("topic://Latinsert/app/latin$en-us"));
}
