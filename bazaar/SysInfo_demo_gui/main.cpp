#include <CtrlLib/CtrlLib.h>

#include <SysInfo/SysInfo.h>
#include <DropGrid/DropGrid.h>

using namespace Upp;

#include "SysInfo_demo_gui.h"

#define IMAGEFILE <SysInfo_demo_gui/SysInfo_demo_gui.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>
#define LAYOUTFILE <SysInfo_demo_gui/SysInfo_demo_gui.lay>
	
	
GUI_APP_MAIN { 
	SysInfoDemo program;
	
	SetLanguage(LNGC_('E','N','U','S', CHARSET_UTF8));
    
	program.Run();
}

void SpecialFolders::Fill() {
	TextDesktop = GetDesktopFolder();
	TextPrograms = GetProgramsFolder();
	TextApplication = GetAppDataFolder();
	TextMusic = GetMusicFolder();
	TextPictures = GetPicturesFolder();
	TextVideo = GetVideoFolder();
	TextPersonal = GetPersonalFolder();
	TextTemplates = GetTemplatesFolder();
	TextDownload = GetDownloadFolder();
	TextTemp = GetTempFolder();
	TextOs = GetOsFolder();
	TextSystem = GetSystemFolder();
	
	DefaultExes.Reset();	
	DefaultExes.AddColumn("Extension", 5);
	DefaultExes.AddColumn("Program", 40);
	const char *ext[] = {".html", ".doc", ".png", ".pdf", ".txt", ".xyz", ""};
	for (int i = 0; *ext[i] != 0; ++i) 
		DefaultExes.Add(ext[i], GetExtExecutable(ext[i]));
#if defined(PLATFORM_WIN32)	
	ButInstalledSoftware.WhenPush = THISBACK(ButInstalledSoftware_Push);
#else
	SoftwareInstalled.Enable(false);
	ButInstalledSoftware.Enable(false);
#endif
}

void SpecialFolders::ButInstalledSoftware_Push() {
#if defined(PLATFORM_WIN32) 	
	ButInstalledSoftware.SetLabel("Getting list ...");
	ButInstalledSoftware.ProcessEvents();
	SoftwareInstalled.Reset();	
	SoftwareInstalled.AddColumn("Name", 60);
	SoftwareInstalled.AddColumn("Version", 15);
	SoftwareInstalled.AddColumn("Vendor", 30);
	SoftwareInstalled.AddColumn("Install Date", 15);
	SoftwareInstalled.AddColumn("Caption", 60);
	SoftwareInstalled.AddColumn("Description", 60);
	SoftwareInstalled.AddColumn("Install State", 10);
	Array <Value> name, version, vendor, caption, installDate, description, state;
	if (GetPackagesInfo(name, version, vendor, installDate, caption, description, state)) {
		for (int i = 0; i < name.GetCount(); ++i) 
			SoftwareInstalled.Add(name[i], version[i], vendor[i], installDate[i], caption[i], description[i], state[i]);
	}
	ButInstalledSoftware.SetLabel("Get installed software list");
#endif
}

