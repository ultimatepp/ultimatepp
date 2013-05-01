#include "SysInfo_in.h"

NAMESPACE_UPP

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

#if !defined(PRODUCT_ULTIMATE)
#define PRODUCT_UNDEFINED                       0x00000000
#define PRODUCT_ULTIMATE                        0x00000001
#define PRODUCT_HOME_BASIC                      0x00000002
#define PRODUCT_HOME_PREMIUM                    0x00000003
#define PRODUCT_ENTERPRISE                      0x00000004
#define PRODUCT_HOME_BASIC_N                    0x00000005
#define PRODUCT_BUSINESS                        0x00000006
#define PRODUCT_STANDARD_SERVER                 0x00000007
#define PRODUCT_DATACENTER_SERVER               0x00000008
#define PRODUCT_SMALLBUSINESS_SERVER            0x00000009
#define PRODUCT_ENTERPRISE_SERVER               0x0000000A
#define PRODUCT_STARTER                         0x0000000B
#define PRODUCT_DATACENTER_SERVER_CORE          0x0000000C
#define PRODUCT_STANDARD_SERVER_CORE            0x0000000D
#define PRODUCT_ENTERPRISE_SERVER_CORE          0x0000000E
#define PRODUCT_ENTERPRISE_SERVER_IA64          0x0000000F
#define PRODUCT_BUSINESS_N                      0x00000010
#define PRODUCT_WEB_SERVER                      0x00000011
#define PRODUCT_CLUSTER_SERVER                  0x00000012
#define PRODUCT_HOME_SERVER                     0x00000013
#define PRODUCT_STORAGE_EXPRESS_SERVER          0x00000014
#define PRODUCT_STORAGE_STANDARD_SERVER         0x00000015
#define PRODUCT_STORAGE_WORKGROUP_SERVER        0x00000016
#define PRODUCT_STORAGE_ENTERPRISE_SERVER       0x00000017
#define PRODUCT_SERVER_FOR_SMALLBUSINESS        0x00000018
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM    0x00000019
#define PRODUCT_UNLICENSED                      0xABCDABCD
#endif

#if !defined(SM_SERVERR2)
#define SM_SERVERR2             89
#endif
#if !defined(VER_SUITE_WH_SERVER)
#define VER_SUITE_WH_SERVER		0x00008000
#endif
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);


