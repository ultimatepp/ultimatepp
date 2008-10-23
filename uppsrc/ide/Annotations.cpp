#include "ide.h"

void AssistEditor::Annotate(const String& filename)
{
	int fi = GetCppFileIndex(filename);
	CppBase& base = CodeBase();
	for(int j = 0; j < base.GetCount(); j++) {
		const Array<CppItem>& n = base[j];
		for(int k = 0; k < n.GetCount(); k++) {
			const CppItem& m = n[k];
			if(m.file == fi) {
				String coderef = MakeCodeRef(base.GetKey(j), m.qitem);
				SetAnnotation(m.line - 1,
				              GetRefLinks(coderef).GetCount() ? IdeImg::tpp_doc()
				                                              : IdeImg::tpp_pen(),
				              coderef);
			}
		}
	}
}

bool IsCodeItem(RichText& txt, int i)
{
	static Uuid codeitem = CodeItemUuid();
	static Uuid stritem = StructItemUuid();;
	if(i < txt.GetPartCount() && txt.IsPara(i)) {
		Uuid style = txt.GetParaStyle(i);
		return style == codeitem || style == stritem;
	}
	return false;
}

bool IsBeginEnd(RichText& txt, int i)
{
	static Uuid begin = BeginUuid();
	static Uuid end = EndUuid();
	if(i < txt.GetPartCount() && txt.IsPara(i)) {
		Uuid style = txt.GetParaStyle(i);
		return style == begin || style == end;
	}
	return false;
}

bool AssistEditor::GetAnnotationRefs(Vector<String>& tl, String& coderef, int q)
{
	if(annotation_popup.IsOpen())
		annotation_popup.Close();
	if(q < 0)
		q = GetActiveAnnotationLine();
	if(q < 0)
		return false;
	coderef = GetAnnotation(q);
	if(IsNull(coderef))
		return false;
	tl = GetRefLinks(coderef);
	return true;
}

int BestMatch(const char *s, const char *t)
{
	int i = 0;
	while(s[i] == t[i] && *s)
		i++;
	return i;
}

bool AssistEditor::GetAnnotationRef(String& t, String& coderef, int q)
{
	Vector<String> tl;
	if(!GetAnnotationRefs(tl, coderef, q))
		return false;
	if(tl.GetCount() == 0)
		return true;
	String path = theide ? theide->editfile : Null;
	int mi = 0;
	int m = 0;
	for(int i = 0; i < tl.GetCount(); i++) {
		int mm = BestMatch(tl[i], path);
		if(mm > m) {
			mi = i;
			m = mm;
		}
	}
	t = tl[mi];
	return true;
}

void AssistEditor::SyncAnnotationPopup()
{
	String coderef;
	String tl;
	if(!GetAnnotationRef(tl, coderef))
		return;
	if(tl.GetCount()) {
		static String   last_path;
		static RichText topic_text;
		String path = GetTopicPath(tl);
		if(path != last_path)
			topic_text = ParseQTF(ReadTopic(LoadFile(path)).text);
		
		RichText result;
		for(int i = 0; i < topic_text.GetPartCount(); i++)
			if(IsCodeItem(topic_text, i) && topic_text.Get(i).format.label == coderef) {
				while(i > 0 && IsCodeItem(topic_text, i)) i--;
				if(!IsCodeItem(topic_text, i)) i++;
				while(IsCodeItem(topic_text, i))
					result.Cat(topic_text.Get(i++));
				while(i < topic_text.GetPartCount() && topic_text.IsPara(i) && !IsCodeItem(topic_text, i)
				      && !IsBeginEnd(topic_text, i))
					result.Cat(topic_text.Get(i++));
				break;
			}
		annotation_popup.Pick(result, GetRichTextStdScreenZoom());
	}
	else
		annotation_popup.SetQTF("[A1 [@b* " + DeQtf(coderef) + "]&Not documented yet - click to document");
	Rect r = GetLineScreenRect(GetActiveAnnotationLine());
	int h = annotation_popup.GetHeight(580);
	h = min(h, 550);
	int y = r.top - h - 16;
	if(y < GetWorkArea().top)
		y = r.bottom;
	annotation_popup.SetRect(r.left, y, 600, h + 16);
	annotation_popup.Ctrl::PopUp(this, false, false, true);
}

void AssistEditor::OpenTopic(String topic, String create)
{
	if(theide)
		theide->OpenTopic(topic, create);
}

void AssistEditor::EditAnnotation()
{
	String coderef;
	Vector<String> tl;
	if(!GetAnnotationRefs(tl, coderef))
		return;
	SetCursor(GetPos(GetActiveAnnotationLine()));
	if(tl.GetCount() > 1) {
		MenuBar bar;
		for(int i = 0; i < tl.GetCount(); i++)
			bar.Add(tl[i], THISBACK2(OpenTopic, tl[i] + '#' + coderef, String()));
		bar.Execute();
		return;
	}
	if(tl.GetCount()) {
		OpenTopic(tl[0] + '#' + coderef, String());
		return;
	}
	for(int i = GetCursorLine() - 1; i >= 0; i--) {
		String coderef2;
		if(GetAnnotationRefs(tl, coderef2, i) && tl.GetCount()) {
			MenuBar bar;
			for(int i = 0; i < tl.GetCount(); i++)
				bar.Add(tl[i], THISBACK2(OpenTopic, tl[i] + '#' + coderef2, coderef));
			bar.Execute();
			return;
		}
	}
}