void SystemInfo::Fill() {
	String manufacturer, productName, version, mbSerial;
	Date releaseDate;
	int numberOfProcessors;
	GetSystemInfo(manufacturer, productName, version, numberOfProcessors, mbSerial);
	
	TextManufacturer = manufacturer;
	TextProductName = productName;
	TextVersion = version;
	TextNumber = FormatInt(numberOfProcessors);
	TextSpeed = Format("%.3f GHz", GetCpuSpeed()/1000.);
	TextTemperature = FormatDouble(GetCpuTemperature());
	bool discharging;
	int percentage, remainingMin;
	if (GetBatteryStatus(discharging, percentage, remainingMin)) {
		TextAC = discharging ? "Off" : "On";
		if (!IsNull(remainingMin))
			TextRemaining = FormatInt(remainingMin) + " min";
		else
			TextRemaining = "";
	}
	TextMotherboardSerial = mbSerial;

 	String biosVersion, biosSerial;
 	Date biosReleaseDate;
	GetBiosInfo(biosVersion, biosReleaseDate, biosSerial);
	TextBiosVersion = biosVersion;
	TextBiosRelease = AsString(biosReleaseDate);
	TextBiosSerial = biosSerial;
	
	String kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion;
	if (GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion)) {
		TextKernel = kernel;
		TextKerVersion = kerVersion;
		TextKerArchitecture = kerArchitecture;
		TextDistro = distro;
		TextDistVersion = distVersion;
		TextDesktop = desktop;
		TextDeskVersion = deskVersion;
	}

	String nname, domain, ip4, ip6;
	if (GetNetworkInfo(nname, domain, ip4, ip6)) {
		Textnname = nname;
		TextDomain = domain;
		TextIp4 = ip4;
		TextIp6 = ip6;
	}
	
	int memoryLoad;
	uint64 totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual;
	if (GetMemoryInfo(memoryLoad, totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual)) {
		TextMemPercentage = FormatInt(memoryLoad);
		TextMemTotPhysicalbytes = Format64(totalPhys);
		TextMemTotPhysical = BytesToString(totalPhys);
		TextMemFreePhysicalbytes = Format64(freePhys);
		TextMemFreePhysical = BytesToString(freePhys);			
		TextMemTotPagingbytes = Format64(totalPageFile);
		TextMemTotPaging = BytesToString(totalPageFile);
		TextMemFreePagingbytes = Format64(freePageFile);
		TextMemFreePaging = BytesToString(freePageFile);
		TextMemTotVirtualbytes = Format64(totalVirtual);
		TextMemTotVirtual = BytesToString(totalVirtual);
		TextMemFreeVirtualbytes = Format64(freeVirtual);
		TextMemFreeVirtual = BytesToString(freeVirtual);
	}
	String compiler, mode;
	Time tim;
	int compilerVersion, bits;
	GetCompilerInfo(compiler, compilerVersion, tim, mode, bits);
	TextCompiler = compiler;
	TextCompilerVersion = FormatInt(compilerVersion);
	TextCompilationDate = Format(tim);	
	TextCompilationMode = mode;
	TextCompilationBits = FormatInt(bits);
	
	Processors.Reset();
	Processors.AddColumn("Processor", 6);
	Processors.AddColumn("Vendor", 12);
	Processors.AddColumn("Identifier", 40);
	Processors.AddColumn("Architecture", 30);
	Processors.AddColumn("Speed MHz", 10);
	String vendor, identifier, architecture; 
	int speed;
	for (int i = 0; i < numberOfProcessors; ++i) {
		GetProcessorInfo(i, vendor, identifier, architecture, speed);		
		Processors.Add(Format("#%d", i), vendor, identifier, architecture, FormatInt(speed));	
	}
#if defined(PLATFORM_WIN32) 	
	Video.Reset();
	Video.AddColumn("Name", 10);
	Video.AddColumn("Description", 10);
	Video.AddColumn("Video Processor", 10);
	Video.AddColumn("Adapter RAM Mb", 6);
	Video.AddColumn("Video Mode Description", 10);
	Array <Value> name, description, videoProcessor, videoMode, ram;
	if (GetVideoInfo(name, description, videoProcessor, ram, videoMode)) {
		for (int i = 0; i < name.GetCount(); ++i)
			Video.Add(name[i], description[i], videoProcessor[i], ram[i], videoMode[i]);
	} else
		Video.Add("Video data not accesible");
#else
	Video.Enable(false);
