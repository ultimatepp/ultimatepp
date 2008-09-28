#include "Browser.h"

#define CODEITEM  "37138531426314131252341829483370"
#define STRUCTITEM "37138531426314131252341829483380"

static const char styles[] =
	"[ $$0,0#00000000000000000000000000000000:Default]"
	"[i448;a25;kKO9;2 $$1,0#" CODEITEM ":codeitem]"
	"[i448;a25;kKO9;3 $$2,0#" STRUCTITEM ":structitem]"
	"[l288;2 $$3,0#27521748481378242620020725143825:desc]"
	"[H6;0 $$4,0#05600065144404261032431302351956:begin]"
	"[l288;a25;kK~~~.1408;@3;2 $$5,0#61217621437177404841962893300719:param]"
	"[0 $$7,0#96390100711032703541132217272105:end]"
;

void TopicEditor::JumpToDefinition()
{
	PostCallback(callback1(IdeGotoLink, editor.GetFormatInfo().label));
}

void TopicEditor::Label(String& label)
{
	Save();
	if(ref.browser.item.IsMulti())
		ref.browser.item.ClearSelection();
	ref.browser.item.Multi(false);
	ref.Title("Reference");
	ref.Set(label);
	ref.classlist.Hide();
	if(ref.Execute() != IDOK)
		return;
	label = ref.Get();
}

Uuid CodeItemUuid()
{
	return ScanUuid(CODEITEM);
}

Uuid StructItemUuid()
{
	return ScanUuid(STRUCTITEM);
}

void TopicEditor::FindBrokenRef()
{
	Uuid codeitem = CodeItemUuid();
	Uuid stritem = StructItemUuid();;
	for(;;) {
		if(IsNull(topicpath))
			return;
		const RichText& txt = editor.Get();
		int c = editor.GetCursor();
		int i = txt.FindPart(c);
		while(++i < txt.GetPartCount()) {
			if(txt.IsPara(i)) {
				Uuid style = txt.GetParaStyle(i);
				if(style == codeitem || style == stritem) {
					RichPara para = txt.Get(i);
					if(para.format.label == "noref")
						continue;
					if(!IsNull(para.format.label)) {
						String nest;
						String key;
						if(SplitNestKey(para.format.label, nest, key)) {
							int q = BrowserBase().Find(nest);
							if(q >= 0 || BrowserBase()[q].key.Find(key) >= 0)
								continue;
						}
					}
					editor.Move(txt.GetPartPos(i));
					return;
				}
			}
		}
		if(!topic.IsCursor())
			break;
		c = topic.GetCursor() + 1;
		if(c >= topic.GetCount()) {
			PromptOK("No more invalid references.");
			break;
		}
		topic.SetCursor(c);
		if(!IsNull(topicpath))
			editor.Move(0);
	}
}

void TopicEditor::Tools(Bar& bar)
{
	bar.Add("Insert code item..", IdeCommonImg::InsertItem(), THISBACK(InsertItem))
	   .Key(K_CTRL_INSERT);
	String l = editor.GetFormatInfo().label;
	bool b = l.GetCount() > 2 && l[0] == ':' && l[1] == ':';
	bar.Add(b, "See referenced code", IdeCommonImg::Source(), THISBACK(JumpToDefinition))
	   .Key(K_ALT_J);
	bar.Add("Find broken references..", IdeCommonImg::FindBrokenRef(), THISBACK(FindBrokenRef))
	   .Key(K_CTRL_F3);
#ifdef REPAIR
	bar.Separator();
	bar.Add("Repair!", CtrlImg::Toggle(), THISBACK(Repair)).Key(K_ALT_F5);
	bar.Separator();
#endif
}

void TopicEditor::MainTool(Bar& bar)
{
	editor.StyleTool(bar);
	bar.Gap();
	editor.FontTools(bar);
	bar.Gap();
	editor.InkTool(bar);
	editor.PaperTool(bar);
	bar.Gap();
	editor.LanguageTool(bar);
	editor.SpellCheckTool(bar);
	bar.Gap();
	editor.IndexEntryTool(bar);
	bar.Break();
	editor.HyperlinkTool(bar, 300, K_CTRL_H);
	bar.Gap();
	editor.ParaTools(bar);
	bar.Gap();
	editor.EditTools(bar);
	bar.Gap();
	bar.Add("Print", CtrlImg::print(), THISBACK(Print))
	   .Key(K_CTRL_P);
	bar.GapRight();
	bar.Break();
	editor.LabelTool(bar, 500, K_CTRL_M, "Code reference");
	bar.Gap();
	Tools(bar);
	bar.Gap();
	editor.TableTools(bar);
}

