#include "SysInfo_in.h"

NAMESPACE_UPP

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

#ifndef PRODUCT_UNLICENSED
	#define PRODUCT_UNLICENSED     			0xABCDABCD	
#endif
#ifndef PRODUCT_BUSINESS
	#define PRODUCT_BUSINESS				0x00000006	
#endif
#ifndef PRODUCT_BUSINESS_N
	#define PRODUCT_BUSINESS_N				0x00000010	
#endif
#ifndef PRODUCT_CLUSTER_SERVER
	#define PRODUCT_CLUSTER_SERVER			0x00000012	
#endif
#ifndef PRODUCT_CLUSTER_SERVER_V
	#define PRODUCT_CLUSTER_SERVER_V		0x00000040	
#endif
#ifndef PRODUCT_CORE
	#define PRODUCT_CORE					0x00000065	
#endif
#ifndef PRODUCT_CORE_N
	#define PRODUCT_CORE_N					0x00000062	
#endif
#ifndef PRODUCT_CORE_COUNTRYSPECIFIC
	#define PRODUCT_CORE_COUNTRYSPECIFIC	0x00000063	
#endif
#ifndef PRODUCT_CORE_SINGLELANGUAGE
	#define PRODUCT_CORE_SINGLELANGUAGE		0x00000064	
#endif
#ifndef PRODUCT_DATACENTER_EVALUATION_SERVER
	#define PRODUCT_DATACENTER_EVALUATION_SERVER	0x00000050	
#endif
#ifndef PRODUCT_DATACENTER_SERVER
	#define PRODUCT_DATACENTER_SERVER		0x00000008	
#endif
#ifndef PRODUCT_DATACENTER_SERVER_CORE
	#define PRODUCT_DATACENTER_SERVER_CORE	0x0000000C	
#endif
#ifndef PRODUCT_DATACENTER_SERVER_CORE_V
	#define PRODUCT_DATACENTER_SERVER_CORE_V	0x00000027	
#endif
#ifndef PRODUCT_DATACENTER_SERVER_V
	#define PRODUCT_DATACENTER_SERVER_V			0x00000025	
#endif
#ifndef PRODUCT_ENTERPRISE
	#define PRODUCT_ENTERPRISE				0x00000004	
#endif
#ifndef PRODUCT_ENTERPRISE_E
	#define PRODUCT_ENTERPRISE_E			0x00000046	
#endif
#ifndef PRODUCT_ENTERPRISE_N_EVALUATION
	#define PRODUCT_ENTERPRISE_N_EVALUATION	0x00000054	
#endif
#ifndef PRODUCT_ENTERPRISE_N
	#define PRODUCT_ENTERPRISE_N			0x0000001B	
#endif
#ifndef PRODUCT_ENTERPRISE_EVALUATION
	#define PRODUCT_ENTERPRISE_EVALUATION	0x00000048	
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER
	#define PRODUCT_ENTERPRISE_SERVER		0x0000000A	
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_CORE
	#define PRODUCT_ENTERPRISE_SERVER_CORE	0x0000000E
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_CORE_V
	#define PRODUCT_ENTERPRISE_SERVER_CORE_V	0x00000029	
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_IA64
	#define PRODUCT_ENTERPRISE_SERVER_IA64	0x0000000F	
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_V
	#define PRODUCT_ENTERPRISE_SERVER_V		0x00000026	
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT		0x0000003B	
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL		0x0000003C	
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC	0x0000003D	
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC	0x0000003E	
#endif
#ifndef PRODUCT_HOME_BASIC
	#define PRODUCT_HOME_BASIC				0x00000002	
#endif
#ifndef PRODUCT_HOME_BASIC_E
	#define PRODUCT_HOME_BASIC_E			0x00000043	
#endif
#ifndef PRODUCT_HOME_BASIC_N
	#define PRODUCT_HOME_BASIC_N			0x00000005
#endif
#ifndef PRODUCT_HOME_PREMIUM
	#define PRODUCT_HOME_PREMIUM			0x00000003
#endif
#ifndef PRODUCT_HOME_PREMIUM_E
	#define PRODUCT_HOME_PREMIUM_E			0x00000044	
#endif
#ifndef PRODUCT_HOME_PREMIUM_N
	#define PRODUCT_HOME_PREMIUM_N			0x0000001A	
