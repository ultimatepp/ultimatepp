#include <GoogleTranslator/GoogleTranslator.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String str_status = "";
	GoogleTranslator g_instance;
	Cout() << "Set proxy: 172.16.0.65 , port 3128" << "\n";
	Cout() << "Translate \"translator\" word" << "\n";
	g_instance.SetProxy("172.16.0.65", 3128);
	if(g_instance.Translate("translator", String("en"), String("ro"))){
		Cout() << "Translation: " << g_instance.GetFormatedText() << "\n";
		if(g_instance.GetCorrectionText()!="translator"){
			Cout() << "Send corrected translator if not correct: \"translator\"" << "\n";
			if(g_instance.SetCorrectionText("translator", str_status))
				Cout() << str_status;
		}
	}
}