#endif	
	Drives.Reset();
	Drives.AddColumn("Path", 10);
	Drives.AddColumn("Type", 10);
	Drives.AddColumn("Volume", 10);
	Drives.AddColumn("Max name", 5);
	Drives.AddColumn("File System", 5);
	Drives.AddColumn("Total User bytes", 10);
	Drives.AddColumn("Total User", 6);	
	Drives.AddColumn("Free User bytes", 10);
	Drives.AddColumn("Free User", 6);
	Drives.AddColumn("Total Free bytes", 10);
	Drives.AddColumn("Total Free", 6);
	Vector<String> drives;
	drives = GetDriveList();
	for (int i = 0; i < drives.GetCount(); ++i) {
		Vector <Value> row;
		row.Clear();
		row.Add(drives[i]);
		String type, volume, fileSystem;
		int maxName;
		bool mounted = GetDriveInformation(drives[i], type, volume, maxName, fileSystem);
		if (mounted) {
			row.Add(type);
			row.Add(volume);
			row.Add(maxName);
			row.Add(fileSystem);
			uint64 freeBytesUser, totalBytesUser, totalFreeBytes;
			if(!GetDriveSpace(drives[i], freeBytesUser, totalBytesUser, totalFreeBytes))
				row.Add("Installed but no access to drive");
			else {
				row.Add(Format64(totalBytesUser));
				row.Add(BytesToString(totalBytesUser));				
				row.Add(Format64(freeBytesUser));
				row.Add(BytesToString(freeBytesUser));
				row.Add(Format64(totalFreeBytes));
				row.Add(BytesToString(totalFreeBytes));
			}
		} else
			row.Add("Not mounted"); 
		
		Drives.Add(row);
	}
	Adapters.Reset();
	Adapters.AddColumn("Type", 10);
	Adapters.AddColumn("Description", 20);
	Adapters.AddColumn("Fullname", 20);
	Adapters.AddColumn("MAC", 10);
	Adapters.AddColumn("IP4", 10);
	Adapters.AddColumn("IP6", 10);
	Array <NetAdapter> adapters = GetAdapterInfo();	
	for (int i = 0; i < adapters.GetCount(); ++i) 
		Adapters.Add(adapters[i].type, adapters[i].description, adapters[i].fullname, 
				   adapters[i].mac, adapters[i].ip4, adapters[i].ip6);
	ButUpdate.WhenPush = THISBACK(ButUpdate_Push);
}

void SystemInfo::ButUpdate_Push() {
	Fill();
}


#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

#define SetBit(uIntFlagBuff, Bit)  		(uIntFlagBuff |= (Bit))
#define ClearBit(uIntFlagBuff, BitNum) 	(uIntFlagBuff  = ((uIntFlagBuff) | (BitNum)) ^ (BitNum) )
static const char   *styles[] 	 = {"WS_CAPTION","WS_BORDER","WS_TABSTOP","WS_CHILD","WS_THICKFRAME","WS_VISIBLE","WS_VSCROLL","WS_HSCROLL","WS_SYSMENU","WS_SIZEBOX","WS_POPUP","WS_MINIMIZE","WS_MAXIMIZE","WS_MINIMIZEBOX","WS_MAXIMIZEBOX","WS_GROUP","WS_DLGFRAME","WS_CLIPSIBLINGS","WS_CLIPCHILDREN","WS_CHILDWINDOW","WS_DISABLED"};
static const uint64 stylesbits[] = {WS_CAPTION,WS_BORDER,WS_TABSTOP,WS_CHILD,WS_THICKFRAME,WS_VISIBLE,WS_VSCROLL,WS_HSCROLL,WS_SYSMENU,WS_SIZEBOX,WS_POPUP,WS_MINIMIZE,WS_MAXIMIZE,WS_MINIMIZEBOX,WS_MAXIMIZEBOX,WS_GROUP,WS_DLGFRAME,WS_CLIPSIBLINGS,WS_CLIPCHILDREN,WS_CHILDWINDOW,WS_DISABLED};
static const char   *exstyles[]  = {"WS_EX_ACCEPTFILES","WS_EX_APPWINDOW","WS_EX_CLIENTEDGE","WS_EX_COMPOSITED","WS_EX_CONTROLPARENT","WS_EX_DLGMODALFRAME","WS_EX_LAYERED","WS_EX_LAYOUTRTL","WS_EX_LEFTSCROLLBAR","WS_EX_MDICHILD","WS_EX_NOACTIVATE","WS_EX_NOINHERITLAYOUT","WS_EX_NOPARENTNOTIFY"/*,"WS_EX_NOREDIRECTIONBITMAP"*/,"WS_EX_RIGHT","WS_EX_PALETTEWINDOW(WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW|WS_EX_TOPMOST)","WS_EX_STATICEDGE","WS_EX_TOOLWINDOW","WS_EX_TOPMOST","WS_EX_TRANSPARENT","WS_EX_WINDOWEDGE"};
static const uint64 exstylesbits[] = {WS_EX_ACCEPTFILES,WS_EX_APPWINDOW,WS_EX_CLIENTEDGE,WS_EX_COMPOSITED,WS_EX_CONTROLPARENT,WS_EX_DLGMODALFRAME,WS_EX_LAYERED,WS_EX_LAYOUTRTL,WS_EX_LEFTSCROLLBAR,WS_EX_MDICHILD,WS_EX_NOACTIVATE,WS_EX_NOINHERITLAYOUT,WS_EX_NOPARENTNOTIFY/*,WS_EX_NOREDIRECTIONBITMAP*/,WS_EX_RIGHT,WS_EX_PALETTEWINDOW,WS_EX_STATICEDGE,WS_EX_TOOLWINDOW,WS_EX_TOPMOST,WS_EX_TRANSPARENT,WS_EX_WINDOWEDGE};

