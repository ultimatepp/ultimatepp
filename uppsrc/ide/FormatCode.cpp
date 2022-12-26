#include "ide.h"

bool ReFormatJSON_XML(String& text, bool xml)
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
	if((sel ? h - l : editor.GetLength()) > 75 * 1024 * 1024) {
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

void Ide::FormatJSON() { FormatJSON_XML(false); }
void Ide::FormatXML() { FormatJSON_XML(true); }

class ClangFormat final {
public:
	struct Output {
		struct Replacment : Moveable<Replacment> {
			String m_data;
			int m_offset;
			int m_length;
		};

		String m_raw_result;
		int m_code;

		bool IsSuccessful() const { return m_code == 0; }
		Vector<Replacment> FindReplacments() const;
	};

	struct Parameters {
		String m_file;
		int m_offset = Null;
		int m_length = Null;
		bool m_output_replacments_xml = false;
	};

public:
	ClangFormat(Ide* ide);

	Output Execute(const Parameters& params);

private:
	String SaveTemporaryFileToFormat(const String& file);
	Output Execute0(const String& cmd);

	static String GenerateClangFormatNotFoundErrorMsg();

private:
	String m_file;
	int m_offset;
	int m_length;

	Host m_host;
	Ide* m_ide;
};

Vector<ClangFormat::Output::Replacment> ClangFormat::Output::FindReplacments() const
{
	Vector<Replacment> replacmenets;

	XmlParser p(m_raw_result);
	while(!p.IsTag()) {
		p.Skip();
	}
	p.PassTag("replacements");
	while(!p.End()) {
		if(p.Tag("replacement")) {
			Replacment replacment;
			replacment.m_offset = p.Int("offset");
			replacment.m_length = p.Int("length");

			if(p.IsText()) {
				replacment.m_data = p.ReadText();
			}
			replacmenets.Add(replacment);

			p.PassEnd();
		}
		else {
			p.Skip();
		}
	}

	return replacmenets;
}

ClangFormat::ClangFormat(Ide* ide)
	: m_ide(ide)
{
	m_ide->CreateHost(m_host);
}

ClangFormat::Output ClangFormat::Execute(const Parameters& params)
{
	auto temp_file = SaveTemporaryFileToFormat(params.m_file);
	if(temp_file.IsEmpty()) {
		PutConsole(String() << "Error: failed to save temporary file for formatting.");
		return {"", -1};
	}

	String cmd = "clang-format ";
	if(params.m_output_replacments_xml) {
		cmd << "--output-replacements-xml ";
	}
	if(!IsNull(params.m_offset) && !IsNull(params.m_length)) {
		cmd << "--offset=" << IntStr(params.m_offset) << " --length=" << IntStr(params.m_length)
			<< " ";
	}
	cmd << temp_file;

	if(m_ide->IsVerbose()) {
		m_ide->PutConsole(cmd);
	}

	auto output = Execute0(cmd);
	DeleteFile(temp_file);

	return output;
}

String ClangFormat::SaveTemporaryFileToFormat(const String& file)
{
	auto file_name = GetFileName(file);

	auto temp_file = file.Left(file.ReverseFind(DIR_SEPS) + 1);
	m_ide->PutConsole(temp_file);
	temp_file += "_ide_file_to_format_" + file_name;

	if(FileExists(temp_file)) {
		DeleteFile(temp_file);
	}

	FileOut out(temp_file);
	m_ide->editor.Save(out, CHARSET_UTF8, TextCtrl::LE_LF);

	return temp_file;
}

ClangFormat::Output ClangFormat::Execute0(const String& cmd)
{
	StringStream ss;
	int code = m_host.Execute(cmd, ss);
	if(code != 0) {
		m_ide->ConsoleClear();
		m_ide->ConsoleShow();
		if(IsNull(code)) {
			PutConsole(String() << "Error: " << GenerateClangFormatNotFoundErrorMsg());
		}
		else {
			PutConsole(String() << "Error: clang-format ended with \"" << IntStr(code)
			                    << "\" error code.");
			if(!ss.GetResult().IsEmpty()) {
				PutConsole(String() << "\nProgram output:\n" << ss.GetResult());
			}
		}
		m_ide->BeepMuteExclamation();
		return {"", code};
	}
	return {ss.GetResult(), code};
}

String ClangFormat::GenerateClangFormatNotFoundErrorMsg()
{
#ifdef PLATFORM_WIN32
	return "Failed to find clang-format command. Make sure you have valid TheIDE installation "
	       "and clang-format is available under bin folder.";
#else
	return "Failed to find clang-format command. Make sure it is added to the enviroment "
	       "variables.";
#endif
}

void Ide::ReformatFile()
{
	int l, h;
	bool sel = editor.GetSelection(l, h);

	ClangFormat::Parameters params;
	params.m_file = editfile;
	if(sel) {
		params.m_offset = l;
		params.m_length = (h - l);
	}
	params.m_output_replacments_xml = true;

	ClangFormat clang_format(this);
	auto output = clang_format.Execute(params);
	if(!output.IsSuccessful()) {
		return;
	}

	Vector<ClangFormat::Output::Replacment> replacmenets;
	try {
		replacmenets = output.FindReplacments();
		if(replacmenets.IsEmpty()) {
			return;
		}
	}
	catch(const XmlError& e) {
		PutConsole(String() << "Error: Failed to parse clang-format ouput with error \"" << e
		                    << "\".");
		return;
	}

	editor.NextUndo();
	int shift = 0;
	for(const auto& replacmenet : replacmenets) {
		int data_count = replacmenet.m_data.GetCount();
		int length = replacmenet.m_length;
		int offset = replacmenet.m_offset + shift;

		if(length > 0) {
			editor.SetSelection(offset, offset + length);
			editor.RemoveSelection();

			shift -= length;
		}
		if(data_count > 0) {
			editor.Insert(offset, replacmenet.m_data);
			shift += data_count;
		}
	}
}

void Ide::ReformatComment() { editor.ReformatComment(); }
