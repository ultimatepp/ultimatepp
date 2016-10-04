#include <Core/Core.h>

using namespace Upp;

#include <AddOn/AddOnLoader.h>
#include <AddOnHost/IAddOns.h>

#define DLLPATH "C:\\upp\\out\\MINGW.Addon.Debug_full.Dll"

CONSOLE_APP_MAIN
{
	AddOnLoader<IDoPlugin> addOns;

	addOns.LoadFolder(DLLPATH);

	for (int i = 0; i < addOns.GetCount(); i++) {
		Cout() << "Name: " << addOns[i]->GetName() <<
				  " v" << addOns[i]->GetMajorRevision() << "." << addOns[i]->GetMinorRevision() <<
				  " - Action: " <<  addOns[i]->Do() << "\n";
	}
}
