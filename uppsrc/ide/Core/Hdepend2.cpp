#include "Core.h"

#if 0

struct PPInfo {
	VectorMap<String, int>    flags; // "#if... flagXXXX"
	VectorMap<String, String> defines; // #define ...
	Index<String>             includes;
	bool                      guarded;
	
	
	void Do(Stream& in);
};

void RemoveComments(String& l, bool& incomment)
{
	int q = -1;
	int w = -1;
	if(incomment)
		q = w = 0;
	else {
		const char *s = l;
		while(*s) {
			if(*s == '\"')
				s = SkipString(s);
			else
			if(s[0] == '/' && s[1] == '/') {
				q = int(s - ~l);
				SetSpaces(l, q, l.GetCount() - q);
				return;
			}
			else
			if(s[0] == '/' && s[1] == '*') {
				q = int(s - ~l);
				break;
			}
			else
				s++;
		}
		if(q >= 0)
			w = q + 2;
	}
	while(q >= 0) {
		int eq = l.Find("*/", w);
		if(eq < 0) {
			incomment = true;
			SetSpaces(l, q, l.GetCount() - q);
			return;
		}
		SetSpaces(l, q, eq + 2 - q);
		incomment = false;
		q = l.Find("/*");
		w = q + 2;
	}
}

void PPInfo::Do(Stream& in)
{
	LTIMING("PPInfo::Parse");

	flags.Clear();
	defines.Clear();
	includes.Clear();
	guarded = false;

	int  linei = 0;
	bool incomment = false;
	
	String guard_id;
	bool first = true;
	bool second = false;
	
	auto Flag = [&](const String& id) {
		if(id.StartsWith("flag"))
			flags.FindAdd(id);
	};
	
	while(!in.IsEof()) {
		String l = in.GetLine();
		while(*l.Last() == '\\' && !in.IsEof()) {
			l.TrimLast();
			linei++;
			l.Cat(in.GetLine());
		}
		RemoveComments(l, incomment);
		try {
			CParser p(l);
			if(p.Char('#')) {
				if(do_pp) {
					if(p.Id("define") && p.IsId()) {
						p.NoSkipSpaces().NoSkipComments(); // '#define TEST(x)' is different form '#define TEST (x)' - later is parameterless
						String id = p.ReadId();
						if(id == guard_id) // TODO: needs to be better
							guarded = true;
						if(p.Char('(')) {
							id << "(";
							p.SkipSpaces();
							p.Spaces();
							bool was = false;
							while(p.IsId()) {
								if(was)
									id << ", ";
								id << p.ReadId();
								was = true;
							}
							if(p.Char3('.', '.', '.'))
								id << "...";
							p.Char(')');
							id << ")";
						}
						p.Spaces();
						defines.Add(id, p.GetPtr());
					}
					else
					if(p.Id("ifndef") && p.IsId()) {
						String id = p.ReadId();
						Flag(id);
						if(first) {
							guard_id = id;
							second = true;
						}
					}
					else
					if(p.Id("ifdef") && p.IsId()) {
						String id = p.ReadId();
						if(id.StartsWith("flag"))
							flags.Add(id, linei);
					}
					else
					if(p.Id("if"))
						while(!p.IsEof()) {
							if(p.IsId())
						}
					if(p.Id("include"))
						includes.FindAdd(TrimBoth(p.GetPtr()));
				}
			}
		}
		catch(...) {}
		if(first) {
			for(char s : l)
				if(*s != ' ' || *s != '\t')
					first = false;
		}
		
		linei++;
	}
}

#endif