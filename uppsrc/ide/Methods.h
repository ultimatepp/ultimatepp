#ifndef _ide_Methods_h_
#define _ide_Methods_h_

#include "ide.h"

class DirTable : public ArrayCtrl {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;

protected:
	void Modify()  { Update(); }

	EditString      edit;
	SelectDirButton edit_dir;

	void Init(const char *name = NULL);

public:
	DirTable();
	DirTable(const char *name);
};

class DirMap : public ArrayCtrl {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;

protected:
	void Modify()  { Update(); }

	EditString      localpath, remotepath;
	SelectDirButton edit_dir;

public:
	DirMap();
};

class BuilderSetup;

class BuilderSetupInterface : public ParentCtrl {
public:
	virtual ~BuilderSetupInterface() {}

	virtual void New(const String& builder) = 0;
	virtual void OnLoad() = 0;
	virtual void OnCtrlLoad(const String& ctrlKey, const String& value) = 0;
	virtual void OnShow() = 0;
	virtual void InitSetupCtrlsMap(VectorMap<Id, Ctrl*>& map) = 0;
	
	virtual void InitBuilderSetup(BuilderSetup& bs);
};

class AndroidBuilderSetup : public WithBuildMethodsAndroidBuilderSetupLayout<BuilderSetupInterface> {
	typedef AndroidBuilderSetup CLASSNAME;
	
public:
	FrameRight<Button> ndkBrowse;
	FrameRight<Button> ndkDownload;
	FrameRight<Button> jdkBrowse;
	FrameRight<Button> jdkDownload;
	
public:
	AndroidBuilderSetup();
	virtual ~AndroidBuilderSetup();
	
	virtual void New(const String& builder);
	virtual void OnLoad();
	virtual void OnCtrlLoad(const String& ctrlKey, const String& value);
	virtual void OnShow();
	
	virtual void InitSetupCtrlsMap(VectorMap<Id, Ctrl*>& map);

private:
	void OnSdkShow();
	void OnSdkPathChange(const String& sdkPath);
	void OnNdkShow();
	void OnNdkPathInsert();
	void OnNdkPathChange();
	void OnNdkPathChange0(const String& ndkPath);

private:
	void LoadPlatforms(const AndroidSDK& sdk);
	void LoadBuildTools(const AndroidSDK& sdk);
	void LoadToolchains(const AndroidNDK& ndk);
	void LoadCppRuntimes(const AndroidNDK& ndk);
	void LoadDropList(DropList& dropList, const Vector<String>& values, const String& defaultKey = "");
	
private:
	void EnableNdkCtrls(bool enable = true);
	void DisableNdkCtrls();
	void ClearNdkCtrls();
};

class BuilderSetup {
public:
	BuilderSetupInterface* setupCtrl;
	VectorMap<Id, Ctrl*> setupCtrlsMap;
};

class DefaultBuilderSetup : public WithBuildMethodsDefaultBuilderSetupLayout<BuilderSetupInterface> {
public:
	DirTable   path;
	DirTable   include;
	DirTable   lib;
	
public:
	DefaultBuilderSetup();
	virtual ~DefaultBuilderSetup();
	
	virtual void New(const String& builder);
	virtual void OnLoad() {}
	virtual void OnCtrlLoad(const String& ctrlKey, const String& value) {}
	virtual void OnShow() {}
	
	virtual void InitSetupCtrlsMap(VectorMap<Id, Ctrl*>& map);
};

class BuildMethods : public WithBuildMethodsLayout<TopWindow> {
public:
	OpenFileButton open_script;
	TextOption linkmode_lock;
	
	EditStringNotNull name;
	Index<String>     origfile;
	String            default_method;
	
	ArrayMap<String, BuilderSetup> setups;
	
	AndroidBuilderSetup androidSetup;
	DefaultBuilderSetup defaultSetup;
	
	void Load();
	bool Save();

	void NewBuilder();
	void ShowDefault();
	void SetDefault();
	void ChangeMethod();
	void Import();

	void MethodMenu(Bar& bar);
	
	String GetSetupPrefix(const String& setupKey) const;
	String GetSetupPrefix(const Index<String>& buildersGroup) const;
	void   InitSetups();
	void   SwitchSetupView();
	VectorMap<String, String> SieveBuilderVars(const VectorMap<String, String>& map);
	VectorMap<String, String> MapBuilderVars(const VectorMap<String, String>& map);
	
	typedef BuildMethods CLASSNAME;

	BuildMethods();
	
private:
	void          SieveBuilders(Index<String>& sievedBuilders, const Index<String>& builders) const;
	String        BuildersToString(const Index<String>& builders) const;
	Index<String> StringToBuilders(const String& str) const;
	Index<String> GetBuilders() const;
};

#endif