#endif
#ifndef PRODUCT_HOME_PREMIUM_SERVER
	#define PRODUCT_HOME_PREMIUM_SERVER		0x00000022	
#endif
#ifndef PRODUCT_HOME_SERVER
	#define PRODUCT_HOME_SERVER				0x00000013	
#endif
#ifndef PRODUCT_HYPERV
	#define PRODUCT_HYPERV					0x0000002A	
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT
	#define PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT	0x0000001E	
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING
	#define PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING		0x00000020	
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY
	#define PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY		0x0000001F	
#endif
#ifndef PRODUCT_MULTIPOINT_STANDARD_SERVER
	#define PRODUCT_MULTIPOINT_STANDARD_SERVER			0x0000004C	
#endif
#ifndef PRODUCT_MULTIPOINT_PREMIUM_SERVER
	#define PRODUCT_MULTIPOINT_PREMIUM_SERVER			0x0000004D	
#endif
#ifndef PRODUCT_PROFESSIONAL
	#define PRODUCT_PROFESSIONAL			0x00000030	
#endif
#ifndef PRODUCT_PROFESSIONAL_E
	#define PRODUCT_PROFESSIONAL_E			0x00000045	
#endif
#ifndef PRODUCT_PROFESSIONAL_N
	#define PRODUCT_PROFESSIONAL_N			0x00000031	
#endif
#ifndef PRODUCT_PROFESSIONAL_WMC
	#define PRODUCT_PROFESSIONAL_WMC		0x00000067	
#endif
#ifndef PRODUCT_SB_SOLUTION_SERVER_EM
	#define PRODUCT_SB_SOLUTION_SERVER_EM	0x00000036	
#endif
#ifndef PRODUCT_SERVER_FOR_SB_SOLUTIONS
	#define PRODUCT_SERVER_FOR_SB_SOLUTIONS	0x00000033	
#endif
#ifndef PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM
	#define PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM	0x00000037
#endif
#ifndef PRODUCT_SERVER_FOR_SMALLBUSINESS
	#define PRODUCT_SERVER_FOR_SMALLBUSINESS	0x00000018
#endif
#ifndef PRODUCT_SERVER_FOR_SMALLBUSINESS_V
	#define PRODUCT_SERVER_FOR_SMALLBUSINESS_V	0x00000023
#endif
#ifndef PRODUCT_SERVER_FOUNDATION
	#define PRODUCT_SERVER_FOUNDATION		0x00000021	
#endif
#ifndef PRODUCT_SB_SOLUTION_SERVER
	#define PRODUCT_SB_SOLUTION_SERVER		0x00000032	
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER
	#define PRODUCT_SMALLBUSINESS_SERVER	0x00000009	
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER_PREMIUM
	#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM		0x00000019	
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE
	#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE	0x0000003F	
#endif
#ifndef PRODUCT_SOLUTION_EMBEDDEDSERVER
	#define PRODUCT_SOLUTION_EMBEDDEDSERVER		0x00000038	
#endif
#ifndef PRODUCT_STANDARD_EVALUATION_SERVER
	#define PRODUCT_STANDARD_EVALUATION_SERVER	0x0000004F	
#endif
#ifndef PRODUCT_STANDARD_SERVER
	#define PRODUCT_STANDARD_SERVER				0x00000007	
#endif
#ifndef PRODUCT_STANDARD_SERVER_CORE
	#define PRODUCT_STANDARD_SERVER_CORE		0x0000000D	
#endif
#ifndef PRODUCT_STANDARD_SERVER_V
	#define PRODUCT_STANDARD_SERVER_V			0x00000024	
#endif
#ifndef PRODUCT_STANDARD_SERVER_CORE_V
	#define PRODUCT_STANDARD_SERVER_CORE_V		0x00000028	
#endif
#ifndef PRODUCT_STANDARD_SERVER_SOLUTIONS
	#define PRODUCT_STANDARD_SERVER_SOLUTIONS		0x00000034	
#endif
#ifndef PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE
	#define PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE	0x00000035	
#endif
#ifndef PRODUCT_STARTER
	#define PRODUCT_STARTER			0x0000000B	
