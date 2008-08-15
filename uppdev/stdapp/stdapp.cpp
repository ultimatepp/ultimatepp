#include <stdapp/stdapp.hpp>

#define IMAGECLASS stdappImg
#define IMAGEFILE <stdapp/stdapp.iml>
#include <Draw/iml_source.h>

#define TOPICFILE <stdapp/stdappHelp.tpp/all.i>
#include <Core/topic_group.h>

GUI_APP_MAIN
{
    stdapp app;

    /* text configuration file selected
    String cfg_file = ConfigFile();
    app.GetBinCfg(cfg_file);
    text configuration file selected */

    // text configuration file selected
    app.GetTxtCfg();
    // text configuration file selected

    if (app.cfg.show_splash)
    {
        Splash splash(app.cfg.language, SPLASH_PERIOD);
        Ctrl::EventLoop();
    }
    if (app.cfg.restore_cfg)
    {
        if (app.cfg.maximized)
            app.Maximize();
        else if (app.cfg.minimized)
            app.Minimize();
        else
            app.NoCenter();
    }
    app.SetRect(app.cfg.main_window_left, app.cfg.main_window_top, app.cfg.main_window_width, app.cfg.main_window_height);
    app.Init();
    app.Run();

    /* text configuration file selected
    app.SaveBinCfg(cfg_file);
    text configuration file selected */

    // text configuration file selected
    app.SaveTxtCfg();
    // text configuration file selected
}

stdapp::stdapp()
{
    CtrlLayout(*this);
    Icon(stdappImg::app_icon_16(), stdappImg::app_icon_16());
    Zoomable().Sizeable();
    menu.WhenHelp = status;
    AddFrame(menu);
    CtrlLayout(view);
    view.SizePos();
    Add(view);
    AddFrame(tools);
    AddFrame(status);
    help.SetRect(help_window_left, help_window_top, help_window_width, help_window_height);
}

void stdapp::Init()
{
    SetLanguage(cfg.language);
    Title(APP_TITLE);
    menu.Set(THISBACK(MainBar));
    tools.Set(THISBACK(Toolbar));
    status.Set(t_("Ready"));
    CtrlLayout(language);
    CtrlLayout(configuration);
    CtrlLayout(splash_screen);
    CtrlLayoutOKCancel(options, t_("Options"));
    CtrlLayoutOK(about, t_("About ..."));
}

/* text configuration file selected
void stdapp::GetBinCfg(String cfg_file)
{
    LoadFromFile(cfg, cfg_file);
}
text configuration file selected */

// text configuration file selected
void stdapp::GetTxtCfg()
{
    VectorMap<String, String> cfg_data_in = LoadIniFile(IniFile);
    if (cfg_data_in.Get("RESTORE_CFG", Null) == "true")
    {
        int tmp;

        cfg.restore_cfg = true;
        cfg.show_splash = cfg_data_in.Get("SHOW_SPLASH", Null) == "true" ? true : false;
        cfg.maximized = cfg_data_in.Get("MAXIMIZED", Null) == "true" ? true : false;
        cfg.minimized = cfg_data_in.Get("MINIMIZED", Null) == "true" ? true : false;
        tmp = ScanInt(cfg_data_in.Get("TOP_WINDOW_LEFT", Null));
        cfg.main_window_left = IsNull(tmp) ? default_main_window_left : tmp;
        tmp = ScanInt(cfg_data_in.Get("TOP_WINDOW_TOP", Null));
        cfg.main_window_top = IsNull(tmp) ? default_main_window_top : tmp;
        tmp = ScanInt(cfg_data_in.Get("TOP_WINDOW_WIDTH", Null));
        cfg.main_window_width = IsNull(tmp) ? default_main_window_width : tmp;
        tmp = ScanInt(cfg_data_in.Get("TOP_WINDOW_HEIGHT", Null));
        cfg.main_window_height = IsNull(tmp) ? default_main_window_height : tmp;
        tmp = ScanInt(cfg_data_in.Get("LANGUAGE", Null));
        cfg.language = IsNull(tmp) ? default_language : tmp;
    }
}
// text configuration file selected

/* text configuration file selected
void stdapp::SaveBinCfg(String cfg_file)
{
    GetCurrentCfg();

    if(!StoreToFile(cfg, cfg_file))
        Exclamation(t_("Error saving configuration!"));
}
text configuration file selected */

