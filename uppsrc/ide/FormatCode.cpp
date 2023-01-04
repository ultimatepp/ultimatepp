#include "ide.h"

bool ReFormatJSON_XML( String& text, bool xml)
{
	if(xml) {
		try {
			XmlNode n = ParseXML(text);
			text = AsXML(n);
		}
		catch(XmlError) {
			Exclamation("Error passing the XML!");
			return false;
		}
	}
	else {
		Value v = ParseJSON(text);
		if(v.IsError()) {
			Exclamation("Error passing the JSON!");
			return false;
		}
		text = AsJSON(v, true);
	}
	return true;
}

void Ide::FormatJSON_XML(bool xml)
{
	int l, h;
	bool sel = editor.GetSelection(l, h);
	if((sel ? h - l : editor.GetLength()) > 75*1024*1024) {
		Exclamation("Too big to reformat");
		return;
	}
	String text;
	if(sel)
		text = editor.GetSelection();
	else {
		SaveFile();
		text = LoadFile(editfile);
	}
	if(!ReFormatJSON_XML(text, xml))
		return;
	editor.NextUndo();
	if(sel) {
		editor.Remove(l, h - l);
		editor.SetSelection(l, l + editor.Insert(l, text));
	}
	else {
		editor.Remove(0, editor.GetLength());
		editor.Insert(0, text);
	}
}

void Ide::FormatJSON()
{
	FormatJSON_XML(false);
}

void Ide::FormatXML()
{
	FormatJSON_XML(true);
}

void Ide::FormatJSON_XML_File(bool xml)
{
	if(IsNull(editfile))
		return;
	if(GetFileLength(editfile) >= 75*1024*1024)
		Exclamation("Too big to reformat");
	SaveFile();
	String text = LoadFile(editfile);
	if(!ReFormatJSON_XML(text, xml))
		return;
	if(PromptYesNo("Overwrite \1" + editfile + "\1 with reformated " + (xml ? "XML" : "JSON") + "?")) {
		Upp::SaveFile(editfile, text);
		EditAsText();
	}
}
