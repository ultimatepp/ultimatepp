#include "ide.h"

#define LDUMP(x) // DUMP(x)
#define LLOG(x)  // LOG(x)

bool DocEditEnter::Key(dword key, int count)
{
	if(key == K_ENTER) {
		WhenEnter();
		return true;
	}
	return DocEdit::Key(findarg(key, K_CTRL|K_ENTER, K_ALT|K_ENTER, K_SHIFT|K_ENTER) >= 0 ? K_ENTER : key, count);
}

Ide::AIThread Ide::AIChatGPT(const String& instructions, const String& code, const String& prev_id)
{
	Ide::AIThread r;
#ifdef _DEBUG
//	HttpRequest::Trace();
#endif

	HttpRequest http("https://api.openai.com/v1/responses");

	http.Header("Authorization", "Bearer " + ai_key);

	Upp::Json request;
	request("model", ai_model)
	       ("prompt_cache_key", "experimental_query")
	       ("instructions", instructions)
	       ("input", code)
	       ("max_output_tokens", 100000)
	       ("background", true);

	if(prev_id.GetCount())
		request("previous_response_id", prev_id);

	String responseText = http.ContentType("application/json").Post(request).Execute();
		
	Progress pi("AI thinking...");

	http.WhenWait = [&] {
		if(pi.StepCanceled())
			http.Abort();
	};

	if(http.IsError()) {
		r.error << "HTTP request failed: " << http.GetErrorDesc();
		return r;
	}

	Value responseJson = ParseJSON(responseText);

	String responseId = AsString(responseJson["id"]);
	String status = AsString(responseJson["status"]);

	int ms = msecs();
	while(findarg(status, "queued", "in_progress") >= 0) {
		if(pi.StepCanceled()) {
			r.error = "Canceled by user";
			return r;
		}
		
		Sleep(0);
	
		int ms1 = msecs();
		if(ms1 - ms > 1000) {
			ms = ms1;
			HttpRequest poll("https://api.openai.com/v1/responses/" + responseId);
			poll.Header("Authorization", "Bearer " + ai_key);
			poll.WhenWait = [&] {
				if(pi.StepCanceled())
					http.Abort();
			};
		
			responseText = poll.Execute(); // GET is the default
			if(poll.IsError()) {
				r.error << "Polling HTTP request failed: " << poll.GetErrorDesc();
				return r;
			}
		
			responseJson = ParseJSON(responseText);
			if(IsNull(responseJson)) {
				r.error << "Invalid JSON response while polling: " << responseText;
				return r;
			}
			
			status = AsString(responseJson["status"]);
		}
	}

	LOG("Response status: " << status);

	if(status != "completed") {
		r.error << "Response did not complete. Status: " << status;
		return r;
	}

	if(!IsNull(responseJson["error"])) {
		r.error << "OpenAI API error: " << ~responseJson["error"]["message"];
		return r;
	}

	for(Value item : responseJson["output"])
		if(AsString(item["type"]) == "message") {
			Value content = item["content"];
			for(Value part : content) {
				if(part["type"] == "output_text")
					r.result << part["text"];
			}
		}

	r.response_id = AsString(responseJson["id"]);
/*
	if(AsString(responseJson["status"]) == "incomplete") {
		LOG("Warning: generation was incomplete. Reason: "
		    << AsString(responseJson["incomplete_details"]["reason"]));
	}
*/
	Value usage = responseJson["usage"];
	if(!IsNull(usage)) {
		Value inputDetails = usage["input_tokens_details"];
		PutVerbose(String() << "Input bytes: " << code.GetCount());
		PutVerbose(String() << "Input tokens: " << usage["input_tokens"]);
		PutVerbose(String() << "Cached input tokens: " << inputDetails["cached_tokens"]);
		PutVerbose(String() << "Output tokens: " << usage["output_tokens"]);
		PutVerbose(String() << "Total tokens: " << usage["total_tokens"]);
	}
	
	return r;
}

