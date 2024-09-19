#include "RichEdit/RichEdit.h"

using namespace Upp;

String out;

int    ch_major = 0;
int    ch_minor = 0;
String qtf =
"[ $$0,0#00000000000000000000000000000000:Default]"
"[a83;*R6 $$1,3#31310162474203024125188417583966:caption]"
"[b83;*6 $$2,3#07864147445237544204411237157677:title]"
"[b42;a42 $$3,3#45413000475342174754091244180557:text]"
"[l100;C@5*;1 $$4,4#20902679421464641399138805415013:code]"
"[l100;*C$7;1 $$5,5#07531550463529505371228428965313:log]"
"[H6;b73;*7 $$6,3#07864147445237544204111237153677:section]"
;

#ifdef OUT
#undef OUT
#endif

#define OUT(x) out << x << '\n';

String toc;

void Section(const String& title)
{
	String lbl = AsString(++ch_major);
	String text = String().Cat() << lbl << "." << ' ' + TrimBoth(title);
	lbl = "Chapter_" + lbl;
	qtf << "[s6;:" << lbl << ": " << text << "&]";
	ch_minor = 0;
	toc << "&[A3^#" << lbl << "^ " << text << "]&";
}

void FlushDoc(String& docblock)
{
	docblock = TrimBoth(docblock);
	if(docblock.GetCount() == 0)
		return;
	OUT("============= DOC");
	OUT(docblock);
	
	bool title = false;
	
	String style = "[s3;";
	String lbl;
	if(docblock.StartsWith(".")) {
		docblock = AsString(ch_major) + "." + AsString(++ch_minor) + ' ' + TrimBoth(docblock.Mid(1));
		style = ch_minor == 1 ? "[s2;" : "[s2;H4";
		lbl = "Section_" + AsString(ch_major) + "_" + AsString(ch_minor);
		style << ":" <<  lbl << ":";
		title = true;
	}
	
	qtf << style << " ";
	
	const char *s = docblock;

	String plain;
	while(*s)
		if((s == ~docblock || findarg(s[-1], '(', ' ', '\'', '\"') >= 0) && findarg(s[0], '*', '%', '_', '`', '^') >= 0 && s[1] && s[1] != ' ') {
			int c = *s++;
			const char *b = s;
			const char *dc = NULL;
			while(*s && *s != c) {
				if(*s == ':')
					dc = s;
				s++;
			}
			if(c == '^') {
				qtf << "[^";
				if(dc) {
					qtf.Cat(b, dc);
					b = dc + 1;
				}
				else
					qtf.Cat(b, s);
				qtf << "^ ";
			}
			else
				qtf << decode(c, '*', "[* ", '%', "[/ ", '_', "[_ ", '`', title ? "[C@5 " : "[C@5* ", "");
			while(b < s) {
				plain << '`' << *b;
				qtf << '`' << *b++;
			}
			qtf << "]";
			if(*s) s++;
		}
		else {
			plain << '`' << *s;
			qtf << '`' << *s++;
		}

	qtf << "&]";
	if(lbl.GetCount())
		toc << "___[A2^#" << lbl << "^ " << plain << "]&";
	docblock.Clear();
}

void FlushLog(Vector<String>& log)
{
	if(log.GetCount() == 0)
		return;
	OUT("============ LOG");
	OUT(Join(log, "\r\n"));
	
	qtf << "[s5; \1" << Join(log, "\n") << "\1&]&";
	log.Clear();
}

void FlushCode(Vector<String>& code)
{
	while(code.GetCount() && TrimBoth(code[0]).GetCount() == 0)
		code.Remove(0);
	while(code.GetCount() && TrimBoth(code.Top()).GetCount() == 0)
		code.Drop();
	
	bool tabs = true;
	for(auto l : code)
		if(l.GetCount() && *l != '\t') {
			tabs = false;
			break;
		}
	if(tabs)
		for(auto& l : code)
			if(l.GetCount())
				l.Remove(0);
	
	if(code.GetCount() == 0)
		return;
	OUT("============= CODE");
	OUT(Join(code, "\r\n"));

	qtf << "&[s4; \1" << Join(code, "\n") << "\1&]&";
	code.Clear();
}

void MakeTutorial()
{
	String log = LoadFile(GetStdLogPath());
	StringStream ss(log);

	VectorMap<String, Vector<Tuple<int, String>>> logline;
	
	String path;
	int    line;

	while(!ss.IsEof()) {
		String ln = ss.GetLine();
		if(ln.StartsWith("-=>")) {
			SplitTo(ln.Mid(4), '@', path, ln);
			line = atoi(ln) - 1;
		}
		else
		if(ln.StartsWith("=-="))
			logline.Add(String()).Add(MakeTuple(0, ln.Mid(4)));
		else
		if(path.GetCount())
			logline.GetAdd(path).Add(MakeTuple(line, ln));
	}
	
	for(auto&& f : ~logline) {
		if(IsNull(f.key)) {
			Section(f.value[0].b);
			continue;
		}
		Vector<String> src = Split(Filter(LoadFile(f.key), [] (int c) { return c == '\r' ? 0 : c; }), '\n', false);
		int i = 0;
		int logi = 0;
		bool wasdoc = false;
		Vector<String> code;
		while(i < src.GetCount()) {
			String block;
			while(i < src.GetCount() && TrimLeft(src[i]).StartsWith("///")) {
				FlushCode(code);
				Vector<String> logblock;
				while(logi < f.value.GetCount() && f.value[logi].a <= i)
					logblock.Add(f.value[logi++].b);
				FlushLog(logblock);
				
				String line = src[i++];
				int q = line.FindAfter("///");
				if(TrimRight(line).GetCount() > q) {
					if(block.GetCount())
						block << ' ';
					block << TrimBoth(line.Mid(q));
				}
				else
					FlushDoc(block);
				wasdoc = true;
			}
			FlushDoc(block);
			while(i < src.GetCount() && !TrimLeft(src[i]).StartsWith("///")) {
				String tl = TrimLeft(src[i]);
				if(!tl.StartsWith("#if") && !tl.StartsWith("#end"))
					code.Add(src[i]);
				i++;
			}
			DUMPC(code);
			if(!wasdoc)
				code.Clear();
		}
	}
	
	LOG("--------------------------------------------");
	LOG(out);
	
	LOG("--------------------------------------------");
	LOG(qtf);
	
	LOG("--------------------------------------------");
	LOG(toc);
	
	RichEditWithToolBar edit;
	edit.SetReadOnly();
	edit <<= toc + qtf;
	TopWindow win;
	win.Add(edit.SizePos());
	win.Run();
}
