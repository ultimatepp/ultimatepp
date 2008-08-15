#include <stdapp/stdapp.hpp>

void stdapp::Help(Bar& bar)
{
    bar.Add(t_("Help"), THISBACK(OnHelp)).Key(K_F1).Help(t_("Get help with the application"));
    bar.Add(t_("About ..."), THISBACK(OnAbout)).Key(K_CTRL_I).Help(t_("Get information about the application"));
}

void stdapp::OnAbout()
{
    Image logo = UPP_LogoImg::AppLogo();
    about.logo_ctrl.SetImage(logo);
    about.text_1 = APP_TITLE;
    about.text_2 = APP_VERSION;
    about.text_3 = APP_SUBTITLE;
    about.text_4 = APP_COPYRIGHT;
    about.ok <<= THISBACK(OnAboutOK);
    about.Execute();
}

void stdapp::OnAboutOK()
{
    about.Close();
}

void stdapp::OnHelp()
{
    switch(cfg.language)
    {
        case LNG_('D', 'E', 'D', 'E'):
            help.GoTo("topic://stdapp/stdappHelp/Inhaltsverzeichnis$de-de");
            break;
        default:
            help.GoTo("topic://stdapp/stdappHelp/Index$en-us");
    }
    help.Execute();
}
