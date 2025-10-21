#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
    Image image;

    void Paint(Draw& w) override {
        w.DrawRect(GetSize(), Cyan());
        w.DrawImage(10, 10, image);
    }

    MyApp() {
        ImageDraw iw(400, 200);
        iw.Alpha().DrawRect(0, 0, 400, 200, GrayColor(0));
        iw.Alpha().DrawEllipse(0, 0, 400, 200, GrayColor(255));
        iw.DrawEllipse(0, 0, 400, 200, Yellow());
        iw.DrawText(100, 50, "Image", Arial(70).Bold(), Black());
        image = iw;
    }
};

GUI_APP_MAIN
{
    MyApp app;
    app.Title("ImageDraw").Zoomable().Sizeable();
    app.SetRect(0, 0, 420, 240);
    app.Run();
}