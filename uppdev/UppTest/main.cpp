#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Sample1/main.lay>

#include <CtrlCore/lay.h>

struct MyApp : public WithMain<TopWindow> {

    MyApp() {
        CtrlLayout(*this, "My dialog");
        Labeltje.SetData("zomaar ? een test");
    }
};

GUI_APP_MAIN
{

    MyApp().Run();
}
