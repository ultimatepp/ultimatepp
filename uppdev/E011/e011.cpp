#include "e011.h"

unsigned int id, unlockNumber;
Vector<String> recents, paths;
String lastComPort;
bool fullRelease, hardwareFound;
COMPort port;
HelpWin help;

void App::Serialize(Stream& s) {
	int version = VERSIONE, revision = REVISIONE;
	s % version % revision;
	s % unlockNumber;
	s % paths % recents;
	s / lastComPort;

	if(s.IsStoring()) {
		String keys = StoreKeys();
		s % keys;
	}
	else {
		String keys;
		s % keys;
		RestoreKeys(keys);
	}
	return;
}

App::App() {
	int i;
	HKEY hkey;
	Vector<String> subvalues;
	char temp[_MAX_PATH];
	String buff;
	dword len;

	SetLanguage(LNG_('I', 'T', 'I', 'T'));
	SetDefaultCharset(CHARSET_WIN1252);
	Draw::SetStdFont(ScreenSans(11));

	workOpen = false;
	hardwareFound = false;

	CtrlLayout(*this, "E011 - Mappatura contalitri");
	CtrlLayout(mapForm);
	CtrlLayout(conForm);
	WhenClose = THISBACK(Quit);
	Icon(AppImg::E011_16());
	AddFrame(menu);
	AddFrame(TopSeparatorFrame());
	AddFrame(toolbar);
	AddFrame(TopSeparatorFrame());
	menu.Set(THISBACK(MainMenu));
  setToolsBar();
  leftFrame.Horz(tree, bottomFrame);
  leftFrame.SetPos(3000);
  bottomFrame.Vert(mainFrame, console);
 	bottomFrame.SetMin(0, 8000);
 	bottomFrame.SetPos(0);
  mainFrame << conForm << mapForm;
  mainFrame.Vert();

	Add(leftFrame.LeftPosZ(1, 498).TopPosZ(1, 330));
	leftFrame.Zoom(100); // hide all
	Add(status);
	tree <<= THISBACK(TreeCursor);

	status.HSizePosZ(0, 0).BottomPosZ(1, 20);
	status.Set(0, "", 250);
	status.Set(1, "", 40);
	status.Set(2, "", 40);
	status.Set(3, "", 40);
	status.Set(4, "", 20);

	mapForm.data.WhenEnterRow = THISBACK(EvaluateData);
	mapForm.data.AddColumn("File", 0);
	mapForm.data.AddColumn("Volume [dm³]", 2);
	mapForm.data.AddColumn("Livello analogico", 2);
	mapForm.data.AddColumn("Resistenza [Ohm] - Sperimentale", 5).Edit(ei.Add());

	conForm.help_rmax.SetImage(commonImg::help_1_16());
	conForm.help_volume.SetImage(commonImg::help_1_16());
	conForm.help_gradini.SetImage(commonImg::help_1_16());
	conForm.help_rmax <<= THISBACK(HelpRmax);
	conForm.help_volume <<= THISBACK(HelpVolume);
	conForm.help_gradini <<= THISBACK(HelpGradini);
	conForm.rmax <<= THISBACK(CheckVref);
	conForm.validate <<= THISBACK(MakeMap);
	conForm.volume = 1;
	conForm.gradini = 1;
	conForm.rmax.Add(AsString(  6), "127");
	conForm.rmax.Add(AsString( 11), "234");
	conForm.rmax.Add(AsString( 26), "552");
	conForm.rmax.Add(AsString( 39), "828");
	conForm.rmax.Add(AsString( 52), "1104");
	conForm.rmax.Add(AsString( 77), "1635");
	conForm.rmax.Add(AsString(103), "2187");
	conForm.rmax.Add(AsString(128), "2717");
	conForm.rmax.Add(AsString(154), "3269");
	conForm.rmax.SetIndex(conForm.rmax.FindKey("154"));
	conForm.rmax.Enable(false);

	InitTree();

  return;
}

void App::Quit() {
  port.disconnect();
	Close();
}