struct SOptDropGrid: public DropGrid {
	int64 hwnd;
	ArrayOption option;
	typedef SOptDropGrid CLASSNAME;
	
	SOptDropGrid() {
		hwnd = 0;
		ClearButton();
		//AddPlus(THISBACK(action));
		AddColumn("Style", 60).Ctrls<Option>();
		AddColumn("Hexbit");
		AddColumn("Bit", 0).Hidden();
		Width(280);
	}
	void Init(int64 wnd) {
		hwnd = wnd;
		int row = 0;
		//SetData("-- Regular styles --");
		Ready(false);
		StaticText *st;
		Add(0, 0, 0); 
		st = new StaticText; 
		*st= "-- click me --"; 
		*st <<= THISBACK(Action); 
		GetList().SetCtrl(0, 0, st); 
		++row;
		Add(0, 0, 0); 
		st = new StaticText; 
		*st = "-- Regular styles --"; 
		GetList().SetCtrl(1, 0, st); 
		++row;
		Option *po;
		for(int i = 0; i < sizeof(styles)/sizeof(styles[0]); ++i, row++) {
			po = new Option; 
			po->Set((stylesbits[i]&GetWindowLong((HWND)hwnd, GWL_STYLE) ? true : false)); 
			po->SetLabel(styles[i]); 
			*po <<= THISBACK(Action);
			Add(po->Get(), ::Format("0x%08x", (int64)stylesbits[i]), *(int64*)&stylesbits[i]);
			GetList().SetCtrl(row, 0, po);
		}
		Add(0, 0); 
		st = new StaticText; 
		*st = "** Extendes styles **"; 
		GetList().SetCtrl(row, 0, st); 
		++row;
		for(int i = 0; i < sizeof(exstyles)/sizeof(exstyles[0]); row++, ++i){
			po = new Option; 
			po->Set((stylesbits[i]&GetWindowLong((HWND)hwnd,GWL_EXSTYLE)) ? true : false); 
			po->SetLabel(exstyles[i]);  
			*po <<= THISBACK(Action); 
			Add(po->Get(), ::Format("0x%08x", (int64)exstylesbits[i]), *(int64*)&exstylesbits[i]); 
			GetList().SetCtrl(row, 0, po);
		}
		Ready(true);
		/*GetList().SetCursor(0);
		SetIndex(0);
		SetData("-- click me --");*/
	}
	void Action() {
		int rowind = GetCurrentRow();
		uint64 bit = (int64)Get(2);
		if(bit == 0) {
			TopWindow tw; 
			tw.Title("~~~~~"); 
			Image img = Images::hat;
			ImageCtrl ic;
			ic.SetImage(img).SetRect(0, 0, img.GetWidth(), img.GetHeight()); 
			tw.Add(ic); 
			tw.SetRect(::GetMousePos().x - 20, GetMousePos().y - 20, img.GetWidth(), img.GetHeight()); 
			tw.RunAppModal(); 
			return;
		}
		bool on = ((Option*)GetList().GetCtrl(0))->Get() == 1;
		uint64 bits = GetWindowLong((HWND)hwnd, (rowind>3+sizeof(stylesbits)/sizeof(stylesbits[0])) ? GWL_EXSTYLE : GWL_STYLE);
		if(on)
			SetBit(bits, bit);
		else  
			ClearBit(bits, bit);
		SetWindowLong((HWND)hwnd, (rowind > 3 + sizeof(stylesbits)/sizeof(stylesbits[0])) ? GWL_EXSTYLE : GWL_STYLE, (LONG)bits);
		RedrawWindow(GetDesktopWindow(), 0, 0, RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_FRAME|RDW_ERASE);
		RedrawWindow((HWND)hwnd, 0, 0, RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_FRAME|RDW_ERASE);
	}
};

