#include "Browser.h"

int CharFilterID(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

bool ParseTopicFileName(const String& fn, String& topic, int& lang)
{
	String q = GetFileTitle(fn);
	int w = q.Find('$');
	if(w < 0)
		return false;
	topic = q.Mid(0, w);
	lang = LNGFromText(q.Mid(w + 1));
	return lang;
}

Topic ReadTopic(const char *text)
{
	Topic topic;
	CParser p(text);
	try {
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
				topic.text = ZDecompress(b, b.GetLength());
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

Vector<String> GatherLabels(const RichText& text)
{
	Vector<RichValPos> p = text.GetValPos(TopicEditor::TopicPage(), RichText::LABELS);
	Index<String> ref;
	for(int i = 0; i < p.GetCount(); i++)
		if(!IsNull(p[i].data))
			ref.FindAdd(p[i].data.ToString());
	return ref.PickKeys();
}

String WriteTopic(const char *title, const RichText& text)
{
	StringBuffer r;
	r << "TITLE(" << AsCString(title) << ")\r\n";
	String cpsd = ZCompress(AsQTF(text, CHARSET_UTF8, QTF_BODY|QTF_ALL_STYLES));
	r << "COMPRESSED\r\n";
	const char *s = cpsd;
	const char *e = cpsd.End();
	int q = 0;
	while(s < e) {
		int c = (byte)*s++;
		if(c >= 100)
			r.Cat(c / 100 + '0');
		if(c >= 10)
			r.Cat(c / 10 % 10 + '0');
		r.Cat(c % 10 + '0');
		r.Cat(',');
		if((++q & 255) == 0)
			r << "\r\n";
	}
	r << "\r\n\r\n";
	return r;
}