bool GetOsInfo(String &kernel, String &kerVersion, String &kerArchitecture, String &distro, 
			   String &distVersion, String &desktop, String &deskVersion)
{
   	OSVERSIONINFOEX osvi;
   	SYSTEM_INFO si;
   	PGNSI pGNSI;
   	PGPI pGPI;
   	BOOL bOsVersionInfoEx;

   	ZeroMemory(&si, sizeof(SYSTEM_INFO));
   	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

   	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   	if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi)))
      	return false;

   	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
   	pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
	if(NULL != pGNSI)
   		pGNSI(&si);
   	else 
   		GetSystemInfo(&si);

	kerVersion = Format("%d.%d", (int)osvi.dwMajorVersion, (int)osvi.dwMinorVersion);
	kernel = "Windows";
	switch(si.wProcessorArchitecture) {
	case PROCESSOR_ARCHITECTURE_AMD64:	kerArchitecture = "64 bits";			break;
	case PROCESSOR_ARCHITECTURE_IA64:	kerArchitecture = "Itanium 64 bits";	break;
   	case PROCESSOR_ARCHITECTURE_INTEL:	kerArchitecture = "32 bits";			break;
	default:							kerArchitecture = "Unknown";
	}
   	if (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && osvi.dwMajorVersion > 4 ) {
      	// Test for the specific product.
		if (osvi.dwMajorVersion == 6) {
			if (osvi.dwMinorVersion == 1) {
            	if (osvi.wProductType == VER_NT_WORKSTATION)
                	kernel.Cat(" Seven");
            	else 
					kernel.Cat(" Server 2008 R2");
			} else if (osvi.dwMinorVersion == 0) {
         		if (osvi.wProductType == VER_NT_WORKSTATION)
         			kernel.Cat(" Vista");
         		else
         			kernel.Cat(" Server 2008");
			}
         	pGPI = (PGPI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
         	DWORD dwType;
         	if (pGPI(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType)) {
	         	switch(dwType) {
	         	case PRODUCT_UNLICENSED:				distro = "Unlicensed";			break;
	            case PRODUCT_ULTIMATE:					distro = "Ultimate Edition";	break;
	            case PRODUCT_HOME_PREMIUM:				distro = "Home Premium Edition";break;
	            case PRODUCT_HOME_BASIC:	
	            case PRODUCT_HOME_BASIC_N:				distro = "Home Basic Edition";	break;
	            case PRODUCT_ENTERPRISE:				distro = "Enterprise Edition";	break;
	            case PRODUCT_BUSINESS:		
	            case PRODUCT_BUSINESS_N:				distro = "Business Edition";	break;
	            case PRODUCT_STARTER:					distro = "Starter Edition";		break;
	            case PRODUCT_CLUSTER_SERVER:			distro = "Cluster Server Edition";	break;
	            case PRODUCT_DATACENTER_SERVER:			distro = "Datacenter Edition";		break;
	            case PRODUCT_DATACENTER_SERVER_CORE:	distro = "Datacenter Edition (core installation)";		break;
	            case PRODUCT_ENTERPRISE_SERVER:			distro = "Enterprise Edition";		break;
	            case PRODUCT_ENTERPRISE_SERVER_CORE:	distro = "Enterprise Edition (core installation)";		break;
	            case PRODUCT_ENTERPRISE_SERVER_IA64:	distro = "Enterprise Edition for Itanium-based Systems";break;
	            case PRODUCT_SMALLBUSINESS_SERVER:		distro = "Small Business Server";   break;
	            case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:	distro = "Small Business Server Premium Edition";   break;
	            case PRODUCT_STANDARD_SERVER:				distro = "Standard Edition";    break;
	            case PRODUCT_STANDARD_SERVER_CORE:			distro = "Standard Edition (core installation)";    break;
	            case PRODUCT_WEB_SERVER:				distro = "Web Server Edition";	break;
				case PRODUCT_UNDEFINED:					distro = "Undefined";			break;
	         	}
         	}
      	} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
         	if (GetSystemMetrics(SM_SERVERR2) )
         		kernel.Cat(" Server 2003 R2");
         	else if (osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER)
            	kernel.Cat(" Storage Server 2003");
     		else if (osvi.wSuiteMask & VER_SUITE_WH_SERVER)
            	kernel.Cat(" Home Server");
         	else if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	            kernel.Cat(" XP Professional x64 Edition");
         	else 
         		kernel.Cat(" Server 2003");
         	// Test for the server type.
         	if (osvi.wProductType != VER_NT_WORKSTATION ) {
            	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ) {
                	if(osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   		distro = "Datacenter Edition for Itanium-based Systems";
                	else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   		distro = "Enterprise Edition for Itanium-based Systems";
            	}
			} else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ) {
            	if(osvi.wSuiteMask & VER_SUITE_DATACENTER )
             		distro = "Datacenter x64 Edition";
                else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   	distro = "Enterprise x64 Edition";
                else 
                	distro = "Standard x64 Edition";
            } else {
             	if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
                   	distro = "Compute Cluster Edition";
                else if(osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   	distro = "Datacenter Edition";
                else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   	distro = "Enterprise Edition";
                else if (osvi.wSuiteMask & VER_SUITE_BLADE )
                   	distro = "Web Edition";
                else 
                	distro = "Standard Edition";
            }
    	} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 ) {
         	kernel.Cat(" XP");
         	if(osvi.wSuiteMask & VER_SUITE_PERSONAL )
            	distro = "Home Edition";
         	else 
         		distro = "Professional";
      	} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 ) {
         	kernel.Cat(" 2000");
         	if (osvi.wProductType == VER_NT_WORKSTATION)
            	distro = "Professional";
         	else {
            	if(osvi.wSuiteMask & VER_SUITE_DATACENTER)
               		distro = "Datacenter Server";
            	else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
               		distro = "Advanced Server";
            	else 
            		distro = "Server";
         	}
      	} 
       	// Include service pack (if any) and build number.
      	if(osvi.wServicePackMajor > 0)
			kerVersion.Cat(Format(" %s", osvi.szCSDVersion));

      	kerVersion.Cat(Format(" (Build %d)", (int)osvi.dwBuildNumber));
 	} else if (osvi.dwPlatformId == 1) {
 		switch(osvi.dwMinorVersion) {
 		case 0:
 			kernel.Cat(" 95");
 			break;
 		case 10:
 			kernel.Cat(" 98");
 			break;
 		case 90:
 			kernel.Cat(" Millennium");
 			break;
 		}
 		distro = "";
 	} else if (osvi.dwPlatformId == 2) {
    	switch(osvi.dwMajorVersion) {
    	kernel.Cat(" NT");
        case 3:
        	kernel.Cat(" 3.51");
        	break;
        case 4:
         	kernel.Cat(" 4.0");
         	break;
    	}
    	distro = "";
 	}
 	desktop = kernel;
 	distVersion = deskVersion = "";
   	return true;
}
#endif
#ifdef PLATFORM_POSIX