Ide::AIThread Ide::AIGemini(const String& instructions, const String& code, const String& prev_id)
{
	Ide::AIThread r;
	return r;
#if 0
	HttpRequest http("https://generativelanguage.googleapis.com/v1beta/models/" + ai_model + ":generateContent?key=" + ai_key);

	String responseText =
	http.ContentType("application/json").Post(
		Upp::Json("contents", JsonArray() <<
					Upp::Json("role", "user")
		                     ("parts", JsonArray() << Upp::Json("text", code)
		                                           << Upp::Json("text", instructions)))
	).Execute();

	if (http.IsError()) {
		LLOG("HTTP Request failed: " << http.GetErrorDesc());
		return String::GetVoid();
	}

	Value responseJson = ParseJSON(responseText);
	
	if (!IsNull(responseJson["error"])) {
		LLOG("Gemini API Error: " << responseJson["error"]["message"]);
		return String::GetVoid();
	}

	return ~responseJson["candidates"][0]["content"]["parts"][0]["text"];
#endif
}

Ide::AIThread Ide::AI(const String& instructions, const String& code_, const String& prev_id_)
{
	String code = code_;
	String prev_id = prev_id_;
tool_restart:
	LDUMP(code);
	Ide::AIThread r = ai_kind == 0 ? AIChatGPT(instructions, code, prev_id)
	                               : AIGemini(instructions, code, prev_id);

	auto TimeStamp = [&](FileIn& in) {
		
	};
	LDUMP(r.result);
	int q = r.result.FindAfter("⚡GET ");
	bool stamp = false;
	if(q < 0) {
		stamp = true;
		q = r.result.FindAfter("⚡GET_TIME ");
	}
	if(q >= 0) {
		String path = r.result.Mid(q);
		for(const char *delim : { "\n", "⚡", " " }) {
			int q = path.Find(delim);
			if(q >= 0)
				path.Trim(q);
		}
		LLOG("GET FILE " << path);
		for(String n : GetUppDirs()) {
			FileIn in(n + "/" + path);
			if(in) {
				code << "TIME: " << Time(in.GetTime()) << "\n";
				if(!stamp) {
					String raw;
					int ii = 0;
					while(!in.IsEof()) {
						String l = in.GetLine();
						code << ++ii << ": " << l << "\n";
						raw << l << "\n";
					}
					LDUMP(path);
					ai_file.GetAdd(path) = raw;
					prev_id = r.response_id;
				}
				goto tool_restart;
			}
		}
	}
	return r;
}

struct SummaryAIDlg : WithSummaryAILayout<TopWindow> {
	SummaryAIDlg();
};

SummaryAIDlg::SummaryAIDlg()
{
	CtrlLayoutOKCancel(*this, "AI summary generator");
	
	list.AddColumn("").With([](One<Ctrl>& ctrl) { // 0
		ctrl.Create<Option>().NoWantFocus();
	});
	list.AddColumn("Package"); // 1
	auto fmt = [](const Value& v)->Value {
		Time t = v;
		return IsNull(t) ? Null :
			   Format("%02.2d-%02.2d-%02.2d %02.2d`:%02.2d`:%02.2d",
	                  t.year, t.month, t.day, t.hour, t.minute, t.second);
	};
	list.AddColumn("Package date").ConvertBy(fmt); // 2
	list.AddColumn("Package size"); // 3
	list.AddColumn("Summary date").ConvertBy(fmt); // 4
	list.AddColumn("Summary size"); // 5
	list.AddIndex(); // 6
	list.ColumnWidths("29 180 117 83 117 83");

	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		int ii = wspc.use_order[i];
		const Package& pk = wspc.GetPackage(ii);
		String n = wspc[ii];
		Time ltm = Null;
		int64 len = 0;
		for(int i = 0; i < pk.GetCount(); i++) {
			String path = SourcePath(n, pk.file[i]);
			if(GetFileExt(path) != ".ai" && FileExists(path)) {
				FindFile ff(path);
				if(ff) {
					ltm = max(Nvl(ltm, Time::Low()), Time(ff.GetLastWriteTime()));
					len += ff.GetLength();
				}
			}
		}
		String spath = SourcePath(n, "summary.ai");
		int64 l = GetFileLength(spath);
		list.Add(false, n, ltm, AttrText(FormatFileSize(len)).Right(), FileGetTime(spath),
		         AttrText(l >= 0 ? FormatFileSize(l) : String()).Right(),
		         ii);
	}
	
	all << [&] {
		bool set = false;
		for(int i = 0; i < list.GetCount(); i++)
			if(!(bool)list.Get(i, 0))
				set = true;
		for(int i = 0; i < list.GetCount(); i++)
			list.Set(i, 0, set);
	};
	
	date << [this] {
		for(int i = 0; i < list.GetCount(); i++)
			if((Time)list.Get(i, 2) > (Time)list.Get(i, 4))
				list.Set(i, 0, true);
	};
}

