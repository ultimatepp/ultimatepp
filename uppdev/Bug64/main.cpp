#include <CtrlLib/CtrlLib.h>

#include <ide/Browser/Browser.h>

GUI_APP_MAIN
{
	RichText txt = ParseQTF(ReadTopic(LoadFile("/media/hda5/Slider$en-us.tpp")).text);
}