#endif

void WindowsList_::Fill() {
	Windows.Reset();
	Windows.AddColumn("Title", 30);
	Windows.AddColumn("Window handle", 8);
	Windows.AddColumn("Process Id", 8);
	Windows.AddColumn("Name", 20);
	Windows.AddColumn("File name", 30);
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	Windows.AddColumn("Window styles", 23).Ctrls<SOptDropGrid>();
	Windows.AddColumn("Window class", 23);
#endif
	Array<int64> widL, pidL;
	Array<String> name, fileName, caption;
	GetWindowsList(widL, pidL, name, fileName, caption);
	for (int i = 0; i < widL.GetCount(); ++i) {
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
		char classname[64];
		GetClassName(reinterpret_cast<HWND>(widL[i]), classname, 64);
		Windows.Add(caption[i], Format64(widL[i]), Format64(pidL[i]), name[i], fileName[i], "Text", classname);
		SOptDropGrid *dg = (SOptDropGrid*)Windows.GetCtrl(i, 5);
		dg->Init(widL[i]);
#else
		Windows.Add(caption[i], Format64(widL[i]), Format64(pidL[i]), name[i], fileName[i]);
#endif
	}
	Windows.SetEditable();
	ButUpdate.WhenPush = THISBACK(ButUpdate_Push);
	#if defined(PLATFORM_WIN32)	
	ButTopmost.WhenPush = THISBACK(ButTopmost_Push);
	#else
	ButTopmost.Disable();
	#endif
	static MenuBar bar;
	//GetList().StdToolBar(bar); //bar(2).Remove();bar(1).Remove();
	//GetList().WhenToolBar=THISBACK(cb);
	Windows.WhenBar = THISBACK(MenuCallback);
}

#if defined(PLATFORM_WIN32)	
void WindowsList_::ButTopmost_Push() {
	int row = Windows.GetCursor();
	if (row < 0)
		return;
	int64 id = ScanInt64(String(Windows.Get(row, 1)));
	Window_TopMost(id);
}
#endif

void WindowsList_::CbCopy() {
	String text;
	HeaderCtrl& header = Windows.HeaderObject();
	for(int i = 0; i < header.GetCount(); i++) {
		if(!header.IsTabVisible(i))
			continue;
		if(!text.IsEmpty())
			text << " | ";
		text << Windows.Get(i);
	}
	WriteClipboardText(text);
}

void WindowsList_::MenuCallback(Bar &bar){
	//Windows.SetClipboard(true,false);
	bar.AddMenu(true, "Copy", CtrlImg::copy(), THISBACK(CbCopy));
}

void WindowsList_::ButUpdate_Push() {
	Fill();
}

void ProcessList::Fill() {
	Process.Reset();
	Process.AddColumn("Id", 6);
	Process.AddColumn("Priority", 6);
	Process.AddColumn("Program", 12);
	Array<int64> pidL;
	pidL.Clear(); 
	Array<String> pNames;
	if (!GetProcessList(pidL, pNames))
		Process.Add("Error getting process info");
	else {
		for (int i = 0; i < pidL.GetCount(); ++i) {
			int priority = GetProcessPriority(pidL[i]);
			Process.Add(Format64(pidL[i]), priority >= 0? FormatInt(priority): "Not accesible", pNames[i]);
		}
	}
	ButUpdate.WhenPush = THISBACK(ButUpdate_Push);
}
void ProcessList::ButUpdate_Push() {
	Fill();
}