String Ide::GetAIFileMap()
{
	String map;
	for(const auto& f : ~CodeIndex()) { // do base classes first
		String n = GetPathNest(f.key);
		if(n.GetCount()) {
			map << "⚡FILE " << UnixPath(f.key.Mid(n.GetCount() + 1)) << "\n";
			String names;
			Index<String> nests;
			for(const AnnotationItem& m : f.value.items) {
				nests.FindAdd(m.nest);
				bool cpp = findarg(GetFileExt(f.key), ".c", ".cpp") >= 0;
				if(m.name.GetCount() && !m.isstatic &&
				   (!cpp && m.kind != CXCursor_MacroDefinition) &&
				   findarg(m.kind, CXCursor_Constructor, CXCursor_Destructor,
				                   CXCursor_ConversionFunction, CXCursor_CXXMethod,
				                   CXCursor_FieldDecl, CXCursor_StructDecl, CXCursor_UnionDecl,
				                   CXCursor_ClassDecl, CXCursor_EnumDecl) < 0 &&
				   *m.name != '(') {
					MergeWith(names, " ", AsString(m.kind) + m.name);
				}
			}
			if(nests.GetCount())
				map << "TYPES " << Join(nests.GetKeys(), " ") << "\n";
			if(names.GetCount())
				map << "NAMES " << names << "\n";
		}
	}
	return map;
}

String Ide::GetAISummaries()
{
	String summaries;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		summaries << LoadFile(SourcePath(wspc[i], "summary.ai"));
	return summaries;
}

void Ide::AISummary()
{
	SummaryAIDlg dlg;

	if(!dlg.ExecuteOK())
		return;

	const Workspace& wspc = GetIdeWorkspace();
	
	String instructions = R"(
		You have been supplied with a map of C++ source files. You can get the content
		of any file in this map if you reply raw unformatted line
		
		⚡GET file
		
		Provide a short summary of ⚡HEADER - about 200 characters.
		Do not hesitate to check other files (implementation files, @includes,
		check the map for candidates) to improve your summary. If ⚡HEADER is missing,
		do nothing. Do not repeat the header name in the summary."
	)";

	AIThread ai = AI(instructions, GetAIFileMap());
	
	if(ai.error.GetCount()) {
		Exclamation("\1" + ai.error);
		return;
	}

	String base_id = ai.response_id;

	for(int i = 0; i < dlg.list.GetCount(); i++) {
		if(dlg.list.Get(i, 0)) {
			int ii = dlg.list.Get(i, 6);
			String n = wspc[ii];
			const Package& pkg = wspc.GetPackage(ii);
			
			String summary;
			
			for(int i = 0; i < pkg.GetCount(); i++) {
				String hdr = n + "/" + pkg.file[i];
				if(GetFileExt(hdr) == ".h") {
					AIThread ai;
					for(int retry = 0; retry < 3; retry++) {
						ai = AI(instructions, "\n⚡HEADER " + hdr, base_id);
						if(ai.error.GetCount() == 0)
							break;
					}
					if(ai.error.GetCount()) {
						Exclamation("\1" + ai.error);
						return;
					}
					if(ai.result.GetCount())
						summary << hdr << ": " << ai.result << "\n";
				}
			}
			
			Upp::SaveFile(SourcePath(~dlg.list.Get(i, 1), "summary.ai"), summary);
		}
	}
}