String NaturalDeQtf(const char *s) {
	StringBuffer r;
	r.Reserve(256);
	bool cm = false;
	while(*s) {
		if(*s == ' ')
			r.Cat(cm ? ' ' : '_');
		else {
			if((byte)*s > ' ' && !IsDigit(*s) && !IsAlpha(*s) && (byte)*s < 128)
				r.Cat('`');
			r.Cat(*s);
			if(*s == ',')
				cm = true;
			else
				cm = false;
		}
		s++;
	}
	return r;
}

static int sSplitT(int c) {
	return c == ';' || c == '<' || c == '>' || c == ',';
}

String DecoratedItem(const String& name, const CppSimpleItem& m, const char *natural)
{
	String qtf = "[%00-00K ";
	Vector<ItemTextPart> n = ParseItemNatural(name, m, natural);
	if(m.virt)
		qtf << "[@B virtual] ";
	if(m.kind == CLASSFUNCTION || m.kind == CLASSFUNCTIONTEMPLATE)
		qtf << "[@B static] ";
	Vector<String> qt = Split(m.qptype, sSplitT, false);
	Vector<String> tt = Split(m.qtype, sSplitT, false);
	for(int i = 0; i < n.GetCount(); i++) {
		ItemTextPart& p = n[i];
		qtf << "[";
		switch(p.type) {
		case ITEM_PNAME:
			qtf << "*";
		case ITEM_NUMBER:
			qtf << "@r";
			break;
		case ITEM_TNAME:
			qtf << "*@g";
			break;
		case ITEM_NAME:
			qtf << "*";
			break;
		case ITEM_UPP:
			qtf << "@c";
			break;
		case ITEM_CPP_TYPE:
		case ITEM_CPP:
			qtf << "@B";
			break;
		default:
			int q = p.type - ITEM_PTYPE;
			if(q >= 0 && q < qt.GetCount() && *qt[q] == ':')
				qtf << "_^" << qt[q] << '^';
			q = p.type - ITEM_TYPE;
			if(q >= 0 && q < tt.GetCount() && *tt[q] == ':')
				qtf << "_^" << tt[q] << '^';
			break;
		}
		qtf << ' ';
		qtf << NaturalDeQtf(String(~m.natural + p.pos, p.len));
		qtf << ']';
	}
	return qtf + "]";
}

void TopicEditor::CreateQtf(const String& item, const String& name, const CppSimpleItem& m,
                            String& p1, String& p2)
{
	String qtf;
	bool str = m.kind == STRUCT || m.kind == STRUCTTEMPLATE;
	if(!str)
		qtf << "[s4 &]";
	String st = str ? "[s2;" : "[s1;";
	String k = st + ':' + DeQtf(item) + ": ";
	if(m.IsTemplate() && str) {
		int q = 0;
		int w = 0;
		while(q < m.natural.GetLength()) {
			if(m.natural[q] == '<')
				w++;
			if(m.natural[q] == '>') {
				w--;
				if(w == 0) {
					q++;
					break;
				}
			}
			q++;
		}
		qtf << "[s2:noref: " << DecoratedItem(name, m, m.natural.Mid(0, q)) << "&][s2 " << k;
		if(q < m.natural.GetLength()) {
			while((byte)m.natural[q] <= 32)
				q++;
			qtf << DecoratedItem(name, m, m.natural.Mid(q));
		}
	}
	else
		qtf << k << DecoratedItem(name, m, m.natural);

	qtf << "&]";
	p1 = qtf;
	qtf.Clear();
	qtf << "[s3%% ";
	String d;
	Vector<String> t = Split(m.tname, ';');
	for(int i = 0; i < t.GetCount(); i++) {
		if(i)
			d << ' ';
		d << "[%-*@g " << DeQtf(t[i]) << "]";
	}
	d.Clear();
	d = "[%% ";
	Vector<String> p = Split(m.pname, ';');
	bool was = false;
	if(!str)
		for(int i = 0; i < p.GetCount(); i++)
			d << " [%-*@r " << DeQtf(p[i]) << "]";
	if(!str && p.GetCount())
		qtf << d << '.';
	qtf << "&]";
	qtf << "[s7 &]";
	p2 = qtf;
	return;
}

