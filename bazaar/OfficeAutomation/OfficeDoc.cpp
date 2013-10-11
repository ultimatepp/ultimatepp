#include <Core/Core.h>

using namespace Upp;

#include <Functions4U/Functions4U.h>

#ifndef PLATFORM_WIN32
#error Sorry: This platform is not supported!. Look for OfficeAutomation in Bazaar Upp Forum to search for info and new news
#endif

#include "OfficeAutomation.h"
#include "OfficeAutomationBase.h"


OfficeDoc::OfficeDoc() {
	Ole::Init();
}

OfficeDoc::~OfficeDoc()	 {
	Ole::Close();
}

bool OfficeDoc::Init(const char *name) {
	return PluginInit(*this, name);
}

bool DocPlugin::IsAvailable() {return false;}
bool OfficeDoc::IsAvailable(const char *_name) {
	for (int i = 0; i < Plugins().GetCount(); ++i) {
		if (Plugins()[i].name == _name && Plugins()[i].type == typeid(OfficeDoc).name()) {
 			void *dat = Plugins()[i].New();
			if (!dat)
				return false;
			bool ret = (static_cast<DocPlugin *>(dat))->IsAvailable();
			Plugins()[i].Delete(dat);
			return ret;
		}
	}
	return false;
}

bool DocPlugin::AddDoc(bool visible) {return false;}
bool OfficeDoc::AddDoc(bool visible) {return (static_cast<DocPlugin *>(GetData()))->AddDoc(visible);}

bool DocPlugin::OpenDoc(String fileName, bool visible) {return false;}
bool OfficeDoc::OpenDoc(String fileName, bool visible) {return (static_cast<DocPlugin *>(GetData()))->OpenDoc(fileName, visible);}

bool DocPlugin::SetFont(String font, int size) {return false;}
bool OfficeDoc::SetFont(String font, int size) {return (static_cast<DocPlugin *>(GetData()))->SetFont(font, size);}

bool DocPlugin::SetBold(bool bold) {return false;}
bool OfficeDoc::SetBold(bool bold) {return (static_cast<DocPlugin *>(GetData()))->SetBold(bold);}

bool DocPlugin::SetItalic(bool italic) {return false;}
bool OfficeDoc::SetItalic(bool italic) {return (static_cast<DocPlugin *>(GetData()))->SetItalic(italic);}

bool DocPlugin::WriteText(String value) {return false;}
bool OfficeDoc::WriteText(String value) {return (static_cast<DocPlugin *>(GetData()))->WriteText(value);}

bool DocPlugin::Select() {return false;}
bool OfficeDoc::Select() {return (static_cast<DocPlugin *>(GetData()))->Select();}

bool DocPlugin::EnableCommandVars(bool) {return false;}
bool OfficeDoc::EnableCommandVars(bool enable) {return (static_cast<DocPlugin *>(GetData()))->EnableCommandVars(enable);}

bool DocPlugin::Replace(String search, String replace) {return false;}
bool OfficeDoc::Replace(String search, String replace) {return (static_cast<DocPlugin *>(GetData()))->Replace(search, replace);}

bool DocPlugin::Print() {return false;}
bool OfficeDoc::Print() {return (static_cast<DocPlugin *>(GetData()))->Print();}

bool DocPlugin::SaveAs(String fileName, String _type) {return false;}
bool OfficeDoc::SaveAs(String fileName, String _type) {return (static_cast<DocPlugin *>(GetData()))->SaveAs(fileName, _type);}

bool DocPlugin::Quit() {return false;}
bool OfficeDoc::Quit() {return (static_cast<DocPlugin *>(GetData()))->Quit();}

bool DocPlugin::SetSaved(bool saved) {return false;}
bool OfficeDoc::SetSaved(bool saved) {return (static_cast<DocPlugin *>(GetData()))->SetSaved(saved);}
