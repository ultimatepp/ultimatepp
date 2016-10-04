#ifndef _ShowQtf_ShowQtf_h
#define _ShowQtf_ShowQtf_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class ShowQtf : public TopWindow {
	DocEdit      text;
	RichTextCtrl qtf;
	Splitter     split;
	
	void Text();
	
public:
	typedef ShowQtf CLASSNAME;
	ShowQtf();
};

void ShowQtf::Text()
{
	qtf <<= ~text;
}

ShowQtf::ShowQtf()
{
	qtf <<= "{{1:1:1:1:1:1:1:1:1:1:1:1:1:1:1:1:1:1:1:1@(192.192.192)-4 ICO:: :: :: :: ::@(255.255.255)-4 25613405:: :: :: :: ::@(192.192.192)-4 DIC:: :: :: :: ::@(255.255.255)-4 :: :: :: :: ::@(192.192.192)-4 Jm:: :: :: :: ::@(255.255.255)-14 Amida`, s`.r`.o`.:: :: :: :: :: :: :: :: :: :: :: :: :: :: ::@(192.192.192)-4 Of:: :: :: :: ::@(255.255.255)-14 :: :: :: :: :: :: :: :: :: :: :: :: :: :: }}";
	text <<= THISBACK(Text);
	split.Vert(text, qtf);
	Add(split.SizePos());
	Sizeable().Zoomable();
}

#endif

