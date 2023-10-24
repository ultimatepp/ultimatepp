#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	HttpRequest http(
		"https://restcountries.com/v3.1/name/germany?fullText=true&fields=name,capital");
	auto content = http.Method(HttpRequest::METHOD_GET).Execute();
	if(content.IsVoid()) {
		Cout() << "Failed to execute GET request wit error code " << http.GetStatusCode()
			   << ".\n";
		return;
	}

	auto json = ParseJSON(content);
	if(json.IsError()) {
		Cout() << "Failed to parse JSON response.";
		return;
	}

	if(json.GetCount() == 0) {
		Cout() << "The JSON is empty. HTTP request returns empty countries list.\n";
		return;
	}
	
	// Let's parse the search results and display them on the terminal.
	Cout() << "Found countries:\n";
	for(const auto& result : json) {
		String common_name;
		String capital;

		common_name = result["name"]["common"];
		for(const auto& result_capital : result["capital"]) {
			capital = result_capital;

			// Some countries like South Africa might have more than one capital city. For this
			// tutorial, let's ignore it and display only the first result on the list.
			break;
		}

		Cout() << "- " << common_name << " with " << capital << " as a capital city.\n";
	}
}
