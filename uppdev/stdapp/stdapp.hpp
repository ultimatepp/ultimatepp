#ifndef STDAPP_HPP
#define STDAPP_HPP

#include <stdapp/stdappDef.hpp>
#include <stdapp/stdappCfg.hpp>
#include <stdapp/stdappSplash.hpp>
#include <stdapp/EnhParentCtrl.hpp>

#define TFILE <stdapp/stdapp.t>
#include <Core/t.h>

#define LAYOUTFILE <stdapp/stdapp.lay>
#include <CtrlCore/lay.h>
#define LAYOUTFILE <stdapp/stdappSettings.lay>
#include <CtrlCore/lay.h>
#define LAYOUTFILE <stdapp/stdappHelp.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS stdappImg
#define IMAGEFILE <stdapp/stdapp.iml>
#include <Draw/iml_header.h>

class stdapp : public WithstdappLayoutMain<TopWindow>
{
private:
    WithstdappLayoutView<EnhancedParentCtrl> view;
    WithstdappLayoutTab<TopWindow> options;
    WithstdappLayoutLang<TopWindow> language;
    WithstdappLayoutCfg<TopWindow> configuration;
    WithstdappLayoutSplash<TopWindow> splash_screen;
    HelpWindow help;
    WithstdappLayoutAbout<TopWindow> about;

    void GetCurrentCfg();
    void MainBar(Bar& bar);
    void Toolbar(Bar& bar);
    #include <stdapp/stdappFile.hpp>
    #include <stdapp/stdappEdit.hpp>
    #include <stdapp/stdappSettings.hpp>
    #include <stdapp/stdappHelp.hpp>
    virtual void Paint(Draw& w);
    bool Key(dword key, int);
	void OnF2Key();
	void OnShiftF3Key();
	void OnShiftCtrl1Key();
	void OnAltAKey();
	void OnCtrlBKey();

    typedef stdapp CLASSNAME;

public:
    Cfg cfg;

    stdapp();
    /* text configuration file selected
    void GetBinCfg(String cfg_file);
    void SaveBinCfg(String cfg_file);
    text configuration file selected */

    // text configuration file selected
    void GetTxtCfg();
    void SaveTxtCfg();
    // text configuration file selected

    void Init();
};

#endif
