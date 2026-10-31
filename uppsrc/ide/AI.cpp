#include "ide.h"

void Ide::AIReview()
{
#ifdef _DEBUG
	HttpRequest::Trace();
#endif

	JsonArray a;

	HttpRequest http(ai_url + ai_key);

	String code;
	for(int i = 0; i < actual.file.GetCount(); i++) {
		String p = SourcePath(GetActivePackage(), actual.file[i]);
		if(findarg(GetFileExt(p), ".cpp", ".h", ".i", ".c") >= 0) {
			code << "\n=========== " << GetFileName(p) << "\n";
			if(NormalizePath(p) == NormalizePath(editfile)) {
				code << "// *** AI review starts here\n";
				code << LoadFile(p);
				code << "// AI review ends here\n";
			}
		}
		if(code.GetCount() > 200000)
			break;
	}
	
	DDUMP(code.GetCount());
	DDUMP(code);

	String responseText =
	http.ContentType("application/json").Post(
		Upp::Json("contents", JsonArray()
		                      << Upp::Json("role", "user")
		                                  ("parts", JsonArray() << Upp::Json("text", code)
		                                                        << Upp::Json("text", "review, but only the code delimited with comments *** AI review.")))
	).Execute();

	if (http.IsError()) {
		Cout() << "HTTP Request failed: " << http.GetErrorDesc() << "\n";
		return;
	}

	Value responseJson = ParseJSON(responseText);
	
	DDUMP(http.GetContent());

	if (!IsNull(responseJson["error"])) {
		Cout() << "Gemini API Error: " << responseJson["error"]["message"] << "\n";
		return;
	}

	Value rs = responseJson["candidates"];
	if(rs.GetCount() == 0)
		return;
	
	PromptOK(MarkdownConverter().ToQtf(~rs[0]["content"]["parts"][0]["text"]));
}