void ScreenGrabTab::Fill() {
	EditFileNameGrab <<= AppendFileName(GetDesktopFolder(), "ScreenGrab.avi");
	EditTime <<= 5;
	EditFrameRate <<= 1; 
	OpGrabMouse <<= true;
	SwGrabMode.Add("Desktop");
	SwGrabMode.Add("Window");
	SwGrabMode.Add("Rectangle");
	SwGrabMode.MinCaseHeight(20);
	SwGrabMode.SetData(0);
	SwGrabMode.WhenAction = THISBACK(SwGrabMode_Action);
	SwGrabMode_Action();
	ButGrab.WhenPush = THISBACK(ButGrab_Push);

	//ButGrab.Enable(false);
	//EditFileNameGrab.Enable(false);
	//EditTime.Enable(false);
	//EditFrameRate.Enable(false);
	//OpGrabMouse.Enable(false);
	
	String extension = "jpg";
	EditFileNameSnap <<= AppendFileName(GetDesktopFolder(), "ScreenSnap." + extension);
	ButSnap.WhenPush = THISBACK(ButSnap_Push);
}

void ScreenGrabTab::SwGrabMode_Action() {
	switch(int(~SwGrabMode)) {
	case 0:
		EditLeft.Enable(false);
		EditTop.Enable(false);
		EditWidth.Enable(false);
		EditHeight.Enable(false);
		EditWindowTitle.Enable(false);
		break;
	case 1:
		EditLeft.Enable(false);
		EditTop.Enable(false);
		EditWidth.Enable(false);
		EditHeight.Enable(false);
		EditWindowTitle.Enable(true);
		break;
	case 2:
		EditLeft.Enable(true);
		EditTop.Enable(true);
		EditWidth.Enable(true);
		EditHeight.Enable(true);
		EditWindowTitle.Enable(false);
		break;
	}
}

void ScreenGrabTab::ButGrab_Push() {
#if defined(PLATFORM_WIN32) 
	FileDelete(EditFileNameGrab.GetData().ToString());
	
	bool ret;
	if (~SwGrabMode == 0) 
		ret = Record_Desktop(~EditFileNameGrab, EditTime, EditFrameRate, OpGrabMouse);
	else if (~SwGrabMode == 1) 
		ret = Record_Window(~EditFileNameGrab, EditTime, GetWindowIdFromCaption(~EditWindowTitle, false), EditFrameRate, OpGrabMouse);
	else if (~SwGrabMode == 2) 
		ret = Record_DesktopRectangle(~EditFileNameGrab, EditTime, EditLeft, EditTop, EditWidth, EditHeight, EditFrameRate, OpGrabMouse);
	else
		throw Exc("Unexpected value");
	if (!ret)
		Exclamation("Error on grabbing");
#endif
}

void MouseKeyboard::Fill() { 
	OnTimer();
#ifndef flagNO_XTEST
	capsLock.WhenAction = THISBACK(OnButLock);
	numLock.WhenAction = THISBACK(OnButLock);
	scrollLock.WhenAction = THISBACK(OnButLock);
	butKey.WhenAction = THISBACK(OnButKey);
	butMouse.WhenAction = THISBACK(OnButMouse);
#else
	capsLock.Enable(false);
	numLock.Enable(false);
	scrollLock.Enable(false);
	butKey.Enable(false);
	butMouse.Enable(false);
#endif
	editAccents <<= "Århus Ørsted Ñandú\ncrème brûlée";
	editAccents.SetFont(Courier(12));
	OnRemoveAccents();
	butRemoveAccents.WhenAction = THISBACK(OnRemoveAccents);
}

void MouseKeyboard::OnTimer() {
	bool caps, num, scroll;
	
	GetKeyLockStatus(caps, num, scroll);
	capsLock <<= caps;
	numLock <<= num;
	scrollLock <<= scroll;
}

void MouseKeyboard::OnRemoveAccents() {
	labNoAccents = RemoveAccents(~editAccents);
}

#ifndef flagNO_XTEST

void MouseKeyboard::OnButLock() {
	SetKeyLockStatus(~capsLock, ~numLock, ~scrollLock);	
}

void MouseKeyboard::OnButKey() {
	String fileTest = AppendFileName(GetDesktopFolder(), "test.txt");
	SaveFile(fileTest, "Please wait a second...");
	LaunchFile(fileTest);
	{
		TimeStop t;
		uint64 windowId;
		while(INT64_MAX == (windowId = GetWindowIdFromCaption("test.txt", false))) {
			if (t.Elapsed() > 10000)
				break;
		}
		if (windowId != -1) {
			Sleep(1000);
			Mouse_SetPos(200, 200, windowId);			
			Mouse_LeftClick();
    		Keyb_SendKeys("{HOME}This text is added by Keyb_SendKeys.\n");
    		Keyb_SendKeys("Some chars just for test: \\/:;,.ºª^[]{}´?¿~#@!¡\n");
    		Keyb_SendKeys("These are accented: äáéíóúñÄÁÉÍÓÚÑ\n");
    		//Keyb_SendKeys("And these are not latin at all: 雅言 한국말 韓國말 조선어 朝鮮語\n");
    		Keyb_SendKeys("And these are not latin at all: 韓于页\n");
		}
	}
}

