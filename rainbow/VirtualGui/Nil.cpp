#include "VirtualGui.h"

namespace Upp {

dword NilGUI::GetOptions() { return 0; }
Size NilGUI::GetSize() { return Size(1000, 1000); }
dword NilGUI::GetMouseButtons() { return 0; }
dword NilGUI::GetModKeys() { return 0; }
bool NilGUI::IsMouseIn() { return false; }
bool NilGUI::ProcessEvent(bool *quit) { return true; }
void NilGUI::WaitEvent(int ms) { Sleep(1); }
bool NilGUI::IsWaitingEvent() { return false; }
void NilGUI::WakeUpGuiThread() {}
void NilGUI::SetMouseCursor(const Image& image) {}

SystemDraw& NilGUI::BeginDraw()
{
	sysdraw.SetTarget(&nildraw);
	return sysdraw;
}

void NilGUI::CommitDraw()
{
}

void NilGUI::Quit() {}

};
