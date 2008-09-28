#include "ide.h"

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

void AssistEditor::SyncAnnotationPopup()
{
	if(annotation_popup.IsOpen())
		annotation_popup.Close();
	int q = GetActiveAnnotationLine();
	if(q < 0)
		return;
	String coderef = GetAnnotation(q);
	if(IsNull(coderef))
		return;
	Vector<String> l = GetRefLinks(coderef);
	if(l.GetCount() == 0)
		return;
	static String   last_path;
	static RichText topic_text;
	String path = GetTopicPath(l[0]);
	if(path != last_path)
		topic_text = ParseQTF(ReadTopic(LoadFile(path)).text);
	
	RichText result;
	for(int i = 0; i < topic_text.GetPartCount(); i++)
		if(IsCodeItem(topic_text, i) && topic_text.Get(i).format.label == coderef) {
			while(IsCodeItem(topic_text, i))
				result.Cat(topic_text.Get(i++));
			while(i < topic_text.GetPartCount() && topic_text.IsPara(i) && !IsCodeItem(topic_text, i))
				result.Cat(topic_text.Get(i++));
			break;
		}

	Rect r = GetLineScreenRect(q);
	annotation_popup.SetRect(r.left, r.bottom, 500, 200);
	annotation_popup.Pick(result);
	annotation_popup.Ctrl::PopUp(this, false, false, true);
}