void Ide::AIPane()
{
	if(ai_pane.GetParent())
		IdeRemoveRight(ai_pane);
	else
		IdeSetRight(ai_pane);
}

void Ide::AIChat()
{
	AIPane();
	ai_prompt.SetFocus();
}

void Ide::AIPrompt()
{
	AIAppendOutput(String() << "&[g@B \1" << ~ai_prompt << "\1]&");
	AIPrompt2(~ai_prompt);
	ai_prompt <<= Null;
}

static String general_instructions = R"(
	You are a C++ code assistant. You receive a map of source files and header summaries. You may also receive a ⚡CURRENT_FILE marker identifying a file and either a cursor line or a line range. The map and marker tell you where to look; they do not establish what the source code says.
	
	1. Identify the task.
	Follow the target named or implied by the user. Otherwise, use ⚡CURRENT_FILE and its indicated line or range. A request such as “explain” means “explain the code at this location.” Use the enclosing declaration, statement, or function as needed. Do not ask for clarification when the target is reasonably clear.
	
	2. Issue retrieval commands yourself.
	The user gives you a task. YOU issue retrieval commands. The host reads your command and returns the result. Never ask the user to type, resend, confirm, or reformat a retrieval command.
	
	If the source lines needed for the task are not present in the conversation, your entire next reply must be:
	⚡GET file
	
	Replace file with the exact path from the map. Emit exactly one raw, unformatted line: no indentation, quotation marks, Markdown, explanation, apology, or second command. End the turn there. Do not say “I’ll check the file”; emit the command.
	
	Example: If the user says “explain,” the cursor is at Core/Util.h:410, and the source lines are absent, your entire reply is:
	⚡GET Core/Util.h
	
	3. Handle cached files correctly.
	The first line of a returned file is its timestamp, not a source line. Use a freshly returned file immediately. If you need to consult a previously retrieved copy again, first emit:
	⚡GET_TIME file. Freshly means since the last user prompt which specified ⚡CURRENT_FILE.
	
	The same one-raw-line rule applies. If the returned timestamp matches the cached timestamp and the needed lines are still present, use the cached source. If the timestamp differs or the needed lines are missing, issue ⚡GET for that file. A timestamp alone is not source content.
	
	4. Continue the original task across retrievals.
	A command is a request, not a result. Do not explain or cite missing code until the host has returned the source. After each result, continue the original task without waiting for another user prompt: either issue the next necessary command or answer from the inspected source.
	
	If you previously promised to check a file but did not emit a command, the retrieval is still pending. On a follow-up such as “really?” or “so?”, emit the pending command instead of apologizing, making another promise, or guessing.
	
	5. Verify behavior against definitions.
	When asked what a function or method does, inspect its definition. A definition in a header is sufficient. If the current source contains only a declaration, use the map to locate likely definition sites, starting with the corresponding implementation file. Retrieve and inspect candidates as needed. Verify the enclosing scope and signature so you do not mistake an overload or similarly named function for the target.
	
	If the definition cannot be found after checking the plausible mapped files, say that you could not find it in the supplied sources. Explain only what the declaration establishes. Do not infer implementation behavior from a name, declaration, or map summary.
	
	6. Answer directly and cite the source.
	Once you have enough evidence, answer the user's request without narrating the retrieval process. Always mark location as ⚡file:line - do not use spaces or `.
	When reporting a bug or issue to fix, mark its location as 🛠file:line - do not use spaces or `. Use the exact mapped path and an inspected source line number. Do not count the timestamp as a source line or invent a citation.
)";

void Ide::AIPrompt2(const String& prompt)
{
	SaveFile();
	String p;
	if(IsNull(ai_prev))
		p << GetAIFileMap() << GetAISummaries();
	p << prompt;
	String n = GetPathNest(editfile);
	if(n.GetCount())
		p << "\n⚡CURRENT_FILE " << UnixPath(editfile.Mid(n.GetCount() + 1)) << "\n";
	int l, h;
	if(editor.GetSelection(l, h))
		p << ", lines " << editor.GetLine(l) + 1 << " - " << editor.GetLine(h) + 1 << "\n";
	else
		p << ", line " << editor.GetCursorLine() + 1;

	AIThread result = AI(general_instructions, p, ai_prev);
	AIAppendOutput(AIResult(result));
	if(!ai_pane.GetParent())
		AIPane();
}

String Ide::AIResult(const AIThread& t)
{
	String qtf = MarkdownConverter().ToQtf(t.result);
	for(;;) {
		String hdr = "⚡";
		int l = qtf.Find(hdr);
		bool fix = false;
		if(l < 0) {
			hdr = "🛠";
			l = qtf.Find(hdr);
			if(l < 0)
				break;
			fix = true;
		}
		int sc = qtf.FindAfter(":", l);
		if(sc < 0)
			break;
		while(IsDigit(qtf[sc]))
			sc++;
		String link = Filter(qtf.Mid(l + hdr.GetCount(), sc - l - hdr.GetCount()),
		                     [](int c) { return c == '`' ? 0 : c; });
		qtf.Remove(l, sc - l);
		String s = "[^" + link + "^ \1" + link + "\1]";
		if(fix)
			s << " [^!" + link + "^ Fix]";
		qtf.Insert(l, s);
	}
	ai_prev = t.response_id;
	return qtf;
}