// text configuration file selected
void stdapp::SaveTxtCfg()
{
    GetCurrentCfg();

    String cfg_data_out;
    cfg_data_out
        << APP_TITLE << ": Configuration Text File" << "\n\n"
        << "RESTORE_CFG = " << (cfg.restore_cfg ? "true" : "false") << "\n"
        << "SHOW_SPLASH = " << (cfg.show_splash ? "true" : "false") << "\n"
        << "MAXIMIZED = " << (cfg.maximized ? "true" : "false") << "\n"
        << "MINIMIZED = " << (cfg.minimized ? "true" : "false") << "\n"
        << "TOP_WINDOW_LEFT = " << AsString(cfg.main_window_left) << "\n"
        << "TOP_WINDOW_TOP = " << AsString(cfg.main_window_top) << "\n"
        << "TOP_WINDOW_WIDTH = " << AsString(cfg.main_window_width) << "\n"
        << "TOP_WINDOW_HEIGHT = " << AsString(cfg.main_window_height) << "\n"
        << "LANGUAGE = " << AsString(cfg.language) << "\n"
        ;
    if(!SaveFile(IniFile, cfg_data_out))
        Exclamation(t_("Error saving configuration!"));
}
// text configuration file selected

void stdapp::GetCurrentCfg()
{
    cfg.maximized = IsMaximized();
    cfg.minimized = IsMinimized();
    Rect main_window = GetRect();
    cfg.main_window_left = main_window.left;
    cfg.main_window_top = main_window.top;
    cfg.main_window_width = main_window.right - main_window.left;
    cfg.main_window_height = main_window.bottom - main_window.top;
}


void stdapp::MainBar(Bar& bar)
{
    bar.Add(t_("&File"), THISBACK(File));
    bar.Add(t_("&Edit"), THISBACK(Edit));
    bar.Add(t_("&Settings"), THISBACK(Settings));
    bar.Add(t_("&Help"), THISBACK(Help));
}

void stdapp::Toolbar(Bar& bar)
{
    bar.Add(stdappImg::doc_new(), THISBACK(OnNew)).Tip(t_("New"));
    bar.Add(stdappImg::doc_load(), THISBACK(OnLoad)).Tip(t_("Open ..."));
    bar.Add(stdappImg::doc_save(), THISBACK(OnSave)).Tip(t_("Save"));
    bar.Add(stdappImg::doc_save_as(), THISBACK(OnSaveAs)).Tip(t_("Save as ..."));
    bar.Add(stdappImg::doc_print(), THISBACK(OnPrint)).Tip(t_("Print ..."));
    bar.Separator();
    bar.Add(stdappImg::doc_cut(), THISBACK(OnCut)).Tip(t_("Cut"));
    bar.Add(stdappImg::doc_copy(), THISBACK(OnCopy)).Tip(t_("Copy"));
    bar.Add(stdappImg::doc_paste(), THISBACK(OnPaste)).Tip(t_("Paste"));
    bar.Add(stdappImg::doc_delete(), THISBACK(OnDelete)).Tip(t_("Delete"));
    bar.Separator();
    bar.Add(stdappImg::doc_undo(), THISBACK(OnUndo)).Tip(t_("Undo"));
    bar.Add(stdappImg::doc_redo(), THISBACK(OnRedo)).Tip(t_("Redo"));
}

void stdapp::Paint(Draw& w)
{
    w.DrawRect(GetSize(), SWhite);
}

bool stdapp::Key(dword key, int)
{
    if (key == K_F2)
    {
        OnF2Key();
        return true;
    }
    else if (key == K_SHIFT_F3)
    {
        OnShiftF3Key();
        return true;
    }
    else if (key == K_SHIFT_CTRL_1)
    {
        OnShiftCtrl1Key();
        return true;
    }
    else if (key == K_ALT_A)
    {
        OnAltAKey();
        return true;
    }
    else if (key == K_CTRL_B)
    {
        OnCtrlBKey();
        return true;
    }
    return false;
}

void stdapp::OnF2Key()
{
    Exclamation("You still have to write your own \"F2-key\" code!");
}

void stdapp::OnShiftF3Key()
{
    Exclamation("You still have to write your own \"Shift+F3-key\" code!");
}

void stdapp::OnShiftCtrl1Key()
{
    Exclamation("You still have to write your own \"Shift+Ctrl+1-key\" code!");
}

void stdapp::OnAltAKey()
{
    Exclamation("You still have to write your own \"Alt+A-key\" code!");
}

void stdapp::OnCtrlBKey()
{
    Exclamation("You still have to write your own \"Ctrl+B-key\" code!");
}
