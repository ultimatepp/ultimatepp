#include "Browser.h"

#define CODEITEM   "37138531426314131252341829483370"
#define STRUCTITEM "37138531426314131252341829483380"
#define BEGINSTYLE "05600065144404261032431302351956" 
#define ENDSTYLE   "96390100711032703541132217272105"

static const char styles[] =
	"[ $$0,0#00000000000000000000000000000000:Default]"
	"[i448;a25;kKO9;2 $$1,0#" CODEITEM ":codeitem]"
	"[i448;a25;kKO9;3 $$2,0#" STRUCTITEM ":structitem]"
	"[l288;2 $$3,0#27521748481378242620020725143825:desc]"
	"[H6;0 $$4,0#" BEGINSTYLE ":begin]"
	"[l288;a25;kK~~~.1408;@3;2 $$5,0#61217621437177404841962893300719:param]"
	"[0 $$7,0#" ENDSTYLE ":end]"
;

void IdeGotoCodeRef(const String& ref_id);

void TopicEditor::JumpToDefinition()
{
	PostCallback([=] { IdeGotoCodeRef(editor.GetFormatInfo().label); });
}

void TopicEditor::Label(String& label)
{
	Save();
	WithEditRefLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Code reference");
	dlg.label <<= label;
	if(dlg.Execute() == IDOK)
		label = ~dlg.label;
}

Uuid CodeItemUuid()
{
	return ScanUuid(CODEITEM);
}

Uuid StructItemUuid()
{
	return ScanUuid(STRUCTITEM);
}

Uuid BeginUuid()
{
	return ScanUuid(BEGINSTYLE);
}

Uuid EndUuid()
{
	static Uuid u = ScanUuid(ENDSTYLE);
	return u;
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
					bool found = false;
					String lbl_id = CleanupTppId(para.format.label);
					if(!IsNull(para.format.label))
						for(const auto& f : ~CodeIndex())
							for(const AnnotationItem& m : f.value.items)
								if(FindIndex(AnnotationCandidates(m.id), lbl_id) >= 0) {
									found = true;
									break;
								}
					if(!found) {
						editor.Move(txt.GetPartPos(i));
						return;
					}
				}
			}
		}
		if(!topics_list.IsCursor())
			break;
		c = topics_list.GetCursor() + 1;
		if(c >= topics_list.GetCount()) {
			PromptOK("No more invalid references.");
			break;
		}
		topics_list.SetCursor(c);
		if(!IsNull(topicpath))
			editor.Move(0);
	}
}

void TopicEditor::Tools(Bar& bar)
{
	String l = editor.GetFormatInfo().label;
	bool b = l.GetCount() > 2 && l != "noref";
	bar.Add(b, "See referenced code", IdeCommonImg::Cpp(), THISBACK(JumpToDefinition))
	   .Key(K_ALT_U).Key(K_ALT_I);
	bar.Add("Find broken references..", IdeCommonImg::FindBrokenRef(), THISBACK(FindBrokenRef))
	   .Key(K_CTRL_F3);
	bar.Add("Generate Table of Contents", TopicImg::TOC(), [=] { InsertTableOfContents(); });
}

void TopicEditor::MainTool(Bar& bar)
{
	editor.StyleTool(bar);
	bar.Gap();
	editor.FontTools(bar);
	bar.Gap();
	editor.InkTool(bar);
	editor.PaperTool(bar);
	editor.LastFormatTool(bar);
	bar.Gap();
	editor.LanguageTool(bar);
	editor.SpellCheckTool(bar);
	bar.Gap();
	editor.IndexEntryTool(bar);
	bar.Break();
	editor.HyperlinkTool(bar, Zx(300), K_CTRL_H);
	bar.Gap();
	editor.ParaTools(bar);
	bar.Gap();
	editor.EditTools(bar);
	bar.Gap();
	bar.Add("Print", CtrlImg::print(), THISBACK(Print))
	   .Key(K_CTRL_P);
	bar.GapRight();
	bar.Break();
	editor.LabelTool(bar, Zx(300), K_CTRL_M, "Code reference");
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
	return String(r);
}

String TopicEditor::GetLang() const
{
	int q = topicpath.ReverseFind('@');
	if(q >= 0) {
		int lang = LNGFromText(~topicpath + q + 1);
		if(lang)
			return LNGAsText(lang);
	}
	return "%";
}

String DecoratedItem(const String& name, const String& pretty)
{
	String qtf = "[%00-00K ";
	Vector<ItemTextPart> n = ParsePretty(name, pretty);
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
		}
		qtf << " \1";
		qtf << String(~pretty + p.pos, p.len);
		qtf << "\1]";
	}
	return qtf + "]";
}

