#include "ide.h"

void Ide::SyncExternalDependencies(bool force)
{
	String triplet = GetTargetTriplet();
	
	Vector<String> m = MissingExternalDependencies(triplet);
	
	if(m.GetCount() == 0) {
		if(force)
			PromptOK("No missing external dependencies.");
		return;
	}
	
	Sort(m);
	String xd = Join(m, "\n");
	
	String xd_path = ConfigFile(triplet + ".missing");

	if(!force && LoadFile(xd_path) == xd) {
		::SaveFile(xd_path, xd);
		return;
	}
	
	String txt = "There are missing external dependencies";
	String h = GetTargetTriplet();
	if(h.GetCount())
		txt << " for the target [* \1" << h << "\1]";
	
	if(PromptYesNo(txt + ":&&[* \1" + Join(m, " ") +
	               "\1]&&Install? [/ (it can be done later from the Project menu)")) {
		UrepoConsole console;
		
		if(!InstallMissingExternalDependencies([&](const String& cmd, const String& chdir)
		                                       { return console.System(cmd, chdir); },
		                                       TheIde()->GetTargetTriplet())) {
			console.Log("There were errors.", SRed());
			console.Perform();
		}

		::SaveFile(xd_path, Join(MissingExternalDependencies(triplet), "\n"));
	}
}

void Ide::CreateSBOM()
{
	SyncExternalDependencies(true);
	SelectSaveFile("*.json\t*.*", MakeBuild::CreateSBOM(GetTargetTriplet()));
}