void TopicEditor::InsertItem()
{
	if(IsNull(topicpath))
		return;
	Save();
	ref.Title("Insert");
	if(ref.browser.item.IsCursor())
		ref.browser.item.SetFocus();
	ref.browser.item.Multi();
	ref.classlist.Show();
	int c = ref.Execute();
	if(c == IDCANCEL)
		return;
	if(c == IDYES) {
		String qtf = "&{{1 ";
		for(int i = 0; i < ref.browser.nesting.GetCount(); i++) {
			const CppNestingInfo& f = ValueTo<CppNestingInfo>(ref.browser.nesting.Get(i, 2));
			if(i)
				qtf << ":: ";
			qtf << DeQtf(f.nesting);
		}
		qtf << "}}&";
		editor.PasteText(ParseQTF(qtf));
		return;
	}
	String p1, p2;
	if(ref.browser.item.IsSelection()) {
		for(int i = 0; i < ref.browser.item.GetCount(); i++)
			if(ref.browser.item.IsSelected(i)) {
				String a1, a2;
				const CppItemInfo& m = ref.browser.GetItemInfo(i);
				CreateQtf(ref.browser.GetItem(i), m.name, m, a1, a2);
				p1 << p2 << a1;
				p2 = a2;
			}
	}
	else
	if(ref.browser.item.IsCursor()) {
		const CppItemInfo& m = ref.browser.GetItemInfo();
		CreateQtf(ref.browser.GetItem(), m.name, m, p1, p2);
	}
	else
		return;
	editor.BeginOp();
	int a = editor.GetCursor();
	editor.PasteText(ParseQTF(styles + p1));
	c = editor.GetCursor();
	editor.PasteText(ParseQTF(styles + p2));
	editor.Move(a);
	editor.Move(c);
}

void   TopicEditor::FixTopic()
{
	String nest;
	if(!EditText(nest, "Fix topic", "Nest"))
		return;
	if(nest[0] != ':')
		nest = "::" + nest;
	CppBase& base = BrowserBase();
	int q = base.Find(nest);
	if(q < 0) {
		Exclamation("Nest not found");
		return;
	}
	CppNest& n = base[q];
	Index<String> natural;
	Vector<String> link;
	for(int i = 0; i < n.GetCount(); i++) {
		const CppItem& m = n[i];
		String nat;
		if(m.virt)
			nat << "virtual ";
		if(m.kind == CLASSFUNCTION || m.kind == CLASSFUNCTIONTEMPLATE)
			nat << "static ";
		nat << m.natural;
		natural.Add(nat);
		link.Add(nest + "::" + n.key[i]);
	}
	RichText result;
	const RichText& txt = editor.Get();
	bool started = false;

	int l, h;
	if(editor.GetSelection(l, h)) {
		l = txt.FindPart(l);
		h = txt.FindPart(h);
	}
	else {
		l = 0;
		h = txt.GetPartCount();
	}

	for(int i = 0; i < txt.GetPartCount(); i++)
		if(txt.IsPara(i)) {
			RichPara p = txt.Get(i);
			if(i >= l && i < h) {
				WString h = p.GetText();
				String nat;
				const wchar *s = h;
				while(*s)
					if(*s == 160 || *s == ' ') {
						nat.Cat(' ');
						while(*s == 160 || *s == ' ') s++;
					}
					else
						nat.Cat(*s++);
				int q = natural.Find(nat);
				if(q >= 0) {
					started = true;
					const CppSimpleItem& m = n[q];
					String p1, p2;
					CreateQtf(link[q], n.name[q], m, p1, p2);
					p1 = "[s7; &]" + p1;
					RichText h = ParseQTF(styles + p1);
					if(h.GetPartCount())
						h.RemovePart(h.GetPartCount() - 1);
					result.CatPick(h);
				}
				else
				if(!started || p.GetLength())
					result.Cat(p);
			}
			else
				result.Cat(p);
		}
		else {
			RichTable b;
			b <<= txt.GetTable(i);
			result.CatPick(b);
		}
	RichPara empty;
	result.Cat(empty);
	editor.BeginOp();
	editor.SetSelection(0, txt.GetLength());
	editor.PasteText(result);
}

String TopicEditor::GetFileName() const
{
	return grouppath;
}

void TopicEditor::Save()
{
	SaveTopic();
	SaveInc();
}

void TopicEditor::SetFocus()
{
	if(editor.IsEnabled())
		editor.SetFocus();
	else
		Ctrl::SetFocus();
}
