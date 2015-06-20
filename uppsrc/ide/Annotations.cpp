#include "ide.h"

void AssistEditor::Annotate(const String& filename)
{
	int fi = GetSourceFileIndex(filename);
	CppBase& base = CodeBase();
	ClearAnnotations();
	for(int j = 0; j < base.GetCount(); j++) {
		String nest = base.GetKey(j);
		if(*nest != '@') { // Annotations of anonymous structures not suported
			const Array<CppItem>& n = base[j];
			for(int k = 0; k < n.GetCount(); k++) {
				const CppItem& m = n[k];
				if(m.file == fi) {
					String coderef = MakeCodeRef(nest, m.qitem);
					SetAnnotation(m.line - 1,
					              GetRefLinks(coderef).GetCount() ? IdeImg::tpp_doc()
					                                              : IdeImg::tpp_pen(),
					              coderef);
				}
			}
		}
	}
}

bool IsCodeItem(RichText& txt, int i)
{
	static Uuid codeitem = CodeItemUuid();
	static Uuid stritem = StructItemUuid();
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
		int mm = GetMatchLen(tl[i], path);
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
		String cr = coderef;
		for(int pass = 0; pass < 2; pass++) {
			for(int i = 0; i < topic_text.GetPartCount(); i++)
				if(IsCodeItem(topic_text, i) && topic_text.Get(i).format.label == cr) {
					while(i > 0 && IsCodeItem(topic_text, i)) i--;
					if(!IsCodeItem(topic_text, i)) i++;
					while(IsCodeItem(topic_text, i))
						result.Cat(topic_text.Get(i++));
					while(i < topic_text.GetPartCount() && !IsCodeItem(topic_text, i)
					      && !IsBeginEnd(topic_text, i)) {
						if(topic_text.IsPara(i))
							result.Cat(topic_text.Get(i++));
						else {
							RichTable table(topic_text.GetTable(i++), 1);
							result.CatPick(pick(table));
						}
					}
					pass = 2;
					break;
				}
			if(pass == 0 && !LegacyRef(cr))
				break;
		}
		result.SetStyles(topic_text.GetStyles());
		annotation_popup.Pick(pick(result), GetRichTextStdScreenZoom());
	}
	else
		if(SyncRefsFinished)
			annotation_popup.SetQTF("[A1 [@b* " + DeQtf(coderef) + "]&Not documented yet - click to document");
		else
			annotation_popup.SetQTF("[A1 [@b* " + DeQtf(coderef) + "]&Documentation not loaded yet");
	Rect r = GetLineScreenRect(GetActiveAnnotationLine());
	int h = annotation_popup.GetHeight(580);
	h = min(h, 550);
	int y = r.top - h - 16;
	if(y < GetWorkArea().top)
		y = r.bottom;
	annotation_popup.SetRect(r.left, y, 600, h + 16);
	annotation_popup.Ctrl::PopUp(this, false, false, true);
}

void AssistEditor::OpenTopic(String topic, String create, bool before)
{
	if(theide)
		theide->OpenTopic(topic, create, before);
}

void AssistEditor::NewTopic(String group, String coderef)
{
	if(!theide)
		return;
	String ef = theide->editfile;
	String n = GetFileTitle(ef);
	theide->EditFile(AppendFileName(PackageDirectory(theide->GetActivePackage()), group + ".tpp"));
	if(!theide->designer)
		return;
	TopicEditor *te = dynamic_cast<TopicEditor *>(&theide->designer->DesignerCtrl());
	if(!te)
		return;
	String scope, item;
	SplitCodeRef(coderef, scope, item);
	if(!te->NewTopicEx(IsNull(scope) ? n : Join(Split(scope, ':'), "_"), coderef))
		theide->EditFile(ef);
}

void AssistEditor::EditAnnotation(bool fastedit)
{
	if(!SyncRefsFinished)
		return;
	String coderef;
	Vector<String> tl;
	if(!GetAnnotationRefs(tl, coderef))
		return;
	SetCursor(GetPos(GetActiveAnnotationLine()));
	if(tl.GetCount() > 1) {
		MenuBar bar;
		for(int i = 0; i < tl.GetCount(); i++)
			bar.Add(tl[i], THISBACK3(OpenTopic, tl[i] + '#' + coderef, String(), false));
		bar.Execute();
		return;
	}
	if(tl.GetCount()) {
		OpenTopic(tl[0] + '#' + coderef, String(), false);
		return;
	}
	String scope, item;
	SplitCodeRef(coderef, scope, item);
	const CppItem *m = GetCodeRefItem(coderef);
	int access = m ? m->access : 0;
	VectorMap<String, String> tpp;
	int backi = 0;
	for(int pass = 0; pass < 2; pass++) {
		for(int i = GetCursorLine(); pass ? i < GetLineCount() : i >= 0; pass ? i++ : i--) {
			String coderef2;
			if(GetAnnotationRefs(tl, coderef2, i) && tl.GetCount()) {
				String scope2, item2;
				SplitCodeRef(coderef2, scope2, item2);
				m = GetCodeRefItem(coderef2);
				if(scope2 == scope && m && m->access == access && tl.GetCount() == 1 && fastedit) {
					OpenTopic(tl[0] + '#' + coderef2, coderef, false);
					return;
				}
				for(int j = 0; j < tl.GetCount(); j++)
					if(tpp.Find(tl[j]) < 0)
						tpp.Add(tl[j], coderef2);
			}
		}
		if(pass == 0)
			backi = tpp.GetCount();
	}
	MenuBar bar;
	if(tpp.GetCount()) {
		for(int i = 0; i < tpp.GetCount(); i++) {
			String l = tpp.GetKey(i);
			bar.Add(l, THISBACK3(OpenTopic, l + '#' + tpp[i], coderef, i >= backi));
		}
		bar.Separator();
	}
	bar.Add("New reference topic..", THISBACK2(NewTopic, "src", coderef));
	bar.Add("New implementation topic..", THISBACK2(NewTopic, "srcimp", coderef));
	bar.Execute();
}
