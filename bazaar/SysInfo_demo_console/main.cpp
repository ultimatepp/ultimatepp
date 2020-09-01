#include <Core/Core.h>
#include <SysInfo/SysInfo.h>


using namespace Upp;


void Test()
{
	UppLog() << "\n" << "SysInfo functions demo";
	
	UppLog() << "\n" << "\nSpecial folders";
	UppLog() << "\n" << Format("Desktop:          %s", GetDesktopFolder());
	UppLog() << "\n" << Format("Programs:         %s", GetProgramsFolder());
	UppLog() << "\n" << Format("Application Data: %s", GetAppDataFolder());
	UppLog() << "\n" << Format("Music:            %s", GetMusicFolder());
	UppLog() << "\n" << Format("Pictures:         %s", GetPicturesFolder());
	UppLog() << "\n" << Format("Video:            %s", GetVideoFolder());
	UppLog() << "\n" << Format("Personal:         %s", GetPersonalFolder());
	UppLog() << "\n" << Format("Templates:        %s", GetTemplatesFolder());
	UppLog() << "\n" << Format("Download:         %s", GetDownloadFolder());
//	UppLog() << "\n" << Format("Root:             %s", GetRootFolder());
	UppLog() << "\n" << Format("Temp:             %s", GetTempFolder());
	UppLog() << "\n" << Format("Os:               %s", GetOsFolder());
	UppLog() << "\n" << Format("System:           %s", GetSystemFolder());
	
	UppLog() << "\n" << "\nNetwork info";
	String nname, domain, ip4, ip6;
	VERIFY(GetNetworkInfo(nname, domain, ip4, ip6));
	UppLog() << "\n" << Format("Name '%s', Domain '%s', IP4 '%s', IP6 '%s'", nname, domain, ip4, ip6);
	
	UppLog() << "\n" << "\nNetwork adapter info";
	Array <NetAdapter> netAdapters = GetAdapterInfo();
	for (int i = 0; i < netAdapters.GetCount(); ++i) 
		UppLog() << "\n" << Format("- %s %s %s\n  %s\n  IP4: %s IP6: %s", netAdapters[i].type, 
						netAdapters[i].mac, netAdapters[i].fullname, 
						netAdapters[i].description, netAdapters[i].ip4, netAdapters[i].ip6);
	
	UppLog() << "\n" << Format("\nFirst free available socket port from 49152 is %d", GetAvailableSocketPort(49152));
	
	UppLog() << "\n" << "\nSystem info:";		
	String manufacturer, productName, version, mbSerial;
	Date releaseDate;
	int numberOfProcessors;
	GetSystemInfo(manufacturer, productName, version, numberOfProcessors, mbSerial);
	UppLog() << "\n" << Format("System manufacturer '%s', product name '%s',\n version '%s', number of processors: %d"
		", motherboard serial: '%s'", manufacturer, productName, version, numberOfProcessors, mbSerial);
 	
 	UppLog() << "\n" << Format("Real CPU Speed: %.3f GHz", GetCpuSpeed()/1000.);
 	
 	UppLog() << "\n" << "\nBattery info";
 	bool present;
 	//int designCapacity,lastFullCapacity;
	//String batVendor, type, model, serial;
	if (!GetBatteryInfo(present/*, designCapacity, lastFullCapacity, batVendor, type, model, serial*/))
		UppLog() << "\n" << "There is no battery";
	else {
		//Puts(Format("Vendor: %s, Type: %s, Model: %s, Serial: %s", batVendor, type, model, serial));
		//Puts(Format("Design capacity: %d mAh, Last full capacity: %d mAh", designCapacity, lastFullCapacity));
		if (!present)
			UppLog() << "\n" << "Battery not installed";
		else {
 			bool discharging;
 			int percentage, remainingMin;
 			GetBatteryStatus(discharging, percentage, remainingMin); 
			UppLog() << "\n" << Format("Working with battery: %s, Percentage: %d%%, Remaining: %d min", discharging ? "yes": "no", percentage, remainingMin);
		}
	}
 	String biosVersion, biosSerial;
 	Date biosReleaseDate;
	GetBiosInfo(biosVersion, biosReleaseDate, biosSerial);
	UppLog() << "\n" << Format("\nBios version '%s',\n release date '%s', serial: '%s'", biosVersion, AsString(biosReleaseDate), biosSerial);
	String vendor, identifier, architecture; 
	int speed;
	for (int i = 0; i < numberOfProcessors; ++i) {
		GetProcessorInfo(i, vendor, identifier, architecture, speed);		
		UppLog() << "\n" << Format("\nProcessor #%d: Vendor '%s',\n identifier '%s',\n architecture '%s', speed %d MHz", i, vendor, identifier, architecture, speed);	
	}
	
	UppLog() << "\n" << "\nMemory info:";	
	int memoryLoad;
	uint64 totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual;
	VERIFY(GetMemoryInfo(memoryLoad, totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual));
	UppLog() << "\n" << Format("Percent of memory in use: %d%c", memoryLoad, '%');
	UppLog() << "\n" << Format("Total physical memory:    %s bytes (%s)", Format64(totalPhys), BytesToString(totalPhys));
	UppLog() << "\n" << Format("Free physical memory:     %s bytes (%s)", Format64(freePhys), BytesToString(freePhys));	
	UppLog() << "\n" << Format("Total paging file:        %s bytes (%s)", Format64(totalPageFile), BytesToString(totalPageFile));	
	UppLog() << "\n" << Format("Free paging file:         %s bytes (%s)", Format64(freePageFile), BytesToString(freePageFile));	
	UppLog() << "\n" << Format("Total virtual memory:     %s bytes (%s)", Format64(totalVirtual), BytesToString(totalVirtual));	
	UppLog() << "\n" << Format("Free virtual memory:      %s bytes (%s)", Format64(freeVirtual), BytesToString(freeVirtual));
	
	UppLog() << "\n" << "\nOs info:";
	String kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion;
	VERIFY(GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion)); 	
	UppLog() << "\n" << Format("Kernel:  %s, version: %s,\n architecture: %s", kernel, kerVersion, kerArchitecture);
	UppLog() << "\n" << Format("Distro:  %s, version: %s", distro, distVersion, desktop, deskVersion);
	UppLog() << "\n" << Format("Desktop: %s, version: %s", desktop, deskVersion);
	
	String compiler, mode;
	Time tim;
	int compilerVersion, bits;
	GetCompilerInfo(compiler, compilerVersion, tim, mode, bits);
	UppLog() << "\n" << Format("\nProgram compiled with %s version %d. Compilation date: %s. Mode: %s. Bits: %d", compiler, compilerVersion, Format(tim), mode, bits);
	
	UppLog() << "\n" << "\nDefault exes info:";
	const char *ext[] = {".html", ".doc", ".png", ".pdf", ".txt", ".xyz", ""};
	for (int i = 0; *ext[i] != 0; ++i) 
		UppLog() << "\n" << Format("Default program for '%s' is '%s'", ext[i], GetExtExecutable(ext[i]));
	
	UppLog() << "\n" << "\nDrives list:";
	Vector<String> drives;
	drives = GetDriveList();
	for (int i = 0; i < drives.GetCount(); ++i) {
		UppLog() << "\n" << Format("Drive path:'%s'", drives[i]);
		String type, volume, fileSystem;
		//uint64 serial;
		int maxName;
		bool mounted = GetDriveInformation(drives[i], type, volume, /*serial,*/ maxName, fileSystem);
		if (mounted) {
			UppLog() << "\n" << Format(" Type: '%s', Volume: '%s', "/*Serial: %0n,*/"\n MaxName: %d, File System: %s", type, volume, /*serial, */maxName, fileSystem);
			uint64 freeBytesUser, totalBytesUser, totalFreeBytes;
			if(!GetDriveSpace(drives[i], freeBytesUser, totalBytesUser, totalFreeBytes))
				UppLog() << "\n" << " Mounted but no access to drive";
			else {
				UppLog() << "\n" << Format(" Free Bytes User:  %s (%s)", Format64(freeBytesUser), BytesToString(freeBytesUser));
				UppLog() << "\n" << Format(" Total Bytes User: %s (%s), Total Free Bytes: %s (%s)", Format64(totalBytesUser), BytesToString(totalBytesUser), Format64(totalFreeBytes), BytesToString(totalFreeBytes));
			}
		} else
			UppLog() << "\n" << " Not mounted";
	}
	UppLog() << "\n\nOther Info:";
	int64 id = GetProcessId();
	UppLog() << "\n" << Sprintf("Process Id:          %ld", id);
	UppLog() << "\n" << Format("Process name:        '%s'", GetProcessName(id));
	UppLog() << "\n" << Format("Process file name:   '%s'", GetProcessFileName(id));
	int priority = GetProcessPriority(id);
	UppLog() << "\n" << Format("Process priority is: %s", priority >= 0? AsString(priority): "Not accesible");
	UppLog() << "\n" << Format("Now changed to high priority: %s", SetProcessPriority(id, 8)? "Yes": "No");
	priority = GetProcessPriority(id);
	UppLog() << "\n" << Format("Process priority is: %s", priority >= 0? AsString(priority): "Not accesible");

	Snap_Desktop(GetExeDirFile("Desktop")); 
	UppLog() << "\n" << "Screenshot saved in " + GetExeDirFile("Desktop");
	
	#ifdef flagDEBUG
	UppLog() << "\n" << "\nLaunch file 'test.txt':";
	String fileTest = GetExeDirFile("test.txt");
	String strTest = "\nIf modify 'test.txt' it will ask you to save or not the file" 
					 "\nIf you answer Yes or No the program will be terminated" 
					 "\nIf you answer Cancel or wait more than 2 seconds the program will be killed" 
					 "\nClick enter in the command window to continue";
	SaveFile(fileTest, "This is a test" + strTest);
	UppLog() << "\n" << strTest;
	
	LaunchFile(fileTest);
	{
		TimeStop t;
		int64 windowId;
		while(-1 == (windowId = GetWindowIdFromCaption("test.txt", false))) {
			if (t.Seconds() > 30)
				break;
		}
		if (windowId != -1) {
			Sleep(1000);
			int left, top, right, bottom;
			Window_GetRect(windowId, left, top, right, bottom);	
			UppLog() << "\n" << Format("Editor window is located at %d, %d, %d, %d", (int)left, (int)top, (int)right, (int)bottom);
			UppLog() << "\n" << "Editor window id is " + Format64(windowId); 
			            
			int x, y;
			Mouse_GetPos(x, y);
			UppLog() << "\n" << Format("Mouse pos is %d, %d", (int)x, (int)y);
			Mouse_SetPos(200, 200, windowId);

#if defined(PLATFORM_WIN32) || !defined(flagNO_XTEST)	
			Mouse_LeftClick();
    		Keyb_SendKeys("{HOME}This text is added by Keyb_SendKeys.\n");
    		Keyb_SendKeys("And the window resized and moved by Window_SetRect.\n", 0, 0);
    		Keyb_SendKeys("And a window capture in c:\\Windowgrab.bmp.\n", 0, 0);
    		Keyb_SendKeys("Some chars just for test: \\/:;,.ºª^[]{}´?¿~#@!¡\n", 0, 0);  		
#endif
#if defined(PLATFORM_WIN32)			
    		Window_SetRect(windowId, 10, 10, 800, 400);
#endif    	
    		Window_SaveCapture(windowId, GetExeDirFile("Windowgrab"));
		}
	}

	int64 processId;
	TimeStop t;
	while(-1 == (processId = GetProcessIdFromWindowCaption("test.txt", false))) {
		if (t.Elapsed() > 10000)
			break;
	}
	if (processId == -1) 
		UppLog() << "\n" << "Window was closed before!";
	else {
		UppLog() << "\n" << "Ending process in max 2 seconds";
		VERIFY(ProcessTerminate(processId, 2000));
	}
	#endif
	
	UppLog() << "\n" << "\nWindows list:";
	Array<int64> widL, pidL;
	Array<String> name, fileName, caption;
	GetWindowsList(widL, pidL, name, fileName, caption);
	for (int i = 0; i < widL.GetCount(); ++i) {
		UppLog() << "\n" << Format("Window hwnd: %6.f, processId: %6.f, Name: %s", (double)widL[i], (double)pidL[i], name[i]);
		UppLog() << "\n" << Format(" File name: %s", fileName[i]);
		UppLog() << "\n" << Format(" Window caption: '%s'", caption[i]);
	}
	
	UppLog() << "\n" << "\nProcess list:";
	pidL.Clear(); 
	Array<String> pNames;
	VERIFY(GetProcessList(pidL, pNames));	
	for (int i = 0; i < pidL.GetCount(); ++i) {
		int priority = GetProcessPriority(pidL[i]);
		UppLog() << "\n" << Format("Id %6.f: Priority: %s, Program: %s", (double)pidL[i], priority >= 0? AsString(priority): "Not accesible", pNames[i]);
	}
} 


CONSOLE_APP_MAIN
{	
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	UppLog() << "\n" << "\nSysInfo: Basic system identification:";
	String kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion;
	VERIFY(GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion));
	UppLog() << "\n" << Format("Kernel:  %s, version: %s,\narchitecture: %s", kernel, kerVersion, kerArchitecture);
	UppLog() << "\n" << Format("Distro:  %s, version: %s", distro, distVersion, desktop, deskVersion);
	UppLog() << "\n" << Format("Desktop: %s, version: %s", desktop, deskVersion);
	
    Test();
    
	#ifdef flagDEBUG
	UppLog() << "\n" << "Introduce enter to continue, or (l) to log off, (r) to reboot or (s) to shutdown\n";
	char str[50];
	fgets(str, 49, stdin);
	if (*str == 'l')
		Shutdown("logoff");
	else  if (*str == 'r')
		Shutdown("reboot");
	else if (*str == 's')
		Shutdown("shutdown");
	#endif
		
	#ifdef flagDEBUG
	UppLog() << "\n";
	Cout() << "\nPress enter key to end";
	ReadStdIn();
	#endif  
}
