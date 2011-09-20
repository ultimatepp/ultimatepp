#include "AccessKey.h"

#define KEYGROUPNAME "Ide"
#define KEYNAMESPACE IdeKeys
#define KEYFILE      <AccessKey/test.key>
#include             <CtrlLib/key_header.h>

#define KEYGROUPNAME "Ide"
#define KEYNAMESPACE IdeKeys
#define KEYFILE      <AccessKey/test.key>
#include             <CtrlLib/key_source.h>

AccessKey::AccessKey()
{
	CtrlLayoutOKCancel(*this, "Window title");
}

GUI_APP_MAIN
{
	Ctrl::AddFlags(Ctrl::AKD_CONSERVATIVE);
	AccessKey().Run(Accept());
	String s;
	s.Cat()
}
