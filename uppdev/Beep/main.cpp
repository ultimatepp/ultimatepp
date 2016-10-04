#include <CtrlLib/CtrlLib.h>

void sBeepInformation()
{
	if(fork()) return;
	system("aplay /usr/share/sounds/info.wav");
	abort();
}

GUI_APP_MAIN
{
	TopWindow h;
	h.SetRect(-100, -100, 10, 10);
	h.Open();
	Vector<int> nets = GetPropertyInts(Xroot, XAtom("_NET_FRAME_EXTENTS"));
	DUMPC(nets);
	nets = GetPropertyInts(Xroot, XAtom("_NET_WORKAREA"));
	DUMPC(nets);
	h.Close();
	do {
		DUMP(Ctrl::GetWorkArea());
//		sBeepInformation();
	}
	while(PromptYesNo("Continue?"));
}
