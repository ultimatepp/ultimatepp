#include "ide.h"

#ifndef PLATFORM_WIN32

String Ide::GetTargetTriplet()
{
	return Null;
}

#endif

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
	
	String cmd = InstallMissingExternalDependenciesCommand(triplet);
	if(cmd.GetCount()) {
		WriteClipboardText(cmd);
		txt << " installable by command:&&[* \1" << cmd
		    << "\1]&[/ (the command was placed on clipboard)]&&"
		    << "Execute?";
	}
	else
		txt << ":&&[* \1" + Join(m, " ") <<
		       "\1]&&Install?";
	txt << " [/ (this can be invoked again later from the Project menu)";
	
	if(PromptYesNo(txt)) {
		UrepoConsole console;
		
		if(InstallMissingExternalDependencies([&](const String& cmd, const String& chdir)
		                                       { return console.System(cmd, chdir); },
		                                       TheIde()->GetTargetTriplet()))
			console.Log("OK", SGreen());
		else
			console.Log("There were errors.", SRed());

		console.Perform();

		::SaveFile(xd_path, Join(MissingExternalDependencies(triplet), "\n"));
	}
}

void Ide::CreateSBOM()
{
	SyncExternalDependencies(true);
	SelectSaveFile("*.json\t*.*", MakeBuild::CreateSBOM(GetTargetTriplet()));
}
