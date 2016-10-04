#include <stdapp/stdappSplash.hpp>

#define IMAGECLASS UPP_LogoImg
#define IMAGEFILE <stdapp/UPP_Logo_033.iml>
#include <Draw/iml_source.h>

Splash::Splash(const int language, const int ms)
{
    SetLanguage(language);
    SetRect(GetWorkArea().CenterRect(MakeSplash(*this, widgets) + 2));
    SetFrame(BlackFrame());
    PopUp(NULL, false, false, true);
    SetTimeCallback(ms, THISBACK(CloseSplash));
}

Size Splash::MakeSplash(Ctrl& parent, Array<Ctrl>& widgets)
{
    Image logo = UPP_LogoImg::AppLogo();

    Size logo_size = logo.GetSize();
    Size rect_size;
    rect_size.cx = max(SPLASH_RECT_CX, logo_size.cx);
    rect_size.cy = max(SPLASH_RECT_CY, logo_size.cy);

    parent.Add(widgets.Create<StaticRect>().Color(SPLASH_PAPER_COLOR).SizePos());

    ImageCtrl& image = widgets.Create<ImageCtrl>();
    image.SetImage(logo);
    image.LeftPos(0, logo_size.cx).VSizePos();
    parent.Add(image);

    Label& label_1 = widgets.Create<Label>();
    label_1.SetFont(SPLASH_FONT_1(SPLASH_FONT_SIZE_1).Bold());
    label_1.SetInk(SPLASH_INK_COLOR_1);
    label_1 = APP_TITLE;
    label_1.SetAlign(ALIGN_CENTER);
    label_1.RightPos(0, rect_size.cx - logo_size.cx).TopPos(rect_size.cy * 1 / 9, 20);
    parent.Add(label_1);

    Label& label_2 = widgets.Create<Label>();
    label_2.SetFont(SPLASH_FONT_2(SPLASH_FONT_SIZE_2).Bold());
    label_2.SetInk(SPLASH_INK_COLOR_2);
    label_2 = APP_VERSION;
    label_2.SetAlign(ALIGN_CENTER);
    label_2.RightPos(0, rect_size.cx - logo_size.cx).TopPos(rect_size.cy * 3 / 9, 20);
    parent.Add(label_2);

    Label& label_3 = widgets.Create<Label>();
    label_3.SetFont(SPLASH_FONT_3(SPLASH_FONT_SIZE_3).Italic());
    label_3.SetInk(SPLASH_INK_COLOR_3);
    label_3 = APP_SUBTITLE;
    label_3.SetAlign(ALIGN_CENTER);
    label_3.RightPos(0, rect_size.cx - logo_size.cx).TopPos(rect_size.cy * 5 / 9, 20);
    parent.Add(label_3);

    Label& label_4 = widgets.Create<Label>();
    label_4.SetFont(SPLASH_FONT_4(SPLASH_FONT_SIZE_4));
    label_4.SetInk(SPLASH_INK_COLOR_4);
    label_4 = APP_COPYRIGHT;
    label_4.SetAlign(ALIGN_CENTER);
    label_4.RightPos(0, rect_size.cx - logo_size.cx).TopPos(rect_size.cy * 7 / 9, 20);
    parent.Add(label_4);

    return rect_size;
}

void Splash::CloseSplash()
{
    Close();
}