bool GetOsInfo_CheckLsb(String &distro, String &distVersion) {
	StringParse lsb;			
	lsb = LoadFile_Safe("/etc/lsb-release");		
	if (lsb == "")
		return false;
	if(!lsb.GoAfter("DISTRIB_ID="))
		return false;				
	distro = ToLower(lsb.GetText());
	if (distro == "")
		return false;
	lsb.GoAfter_Init("DISTRIB_RELEASE=");
	distVersion = lsb.GetText();

	return true;
}

bool GetOsInfo(String &kernel, String &kerVersion, String &kerArchitecture, String &distro, String &distVersion, String &desktop, String &deskVersion)
{
	struct utsname buf;
	
	if (0 == uname(&buf)) {
		kernel = buf.sysname;
    	kerVersion = String(buf.release) + " " + String(buf.version);
        kerArchitecture = buf.machine;
	}
	if (kernel == "")
		kernel = LoadFile_Safe("/proc/sys/kernel/ostype");
	if (kernel == "")
		kernel = LoadFile_Safe("/proc/version");
	if (kernel == "") {
		if (Sys("sysctl_cmd -n kern.version").Find("FreeBSD") >= 0)
			kernel = "freebsd";
	}
	if (kerVersion == "")
		kerVersion = LoadFile_Safe("/proc/sys/kernel/osrelease") + " " + LoadFile_Safe("/proc/sys/kernel/version");
	if (kerArchitecture == "")
		kerArchitecture = Sys("uname -m");	// Kernel. See too /proc/version, /proc/version_signature and uname -a looking for architecture
	
	if (kernel == "")
		kernel = kerVersion = kerArchitecture = "UNKNOWN";	
	
	// Desktop
    if(GetEnv("GNOME_DESKTOP_SESSION_ID").GetCount() || GetEnv("GNOME_KEYRING_SOCKET").GetCount()) {
		desktop = "gnome";
		StringParse gnomeVersion = Sys("gnome-about --version");
		gnomeVersion.GoAfter("gnome-about");
		deskVersion = gnomeVersion.GetText();
	} else if(GetEnv("KDE_FULL_SESSION").GetCount() || GetEnv("KDEDIR").GetCount() || GetEnv("KDE_MULTIHEAD").GetCount()) {
        desktop = "kde"; 
        StringParse konsole = Sys("konsole --version");
        konsole.GoAfter("KDE:");
        deskVersion = konsole.GetText("\r\n");						
		if (deskVersion == "")		
			deskVersion = GetEnv("KDE_SESSION_VERSION");        
	} else {
		StringParse desktopStr;
		if (Sys("xprop -root _DT_SAVE_MODE").Find("xfce") >= 0)
			desktop = "xfce";
		else if ((desktopStr = Sys("xprop -root")).Find("ENLIGHTENMENT") >= 0) {
			desktop = "enlightenment";
			desktopStr.GoAfter("ENLIGHTENMENT_VERSION(STRING)", "=");
			desktopStr = desktopStr.GetText();
			if (desktopStr.GetText() == "Enlightenment")
				deskVersion = desktopStr.GetText();
		} else
			desktop = GetEnv("DESKTOP_SESSION");
	}
	if (desktop == "")
		desktop = deskVersion = "UNKNOWN";
	
	// Distro
	if (GetOsInfo_CheckLsb(distro, distVersion))
		;
	else if (FileExists("/usr/share/doc/ubuntu-minimal") || FileExists("/usr/share/ubuntu-docs"))
		distro = "ubuntu";
	else if (FileExists("/etc/fedora-release")) {
		distro = "fedora";
		StringParse strFile = LoadFile_Safe("/etc/fedora-release");
		String str;
		do {
			str = strFile.GetText();
			if ((str != "fedora") && (str != "release"))
				distVersion << str << " ";
		} while (str != "");
	} else if (FileExists("/etc/redhat-release")) {
		distro = "redhat";
		distVersion = LoadFile_Safe("/etc/redhat-release");
	} else if (FileExists("/etc/SuSE-release")) {
		StringParse strFile = LoadFile_Safe("/etc/SuSE-release");
		distro = strFile.GetText();
		strFile.GoAfter_Init("VERSION", "=");
		distVersion = strFile.GetText();
	} else if (FileExists("/etc/mandrake-release")) {
		distro = "mandrake";			
		distVersion = LoadFile_Safe("/etc/mandrake-release");
	} else if (FileExists("/etc/mandriva-release")) {
		distro = "mandriva";	
		distVersion = LoadFile_Safe("/etc/mandriva-release");
	} else if (FileExists("/etc/aurox-release")) {
		distro = "aurox";
		distVersion = LoadFile_Safe("/etc/aurox-release");
	} else if (FileExists("/etc/altlinux-release")) {
		distro = "altlinux";			
		distVersion = LoadFile_Safe("/etc/altlinux-releas");
	} else if (FileExists("/etc/yellowdog-release")) {
		distro = "yellowdog";
		distVersion = LoadFile_Safe("/etc/gentoo-yellowdog");
	} else if (FileExists("/etc/gentoo-release")) {
		distro = "gentoo";
		distVersion = LoadFile_Safe("/etc/gentoo-release");
	} else if (FileExists("/usr/portage")) {
		distro = "gentoo";
		distVersion = LoadFile_Safe("/usr/portage");
	} else if (FileExists("/etc/slackware-version")) {
		distro = "slackware";
		StringParse strFile = LoadFile_Safe("/etc/slackware-version");
		strFile.GetText();
		distVersion = strFile.GetText();
	} else if (FileExists("/etc/debian_version")) {
		distro = "debian";
		distVersion = LoadFile_Safe("/etc/debian_version");
	} else if (LoadFile_Safe("/etc/release").Find("Solaris") >= 0)
		distro = "solaris";
	else if (Sys("uname -r").Find("solaris") >= 0)
		distro = "solaris";
	else {					// If not try with /etc/osname_version
		distro = LoadFile_Safe("/etc/osname_version");
		distVersion = "";
	} 
	if (distro == "") 
		distro = LoadFile_Safe("/etc/issue");
	if (distro == "") 
		distro = distVersion = "UNKNOWN";
	
	return true;
}

#endif

END_UPP_NAMESPACE