#include "Browser.h"

#ifdef _DEBUG
//#define REPAIR
#endif

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

struct FindBrokenRefIterator : RichText::Iterator {
	int cursor;

	virtual bool operator()(int pos, const RichPara& para)
	{
		if(pos >= cursor && para.format.label[0] == ':' && para.format.label[1] == ':') {
			String nest;
			String key;
			if(para.GetLength() && SplitNestKey(para.format.label, nest, key)) {
				int q = BrowserBase().Find(nest);
				if(q >= 0 && BrowserBase()[q].key.Find(key) >= 0)
					return false;
			}
			cursor = pos;
			return true;
		}
		return false;
	}
};

void TopicEditor::FindBrokenRef()
{
	Progress pi;
	pi.SetTotal(topic.GetCount());
	for(;;) {
		if(IsNull(topicpath))
			return;
		pi.SetText(GetFileTitle(topicpath));
		pi.SetPos(topic.GetCursor());
		FindBrokenRefIterator fi;
		fi.cursor = editor.GetCursor();
		if(editor.Get().Iterate(fi)) {
			editor.Move(fi.cursor);
			break;
		}
		if(!topic.IsCursor())
			break;
		int c = topic.GetCursor() + 1;
		if(c >= topic.GetCount()) {
			PromptOK("No more broken references.");
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
	   .Key(K_INSERT);
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

String DecoratedItem(const CppItemInfo& m, const char *natural)
{
	String qtf = "[%00-00K ";
	Vector<ItemTextPart> n = ParseItemNatural(m, natural);
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

static const char styles[] =
	"[ $$0,0#00000000000000000000000000000000:Default]"
	"[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483370:item]"
	"[i448;a25;kKO9;3 $$2,0#37138531426314131252341829483380:class]"
	"[l288;2 $$3,0#27521748481378242620020725143825:desc]"
	"[H6;0 $$4,0#05600065144404261032431302351956:begin]"
	"[l288;a25;kK~~~.1408;@3;2 $$5,0#61217621437177404841962893300719:param]"
	"[0 $$7,0#96390100711032703541132217272105:end]"
;

void TopicEditor::CreateQtf(const String& item, const CppItemInfo& m, String& p1, String& p2)
{
	String qtf;
	bool str = m.kind == STRUCT || m.kind == STRUCTTEMPLATE;
	if(!str)
		qtf << "[s4 &]";
	qtf << (str ? "[s2;:" : "[s1;:") << DeQtf(item) << ": ";
	if(m.IsTemplate()) {
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
		qtf << DecoratedItem(m, m.natural.Mid(0, q));
		if(q < m.natural.GetLength()) {
			while((byte)m.natural[q] <= 32)
				q++;
			qtf << '&' << DecoratedItem(m, m.natural.Mid(q));
		}
	}
	else
		qtf << DecoratedItem(m, m.natural);

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
				CreateQtf(ref.browser.GetItem(i), ref.browser.GetItemInfo(i), a1, a2);
				p1 << p2 << a1;
				p2 = a2;
			}
	}
	else
	if(ref.browser.item.IsCursor())
		CreateQtf(ref.browser.GetItem(), ref.browser.GetItemInfo(), p1, p2);
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