void MouseKeyboard::OnButMouse()
{
	uint64 wnd = GetWindowIdFromCaption("SysInfo", true);
	
	if (wnd == -1) {
		Exclamation("Window not found");
		return;
	}
	int left, top, right, bottom;
	Window_GetRect(wnd, left, top, right, bottom);
#ifdef PLATFORM_POSIX
	top -= 10;	
#endif
	Mouse_SetPos(left+100, top+5, 0);
	Sleep(100);
	Mouse_LeftDown();
	Sleep(100);
	Mouse_SetPos(left+100, top+200, 0);
	Sleep(100);
	Mouse_LeftUp();
	Ctrl::ProcessEvents();
	Sleep(300);
	Mouse_LeftDown();
	Sleep(100);
	Mouse_SetPos(left+300, top+200, 0);
	Sleep(100);
	Mouse_LeftUp();
	Ctrl::ProcessEvents();
	Sleep(300);
	Mouse_LeftDown();
	Sleep(100);
	Mouse_SetPos(left+100, top+5, 0);
	Sleep(100);
	Mouse_LeftUp();
}

#endif

void ScreenGrabTab::ButSnap_Push()
{
	FileDelete(EditFileNameSnap.GetData().ToString());

	if (~SwGrabMode == 0) 
		Snap_Desktop(~EditFileNameSnap);
	else if (~SwGrabMode == 1) {
		int64 wId = GetWindowIdFromCaption(~EditWindowTitle, true);
		if (wId == -1)
			Exclamation("No window found named " + String(~EditWindowTitle));
		else 
			Snap_Window(~EditFileNameSnap, wId);
	} else if (~SwGrabMode == 2) 
		Snap_DesktopRectangle(~EditFileNameSnap, ~EditLeft, ~EditTop, ~EditWidth, ~EditHeight);
	else
		throw Exc("Unexpected value");
}

SysInfoDemo::SysInfoDemo()
{
	Title("SysInfo");
	Icon(Images::Computer());
	LargeIcon(Images::Computer());
	
	systemInfo.Fill();
	filesTab.Add(systemInfo.SizePos(), "System Info");	
	windowsList.Fill();
	filesTab.Add(windowsList.SizePos(), "Windows list");	
	processList.Fill();
	filesTab.Add(processList.SizePos(), "Process list");	
	specialFolders.Fill();
	filesTab.Add(specialFolders.SizePos(), "Special Folders/Executables/Installed Software");	
	screenGrab.Fill();
	filesTab.Add(screenGrab.SizePos(), "Screen Grab");	
	mouseKeyboard.Fill();
	filesTab.Add(mouseKeyboard.SizePos(), "Mouse & Keyboard");	
	
	AddFrame(menu);
	AddFrame(TopSeparatorFrame());
	AddFrame(info);
	Add(filesTab.SizePos());
	
	menu.Set(THISBACK(MainMenu));
	menu.WhenHelp = info;

	timeCallback.Set(-500, THISBACK(TimerFun));

	Sizeable().Zoomable();
}

void SysInfoDemo::TimerFun() {
	mouseKeyboard.OnTimer();
}

void SysInfoDemo::MainMenu(Bar& bar) {
	menu.Add("Menu", THISBACK(FileMenu));
}

void SysInfoDemo::FileMenu(Bar& bar)
{
	bar.Add("About...", THISBACK(About)).Help("SysInfo demo gui: SysInfo package demo");
	bar.Separator();
	bar.Add("Exit", THISBACK(Exit)).Help("Leave the program");
}

void SysInfoDemo::About()
{
	Prompt("SysInfo demo gui", Images::Computer(), DeQtf("SysInfo package demo"), "Close");
}

void SysInfoDemo::Exit() 
{
	Break();
}