void Ide::AIAppendOutput(const String& qtf)
{
	ai_qtf << qtf;
	ai_output.SetQTF(ai_qtf);
	ai_output.ScrollEnd();
}

void Ide::AILink(const String& link)
{
	if(*link == '!') {
		String path = link.Mid(1);
		if(!GotoPosition(path))
			return;
		int q = path.Find(':');
		if(q >= 0)
			path.Trim(q);
		LLOG("FIX " << path);
		String raw = ai_file.Get(path, editor.Get());
		if(IsNull(raw)) {
			AIAppendOutput("[g Uknown file: \1" + path);
			return;
		}
		AIThread r = AI(general_instructions,
		                "Suggest the fix for issue " + path + ". Fix should be a continous range of lines. "
		                "Show the range of lines you are replacing with line1:line2 range in the first line. "
		                "Do not add any text or explanation, just the required header and the raw code."
		                "Do not use any formatting, I will be using the code as is.",
		                ai_prev);
		ai_prev = r.response_id;
		int p = r.result.Find('\n');
		if(p > 0) {
			String h = r.result.Mid(0, p);
			String code = r.result.Mid(p + 1);
			h.TrimStart("⚡");
			CParser p(h);
			int line1, line2;
			try {
				line1 = p.ReadInt() - 1;
				p.Char(':');
				line2 = p.ReadInt() - 1;
				Vector<String> l = Split(raw, '\n', false);
				
				
				
				if(line1 <= line2 && line1 < l.GetCount() && line2 < l.GetCount() && line1 >= 0) {
					l.Remove(line1, line2 - line1 + 1);
					Vector<String> fix = Split(code, '\n', false);
					l.Insert(line1, fix);
					
					editor.SetSelection(editor.GetPos(line1), editor.GetPos(line2));
					static FileSel dummy;
				    FileDiff& dlg = CreateNewWindow<FileDiff>(dummy);
				    String left = editor.Get();
				    String right = Join(l, "\n");
				    dlg.Set(editfile);
				    dlg.r <<= "Proposed fix";
				    dlg.r.Disable();
				    dlg.diff.Set(left, right);
				    dlg.diff.indent <<= false;
				    dlg.diff.indent << [=, &dlg] { dlg.diff.Set(left, right); };
					dlg.Title("Suggested fix");
					dlg.Icon(IdeImg::DiffClip());
				    dlg.OpenMain();
					return;
				}
			}
			catch(CParser::Error) {}
		}
		AIAppendOutput("[g@R Unexpected reply:&][g \1" + r.result + "\1]");
		return;
	}
	GotoPosition(link);
}
