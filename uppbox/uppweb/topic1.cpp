#include "www.h"

//#BLITZ_PROHIBIT

/*
#define TOPICFILE <uppweb/www.tpp/all.i>
#include <Core/topic_group.h>
*/

Topic ReadTopic0(const char *text)
{
	Topic topic;
	CParser p(text);
	try {
		if(p.Id("topic")) {
			topic.title = p.ReadString();
			p.Char(';');
			topic.text = p.GetPtr();
			return topic;
		}
		while(!p.IsEof()) {
			if(p.Id("TITLE")) {
				p.PassChar('(');
				topic.title = p.ReadString();
				p.PassChar(')');
			}
			else
			if(p.Id("REF")) {
				p.PassChar('(');
				p.ReadString();
				p.PassChar(')');
			}
			else
			if(p.Id("TOPIC_TEXT")) {
				p.PassChar('(');
				topic.text << p.ReadString();
				p.PassChar(')');
			}
			else
			if(p.Id("COMPRESSED")) {
				StringBuffer b;
				b.Reserve(1024);
				while(p.IsInt()) {
					b.Cat(p.ReadInt());
					p.PassChar(',');
				}
				topic.text = b;
			}
			else {
				topic.text << p.GetPtr();
				break;
			}
		}
	}
	catch(CParser::Error e) {
		topic.text = String::GetVoid();
		topic.title = e;
	}
	return topic;
}

void InitWwwTpp()
{
	String wwwtpp = GetHomeDirFile("upp.src/uppbox/uppweb/www.tpp");
	FindFile ff(AppendFileName(wwwtpp, "*"));
	static Vector<String> data;
	while(ff) {
		RLOG(ff.GetName());
		Topic p = ReadTopic0(LoadFile(AppendFileName(wwwtpp, ff.GetName())));
		data.Add(p.text);
		RegisterTopic__("<uppweb/www.tpp/all.i>", GetFileTitle(ff.GetName()), p.title, data.Top(), data.Top().GetCount());
		ff.Next();
	}
};
