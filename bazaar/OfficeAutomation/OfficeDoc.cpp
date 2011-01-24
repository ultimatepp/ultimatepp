#include <Core/Core.h>

#ifndef PLATFORM_WIN32
#error Sorry: This platform is not supported!. Look for OfficeAutomation in Bazaar Upp Forum to search for info and new news
#endif

#include "OfficeAutomationBase.h"
#include "OfficeAutomation.h"


OfficeDoc::OfficeDoc() {
	data = NULL;
	Ole::Init();
}

OfficeDoc::~OfficeDoc()	 {
	if (!data)
		return;
	if (type == DocOPEN)
		delete (OPENDoc*)data;
	else if (type == DocMS)
		delete (MSDoc*)data;		
	
	Ole::Close();
}

bool OfficeDoc::IsAvailable(String strtype) {
	if (strcmp(strtype, "Open") >= 0)
		return OPENDoc::IsAvailable();
	else if (strcmp(strtype, "Microsoft") >= 0)
		return MSDoc::IsAvailable();
	else
		return false;
}

bool OfficeDoc::Init(String strtype) {
	if (data) {
		if (type == DocOPEN)
			delete (OPENDoc*)data;
		else if (type == DocMS)
			delete (MSDoc*)data;	
	}
	if (strcmp(strtype, "Open") >= 0) {
		type = DocOPEN;
		data = new OPENDoc();
	} else if (strcmp(strtype, "Microsoft") >= 0) {
		type = DocMS;
		data = new MSDoc();
	} else
		return false;
	return true;
}

bool OfficeDoc::AddDoc(bool visible) {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->AddDoc(visible);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->AddDoc(visible);
	else
		return false;
}

bool OfficeDoc::OpenDoc(String fileName, bool visible) {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->OpenDoc(fileName, visible);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->OpenDoc(fileName, visible);
	else
		return false;
}

bool OfficeDoc::SetFont(String font, int size) {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->SetFont(font, size);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->SetFont(font, size);
	else
		return false;
}

bool OfficeDoc::SetBold(bool bold) {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->SetBold(bold);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->SetBold(bold);
	else
		return false;
}

bool OfficeDoc::SetItalic(bool italic) {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->SetItalic(italic);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->SetItalic(italic);
	else
		return false;
}

bool OfficeDoc::WriteText(String value) {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->WriteText(value);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->WriteText(value);
	else
		return false;
}

bool OfficeDoc::Select() {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->Select();  
	else if (type == DocMS) 
		return ((MSDoc*)data)->Select();
	else
		return false;
}

bool OfficeDoc::Replace(String search, String replace) {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->Replace(search, replace);  
	else if (type == DocMS) 
		return ((MSDoc*)data)->Replace(search, replace);
	else
		return false;
}

bool OfficeDoc::Print() {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->Print();  
	else if (type == DocMS) 
		return ((MSDoc*)data)->Print();
	else
		return false;
}

bool OfficeDoc::SaveAs(String fileName, String _type) {
	if (!data)
		return false;
	if (type == DocOPEN) {
		int ret;
		ret = ((OPENDoc*)data)->SaveAs(fileName, _type);  
		((OPENDoc*)data)->SetSaved(true);
		return ret;
	} else if (type == DocMS) {
		((MSDoc*)data)->SetSaved(true);
		return ((MSDoc*)data)->SaveAs(fileName, _type);
	} else
		return false;
}

bool OfficeDoc::Quit() {
	if (!data)
		return false;
	if (type == DocOPEN) 
		return ((OPENDoc*)data)->Quit();  
	else if (type == DocMS) 
		return ((MSDoc*)data)->Quit();
	else
		return false;
}


