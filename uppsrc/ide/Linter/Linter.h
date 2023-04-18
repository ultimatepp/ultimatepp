#ifndef TheIde_Linter_h
#define TheIde_Linter_h

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>

#include <ide/Common/Common.h>
#include <ide/Core/Core.h>
#include <ide/ide.h>

#define LAYOUTFILE <ide/Linter/Linter.lay>
#include <CtrlCore/lay.h>

class Linter final {
public:
    Linter() {}
    ~Linter() {}

    bool Exists();
    bool CanCheck();
    void CheckFile();
    void CheckPackage();
    void CheckAll();

    static Value LoadConfig();
    static String GetConfigFilePath();

    void Settings();
    
    void StdMenu(Bar& menu);
    void FileMenu(Bar& menu);
    void PackageMenu(Bar& menu);

private:
    String GetFileName();
    String GetFilePath();
    String GetPackageName();
    String GetPackagePath();
    String GetCmdLine();

    void SysCmd(const String& cmd, const String& text, Stream& fs);
    void DoCheck(Vector<String>& paths);
    void ParseResults(const XmlNode& results);
};

Linter& GetLinter();
bool HasLinter();

struct LinterConfigDlg : WithLinterConfigLayout<TopWindow> {
    LinterConfigDlg();

    void Load();
    void Save();
    void Reset();
    
    struct Pane : WithPaneLayout<ParentCtrl> {
        Pane();

        void    SetData(const Value& data) override;
        Value   GetData() const override;
        void    Load(const String& path, const String& ext);
    };
    
    Pane libs, addons;
};

#define KEYGROUPNAME "Linter"
#define KEYNAMESPACE LinterKeys
#define KEYFILE <ide/Linter/Linter.key>
#include <CtrlLib/key_header.h>

#define IMAGECLASS LinterImg
#define IMAGEFILE <ide/Linter/Linter.iml>
#include <Draw/iml_header.h>

#endif