#endif
#ifndef PRODUCT_STARTER_E
	#define PRODUCT_STARTER_E		0x00000042	
#endif
#ifndef PRODUCT_STARTER_N
	#define PRODUCT_STARTER_N		0x0000002F	
#endif
#ifndef PRODUCT_STORAGE_ENTERPRISE_SERVER
	#define PRODUCT_STORAGE_ENTERPRISE_SERVER			0x00000017	
#endif
#ifndef PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE
	#define PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE		0x0000002E	
#endif
#ifndef PRODUCT_STORAGE_EXPRESS_SERVER
	#define PRODUCT_STORAGE_EXPRESS_SERVER				0x00000014	
#endif
#ifndef PRODUCT_STORAGE_EXPRESS_SERVER_CORE
	#define PRODUCT_STORAGE_EXPRESS_SERVER_CORE			0x0000002B	
#endif
#ifndef PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER
	#define PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER	0x00000060	
#endif
#ifndef PRODUCT_STORAGE_STANDARD_SERVER
	#define PRODUCT_STORAGE_STANDARD_SERVER				0x00000015	
#endif
#ifndef PRODUCT_STORAGE_STANDARD_SERVER_CORE
	#define PRODUCT_STORAGE_STANDARD_SERVER_CORE		0x0000002C	
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER
	#define PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER	0x0000005F	
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER
	#define PRODUCT_STORAGE_WORKGROUP_SERVER			0x00000016	
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER_CORE
	#define PRODUCT_STORAGE_WORKGROUP_SERVER_CORE		0x0000002D	
#endif
#ifndef PRODUCT_UNDEFINED
	#define PRODUCT_UNDEFINED		0x00000000	
#endif
#ifndef PRODUCT_ULTIMATE
	#define PRODUCT_ULTIMATE		0x00000001	
#endif
#ifndef PRODUCT_ULTIMATE_E
	#define PRODUCT_ULTIMATE_E		0x00000047	
#endif
#ifndef PRODUCT_ULTIMATE_N
	#define PRODUCT_ULTIMATE_N		0x0000001C	
#endif
#ifndef PRODUCT_WEB_SERVER
	#define PRODUCT_WEB_SERVER		0x00000011	
#endif
#ifndef PRODUCT_WEB_SERVER_CORE
	#define PRODUCT_WEB_SERVER_CORE	0x0000001D 	
#endif
#ifndef SM_SERVERR2
	#define SM_SERVERR2             89
