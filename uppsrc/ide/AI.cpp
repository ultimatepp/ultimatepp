#include "ide.h"

#define LDUMP(x)
#define LLOG(x) LOG(x)

Ide::AIThread Ide::AIChatGPT(const String& instructions, const String& code)
{
	Ide::AIThread r;
#ifdef _DEBUG
	HttpRequest::Trace();
#endif

	HttpRequest http("https://api.openai.com/v1/responses");

	http.Header("Authorization", "Bearer " + ai_key);

	String responseText =
		http.ContentType("application/json").Post(
			Upp::Json("model", ai_model)
			         ("prompt_cache_key", "experimental_query")
			         ("instructions", instructions)
			         ("input", code)
			         ("max_output_tokens", 100000)
			         ("background", true)
		).Execute();
		
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

Ide::AIThread Ide::AIGemini(const String& instructions, const String& code)
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

Ide::AIThread Ide::AI(const String& instructions, const String& code)
{
	return ai_kind == 0 ? AIChatGPT(instructions, code)
	                    : AIGemini(instructions, code);
}

void Ide::AIReview()
{
	String code;
	for(int i = 0; i < actual.file.GetCount(); i++) {
		String p = SourcePath(GetActivePackage(), actual.file[i]);
		if(findarg(GetFileExt(p), ".cpp", ".h", ".i", ".c") >= 0) {
			code << "\n=========== " << GetActivePackage() + '/' + GetFileName(p) << "\n";
			if(NormalizePath(p) == NormalizePath(editfile)) {
				code << "// *** AI review starts here\n";
				code << LoadFile(p);
				code << "// *** AI review ends here\n";
			}
		}
		if(code.GetCount() > 1000000)
			break;
	}
	
	LDUMP(code.GetCount());
	LDUMP(code);
	
	AIThread result = AI("Review the code, but only the code delimited with comments *** AI review. Use references to code in format <@@filename:line@@> - just the first line, not a range.", code);
	
	
	String qtf = MarkdownConverter().ToQtf(result.result);
	
	for(;;) {
		int l = qtf.Find("`<`@`@");
		if(l < 0)
			break;
		int r = qtf.FindAfter("`@`@`>");
		if(r < 0)
			break;
		String link = qtf.Mid(l + 6, r - l - 12);
		qtf.Remove(l, r - l);
		qtf.Insert(l, "[^" + link + "^ " + link + "]");
	}

	ai_output.SetQTF(qtf);
	ai_output.WhenLink = [this](const String& link) {
		GotoPosition(link);
	};
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