void App::GoOnline(void) {

	if(port.IsOpen()) {
		port.disconnect();
		KillTimeCallback(1);
		status.Set(2, "", 40);
		status.Set(3, "", 40);
		status.Set(4, "", 20);
		setToolsBar();
		hardwareFound = false;
    verA = 0;
    verB = 0;
    verV = 0;
		return;
	}
	else {
		String buff;
		COMPort::errorStatus es;

	  port.connect(lastComPort, COMPort::br19200, COMPort::None, COMPort::db8, COMPort::sb1);
	  es = port.clearError();
		if(es.dwErrors) {
			Exclamation("Errore apertura porta seriale. La porta potrebbe già essere in uso. Cambiare porta o chiudere le applicazioni che ne fanno uso. Errore n. " + AsString(es.dwErrors));
			console.Put("Errore apertura porta seriale. La porta potrebbe già essere in uso. Cambiare porta o chiudere le applicazioni che ne fanno uso. Errore n. " + AsString(es.dwErrors));
			return;
		}
	  ComConfig();

		hardwareFound = false;
    if (TestHardware()) {
      if(TestHardwareType()) {
    		hardwareFound = true;
      }
    }
    if(hardwareFound) {
      mode = SEP1_GetMode();
			AnimatedOnlineIcon();
  	  setToolsBar();
    }
    return;
	}
}

void App::ComConfig(void) {
	port.setHandshaking(false);
	port.setxONxOFF(false);
	port.setBlockingMode(3000,30,300); // imposto il time-out
	port.Setup(32, 32);
	return;
}

bool App::TestHardware(void) {
	char buff[32], buff1[32];
	int i, j;
	COMPort::errorStatus es;
	for(j=0; j<32; j++) buff[j] = 0;
	for(j=0; j<32; j++) buff1[j] = 0;

	for(j=0; j<3; j++) {
		port.ClearReadBuffer();
		SEP1_EscCmd( "\006" );

		i = port.readString(buff, 2);
    if(i == 2) {
    	if(buff[0] == STX) {
		    i = port.readString(buff1, buff[1]);
    		if(i == buff[1]) {
    		 	if(buff1[0] == ACK) return true; // hardware trovato e compatibile
    		}
	  	}
		}
	}
	es = port.clearError();
	if(es.dwErrors) {
		port.ClearReadBuffer();
		Exclamation("Errore n. " + AsString(es.dwErrors) + " su porta " + lastComPort + ". Il collegamento viene chiuso.");
		console.Put("Errore n. " + AsString(es.dwErrors) + " su porta " + lastComPort + ". Il collegamento viene chiuso.");
		port.disconnect();
		return false;
	}
	Exclamation("Nessun hardware trovato. Il collegamento viene chiuso.");
	console.Put("Nessun hardware trovato. Il collegamento viene chiuso.");
	port.disconnect();
  return false;
}

bool App::TestHardwareType(void) {
	char buff[32], buff1[32];
	int i, j, l;
	COMPort::errorStatus es;
	bool b1, b2;

	b1 = false;
	b2 = false;
	for(j=0; j<3; j++) {
		hardware.Clear();
		for(l=0; l<2; l++) {
			i = SEP1_ReadFlash(l);
	 		hardware.Cat((char) (i >> 8));
	    hardware.Cat((char) i);
	    b1 = true;
		}
		if(b1) {
			if(hardware == "E011") {
				verA = 0;
				verB = 0;
				i = SEP1_ReadFlash(VER);
    		verA = (char) (i >> 8);
    		verB = (char) i;
    		verV = (char) SEP1_ReadFlash(REV);
    		if(compatibility_verA >= verA && compatibility_verB >= verB) b2 = true;
    		else {
    			Exclamation("Questo programma non è aggiornato per il contalitri in uso. Il programma supporta contalitri con firmware fino a: V" + AsString(compatibility_verA) + "." + AsString(compatibility_verB) + ". Il contalitri collegato ha un firmware con versione maggiore: V" + AsString(verA) + "." + AsString(verB) + ". Il collegamento viene chiuso.");
    			console.Put("Questo programma non è aggiornato per il contalitri in uso. Il programma supporta contalitri con firmware fino a: V" + AsString(compatibility_verA) + "." + AsString(compatibility_verB) + ". Il contalitri collegato ha un firmware con versione maggiore: V" + AsString(verA) + "." + AsString(verB) + ". Il collegamento viene chiuso.");
    			b2 = false;
    			return false;
    		}
			}
			else {
				Exclamation("L'hardware collegato non è un contalitri \'E011\', impossibile proseguire. Il collegamento viene chiuso.");
				console.Put("L'hardware collegato non è un contalitri \'E011\', impossibile proseguire. Il collegamento viene chiuso.");
				b1 = false;
				return false;
			}
		}
		if(b1 && b2) {
			status.Set(2, hardware, 50);
			status.Set(3, "V" + AsString(verA) + "." + AsString(verB), 35);
			return true;
		}
	}
	GoOnline();
  return false;
}

