#include <CtrlLib/CtrlLib.h>

#include "SysInfo/SysInfo.h"
#include "SysInfo_demo_gui.h"

#define IMAGEFILE <SysInfo_demo_gui/SysInfo_demo_gui.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>
#define LAYOUTFILE <SysInfo_demo_gui/SysInfo_demo_gui.lay>

using namespace Upp;

	
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

	String nname, domain;
	if (GetNetworkInfo(nname, domain)) {
		Textnname = nname;
		TextDomain = domain;
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
	int compilerVersion;
	GetCompilerInfo(compiler, compilerVersion, tim, mode);
	TextCompiler = compiler;
	TextCompilerVersion = FormatInt(compilerVersion);
	TextCompilationDate = Format(tim);	
	TextCompilationMode = mode;
	
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
	Array<String> drives;
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
	Array <NetAdapter> adapters = GetAdapterInfo();	
	for (int i = 0; i < adapters.GetCount(); ++i) 
		Adapters.Add(adapters[i].type, adapters[i].description, adapters[i].fullname, 
				   adapters[i].mac);
	ButUpdate.WhenPush = THISBACK(ButUpdate_Push);
}

void SystemInfo::ButUpdate_Push() {
	Fill();
}

void WindowsList_::Fill() {
	Windows.Reset();
	Windows.AddColumn("Title", 30);
	Windows.AddColumn("Window handle", 6);
	Windows.AddColumn("Process Id", 6);
	Windows.AddColumn("Name", 20);
	Windows.AddColumn("File name", 30);
	Array<long> widL, pidL;
	Array<String> name, fileName, caption;
	GetWindowsList(widL, pidL, name, fileName, caption);
	for (int i = 0; i < widL.GetCount(); ++i) 
		Windows.Add(caption[i], FormatLong(widL[i]), FormatLong(pidL[i]), name[i], fileName[i]);
	ButUpdate.WhenPush = THISBACK(ButUpdate_Push);
}

void WindowsList_::ButUpdate_Push() {
	Fill();
}

void ProcessList::Fill() {
	Process.Reset();
	Process.AddColumn("Id", 6);
	Process.AddColumn("Priority", 6);
	Process.AddColumn("Program", 12);
	Array<long> pidL;
	pidL.Clear(); 
	Array<String> pNames;
	if (!GetProcessList(pidL, pNames))
		Process.Add("Error getting process info");
	else {
		for (int i = 0; i < pidL.GetCount(); ++i) {
			int priority = GetProcessPriority(pidL[i]);
			Process.Add(FormatLong(pidL[i]), priority >= 0? FormatInt(priority): "Not accesible", pNames[i]);
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
#if defined(PLATFORM_WIN32) 
	SwGrabMode.Add("Rectangle");
#endif
	SwGrabMode.MinCaseHeight(20);
	SwGrabMode.SetData(0);
	SwGrabMode.WhenAction = THISBACK(SwGrabMode_Action);
	SwGrabMode_Action();
#if defined(PLATFORM_WIN32) 	
	ButGrab.WhenPush = THISBACK(ButGrab_Push);
#else
	ButGrab.Enable(false);
	EditFileNameGrab.Enable(false);
	EditTime.Enable(false);
	EditFrameRate.Enable(false);
	OpGrabMouse.Enable(false);
	EditLeft.Hide();
	EditTop.Hide();
	EditWidth.Hide();
	EditHeight.Hide();
	Left.Hide();
	Top.Hide();
	Width.Hide();
	Height.Hide();
#endif
	
	String extension;
#if defined(PLATFORM_WIN32) 
	extension = "bmp";
#else
	extension = "xwd";
#endif
	EditFileNameSnap <<= AppendFileName(GetDesktopFolder(), "ScreenSnap." + extension);
	ButSnap.WhenPush = THISBACK(ButSnap_Push);
}

void ScreenGrabTab::SwGrabMode_Action() {
	switch((int)SwGrabMode.GetData()) {
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
	if (SwGrabMode.GetData() == 0) 
		ret = Record_Desktop(EditFileNameGrab, EditTime, EditFrameRate, OpGrabMouse);
	else if (SwGrabMode.GetData() == 1) 
		ret = Record_Window(EditFileNameGrab, EditTime, GetWindowIdFromCaption(EditWindowTitle, false), EditFrameRate, OpGrabMouse);
	else if (SwGrabMode.GetData() == 2) 
		ret = Record_DesktopRectangle(EditFileNameGrab, EditTime, EditLeft, EditTop, EditWidth, EditHeight, EditFrameRate, OpGrabMouse);
	else
		throw Exc("Unexpected value");
	if (!ret)
		Exclamation("Error on grabbing");
#endif
}

void MouseKeyboard::Fill() {
	OnTimer();
	capsLock.WhenAction = THISBACK(OnButLock);
	numLock.WhenAction = THISBACK(OnButLock);
	scrollLock.WhenAction = THISBACK(OnButLock);
	butKey.WhenAction = THISBACK(OnButKey);
	butMouse.WhenAction = THISBACK(OnButMouse);
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

void MouseKeyboard::OnButLock() {
	SetKeyLockStatus(~capsLock, ~numLock, ~scrollLock);	
}

void MouseKeyboard::OnButKey() {
	String fileTest = AppendFileName(GetDesktopFolder(), "test.txt");
	SaveFile(fileTest, "Please wait a second...");
	LaunchFile(fileTest);
	{
		TimeStop t;
		long windowId;
		while(-1 == (windowId = GetWindowIdFromCaption("test.txt", false))) {
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
	long wnd = GetWindowIdFromCaption("SysInfo", true);
	
	if (wnd == -1) {
		Exclamation("Window not found");
		return;
	}
	long left, top, right, bottom;
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

void ScreenGrabTab::ButSnap_Push()
{
	FileDelete(EditFileNameSnap.GetData().ToString());

	if (SwGrabMode.GetData() == 0) 
		Snap_Desktop(EditFileNameSnap);
	else if (SwGrabMode.GetData() == 1) 
		Snap_Window(EditFileNameSnap, GetWindowIdFromCaption(EditWindowTitle, false));
	else if (SwGrabMode.GetData() == 2) 
		Snap_DesktopRectangle(EditFileNameSnap, EditLeft, EditTop, EditWidth, EditHeight);
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

	SetTimeCallback(-500, THISBACK(TimerFun));

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
