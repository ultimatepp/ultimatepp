#include <CtrlLib/CtrlLib.h>

#define TFILE <Gui24/Gui24.t>
#include <Core/t.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
    MyAppWindow() {
        Title(t_("My application"));
        Zoomable().Sizeable().SetRect(0, 0, 550, 100);
    }
    
    void Paint(Draw& w) override {
        w.DrawRect(GetSize(), SLtYellow);
        w.DrawText(20, 20, t_("Hello translation engine!"), Arial(30), SBlue);
    }
};

GUI_APP_MAIN
{
	// Set system language for whole application
	SetLanguage(GetSystemLNG());
    
    // Uncomment to force specific language...
    // SetLanguage("en-us"); // English = default
    // SetLanguage("cs-cz"); // Czech
    // SetLanguage("de-de"); // German
    // SetLanguage("es-es"); // Spanish
    // SetLanguage("fr-fr"); // French
    // SetLanguage("it-it"); // Italian
    // SetLanguage("ja-jp"); // Japanese
    // SetLanguage("pl-pl"); // Polish
    // SetLanguage("pt-pt"); // Portuguese
    // SetLanguage("ru-ru"); // Russian
    // SetLanguage("tr-tr"); // Turkish
    // SetLanguage("zh-cn"); // Simplified Chinese
    // SetLanguage("zh-tw"); // Traditional Chinese
    
    MyAppWindow().Run();
}