void App::ComChange(void) {
	int i;
	HKEY hkey;
	Vector<String> values, data;
	char temp[512];
	dword len;
	String s;

	// recupero i nomi delle porte seriali installate
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", 0, KEY_READ, &hkey) == ERROR_SUCCESS) {
		for(dword dw = 0; len = sizeof(temp), RegEnumValue(hkey, dw, temp, &len, 0, 0, 0, 0) == ERROR_SUCCESS; dw++)
			values.Add(temp);

		RegCloseKey(hkey);
	}
	for(i = 0; i < values.GetCount(); i++) {
		data.Add(GetWinRegString(values[i], "Hardware\\DeviceMap\\SerialComm"));
	}

	DropSelect(lastComPort, "E011 - Selezione porta seriale", "Porta", data);
	status.Set(1, lastComPort, 40);
	return;
}

struct AboutDlg : TopWindow {
	RichTextView about;

	typedef AboutDlg CLASSNAME;

	AboutDlg() {
		SetRect(0, 0, 640, 480);
		about.LeftPosZ(2, 636).TopPosZ(2, 476);
		String a = AcquireTopic("topic://E011/help/_about$it-it");
		a = MyKeyReplace(a, "licenza", GetVolumeSn(GetExeDirFile("").Left(3), 6));
		a = MyKeyReplace(a, "version", FormatInt(VERSIONE));
		a = MyKeyReplace(a, "revision", FormatInt(REVISIONE));
		about.SetQTF(a, Zoom(150, 1024));
		about.SetZoom(Zoom(12, 10));
		Background(PaintRect(ColorDisplay(), SWhite()));
		Add(about);
		Title(t_("About E011"));
		return;
	}
};

void App::About() {
	AboutDlg().Execute();
	return;
}

void App::Animation() {
	if(port.IsOpen()) {
		RealTimeD rtd;
  	rtd.Perform();
	}
}

void App::AnimatedOnlineIcon() {

	if(port.IsOpen()) {
		if(icon_step == 0) status.Set(4, PaintRect(Single<DisplayOnline>()), 20);
		else if(icon_step == 1) status.Set(4, PaintRect(Single<DisplayOnline_1>()), 20);
		else if(icon_step == 2) status.Set(4, PaintRect(Single<DisplayOnline_2>()), 20);
		else if(icon_step == 3) status.Set(4, PaintRect(Single<DisplayOnline_3>()), 20);
		icon_step++;
		if(icon_step > 3) icon_step = 0;
		SetTimeCallback(250, THISBACK(AnimatedOnlineIcon), 1);
	}
	else {
		KillTimeCallback(1);
		status.Set(2, "", 40);
		status.Set(3, "", 40);
		status.Set(4, "", 20);
		setToolsBar();
	}
	return;
}

void App::DownloadMap(void) {
  int i,j,k;

	if(workOpen) {
		if(PromptYesNo("Desideri salvare la mappatura aperta prima di scaricare ?") == 1) {
			SaveFile();
		}
	}

	i = SEP1_ReadFlash(VER);
	verA = (i & 0xff00) >> 8;
	verB = i & 0x00ff;
	i = SEP1_ReadFlash(REV);
	verV = (char)i;

	status.Set(0, "", 250);
	mapForm.description.Clear();
	mapForm.data.Clear();
	if(SEP1_ReadFlash(CFG) == 1) {
	  workOpen = false;
	  j = SEP1_ReadFlash(LFS); // Volume unitario
	  k = SEP1_ReadFlash(NS); // Numero di gradini
	  conf.vu = j;
	  conf.ng = k;
		if((verA*256 + verB) >= 258) {
			i = SEP1_ReadFlash(VREF);
			conf.vref = i;
		}
		if((verA*256 + verB) >= 260) {
			conf.i_doppler = (bool)SEP1_ReadFlash(I_DOPPLER);
		}

	  double t, r, a;
	  if((verA*256 + verB) >= 258) {
	    t = (double)(5.00 / 256) * conf.vref;
	  }
	  else {
	  	if(verV == 0) t = 5.00;
	  	else t = 5.00;
	  }
		for(i=0; i<=k; i++) {
			mapForm.data.Set(i, 1, i*j);
			mapForm.data.Set(i, 0, i+MAP_BEG);
			a = SEP1_ReadFlash(i+MAP_BEG);
			mapForm.data.Set(i, 2, Format("%04X", a));
			r = ((t / 4096) * a) / I5;
			mapForm.data.Set(i, 3, (int)r);
		}
		workOpen = true;
		setToolsBar();
		leftFrame.NoZoom();
		bottomFrame.NoZoom();
		mainFrame.Zoom(100);
		console.Clear();
		console.Put("Mappatura caricata con successo");
	}
	else {
		Exclamation("Il contalitri collegato non è ancora mappato. Impossibile proseguire.");
		console.Put("Il contalitri collegato non è ancora mappato. Impossibile proseguire.");
	}
	return;
}

