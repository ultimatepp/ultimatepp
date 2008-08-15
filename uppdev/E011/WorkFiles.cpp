#include "e011.h"

void App::OpenFile() {
	String filename;
	int i;

	if(workOpen) {
		if(PromptYesNo("Desideri salvare la mappatura in corso prima di aprirne un'alta ?") == 1) {
			SaveFile();
		}
	}

	fs.Type("cdf files", "*.bdc").DefaultExt("bdc").ActiveDir(paths.At(0));
	if(!fs.ExecuteOpen(t_("Select file"))) {
		return;
	}
	filename = fs;
	status.Set(0, filename, 250);
	LoadFromFile(mappa, filename);

	if(port.IsOpen()) {
		if(mappa.version != verA*256 + verB && mappa.variant != verV) {
			if(PromptYesNo("Attenzione! Questa mappatura è stata creata per un contalitri di versione diversa: V" + Format("%i", (mappa.version & 0xff00) / 256) + "."	+ Format("%i", mappa.version & 0xff) + "." + Format("%i", mappa.variant) + " Continuando i dati potrebbero non essere coerenti. Desideri proseguire?") != IDOK) {
				return;
			}
		}
	}
	else {
		verA = (mappa.version & 0xff00) >> 8;
		verB = mappa.version & 0x00ff;
		verV = mappa.variant;
	}

	conf.vu = mappa.vu;
	conf.ng = mappa.ng;
	conf.vref = mappa.vref;
	conf.i_doppler = mappa.i_doppler;

	mapForm.data.Clear();
	mapForm.description.Set((char*)~mappa.descrizione);
	for(i=0; i<mappa.dati.GetCount(); i++) {
		mapForm.data.Set(i, 2, Format("%04X", (int)mappa.dati[i]));
		mapForm.data.Set(i, 1, i*conf.vu);
		mapForm.data.Set(i, 0, i+24);
		mapForm.data.Set(i, 3, mappa.resistenza[i]);
	}
	workOpen = true;
	setToolsBar();
	leftFrame.NoZoom();
	bottomFrame.NoZoom();
	mainFrame.Zoom(100);
	console.Clear();
	return;
}

void App::NewFile() {
	int i;

	if(workOpen) {
		if(PromptYesNo("Desideri salvare la mappatura in corso prima di crearne una nuova ?") == 1) {
			SaveFile();
		}
	}
	if(!hardwareFound) {
		//Exclamation("Non è possibile creare una mappatura se non si è collegati ad un contalitri.");
		//console.Put("Non è possibile creare una mappatura se non si è collegati ad un contalitri.");
		verA = VERSIONE;
		verB = REVISIONE;
		verV = 0;
		//return;
	}

	mapForm.description.Clear();
	mapForm.data.Clear();
	conf.vu = 1;
	conf.ng = 1;
	conf.vref = 154;
	conf.i_doppler = 0;

	workOpen = false;
	setToolsBar();
	leftFrame.NoZoom();
	bottomFrame.NoZoom();
	mainFrame.Zoom(100);
	console.Clear();
	return;
}

void App::CloseFile() {

	if(workOpen) {
		if(PromptYesNo("Desideri salvare la mappatura in corso prima di chiudere ?") == 1) {
			SaveFile();
		}
	}
	status.Set(0, "", 250);
	mapForm.description.Clear();
	mapForm.data.Clear();
	workOpen = false;
	setToolsBar();
	leftFrame.Zoom(100); // hide all
	console.Clear();
	return;
}

void App::SaveFile() {
	String filename;
	int i;

	fs.Type("cdf files", "*.bdc").DefaultExt("bdc").ActiveDir(paths.At(0));
	if(!fs.ExecuteSaveAs(t_("Select file"))) {
		return;
	}
	filename = fs;

	mappa.descrizione = ~mapForm.description;
	mappa.vu = conf.vu;
	mappa.ng = conf.ng;
	mappa.vref = conf.vref;
	mappa.i_doppler = conf.i_doppler;
	mappa.version = (verA*256 + verB);
	mappa.variant = verV;
	mappa.dati.Clear();
	for(i=0; i<mapForm.data.GetCount(); i++) {
		mappa.dati.At(i) = ScanInt(String(mapForm.data.Get(i, 2)), 0, 16);
		mappa.resistenza.At(i) = mapForm.data.Get(i, 3);
	}
	StoreToFile(mappa, filename);
	setToolsBar();
	return;
}

void App::DeleteFile() {

}
