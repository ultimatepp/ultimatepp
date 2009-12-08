#include <CtrlLib/CtrlLib.h>

#include "SysInfo/SysInfo.h"
#include "SysInfo_demo_gui.h"

#define IMAGEFILE <SysInfo_demo_gui/SysInfo_demo_gui.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>
#define LAYOUTFILE <SysInfo_demo_gui/SysInfo_demo_gui.lay>

//using namespace Upp;

GUI_APP_MAIN
{
	SysInfoDemo program;
	
	SetLanguage(LNGC_('E','N','U','S', CHARSET_UTF8));
    
	program.Run();
}

void SpecialFolders::Fill()
{
	TextDesktop.SetText(GetDesktopFolder());
	TextPrograms.SetText(GetProgramsFolder());
	TextApplication.SetText(GetAppDataFolder());
	TextMusic.SetText(GetMusicFolder());
	TextPictures.SetText(GetPicturesFolder());
	TextVideo.SetText(GetVideoFolder());
	TextPersonal.SetText(GetPersonalFolder());
	TextTemplates.SetText(GetTemplatesFolder());
	TextDownload.SetText(GetDownloadFolder());
	TextTemp.SetText(GetTempFolder());
	TextOs.SetText(GetOsFolder());
	TextSystem.SetText(GetSystemFolder());
	
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
void SpecialFolders::ButInstalledSoftware_Push()
{
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

void SystemInfo::Fill()
{
	String manufacturer, productName, version;
	Date releaseDate;
	int numberOfProcessors;
	GetSystemInfo(manufacturer, productName, version, numberOfProcessors);
	
	TextManufacturer.SetText(manufacturer);
	TextProductName.SetText(productName);
	TextVersion.SetText(version);
	TextNumber.SetText(AsString(numberOfProcessors));
	TextSpeed.SetText(Format("%.3f GHz", GetCpuSpeed()/1000.));

 	String biosVersion;
 	Date biosReleaseDate;
	GetBiosInfo(biosVersion, biosReleaseDate);
	TextBiosVersion.SetText(biosVersion);
	TextBiosRelease.SetText(AsString(biosReleaseDate));
	
	String kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion;
	if (GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion)) {
		TextKernel.SetText(kernel);
		TextKerVersion.SetText(kerVersion);
		TextKerArchitecture.SetText(kerArchitecture);
		TextDistro.SetText(distro);
		TextDistVersion.SetText(distVersion);
		TextDesktop.SetText(desktop);
		TextDeskVersion.SetText(deskVersion);
	}

	int memoryLoad;
	uint64 totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual;
	if (GetMemoryInfo(memoryLoad, totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual)) {
		TextMemPercentage.SetText(FormatInt(memoryLoad));
		TextMemTotPhysicalbytes.SetText(Format64(totalPhys));
		TextMemTotPhysical.SetText(BytesToString(totalPhys));
		TextMemFreePhysicalbytes.SetText(Format64(freePhys));
		TextMemFreePhysical.SetText(BytesToString(freePhys));			
		TextMemTotPagingbytes.SetText(Format64(totalPageFile));
		TextMemTotPaging.SetText(BytesToString(totalPageFile));
		TextMemFreePagingbytes.SetText(Format64(freePageFile));
		TextMemFreePaging.SetText(BytesToString(freePageFile));
		TextMemTotVirtualbytes.SetText(Format64(totalVirtual));
		TextMemTotVirtual.SetText(BytesToString(totalVirtual));
		TextMemFreeVirtualbytes.SetText(Format64(freeVirtual));
		TextMemFreeVirtual.SetText(BytesToString(freeVirtual));
	}
	String compiler, date;
	int compilerVersion;
	GetCompilerInfo(compiler, compilerVersion, date);
	TextCompiler.SetText(compiler);
	TextCompilerVersion.SetText(FormatInt(compilerVersion));
	TextCompilationDate.SetText(date);	
	
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
	ButUpdate.WhenPush = THISBACK(ButUpdate_Push);
}
void SystemInfo::ButUpdate_Push()
{
	Fill();
}

void WindowsList_::Fill()
{
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
void WindowsList_::ButUpdate_Push()
{
	Fill();
}

void ProcessList::Fill()
{
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
void ProcessList::ButUpdate_Push()
{
	Fill();
}	
	
void ScreenGrabTab::Fill()
{
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

void ScreenGrabTab::SwGrabMode_Action()
{
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

void ScreenGrabTab::ButGrab_Push()
{
#if defined(PLATFORM_WIN32) 
	FileDelete(EditFileNameGrab.GetData().ToString());

	if (SwGrabMode.GetData() == 0) 
		Record_Desktop(EditFileNameGrab, EditTime, EditFrameRate, OpGrabMouse);
	else if (SwGrabMode.GetData() == 1) 
		Record_Window(EditFileNameGrab, EditTime, GetWindowIdFromCaption(EditWindowTitle, false), EditFrameRate, OpGrabMouse);
	else if (SwGrabMode.GetData() == 2) 
		Record_DesktopRectangle(EditFileNameGrab, EditTime, EditLeft, EditTop, EditWidth, EditHeight, EditFrameRate, OpGrabMouse);
	else
		throw Exc("Unexpected value");
#endif
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
	
	AddFrame(menu);
	AddFrame(TopSeparatorFrame());
	AddFrame(info);
	Add(filesTab.SizePos());
	
	menu.Set(THISBACK(MainMenu));
	menu.WhenHelp = info;

	Sizeable().Zoomable();
}

void SysInfoDemo::MainMenu(Bar& bar)
{
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