#endif
#ifndef VER_SUITE_WH_SERVER
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
   	if (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && osvi.dwMajorVersion > 4) {
		if (osvi.dwMajorVersion == 6) {
			if (osvi.dwMinorVersion == 3) {
            	if (osvi.wProductType == VER_NT_WORKSTATION)
                	kernel.Cat(" 8.1");
            	else 
					kernel.Cat(" Server 2012 R2");
			} else if (osvi.dwMinorVersion == 2) {
            	if (osvi.wProductType == VER_NT_WORKSTATION)
                	kernel.Cat(" 8");
            	else 
					kernel.Cat(" Server 2012");
			} else if (osvi.dwMinorVersion == 1) {
            	if (osvi.wProductType == VER_NT_WORKSTATION)
                	kernel.Cat(" 7");
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
	         	case PRODUCT_UNLICENSED:				distro = "Unlicensed"; break;
	            case PRODUCT_BUSINESS:					distro = "Business Edition"; break;
	            case PRODUCT_BUSINESS_N:				distro = "Business Edition N"; break;
				case PRODUCT_CLUSTER_SERVER:			distro = "HPC Edition"; break;
                case PRODUCT_CLUSTER_SERVER_V:			distro = "Server Hyper Core V"; break;
                case PRODUCT_CORE:						distro = "Windows 8"; break;
                case PRODUCT_CORE_N:					distro = "Windows 8 N"; break;
                case PRODUCT_CORE_COUNTRYSPECIFIC:		distro = "Windows 8 China"; break;
                case PRODUCT_CORE_SINGLELANGUAGE:		distro = "Windows 8 Single Language"; break;
                case PRODUCT_DATACENTER_EVALUATION_SERVER:	distro = "Server Datacenter (evaluation installation)"; break;
                case PRODUCT_DATACENTER_SERVER:			distro = "Server Datacenter (full installation)"; break;
                case PRODUCT_DATACENTER_SERVER_CORE:	distro = "Server Datacenter (core installation)"; break;
                case PRODUCT_DATACENTER_SERVER_CORE_V:	distro = "Server Datacenter without Hyper-V (core installation)"; break;
                case PRODUCT_DATACENTER_SERVER_V:		distro = "Server Datacenter without Hyper-V (full installation)"; break;
                case PRODUCT_ENTERPRISE:				distro = "Enterprise"; break;
                case PRODUCT_ENTERPRISE_E:				distro = "Not supported"; break;
                case PRODUCT_ENTERPRISE_N_EVALUATION:	distro = "Enterprise N (evaluation installation)"; break;
                case PRODUCT_ENTERPRISE_N:				distro = "Enterprise N"; break;
                case PRODUCT_ENTERPRISE_EVALUATION:		distro = "Server Enterprise (evaluation installation)"; break;
                case PRODUCT_ENTERPRISE_SERVER:			distro = "Server Enterprise (full installation)"; break;
                case PRODUCT_ENTERPRISE_SERVER_CORE:	distro = "Server Enterprise (core installation)"; break;
                case PRODUCT_ENTERPRISE_SERVER_CORE_V:	distro = "Server Enterprise without Hyper-V (core installation)"; break;
                case PRODUCT_ENTERPRISE_SERVER_IA64:	distro = "Server Enterprise for Itanium-based Systems"; break;
                case PRODUCT_ENTERPRISE_SERVER_V:		distro = "Server Enterprise without Hyper-V (full installation)"; break;
                case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT:		distro = "Windows Essential Server Solution Management"; break;
                case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL:		distro = "Windows Essential Server Solution Additional"; break;
                case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC:	distro = "Windows Essential Server Solution Management SVC"; break;
                case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC:	distro = "Windows Essential Server Solution Additional SVC"; break;
                case PRODUCT_HOME_BASIC:				distro = "Home Basic"; break;
                case PRODUCT_HOME_BASIC_E:				distro = "Not supported"; break;
                case PRODUCT_HOME_BASIC_N:				distro = "Home Basic N"; break;
                case PRODUCT_HOME_PREMIUM:				distro = "Home Premium"; break;
                case PRODUCT_HOME_PREMIUM_E:			distro = "Not supported"; break;
                case PRODUCT_HOME_PREMIUM_N:			distro = "Home Premium N"; break;
                case PRODUCT_HOME_PREMIUM_SERVER:		distro = "Windows Home Server 2011"; break;
                case PRODUCT_HOME_SERVER:				distro = "Windows Storage Server 2008 R2 Essentials"; break;
                case PRODUCT_HYPERV:					distro = "Microsoft Hyper-V Server"; break;
                case PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT:	distro = "Windows Essential Business Server Management Server"; break;
                case PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING:	distro = "Windows Essential Business Server Messaging Server"; break;
                case PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY:	distro = "Windows Essential Business Server Security Server"; break;
                case PRODUCT_MULTIPOINT_STANDARD_SERVER:		distro = "Windows MultiPoint Server Standard (full installation)"; break;
                case PRODUCT_MULTIPOINT_PREMIUM_SERVER:	distro = "Windows MultiPoint Server Premium (full installation)"; break;
                case PRODUCT_PROFESSIONAL:				distro = "Professional"; break;
                case PRODUCT_PROFESSIONAL_E:			distro = "Not supported"; break;
                case PRODUCT_PROFESSIONAL_N:			distro = "Professional N"; break;
                case PRODUCT_PROFESSIONAL_WMC:			distro = "Professional with Media Center"; break;
                case PRODUCT_SB_SOLUTION_SERVER_EM:		distro = "Server For SB Solutions EM"; break;
                case PRODUCT_SERVER_FOR_SB_SOLUTIONS:	distro = "Server For SB Solutions"; break;
                case PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM:	distro = "Server For SB Solutions EM"; break;
                case PRODUCT_SERVER_FOR_SMALLBUSINESS:		distro = "Windows Server 2008 for Windows Essential Server Solutions"; break;
                case PRODUCT_SERVER_FOR_SMALLBUSINESS_V:	distro = "Windows Server 2008 without Hyper-V for Windows Essential Server Solutions"; break;
                case PRODUCT_SERVER_FOUNDATION:			distro = "Server Foundation"; break;
                case PRODUCT_SB_SOLUTION_SERVER:		distro = "Windows Small Business Server 2011 Essentials"; break;
                case PRODUCT_SMALLBUSINESS_SERVER:		distro = "Windows Small Business Server"; break;
                case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:		distro = "Small Business Server Premium"; break;
                case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE:	distro = "Small Business Server Premium (core installation)"; break;
                case PRODUCT_SOLUTION_EMBEDDEDSERVER:			distro = "Windows MultiPoint Server"; break;
                case PRODUCT_STANDARD_EVALUATION_SERVER:		distro = "Server Standard (evaluation installation)"; break;
                case PRODUCT_STANDARD_SERVER:			distro = "Server Standard"; break;
                case PRODUCT_STANDARD_SERVER_CORE:		distro = "Server Standard (core installation)"; break;
                case PRODUCT_STANDARD_SERVER_V:			distro = "Server Standard without Hyper-V"; break;
                case PRODUCT_STANDARD_SERVER_CORE_V:	distro = "Server Standard without Hyper-V (core installation)"; break;
                case PRODUCT_STANDARD_SERVER_SOLUTIONS:	distro = "Server Solutions Premium"; break;
                case PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE:	distro = "Server Solutions Premium (core installation)"; break;
                case PRODUCT_STARTER:					distro = "Starter"; break;
                case PRODUCT_STARTER_E:					distro = "Not supported"; break;
                case PRODUCT_STARTER_N:					distro = "Starter N"; break;
                case PRODUCT_STORAGE_ENTERPRISE_SERVER:	distro = "Storage Server Enterprise"; break;
                case PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE:		distro = "Storage Server Enterprise (core installation)"; break;
                case PRODUCT_STORAGE_EXPRESS_SERVER:				distro = "Storage Server Express"; break;
                case PRODUCT_STORAGE_EXPRESS_SERVER_CORE:			distro = "Storage Server Express (core installation)"; break;
                case PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER:	distro = "Storage Server Standard (evaluation installation)"; break;
                case PRODUCT_STORAGE_STANDARD_SERVER:				distro = "Storage Server Standard"; break;
                case PRODUCT_STORAGE_STANDARD_SERVER_CORE:			distro = "Storage Server Standard (core installation)"; break;
                case PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER:	distro = "Storage Server Workgroup (evaluation installation)"; break;
                case PRODUCT_STORAGE_WORKGROUP_SERVER:				distro = "Storage Server Workgroup"; break;
                case PRODUCT_STORAGE_WORKGROUP_SERVER_CORE:			distro = "Storage Server Workgroup (core installation)"; break;
                case PRODUCT_UNDEFINED:			distro = "An unknown product"; break;
                case PRODUCT_ULTIMATE:			distro = "Ultimate"; break;
                case PRODUCT_ULTIMATE_E:		distro = "Not supported"; break;
                case PRODUCT_ULTIMATE_N:		distro = "Ultimate N"; break;
                case PRODUCT_WEB_SERVER:		distro = "Web Server (full installation)"; break;
                case PRODUCT_WEB_SERVER_CORE:	distro = "Web Server (core installation)"; break;
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

static bool GetOsInfoCheckRelease(String &distro, String &distVersion) {
	StringParse release;			
	for (FindFile ff("/etc/*-release"); ff; ff.Next())
		release += LoadFile_Safe(ff.GetPath());		
	if (release.IsEmpty())
		return false;
	if(!release.GoAfter("DISTRIB_ID=")) 
		if (!release.GoAfter("ID="))
			return false;				
	distro = ToLower(release.GetText());
	if (distro.IsEmpty())
		return false;
	if (!release.GoAfter_Init("DISTRIB_RELEASE="))
		if (!release.GoAfter_Init("VERSION_ID="))
			return false;
	distVersion = ToLower(release.GetText());

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
	if (kernel.IsEmpty())
		kernel = LoadFile_Safe("/proc/sys/kernel/ostype");
	if (kernel.IsEmpty())
		kernel = LoadFile_Safe("/proc/version");
	if (kernel.IsEmpty()) {
		if (Sys("sysctl_cmd -n kern.version").Find("FreeBSD") >= 0)
			kernel = "freebsd";
	}
	if (kernel.IsEmpty()) 
		kernel = ToLower(Sys("uname -s"));
	
	if (kerVersion.IsEmpty())
		kerVersion = LoadFile_Safe("/proc/sys/kernel/osrelease") + " " + LoadFile_Safe("/proc/sys/kernel/version");
	if (kerVersion.IsEmpty())
		kerArchitecture = ToLower(Sys("uname -v"));
		
	if (kerArchitecture.IsEmpty())
		kerArchitecture = ToLower(Sys("uname -m"));

	// printenv
    if (GetEnv("GNOME_DESKTOP_SESSION_ID").GetCount() || GetEnv("GNOME_KEYRING_SOCKET").GetCount()) {
		desktop = "gnome";
		StringParse gnomeVersion = Sys("gnome-about --version");
		gnomeVersion.GoAfter("gnome-about");
		deskVersion = gnomeVersion.GetText();
	} else if (GetEnv("KDE_FULL_SESSION").GetCount() || GetEnv("KDEDIR").GetCount() || GetEnv("KDE_MULTIHEAD").GetCount()) {
        desktop = "kde"; 
        StringParse konsole = Sys("konsole --version");
        konsole.GoAfter("KDE", ":");
        deskVersion = konsole.GetText();						
		if (deskVersion.IsEmpty())		
			deskVersion = GetEnv("KDE_SESSION_VERSION");        
	} else if (GetEnv("MATE_KEYRING_CONTROL").GetCount()) {
		desktop = "mate"; 
		StringParse mateVersion = Sys("mate-about --version");
		while (true) {
			String str = mateVersion.GetText();
			if (atof(str) > 0) {
				deskVersion = str;
				break;
			} else if (str.IsEmpty())
				break;
		}
	} else {
		String str = GetEnv("CINNAMON_VERSION");
		if (!str.IsEmpty()) {
			desktop = "cinnamon";
			deskVersion = str;
		} else {
			StringParse desktopStr = ToLower(Sys("xprop -root"));
			if (desktopStr.Find("lxde") >= 0 || ToLower(GetEnv("DESKTOP_SESSION")) == "lxde") {
				desktop = "lxde";
				desktopStr.GoAfter("_ob_version", "=");
				deskVersion = desktopStr.GetText();
			} else if (desktopStr.Find("xfce") >= 0) {
				desktopStr.GoAfter("_dt_save_mode", "=");
				desktop = desktopStr.GetText();
				StringParse xfceVersion = Sys(Format("%s-about --version", desktop));
				while (true) {
					String str = xfceVersion.GetText();
					if (atof(str) > 0) {
						deskVersion = str;
						break;
					} else if (str.IsEmpty())
						break;
				}			
			} else if (desktopStr.Find("enlightenment") >= 0) {
				desktop = "enlightenment";
				desktopStr.GoAfter("enlightenment_version", "=");
				desktopStr = desktopStr.GetText();
				if (desktopStr.GetText() == "enlightenment")
					deskVersion = desktopStr.GetText();
			} else
				desktop = GetEnv("DESKTOP_SESSION");
		}
	}
	
	if (GetOsInfoCheckRelease(distro, distVersion))
		;
	else if (FileExists("/usr/share/doc/ubuntu-minimal") || FileExists("/usr/share/ubuntu-docs"))
		distro = "ubuntu";
	else if (FileExists("/etc/fedora-release")) {
		distro = "fedora";
		StringParse strFile = LoadFile_Safe("/etc/fedora-release");
		strFile.GoAfter("release");
		distVersion = strFile.GetText();
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
	else if (ToLower(Sys("uname -r")).Find("freebsd") >= 0)
		distro = "freebsd";
	else if (ToLower(Sys("uname -r")).Find("solaris") >= 0)
		distro = "solaris";
	else {					
		distro = LoadFile_Safe("/etc/osname_version");
		distVersion = "";
	} 
	if (distro.IsEmpty()) 
		distro = LoadFile_Safe("/etc/issue");
	if (distro.IsEmpty()) 
		distro = distVersion = "UNKNOWN";
	
	return true;
}

#endif

END_UPP_NAMESPACE