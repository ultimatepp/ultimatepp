#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <GetLayoutId/MyApp.lay>
#include <CtrlCore/lay.h>

class MyApp : public WithMyAppLayout<TopWindow> {
public:
	typedef MyApp CLASSNAME;
	
	void Load();
	void Save();
	
	MyApp();
};

void MyApp::Load()
{
	Value v = ParseJSON(LoadFile(ConfigFile("data.json")));
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		if(dynamic_cast<EditField *>(q))
			*q <<= v[q->GetLayoutId()]; // If missing in JSON, sets Null, which is OK
}

void MyApp::Save()
{
	Json json;
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		if(dynamic_cast<EditField *>(q)) {
			String id = q->GetLayoutId();
			if(id.GetCount())
				json(id, ~*q);
		}
	SaveFile(ConfigFile("data.json"), json);
}

MyApp::MyApp()
{
	CtrlLayoutOKCancel(*this, "Window title");
}

GUI_APP_MAIN
{
	MyApp app;
	app.Load();
	if(app.Execute() == IDOK)
		app.Save();
}
