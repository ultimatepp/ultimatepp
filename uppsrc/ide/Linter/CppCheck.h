#ifndef _ide_Linter_CppCheck_h_
#define _ide_Linter_CppCheck_h_

#include "Linter.h"

#define LAYOUTFILE <ide/Linter/CppCheckConfig.lay>
#include <CtrlCore/lay.h>

class CppCheck final : public Linter {
public:
	String GetConfigFilePath() final;
	Value  LoadConfig() final;
	bool   Exists() final;
	void   Settings() final;

protected:
	String MakeCmdLine(Vector<String>& paths) final;
	void   OnResults(const String& results) final;
private:
	void DecodeXML(const XmlNode& results);
};

struct CppCheckConfigDlg final : Linter::Config, WithCppCheckConfigLayout<TopWindow> {
	CppCheckConfigDlg();

	void Load() final;
	void Save() final;
	void Reset() final;
	
	struct Pane : WithCppCheckConfigPaneLayout<ParentCtrl> {
		Pane();

		void	SetData(const Value& data) final;
		Value	GetData() const final;
		void	Load(const String& path, const String& ext);
	} libs, addons;
};
#endif
