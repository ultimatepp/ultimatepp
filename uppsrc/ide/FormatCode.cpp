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
		Vector<Replacment> FindNormalizedReplacmenets(const WString& file_text) const;
	};

	struct Parameters {
		String m_file;
		int m_start_line = Null;
		int m_end_line = Null;
		bool m_output_replacments_xml = false;
	};

public:
	using Replacmenets = Vector<Output::Replacment>;

public:
	ClangFormat(Ide* ide);

	void PutErrorOnConsole(const String& output);
	static bool HasAssociatedClangFormatFile(const String& file);
	Output Execute(const Parameters& params);

private:
	static bool HasClangFormatFile(const String& dir);

	String SaveTemporaryFileToFormat(const String& file);
	Output Execute0(const String& cmd);

	static String GenerateClangFormatNotFoundErrorMsg();
	static String GenerateClangFormatFileNotFoundErrorMsg();

private:
	String m_file;

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

Vector<ClangFormat::Output::Replacment>
ClangFormat::Output::FindNormalizedReplacmenets(const WString& file_text) const
{
	auto reps = FindReplacments();
	if(reps.IsEmpty()) {
		return {};
	}

	if(file_text.GetCount() == Utf8Len(file_text)) {
		return reps;
	}

	Vector<Tuple<int, int>> long_chars;
	for(int i = 0; i < file_text.GetCount(); i++) {
		const int len = Utf8Len(file_text[i]);
		if(len > 1) {
			long_chars.Add({i, len});
		}
	}

	int shift = 0;
	for(auto& rep : reps) {
		shift += abs(rep.m_length - Utf8Len(rep.m_data.ToWString()));

		const auto current_offset = rep.m_offset;
		for(const auto& long_char : long_chars) {
			if((static_cast<int>(long_char.Get(0)) - shift) < current_offset) {
				rep.m_offset -= (static_cast<int>(long_char.Get(1)) - 1);
				continue;
			}
			break;
		}
	}

	return reps;
}

ClangFormat::ClangFormat(Ide* ide)
	: m_ide(ide)
{
	m_ide->CreateHost(m_host);
}

void ClangFormat::PutErrorOnConsole(const String& output)
{
	if(!m_ide->IsVerbose()) {
		m_ide->ConsoleClear();
	}
	m_ide->ConsoleShow();
	PutConsole(output);
	m_ide->BeepMuteExclamation();
}

bool ClangFormat::HasAssociatedClangFormatFile(const String& file)
{
	String dir = GetFileFolder(file);

	while(!dir.IsEmpty()) {
		if(HasClangFormatFile(dir)) {
			return true;
		}

		auto dir_sep_pos = dir.ReverseFind(DIR_SEPS);
		if(dir_sep_pos == -1) {
			break;
		}
		dir = dir.Left(dir.ReverseFind(DIR_SEPS));
	}

	return false;
}

bool ClangFormat::HasClangFormatFile(const String& dir)
{
	const Vector<String> valid_files = {".clang-format", "_clang-format"};
	for(const auto& valid_file : valid_files) {
		String path = dir + DIR_SEPS + valid_file;
		Logd() << path;
		if(FileExists(path)) {
			return true;
		}
	}
	return false;
}

ClangFormat::Output ClangFormat::Execute(const Parameters& params)
{
	if(!HasAssociatedClangFormatFile(params.m_file)) {
		PutErrorOnConsole(GenerateClangFormatFileNotFoundErrorMsg());
		return {"", -1};
	}

	auto temp_file = SaveTemporaryFileToFormat(params.m_file);
	if(temp_file.IsEmpty()) {
		PutConsole("Error: failed to save temporary file for formatting.");
		return {"", -1};
	}

	String cmd = "clang-format ";
	if(params.m_output_replacments_xml) {
		cmd << "--output-replacements-xml ";
	}
	if(!IsNull(params.m_start_line) && !IsNull(params.m_end_line)) {
		cmd << "--lines=" << IntStr(params.m_start_line) << ":" << IntStr(params.m_end_line)
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

	auto dir_sep_pos = file.ReverseFind(DIR_SEPS);
	if(dir_sep_pos == -1) {
		return {};
	}

	auto temp_file = file.Left(dir_sep_pos + 1);
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
		String error;
		if(IsNull(code)) {
			error << "Error: " << GenerateClangFormatNotFoundErrorMsg();
		}
		else {
			error << "Error: clang-format ended with \"" << IntStr(code) << "\" error code.\n";
			if(!ss.GetResult().IsEmpty()) {
				error << "\nProgram output:\n" << ss.GetResult();
			}
		}
		PutErrorOnConsole(error);

		return {"", code};
	}
	return {ss.GetResult(), code};
}

String ClangFormat::GenerateClangFormatNotFoundErrorMsg()
{
#ifdef PLATFORM_WIN32
	return "Failed to find clang-format command. Please make sure you have valid TheIDE "
		   "installation and clang-format is available under bin folder.";
#else
	return "Failed to find clang-format command. Please make sure it is added to the "
		   "enviroment variables.";
#endif
}

String ClangFormat::GenerateClangFormatFileNotFoundErrorMsg()
{
	return "Error: Failed to find associated .clang-format or _clang-format. "
		   "Make sure this file exists in your source tree. We highly "
		   "recommend to keep it on assembly level. Moreover, please keep in mind that "
		   ".clang-format file should be bundle with U++ installation and available for "
		   "common assemblies. If you do not have this file try to make fresh instalaltion "
		   "of U++.";
}

void Ide::ReformatFile()
{
	int64 l, h;
	bool sel = editor.GetSelection(l, h);

	ClangFormat::Parameters params;
	params.m_file = editfile;
	if(sel) {
		params.m_start_line = editor.GetLine(l) + 1;
		params.m_end_line = editor.GetLine(h) + 1;
	}
	params.m_output_replacments_xml = true;

	ClangFormat clang_format(this);
	auto output = clang_format.Execute(params);
	if(!output.IsSuccessful()) {
		return;
	}

	Vector<ClangFormat::Output::Replacment> replacmenets;
	try {
		replacmenets = output.FindNormalizedReplacmenets(editor.GetW());
		if(replacmenets.IsEmpty()) {
			return;
		}
	}
	catch(const XmlError& e) {
		clang_format.PutErrorOnConsole(
			String("Error: Failed to parse clang-format ouput with error \"" << e) << "\".");
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