void App::UploadMap(void) {
  unsigned int i, j, k;

	if(!workOpen) {
		Exclamation("Aprire una mappa o scaricarla da un contalitri prima di caricarla verso l'hardware.");
		console.Put("Aprire una mappa o scaricarla da un contalitri prima di caricarla verso l'hardware.");
		return;
	}

	if(SEP1_ReadFlash(CFG) == 1) {
	  if(PromptOKCancel("Il contalitri collegato è già mappato. Proseguire comunque?") != IDOK) {
	    return;
	  }
	}

	j = conf.vu;
	k = conf.ng;
	for(i=0; i<=k; i++) {
		SEP1_WriteFlash(i+MAP_BEG, ScanInt(String(mapForm.data.Get(i, 2)), 0, 16));
		Sleep(50);
	}
	SEP1_WriteFlash(i+MAP_BEG, ScanInt(String(mapForm.data.Get(i-1, 2)), 0, 16)); // non è un errore, questo serve.
	Sleep(50);
	SEP1_WriteFlash(LFS, j);
	Sleep(50);
	SEP1_WriteFlash(NS, k);
	Sleep(50);
	if((verA*256 + verB) >= 258) {
		i = conf.vref;
		SEP1_WriteFlash(VREF, i);
		Sleep(50);
	}
	if((verA*256 + verB) >= 260) {
		SEP1_WriteFlash(I_DOPPLER, conf.i_doppler);
		Sleep(50);
	}
	SEP1_WriteFlash(CFG, 1);
	Sleep(50);

	// reset MPU (critico)
	SEP1_EscCmd("R");

	return;
}

void App::VerifyMap() {
	bool er;

	er = false;
	if(!workOpen) {
		Exclamation("Aprire una mappa salvata per verificare la corrispondenza col contalitri collegato.");
		console.Put("Aprire una mappa salvata per verificare la corrispondenza col contalitri collegato.");
		return;
	}
	if(SEP1_ReadFlash(CFG) != 1) {
	  Exclamation("Il contalitri collegato non è mappato.");
	  console.Put("Il contalitri collegato non è mappato.");
	  return;
	}

	console.Put("Inizio confronto.");
	if(conf.vu != SEP1_ReadFlash(LFS)) {
	  console.Put("Errore: LFS non corrisponde.");
	  er = true;
	}
	if(conf.ng != SEP1_ReadFlash(NS)) {
	  console.Put("Errore: NS non corrisponde.");
	  er = true;
	}
	if(conf.vref != SEP1_ReadFlash(VREF)) {
	  console.Put("Errore: VREF non corrisponde.");
	  er = true;
	}
	if(conf.i_doppler != SEP1_ReadFlash(I_DOPPLER)) {
	  console.Put("Errore: DP non corrisponde.");
	  er = true;
	}

	int i;
	for(i=0; i<mapForm.data.GetCount(); i++) {
		if(ScanInt(String(mapForm.data.Get(i, 2)), 0, 16) != SEP1_ReadFlash(i+MAP_BEG)) {
	  	console.Put("Errore: file " + AsString(i) + " non corrisponde.");
	  	er = true;
		}
	}
	if(!er) {
	  console.Put("Confronto concluso. Non sono state rilevate differenze.");
	}
	else {
	  console.Put("Confronto concluso con errori.");
	}
	return;
}