String CreateQtf(const AnnotationItem& m, const String& lang, bool onlyhdr = false)
{
	String qtf;
	bool str = IsStruct(m.kind);
	if(!str)
		qtf << "[s4 &]";
	String st = str ? "[s2;" : "[s1;";
	String k = st + ":" + DeQtf(m.id) + ": ";
	String pretty = m.pretty;
	if(IsStruct(m.kind)) {
		pretty = m.pretty0; // because otherwise it is just a name
		int q = pretty.ReverseFind("{");
		if(q)
			pretty.Trim(q);
		pretty = TrimBoth(pretty);
	}
	if(IsTemplate(m.kind) && str) {
		int q = 0;
		int w = 0;
		while(q < pretty.GetLength()) {
			if(pretty[q] == '<')
				w++;
			if(pretty[q] == '>') {
				w--;
				if(w == 0) {
					q++;
					break;
				}
			}
			q++;
		}
		qtf << "[s2:noref: " << DecoratedItem(m.name, pretty.Mid(0, q)) << "&][s2 " << k;
		if(q < pretty.GetLength()) {
			while((byte)pretty[q] <= 32)
				q++;
			qtf << DecoratedItem(m.name, pretty.Mid(q));
		}
	}
	else
		qtf << k << DecoratedItem(m.name, pretty);

	qtf << "&]";
	if(onlyhdr)
		return qtf;

	qtf << "[s3%" << lang << " ";

	if(!str) {
		Vector<ItemTextPart> n = ParsePretty(m.name, pretty);
		if(!str) {
			bool was;
			for(const auto& h : n)
				if(h.type == ITEM_PNAME) {
					qtf << " [%-*@r \1" << pretty.Mid(h.pos, h.len) << "\1]";
					was = true;
				}
			if(was)
				qtf << " .";
		}
	}
	qtf << "&]";
	qtf << "[s7 &]";
	return qtf;
}

void TopicEditor::InsertNew(const AnnotationItem& m)
{
	editor.BeginOp();
	editor.PasteText(ParseQTF(styles + CreateQtf(m, GetLang())));
	editor.PrevPara();
	editor.PrevPara();
}

void TopicEditor::GoTo(const String& _topic, const String& link, const AnnotationItem& create, bool before)
{
	if(topics_list.FindSetCursor(_topic) && !IsNull(link)) {
		editor.Select(editor.GetLength(), 0);
		for(String cr : AnnotationCandidates(link))
			if(editor.GotoLabel([&](const WString& id) { return cr == CleanupTppId(id.ToString()); }))
				break;
		if(create.id.GetCount()) {
			if(!before)
				for(int pass = 0; pass < 2; pass++)
					for(;;) {
						int c = editor.GetCursor();
						RichText::FormatInfo f = editor.GetFormatInfo();
						if(f.styleid == BeginUuid() || (IsNull(f.label) || f.label == "noref") && pass)
							break;
						editor.NextPara();
						if(editor.GetCursor() == c)
							break;
					}
			InsertNew(create);
		}
	}
}

void TopicEditor::GoToPart(int ii)
{
	if(ii >= 0 && ii < editor.Get().GetPartCount())
		editor.Move(editor.Get().GetPartPos(ii));
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

void TopicEditor::InsertTableOfContents()
{
	struct MyIter : RichText::Iterator {
		RichPara::Number n;
		RichEdit *editor;
		int label = 0;
		String toc_qtf = "[A5* Table of Contents&]";
		
		bool operator()(int pos, const RichPara& para) override {
			int lvl = para.format.GetNumberLevel();
			if(lvl > 0) {
				n.Next(para.format);
				editor->SetSelection(pos, pos);
				RichText::FormatInfo fmt = editor->GetFormatInfo();
				fmt.label = AsString(++label);
				editor->ApplyFormatInfo(fmt);
				String s = para.GetText().ToString();
				s.TrimStart("\t");
				toc_qtf << "[^#" << fmt.label << "^ \1" << String(' ', (lvl - 1) * 4) << n.AsText(para.format) << " " << s << "\1&]";
			}
			return false;
		};
	} it;

	it.editor = &editor;
	editor.RemoveSelection();
	editor.BeginOp();
	int cursor = editor.GetCursor();
	editor.Get().Iterate(it);
	editor.SetSelection(cursor, cursor);
	editor.PasteText(ParseQTF(it.toc_qtf));
}

void TopicEditor::SetFocus()
{
	if(editor.IsEnabled())
		editor.SetFocus();
	else
		Ctrl::SetFocus();
}
