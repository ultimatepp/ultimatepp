#include "ide.h"

#define LDUMP(x)
#define LLOG(x) LOG(x)

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
			LOG("Response status: " << status);
		}
	}
	
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

Ide::AIThread Ide::AI(const String& instructions, const String& code, const String& prev_id)
{
	return ai_kind == 0 ? AIChatGPT(instructions, code, prev_id)
	                    : AIGemini(instructions, code, prev_id);
}

String Ide::AIResult(const AIThread& t)
{
	DUMP(t.result);
	String qtf = MarkdownConverter().ToQtf(t.result);
	for(;;) {
		int l = qtf.Find("`<`@`@");
		if(l < 0)
			break;
		int r = qtf.FindAfter("`@`@`>");
		if(r < 0)
			break;
		String link = qtf.Mid(l + 6, r - l - 12);
		qtf.Remove(l, r - l);
		qtf.Insert(l, "[^" + link + "^ " + link + "] [^F" + link + "^ Suggest fix]");
	}
	ai_prev = t.response_id;
	return qtf;
}
	
void Ide::AIReview()
{
	String code;
	String review_file;
	for(int i = 0; i < actual.file.GetCount(); i++) {
		String p = SourcePath(GetActivePackage(), actual.file[i]);
		if(findarg(GetFileExt(p), ".cpp", ".h", ".i", ".c", ".lay", ".key") >= 0) {
			String file = GetActivePackage() + '/' + GetFileName(p);
			code << "\n=========== " << file << "\n";
			if(NormalizePath(p) == NormalizePath(editfile)) {
				review_file = file;
				FileIn in(editfile);
				int ii = 0;
				while(!in.IsEof())
					code << ++ii << ": " << in.GetLine();
			}
			else
				code << LoadFile(p);
		}
		if(code.GetCount() > 1000000) {
			Exclamation("Request is too large");
			return;
		}
	}
	
	if(review_file.GetCount() == 0)
		Exclamation("Cannot review current file");
	
	LDUMP(code.GetCount());
	LDUMP(code);
	
	String prompt = "Review code in file " + review_file;
	int l, h;
	if(editor.GetSelection(l, h))
		prompt << ", only lines " << editor.GetLine(l) << " - " << editor.GetLine(h);
	prompt << "Use references to code in the format <@@filename:line@@> - just the first line, not a range.";
	AIThread result = AI(prompt, code);
	
	String qtf = AIResult(result);

	ai_qtf.Clear();
	AIAppendOutput(qtf);

	if(!ai_pane.GetParent())
		AIPane();

#if 0
	String review_file;

	for(int i = 1; i < 100000000; i++) {
		review_file = "ai_review_" + AsString(i) + ".qtf";
		if(!FileExists(review_file))
			break;
	}
	Upp::SaveFile(review_file, );
	
	EditFile(review_file);
#endif
}

void Ide::AIPane()
{
	if(ai_pane.GetParent())
		IdeRemoveRight(ai_pane);
	else
		IdeSetRight(ai_pane);
}

void Ide::AIPrompt()
{
	ai_qtf << "&[g@B \1" << ~ai_prompt << "\1]&";
	AIPrompt2(~ai_prompt);
	ai_prompt <<= Null;
}

void Ide::AIPrompt2(const String& prompt)
{
	AIThread result = AI(Null, prompt, ai_prev);
	AIAppendOutput(AIResult(result));
	if(!ai_pane.GetParent())
		AIPane();
}

void Ide::AIAppendOutput(const String& qtf)
{
	ai_qtf << qtf;
	ai_output.SetQTF(ai_qtf);
	ai_output.ScrollEnd();
}

void Ide::AILink(const String& link)
{
	GotoPosition(link);
	if(*link == 'F') {
		ai_qtf << "&[g@B Fix for \1" << link.Mid(1) << "\1]&";
		AIThread r = AI(Null,
		                "Suggest the fix for issue " + link.Mid(1) + ". Fix the whole function. "
		                "Show the range of lines you are replacing with <##line:count##> header. "
		                "Do not add any text or explanation, just the required header and the code."
		                "Do not use any formatting, I will be using the code as is.",
		                ai_prev);
		ai_prev = r.response_id;
		int p = r.result.Find('\n');
		if(p > 0) {
			String h = r.result.Mid(0, p);
			String code = r.result.Mid(p + 1);
			CParser p(h);
			int line, count;
			try {
				p.Char3('<', '#', '#');
				line = p.ReadInt();
				p.Char(':');
				count = p.ReadInt();
				WriteClipboardText(code);
				GotoPos(Point(0, line));
				line += count;
				editor.SetSelection(editor.GetCursor(), editor.GetPos(line + count));
				CompareWithClipboard();
				AIAppendOutput(HighlightCodeQtf(code.ToWString(), 4));
				return;
			}
			catch(CParser::Error) {}
		}
		AIAppendOutput("[g@R Unexpected reply:&][g \1" + r.result + "\1]");
			return;
	}
}
