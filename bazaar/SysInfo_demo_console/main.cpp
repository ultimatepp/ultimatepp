#include <Core/Core.h>
#include <SysInfo/SysInfo.h>


using namespace Upp;

static bool end = false;

void TestGetchar()
{
	if (end)
		getchar();
}

void Puts(String s)
{
	puts(s);
	String file = AppendFileName(GetDesktopFolder(), "log");
	SaveFile(file, LoadFile(file) + "\n" + s);
}

void Test()
{
	Puts("SysInfo functions demo");
	
	Puts("\nSpecial folders");
	Puts(Format("Desktop:          %s", GetDesktopFolder()));
	Puts(Format("Programs:         %s", GetProgramsFolder()));
	Puts(Format("Application Data: %s", GetAppDataFolder()));
	Puts(Format("Music:            %s", GetMusicFolder()));
	Puts(Format("Pictures:         %s", GetPicturesFolder()));
	Puts(Format("Video:            %s", GetVideoFolder()));
	Puts(Format("Personal:         %s", GetPersonalFolder()));
	Puts(Format("Templates:        %s", GetTemplatesFolder()));
	Puts(Format("Download:         %s", GetDownloadFolder()));
//	Puts(Format("Root:             %s", GetRootFolder()));
	Puts(Format("Temp:             %s", GetTempFolder()));
	Puts(Format("Os:               %s", GetOsFolder()));
	Puts(Format("System:           %s", GetSystemFolder()));
	
	Puts("\nNetwork info");
	String nname, domain, ip4, ip6;
	if (GetNetworkInfo(nname, domain, ip4, ip6))
		Puts(Format("Name '%s', Domain '%s', IP4 '%s', IP6 '%s'", nname, domain, ip4, ip6));
	else
		Puts("Problem reading newtwork data");
	
	Puts("\nNetwork adapter info");
	Array <NetAdapter> netAdapters = GetAdapterInfo();
	for (int i = 0; i < netAdapters.GetCount(); ++i) 
		Puts(Format("- %s %s %s\n  %s\n  IP4: %s IP6: %s", netAdapters[i].type, 
						netAdapters[i].mac, netAdapters[i].fullname, 
						netAdapters[i].description, netAdapters[i].ip4, netAdapters[i].ip6));
	
	Puts(Format("\nFirst free available socket port from 49152 is %d", GetAvailableSocketPort(49152)));
	
	Puts("\nSystem info:");		
	String manufacturer, productName, version, mbSerial;
	Date releaseDate;
	int numberOfProcessors;
	GetSystemInfo(manufacturer, productName, version, numberOfProcessors, mbSerial);
	Puts(Format("System manufacturer '%s', product name '%s',\n version '%s', number of processors: %d"
		", motherboard serial: '%s'", manufacturer, productName, version, numberOfProcessors, mbSerial));
 	
 	Puts(Format("Real CPU Speed: %.3f GHz", GetCpuSpeed()/1000.));
 	
 	Puts("\nBattery info");
 	bool present;
 	//int designCapacity,lastFullCapacity;
	//String batVendor, type, model, serial;
	if (!GetBatteryInfo(present/*, designCapacity, lastFullCapacity, batVendor, type, model, serial*/))
		Puts("There is no battery");
	else {
		//Puts(Format("Vendor: %s, Type: %s, Model: %s, Serial: %s", batVendor, type, model, serial));
		//Puts(Format("Design capacity: %d mAh, Last full capacity: %d mAh", designCapacity, lastFullCapacity));
		if (!present)
			Puts("Battery not installed");
		else {
 			bool discharging;
 			int percentage, remainingMin;
 			GetBatteryStatus(discharging, percentage, remainingMin); 
			Puts(Format("Working with battery: %s, Percentage: %d%%, Remaining: %d min", discharging ? "yes": "no", percentage, remainingMin));
		}
	}
 	String biosVersion, biosSerial;
 	Date biosReleaseDate;
	GetBiosInfo(biosVersion, biosReleaseDate, biosSerial);
	Puts(Format("\nBios version '%s',\n release date '%s', serial: '%s'", biosVersion, AsString(biosReleaseDate), biosSerial));
	String vendor, identifier, architecture; 
	int speed;
	for (int i = 0; i < numberOfProcessors; ++i) {
		GetProcessorInfo(i, vendor, identifier, architecture, speed);		
		Puts(Format("\nProcessor #%d: Vendor '%s',\n identifier '%s',\n architecture '%s', speed %d MHz", i, vendor, identifier, architecture, speed));	
	}
	Puts("\nPress enter to continue...");	TestGetchar();
	
	Puts("\nMemory info:");	
	int memoryLoad;
	uint64 totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual;
	if (!GetMemoryInfo(memoryLoad, totalPhys, freePhys, totalPageFile, freePageFile, totalVirtual, freeVirtual))
		Puts("Impossible to get memory information");
	else {
		Puts(Format("Percent of memory in use: %d%c", memoryLoad, '%'));
		Puts(Format("Total physical memory:    %s bytes (%s)", Format64(totalPhys), BytesToString(totalPhys)));
		Puts(Format("Free physical memory:     %s bytes (%s)", Format64(freePhys), BytesToString(freePhys)));	
		Puts(Format("Total paging file:        %s bytes (%s)", Format64(totalPageFile), BytesToString(totalPageFile)));	
		Puts(Format("Free paging file:         %s bytes (%s)", Format64(freePageFile), BytesToString(freePageFile)));	
		Puts(Format("Total virtual memory:     %s bytes (%s)", Format64(totalVirtual), BytesToString(totalVirtual)));	
		Puts(Format("Free virtual memory:      %s bytes (%s)", Format64(freeVirtual), BytesToString(freeVirtual)));
	}
	Puts("\nOs info:");
	String kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion;
	if (!GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion))
		Puts("Error getting Os info");
	else { 	
		Puts(Format("Kernel:  %s, version: %s,\n architecture: %s", kernel, kerVersion, kerArchitecture));
		Puts(Format("Distro:  %s, version: %s", distro, distVersion, desktop, deskVersion));
		Puts(Format("Desktop: %s, version: %s", desktop, deskVersion));
	}
	String compiler, mode;
	Time tim;
	int compilerVersion, bits;
	GetCompilerInfo(compiler, compilerVersion, tim, mode, bits);
	Puts(Format("\nProgram compiled with %s version %d. Compilation date: %s. Mode: %s. Bits: %d", compiler, compilerVersion, Format(tim), mode, bits));
	
	Puts("\nDefault exes info:");
	const char *ext[] = {".html", ".doc", ".png", ".pdf", ".txt", ".xyz", ""};
	for (int i = 0; *ext[i] != 0; ++i) 
		Puts(Format("Default program for '%s' is '%s'", ext[i], GetExtExecutable(ext[i])));
	
	Puts("\nPress enter to continue...");	TestGetchar();
	
	Puts("\nDrives list:");
	Array<String> drives;
	drives = GetDriveList();
	for (int i = 0; i < drives.GetCount(); ++i) {
		Puts(Format("Drive path:'%s'", drives[i]));
		String type, volume, fileSystem;
		//uint64 serial;
		int maxName;
		bool mounted = GetDriveInformation(drives[i], type, volume, /*serial,*/ maxName, fileSystem);
		if (mounted) {
			Puts(Format(" Type: '%s', Volume: '%s', "/*Serial: %0n,*/"\n MaxName: %d, File System: %s", type, volume, /*serial, */maxName, fileSystem));
			uint64 freeBytesUser, totalBytesUser, totalFreeBytes;
			if(!GetDriveSpace(drives[i], freeBytesUser, totalBytesUser, totalFreeBytes))
				Puts(" Mounted but no access to drive");
			else {
				Puts(Format(" Free Bytes User:  %s (%s)", Format64(freeBytesUser), BytesToString(freeBytesUser)));
				Puts(Format(" Total Bytes User: %s (%s), Total Free Bytes: %s (%s)", Format64(totalBytesUser), BytesToString(totalBytesUser), Format64(totalFreeBytes), BytesToString(totalFreeBytes)));
			}
		} else
			Puts (" Not mounted");
	}
	Puts("\nOther Info:");
	int64 id = GetProcessId();
	Puts(Sprintf("Process Id:          %ld", id));
	Puts(Format("Process name:        '%s'", GetProcessName(id)));
	Puts(Format("Process file name:   '%s'", GetProcessFileName(id)));
	int priority = GetProcessPriority(id);
	Puts(Format("Process priority is: %s", priority >= 0? AsString(priority): "Not accesible"));
	Puts(Format("Now changed to high priority: %s", SetProcessPriority(id, 8)? "Yes": "No"));
	priority = GetProcessPriority(id);
	Puts(Format("Process priority is: %s", priority >= 0? AsString(priority): "Not accesible"));

	Window_SaveCapture(0, AppendFileName(GetDesktopFolder(), "Desktop")); 
	Puts ("Screenshot saved in " + AppendFileName(GetDesktopFolder(), "Desktop"));
	
	Puts("\nLaunch file 'test.txt':");
	String fileTest = AppendFileName(GetTempFolder(), "test.txt");
	String strTest = "\nIf modify 'test.txt' it will ask you to save or not the file" 
					 "\nIf you answer Yes or No the program will be terminated" 
					 "\nIf you answer Cancel or wait more than 2 seconds the program will be killed" 
					 "\nClick enter in the command window to continue";
	SaveFile(fileTest, "This is a test" + strTest);
	Puts(strTest);
	
	LaunchFile(fileTest);
	{
		TimeStop t;
		int64 windowId;
		while(-1 == (windowId = GetWindowIdFromCaption("test.txt", false))) {
			if (t.Elapsed() > 10000)
				break;
		}
		if (windowId != -1) {
			Sleep(1000);
			long left, top, right, bottom;
			Window_GetRect(windowId, left, top, right, bottom);	
			Puts(Format("Editor window is located at %d, %d, %d, %d", (int)left, (int)top, (int)right, (int)bottom));
			Puts("Editor window id is " + Format64(windowId)); 
			            
			long x, y;
			Mouse_GetPos(x, y);
			Puts(Format("Mouse pos is %d, %d", (int)x, (int)y));
			Mouse_SetPos(200, 200, windowId);
			
			Mouse_LeftClick();
    		Keyb_SendKeys("{HOME}This text is added by Keyb_SendKeys.\n");
    		Keyb_SendKeys("And the window resized and moved by Window_SetRect.\n", 0, 0);
    		Keyb_SendKeys("And a window capture in c:\\Windowgrab.bmp.\n", 0, 0);
    		Keyb_SendKeys("Some chars just for test: \\/:;,.ºª^[]{}´?¿~#@!¡\n", 0, 0);  		
#if defined(PLATFORM_WIN32)			
    		Window_SetRect(windowId, 10, 10, 800, 400);
#endif    	
    		Window_SaveCapture(windowId, AppendFileName(GetDesktopFolder(), "Windowgrab"));
		}
	}
	Puts("\nPress enter to terminate 'test.txt'");	TestGetchar();

	int64 processId;
	TimeStop t;
	while(-1 == (processId = GetProcessIdFromWindowCaption("test.txt", false))) {
		if (t.Elapsed() > 10000)
			break;
	}
	if (processId == -1) 
		Puts("Window was closed before!");
	else {
		Puts("Ending process in max 2 seconds");
		if (!ProcessTerminate(processId, 2000)) 
			Puts("Impossible to end process");
		else
			Puts("Process terminated");
	}
	Puts("\nPress enter to continue...");	TestGetchar();
	
	Puts("\nWindows list:");
	Array<int64> widL, pidL;
	Array<String> name, fileName, caption;
	GetWindowsList(widL, pidL, name, fileName, caption);
	for (int i = 0; i < widL.GetCount(); ++i) {
		Puts(Format("Window hwnd: %6.f, processId: %6.f, Name: %s", (double)widL[i], (double)pidL[i], name[i]));
		Puts(Format(" File name: %s", fileName[i]));
		Puts(Format(" Window caption: '%s'", caption[i]));
		if ((i/20) > (i-1)/20) {
		    Puts("\nPress enter to continue with the next 20 windows...");	TestGetchar();
		}
	}
	Puts("\nPress enter to continue...");	TestGetchar();
	
	Puts("\nProcess list:");
	pidL.Clear(); 
	Array<String> pNames;
	if (!GetProcessList(pidL, pNames))
		Puts("Error getting process info");
	else {	
		for (int i = 0; i < pidL.GetCount(); ++i) {
			int priority = GetProcessPriority(pidL[i]);
			Puts(Format("Id %6.f: Priority: %s, Program: %s", (double)pidL[i], priority >= 0? AsString(priority): "Not accesible", pNames[i]));
		}
	}
	Puts("\nPress enter to end...");	TestGetchar();
} 



CONSOLE_APP_MAIN
{	
	Puts("\nBasic system identification:");
	String kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion;
	if (!GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion))
		Puts("Error getting Os info");
	else { 	
		Puts(Format("Kernel:  %s, version: %s,\narchitecture: %s", kernel, kerVersion, kerArchitecture));
		Puts(Format("Distro:  %s, version: %s", distro, distVersion, desktop, deskVersion));
		Puts(Format("Desktop: %s, version: %s", desktop, deskVersion));
	}
	Puts("\nPress enter to follow with system identification:");
	getchar();
       
	FileDelete(AppendFileName(GetDesktopFolder(), "log"));
	Puts("Introduce enter or (l) to log off, (r) to reboot or (s) to shutdown");
	char str[50];
	fgets(str, 49, stdin);
	if (*str == 'l')
		Shutdown("logoff");
	else  if (*str == 'r')
		Shutdown("reboot");
	else if (*str == 's')
		Shutdown("shutdown");
		
	Puts("Introduce number of test cycles or just type enter to run it once: ");
	fgets(str, 49, stdin);
	int numTests = atoi(str);
	for (int i = 0; i < numTests; ++i)
		Test();
	end = true;
	Test();
}