void App::MakeMap(void) {

	if(conForm.volume < 1 || conForm.volume > 999) {
		Exclamation("Il volume unitario deve essere compreso tra 1 e 999.");
		console.Put("Il volume unitario deve essere compreso tra 1 e 999.");
		return;
	}
	if(conForm.gradini < 1 || conForm.gradini > 39) {
		Exclamation("Il numero di gradini deve essere compreso tra 1 e 39.");
		console.Put("Il numero di gradini deve essere compreso tra 1 e 39.");
		return;
	}
	if((conForm.volume * conForm.gradini) > 999) {
		Exclamation("Il numero di gradini per il volume unitario supera il limite del contalitri: 999. Ridurre i due valori");
		console.Put("Il numero di gradini per il volume unitario supera il limite del contalitri: 999. Ridurre i due valori");
		return;
	}

	int i;
	if(conf.vu != conForm.volume) {
		mapForm.data.Clear();
		for(i=0; i<=conForm.gradini; i++) {
			mapForm.data.Set(i, 0, i+MAP_BEG);
			mapForm.data.Set(i, 1, i*conForm.volume);
			mapForm.data.Set(i, 2, "0000");
			mapForm.data.Set(i, 3, 0);
		}
	}
	else {
		if(mapForm.data.GetCount() > (1+(int)conForm.gradini)) {
			mapForm.data.SetCount((int)conForm.gradini+1);
		}
		else if(mapForm.data.GetCount() < (1+(int)conForm.gradini)) {
			mapForm.data.SetCount((int)conForm.gradini+1);
			for(i=conf.ng+1; i<mapForm.data.GetCount(); i++) {
				mapForm.data.Set(i, 0, i+MAP_BEG);
				mapForm.data.Set(i, 1, i*conForm.volume);
				mapForm.data.Set(i, 2, "0000");
				mapForm.data.Set(i, 3, 0);
			}
		}
	}

	conf.vu = conForm.volume;
	conf.ng = conForm.gradini;
	conf.vref = atoi(String(conForm.rmax.GetKey(conForm.rmax.GetIndex())));
	conf.i_doppler = conForm.doppler.Get();

	workOpen = true;
	setToolsBar();

	EvaluateData();

	return;
}

void App::LoadConfForm(void) {

	conForm.volume = conf.vu;
	conForm.gradini = conf.ng;
	if( (verA*256+verB) >= 258) {
		conForm.rmax.Enable(true);
		conForm.rmax.SetIndex(conForm.rmax.FindKey(AsString(conf.vref)));
	}
	else {
		conForm.rmax.Enable(false);
	}
	if( (verA*256+verB) >= 260) {
		if(conf.vref <= 52 ) {
			conForm.doppler.Enable(true);
			conForm.doppler.Set(conf.i_doppler);
		}
		else {
			conForm.doppler.Enable(false);
			conForm.doppler = false;
		}
	}
	else {
		conForm.doppler.Enable(false);
		conForm.doppler = false;
	}
	return;
}

void App::CheckVref(void) {

	if( (verA*256+verB) >= 260) {
		if(atoi(String(conForm.rmax.GetKey(conForm.rmax.GetIndex()))) <= 52 ) {
			conForm.doppler.Enable(true);
			conForm.doppler = conf.i_doppler;
		}
		else {
			conForm.doppler.Enable(false);
			conForm.doppler = false;
		}
	}

	return;
}

void App::EvaluateData(void) {
	double v;
	int a, i;

	if((verA*256 + verB) >= 258) {
		v = ((5.00 * atoi(String(conForm.rmax.GetKey(conForm.rmax.GetIndex())))) / 256);
	}
	else {
		if(verV == 0) v = 5.00;
		else v = 5.00;
	}
	for(i=0; i<mapForm.data.GetCount(); i++) {
		mapForm.data.Set(i, 2, Format("%04X", (((double)mapForm.data.Get(i, 3) * I5) / (v / 4096))));
	}
	return;
}

void App::HelpRmax(void) {
	help.OpenHelp("help/_help_rmax$it-it");
	return;
}

void App::HelpVolume(void) {
	help.OpenHelp("help/_help_volume$it-it");
	return;
}

void App::HelpGradini(void) {
	help.OpenHelp("help/_help_gradini$it-it");
	return;
}

GUI_APP_MAIN {
	SetLanguage(LNG_('I', 'T', 'I', 'T'));
	SetDefaultCharset(CHARSET_WIN1252);
	Draw::SetStdFont(ScreenSans(11));

	const Vector<String>& arg = CommandLine();
	for(int i=0; i<arg.GetCount(); i++) {
		if(arg[i] == "--uninstall") {
			Uninstall();
			return;
		}
	}

	App appl;
	LoadFromFile(appl);
	if(lastComPort.GetLength() < 4) {
		lastComPort = "COM1";
	}
	appl.status.Set(1, lastComPort, 40);
	appl.Run();
	StoreToFile(appl);
	FileDelete(GetExeDirFile("e011.log"));
}
