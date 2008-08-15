#include <stdapp/stdapp.hpp>

void stdapp::Settings(Bar& bar)
{
    bar.Add(t_("International"), THISBACK(OnInternational)).Help(t_("Select the application language"));
    bar.Add(t_("Options"), THISBACK(OnOptions)).Key(K_CTRL_O).Help(t_("Customize application"));
}

void stdapp::OnInternational(Bar& bar)
{
    bar.Add("Deutsch", THISBACK1(OnLanguage, LNG_('D', 'E', 'D', 'E')))
        .Check(cfg.language == LNG_('D', 'E', 'D', 'E') ? true : false)
        .Help("Deutsch als Sprache auswählen");
    bar.Add("English", THISBACK1(OnLanguage, LNG_('E', 'N', 'U', 'S')))
        .Check(cfg.language == LNG_('E', 'N', 'U', 'S') ? true : false)
        .Help("Select English as language");
}

void stdapp::OnOptions()
{
    static bool tabs_added = false;

    if (tabs_added)
    {
        options.tab.Reset();
    }
    else
        tabs_added = true;
    options.tab.Add(language, t_("Language"));
    options.tab.Add(configuration, t_("Configuration"));
    options.tab.Add(splash_screen, t_("Splash screen"));
    options.Title(t_("Options"));
    language.current_language <<= cfg.language;
    configuration.restore_cfg <<= cfg.restore_cfg;
    splash_screen.show_splash <<= cfg.show_splash;
    options.ok <<= THISBACK(OnOptionsOK);
    options.cancel <<= THISBACK(OnOptionsCancel);
    options.Execute();
}

void stdapp::OnOptionsOK()
{
    cfg.language = ~language.current_language;
    cfg.restore_cfg = ~configuration.restore_cfg;
    cfg.show_splash = ~splash_screen.show_splash;
    options.Close();
    Init();
}

void stdapp::OnOptionsCancel()
{
    options.Close();
}

void stdapp::OnLanguage(const int language_id)
{
    cfg.language = language_id;
    Init();
}
