#include "GoogleTranslator.h"

// Public methods

GoogleTranslator::GoogleTranslator(){
	isTranslated = false;
	translatedText = "";
	formatedText = "";
	correctionText = "";
	useProxy = false;
	proxyHTTPAddress = "";
	proxyHTTPPort = 0;
	languageFrom = "en";
	languageTo = "fr";
	correctedText = "";
}

bool GoogleTranslator::sendHttpPost(String& url, String& post, String& result_text, Gate2<int, int> _progress){
	// Proxy
	refreshProxy();
	
	httpClient.URL(url);
	//httpClient.Agent("Mozilla/5.0");
	httpClient.TimeoutMsecs(5000);
	httpClient.Post();
	httpClient.PostData(post);
	
	bool status = true;
	
	result_text = httpClient.ExecuteRedirect(HttpClient::DEFAULT_MAX_REDIRECT, 
						HttpClient::DEFAULT_RETRIES, _progress);
	if (!IsNull(httpClient.GetError())||httpClient.IsAborted()){
		status = false;
		result_text = String("Error:").Cat()<<Nvl(httpClient.GetError(), String(""))
			<<String("\n, status: ")<<httpClient.GetStatusCode()<<String(", ")<<httpClient.GetStatusLine()
			<<String("\n, header: ")<<httpClient.GetHeaders();
	}
	
	return status;
}

bool GoogleTranslator::Translate(Gate2<int, int> _progress){
	String url = "www.google.com/translate_a/t?client=t";
	//or
	//String url = "http://translate.google.com/translate_t"
	
	url << String("&sl=") << languageFrom << String("&tl=") << languageTo;
	// or
	//url << String("&langpair=") << languageFrom << String("|") << languageTo;
	
	url << String("&ie=utf-8&oe=utf-8");
	
	String post = String("text=") + UrlEncode(sourceText);
	
	isTranslated = sendHttpPost(url, post, translatedText, _progress);
	
	// Empty other properties
	formatedText = "";
	correctionText = "";
	correctedText = "";
	
	return isTranslated;
}

//... need to finish this procedure and test it
// gtrans=<GoogleTranslatedText>&hl=<ReturnLanguage>&langpair=<SourceLanguage>|<DestinationLanguage>&oe=UTF-8&text=<SourceText>&utrans=<UserTranslation>
// need to test "&hl=ru" if noun, adjective,... is returned in speciffic language
bool GoogleTranslator::SetCorrectionText(String correction_text, String& result_text, Gate2<int, int> _progress){
	if(!isTranslated)
		return false;
	
	String url = "http://translate.google.com/translate_suggestion?";
	//or //? not sure
	//String url = "http://www.google.com/translate_a/t?client=t";
	
	url << String("&sl=") << languageFrom << String("&tl=") << languageTo;
	// or
	//url << String("&langpair=") << languageFrom << String("|") << languageTo;
	
	url << String("&ie=utf-8&oe=utf-8");
	
	url << String("&hl=") << languageTo;
	
	url << String("&text=") << UrlEncode(sourceText);
	url << String("&gtrans=") << UrlEncode(correctionText);
	
	String post = String("utrans=") + UrlEncode(correction_text);
	
	bool status = sendHttpPost(url, post, result_text, _progress);
	
	if(status)
		correctedText = correction_text;
	
	return status;
}

String GoogleTranslator::GetFormatedText(){
	if(!isTranslated)
		return formatedText; // if not translated it is null or have error message
	
	if(formatedText.GetCount()>0)
		return formatedText;
	
	// Common formatting
	translatedText = replace_string ( translatedText, String(" \\r\\n ").Cat(), String("\n").Cat());
	translatedText = replace_string ( translatedText, String("\\r\\n ").Cat(), String("\n").Cat());
	translatedText = replace_string ( translatedText, String("\\\"").Cat(), String("\"").Cat());
	translatedText = replace_string ( translatedText, String("\\\\").Cat(), String("\\").Cat());
	translatedText = replace_string ( translatedText, String("\\n").Cat(), String("\n").Cat());
	translatedText = replace_string ( translatedText, String("\\x3c").Cat(), String("<").Cat());
	translatedText = replace_string ( translatedText, String("\\x3e").Cat(), String(">").Cat());
			
	if(translatedText.StartsWith("\"")){// is string
		formatedText = translatedText;
		
		formatedText.Remove(0, 1);
		formatedText.Remove(formatedText.GetLength()-1, 1);
		
		// Set Correction text
		correctionText = formatedText;
	}
	else if (translatedText.StartsWith("[\"")){ // is terms
		String text_temp = translatedText, text_lf = String("\n");
		
		// For debugging
		//String original_text = translatedText;
		
		int text_temp_pos, text_temp_pos1;
		text_temp_pos = 2;
		
		// Get first word
		text_temp_pos1 = text_temp.Find("\"", text_temp_pos);
		
		if(text_temp_pos1==-1)
			text_temp_pos1 = text_temp.GetLength();
		
		formatedText = text_temp.Mid(text_temp_pos,text_temp_pos1 - text_temp_pos);
		
		// Set first term as text for correction
		correctionText = formatedText;
		
		text_temp_pos = text_temp_pos1+1;
		formatedText<<text_lf;
		
		if(text_temp.Mid(text_temp_pos, 2).StartsWith(",[")){
			text_temp_pos+=2;
			
			while(text_temp.Mid(text_temp_pos, 2).StartsWith("[\"")){
				text_temp_pos+=2;
				// get group name
				text_temp_pos1 = text_temp.Find("\"",text_temp_pos);
				
				if(text_temp_pos1==-1)
					text_temp_pos1= text_temp.GetLength();
				
				if(text_temp_pos,text_temp_pos1 - text_temp_pos >1){
					formatedText<<text_lf;
					formatedText<<t_(text_temp.Mid(text_temp_pos,text_temp_pos1 - text_temp_pos))<<String(":");
				}
				text_temp_pos = text_temp_pos1+1;
				
				// Get group list
				while(text_temp.Mid(text_temp_pos, 2).StartsWith(",\"")){
					text_temp_pos+=2;
					text_temp_pos1 = text_temp.Find("\"",text_temp_pos);
					
					if(text_temp_pos1==-1)
						text_temp_pos1= text_temp.GetLength();
					
					formatedText<<text_lf;
					formatedText<<String("\t")<<text_temp.Mid(text_temp_pos,text_temp_pos1 - text_temp_pos);
					text_temp_pos = text_temp_pos1+1;
				}
				text_temp_pos+=2;//],
			}
			
		}
		// For debug
		//formatedText<<text_lf<<text_temp.Mid(text_temp_pos);
		//formatedText<<text_lf<<original_text;
	};
	
	return formatedText; 
}

String replace_string(String& s1, String& find, String replace){
	String string_result;
	
	int start_pos = 0;
	int found_pos = 0;
	int find_len = find.GetCount();
	int s1_count = s1.GetCount();
	
	while(((found_pos=s1.Find(find, start_pos))!=-1)){
		string_result.Cat(s1.Mid(start_pos, found_pos - start_pos));
		string_result.Cat(replace);
		start_pos = found_pos + find_len;
	};
		
	if(start_pos<s1.GetCount())
		string_result.Cat(s1.Mid(start_pos));
		
	return (string_result);
}

// Private Methods
//----------------------------------------------------------------------------

void GoogleTranslator::refreshProxy(){
	if(useProxy)
		httpClient.Proxy(proxyHTTPAddress, proxyHTTPPort);
	else
		httpClient.Proxy(NULL, 0);
}