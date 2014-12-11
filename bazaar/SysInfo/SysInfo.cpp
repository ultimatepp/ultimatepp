#include "SysInfo_in.h"

#if defined (PLATFORM_POSIX)
#ifdef flagGUI
	//#define SetX11ErrorHandler() {}
	//void SetSysInfoX11ErrorHandler()	{return;}
#else
	//#define SetX11ErrorHandler() {}
	int SysInfoX11ErrorHandler(_XDisplay *, XErrorEvent *)	{return 0;}
	void SetSysInfoX11ErrorHandler()						{XSetErrorHandler(SysInfoX11ErrorHandler);}
#endif
#endif

NAMESPACE_UPP

#define TFILE <SysInfo/SysInfo.t>
#include <Core/t.h>

#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

/////////////////////////////////////////////////////////////////////
// Hardware Info
#if defined(PLATFORM_WIN32) 

bool GetWMIInfo(String system, Array <String> &data, Array <Value> *ret[], String nameSpace = "root\\cimv2") {
	HRESULT hRes;
	
	hRes = CoInitialize(NULL);
	if (hRes != S_OK && hRes != S_FALSE)
		return false;

	hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT,
			RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0);
	if (hRes != S_OK && hRes != RPC_E_TOO_LATE) {
		CoUninitialize();
		return false;
	}
	IWbemLocator* pIWbemLocator = NULL;
	if (CoCreateInstance(CLSID_WbemAdministrativeLocator, NULL, 
		CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IUnknown, (void **)&pIWbemLocator) != S_OK) {
		CoUninitialize();
		return false;
	}
	
	BSTR bstrNamespace = SysAllocString(nameSpace.ToWString());
	IWbemServices* pWbemServices = NULL;
	if (pIWbemLocator->ConnectServer(bstrNamespace, NULL, NULL, NULL, 0, NULL, NULL,
		&pWbemServices) != S_OK) {
		CoUninitialize();
		return false;
	}
	SysFreeString(bstrNamespace);
	
	hRes = CoSetProxyBlanket(pWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
			RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hRes)) {
        pWbemServices->Release();
        pIWbemLocator->Release();     
        CoUninitialize();
        return false;
    }
	
	IEnumWbemClassObject* pEnumerator = NULL;
	String query;
	query << "Select * from " << system;
	WCHAR wquery[1024*sizeof(WCHAR)];
	MultiByteToWideChar(CP_UTF8, 0, query, -1, wquery, sizeof(wquery)/sizeof(wquery[0]));
	BSTR strQuery = SysAllocString(wquery);
	BSTR strQL = SysAllocString(L"WQL");
	hRes = pWbemServices->ExecQuery(strQL, strQuery, WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (hRes != S_OK) {
        pWbemServices->Release();
        pIWbemLocator->Release();     
        CoUninitialize();
        return false;
    }
	SysFreeString(strQuery);
	SysFreeString(strQL);

    IWbemClassObject *pClassObject;
    ULONG uReturn = 0;
    int row = 0;
    bool rt = false;
	while (pEnumerator) {
        hRes = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);
       	if(0 == uReturn) {
       		if (rt)
       			break;
       		else {
	   			pIWbemLocator->Release();
				pWbemServices->Release();
				pEnumerator->Release();
				CoUninitialize(); 
	            return false;
       		}
       	}
		if(hRes != S_OK) {
	        pWbemServices->Release();
	        pIWbemLocator->Release(); 
	        pEnumerator->Release(); 
	        pClassObject->Release();   
	        CoUninitialize();
	        return false;
	    }
		for (int col = 0; col < data.GetCount(); ++col) {
			VARIANT vProp;
			VariantInit(&vProp);
			BSTR strClassProp = SysAllocString(data[col].ToWString());
	        hRes = pClassObject->Get(strClassProp, 0, &vProp, 0, 0);
	        if(hRes != S_OK){
		        pWbemServices->Release();
		        pIWbemLocator->Release(); 
		        pEnumerator->Release(); 
		        pClassObject->Release();   
		        CoUninitialize();
		        return false;
		    }
			SysFreeString(strClassProp);        
			ret[col]->Add(GetVARIANT(vProp));
			VariantClear(&vProp);
			rt = true;
		}
		row++;
    }
	pIWbemLocator->Release();
	pWbemServices->Release();
	pEnumerator->Release();
	pClassObject->Release();
	CoUninitialize(); 
	
	return true;
}

bool GetWMIInfo(String system, String data, Value &res, String nameSpace = "root\\cimv2") {
	Array <Value> arrRes;
	Array <Value> *arrResP[1];
	arrResP[0] = &arrRes;
	Array <String> arrData;
	arrData.Add(data);
	bool ret = GetWMIInfo(system, arrData, arrResP, nameSpace);
	if (ret)
		res = arrRes[0];
	return ret;
}
	
void GetSystemInfo(String &manufacturer, String &productName, String &version, int &numberOfProcessors, String &mbSerial) { 
	manufacturer = "";
	Value vmanufacturer;
	if (GetWMIInfo("Win32_ComputerSystem", "manufacturer", vmanufacturer)) 
		manufacturer = Trim(vmanufacturer);
	if (manufacturer.IsEmpty()) 
		manufacturer = FromSystemCharset(GetWinRegString("SystemManufacturer", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	if (manufacturer.IsEmpty()) {
		StringParse fileData = LoadFile(AppendFileName(GetSystemFolder(), "oeminfo.ini"));
		fileData.GoAfter("Manufacturer=");
		manufacturer = fileData.GetText("\r\n");
	}
	productName = "";
	Value vproductName;
	if (GetWMIInfo("Win32_ComputerSystem", "model", vproductName)) 
		productName = Trim(vproductName);
	if (productName.IsEmpty())
		productName = FromSystemCharset(GetWinRegString("SystemProductName", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	if (productName.IsEmpty())
		productName = FromSystemCharset(GetWinRegString("Model", "SOFTWARE\\Microsoft\\PCHealth\\HelpSvc\\OEMInfo", HKEY_LOCAL_MACHINE));
	
	version = FromSystemCharset(GetWinRegString("SystemVersion", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	numberOfProcessors = atoi(GetEnv("NUMBER_OF_PROCESSORS"));
	Value vmbSerial;
	if (GetWMIInfo("Win32_BaseBoard", "SerialNumber", vmbSerial)) 
		mbSerial = Trim(vmbSerial);	
}

void GetBiosInfo(String &biosVersion, Date &biosReleaseDate, String &biosSerial) { 
	// Alternative is "wmic bios get name" and "wmic bios get releasedate"
	String strBios = FromSystemCharset(GetWinRegString("BIOSVersion", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));	
	if (strBios.IsEmpty())
		strBios = FromSystemCharset(GetWinRegString("SystemBiosVersion", "HARDWARE\\DESCRIPTION\\System", HKEY_LOCAL_MACHINE));	
	for (int i = 0; i < strBios.GetLength(); ++i) {
		if (strBios[i] == '\0')
			biosVersion.Cat(". ");
		else
			biosVersion.Cat(strBios[i]);
	}
	String strDate = FromSystemCharset(GetWinRegString("BIOSReleaseDate", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	if (strDate.IsEmpty())
		strDate = FromSystemCharset(GetWinRegString("SystemBiosDate", "HARDWARE\\DESCRIPTION\\System", HKEY_LOCAL_MACHINE));
	int lang = GetCurrentLanguage();
	SetLanguage(LNG_ENGLISH);
	if (!StrToDate(biosReleaseDate, strDate))
		biosReleaseDate = Null;
	SetLanguage(lang);
	Value vmbSerial;
	if (GetWMIInfo("Win32_BIOS", "SerialNumber", vmbSerial)) 
		biosSerial = Trim(vmbSerial);	
}

bool GetProcessorInfo(int number, String &vendor, String &identifier, String &architecture, int &speed)	{
	String strReg = Format("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d", number);
	vendor = FromSystemCharset(GetWinRegString("VendorIdentifier", strReg, HKEY_LOCAL_MACHINE));	
	identifier = FromSystemCharset(GetWinRegString("ProcessorNameString", strReg, HKEY_LOCAL_MACHINE));	
	architecture = FromSystemCharset(GetWinRegString("Identifier", strReg, HKEY_LOCAL_MACHINE));	
	speed = GetWinRegInt("~MHz", strReg, HKEY_LOCAL_MACHINE);
	
	return true;
}
/*
String GetMacAddressWMI() {
	Value vmac;
	if (!GetWMIInfo("Win32_NetworkAdapterConfiguration", "MacAddress", vmac)) 
		return Null;
	String mac = Trim(vmac);	
	if (mac.GetCount() > 0)
		return mac;
	return Null;
}
*/
#include <iphlpapi.h>

/* This is from the BIND 4.9.4 release, modified to compile by itself */

#define	IN6ADDRSZ	16
#define	INT16SZ		 2

#include <stdint.h>

String inet_ntop4(const unsigned char *src) {
	return Format("%d.%d.%d.%d", src[0], src[1], src[2], src[3]);
}

String inet_ntop6(const unsigned char *src) {
	struct {int base, len;} best, cur;
	uint16_t words[IN6ADDRSZ / INT16SZ];

	memset(words, 0, sizeof words);
	for (int i = 0; i < IN6ADDRSZ; i++) 
		words[int(i / INT16SZ)] |= ((uint8_t)src[i] << ((1 - (i % INT16SZ)) << 3));
	
	best.base = -1;
	cur.base = -1;
	for (int i = 0; i < (IN6ADDRSZ / INT16SZ); i++) {
		if (words[i] == 0) {
			if (cur.base == -1)
				cur.base = i, cur.len = 1;
			else
				cur.len++;
		} else {
			if (cur.base != -1) {
				if (best.base == -1 || cur.len > best.len)
					best = cur;
				cur.base = -1;
			}
		}
	}
	if (cur.base != -1) {
		if (best.base == -1 || cur.len > best.len)
			best = cur;
	}
	if (best.base != -1 && best.len < 2)
		best.base = -1;

	String ret;
	for (int i = 0; i < (IN6ADDRSZ / INT16SZ); i++) {
		if (best.base != -1 && i >= best.base && i < (best.base + best.len)) {
			if (i == best.base)
				ret.Cat(':');
			continue;
		}
		if (i != 0)
			ret.Cat(':');
		if (i == 6 && best.base == 0 && (best.len == 6 || (best.len == 5 && words[5] == 0xffff))) {
			ret.Cat(inet_ntop4(src + 12));
			break;
		}
		ret.Cat(Format("%x", words[i]));
	}
	if (best.base != -1 && (best.base + best.len) == (IN6ADDRSZ / INT16SZ))
		ret.Cat(':');

	return ret;
}

Array <NetAdapter> GetAdapterInfo() {
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG family = AF_UNSPEC;
	DWORD flags = GAA_FLAG_INCLUDE_PREFIX;
	ULONG outBufLen = 0;
	Buffer<BYTE> pBuffer;
	Array <NetAdapter> ret;
	
	switch (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen)) {
	case ERROR_BUFFER_OVERFLOW:
		pBuffer.Alloc(outBufLen);
		pAddresses = (PIP_ADAPTER_ADDRESSES)~pBuffer;
		break;
	case ERROR_NO_DATA:
	case ERROR_INVALID_FUNCTION:
	default:
		return ret;
	}
	if (NO_ERROR != GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen)) 
		return ret;
	
	for (PIP_ADAPTER_ADDRESSES pAdd = pAddresses; pAdd; pAdd = pAdd->Next) {
		NetAdapter &adapter = ret.Add();
		int len = min((DWORD)6, pAdd->PhysicalAddressLength);
		if (len > 0)
			adapter.mac = ToUpper(HexString(pAdd->PhysicalAddress, len, 1, ':'));
		adapter.description = Trim(WideToString(pAdd->Description));
		adapter.fullname = Trim(WideToString(pAdd->FriendlyName));
   
		PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAdd->FirstUnicastAddress;
    	if (pUnicast != NULL) {
          	for (int i = 0; pUnicast != NULL; i++) {
           		if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
           			sockaddr_in *sa_in = (sockaddr_in *)pUnicast->Address.lpSockaddr;
					adapter.ip4 = inet_ntop4((const unsigned char *)&(sa_in->sin_addr));
               	} else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6) {
					sockaddr_in6 *sa_in6 = (sockaddr_in6 *)pUnicast->Address.lpSockaddr;
					adapter.ip6 = inet_ntop6((const unsigned char *)&(sa_in6->sin6_addr));
               	} 
            	pUnicast = pUnicast->Next;
         	}
      	}       	
		switch (pAdd->IfType) {
		case IF_TYPE_ETHERNET_CSMACD: 		adapter.type = "ETHERNET";	break;
		case IF_TYPE_ISO88025_TOKENRING: 	adapter.type = "TOKENRING";	break;
		case IF_TYPE_PPP: 					adapter.type = "MODEM";		break;
		case IF_TYPE_SOFTWARE_LOOPBACK: 	adapter.type = "SOFTWARE_LOOPBACK";		break;
		case IF_TYPE_ATM: 					adapter.type = "ATM";		break;
		case IF_TYPE_IEEE80211: 			adapter.type = "IEEE80211";	break;
		case IF_TYPE_TUNNEL: 				adapter.type = "TUNNEL";	break;
		case IF_TYPE_IEEE1394: 				adapter.type = "IEEE1394";	break;
		default: 							adapter.type = "OTHER";
		}
		if (adapter.type == "ETHERNET" && ToLower(adapter.description).Find("wireless") >= 0)
			adapter.type = "IEEE80211";
	}
	return ret;
}

/*bool GetNetworkInfo(String &name, String &domain)
{
   	LPWKSTA_INFO_100 pBuf = NULL;

   	NET_API_STATUS nStatus = NetWkstaGetInfo(NULL, 100, (LPBYTE *)&pBuf);

	bool ret = true;
   	if (nStatus == NERR_Success) {
       	name = FromSystemCharsetW(pBuf->wki100_computername);
   		domain = FromSystemCharsetW(pBuf->wki100_langroup);
   	}  else
      	ret = false;

   	if (pBuf != NULL)
      	NetApiBufferFree(pBuf);

	if (name.IsEmpty() || !ret) {	// Second try
    	char path[256] = "";
    	WSADATA wsaData;
    	WSAStartup(MAKEWORD(2, 2), &wsaData);
    	gethostname(path, sizeof(path));
    	name = path;
    	domain = "";
    	WSACleanup();		
    	ret = true;
	}
   	return ret;
}*/

String GetHDSerial() {
	Value vmbSerial;
	if (!GetWMIInfo("Win32_PhysicalMedia", "SerialNumber", vmbSerial)) 
		return Null;
	String serial = Trim(vmbSerial);	
	if (serial.GetCount() > 0)
		return serial;
	return Null;
}

bool GetVideoInfo(Array <Value> &name, Array <Value> &description, Array <Value> &videoProcessor, Array <Value> &ram, Array <Value> &videoMode) {
	Array <Value> *res[5];
	res[0] = &name;
	res[1] = &description;
	res[2] = &videoProcessor;
	res[3] = &ram;
	res[4] = &videoMode;
	Array <String> data;
	data.Add("Name");
	data.Add("Description");
	data.Add("VideoProcessor");
	data.Add("AdapterRAM");
	data.Add("VideoModeDescription");
	if (!GetWMIInfo("Win32_VideoController", data, res))
		return false;
	
	for (int row = 0; row < ram.GetCount(); ++row) 
		ram[row] = (atoi(ram[row].ToString()) + 524288)/1048576;
	return true;
}

bool GetPackagesInfo(Array <Value> &name, Array <Value> &version, Array <Value> &vendor, 
Array <Value> &installDate, Array <Value> &caption, Array <Value> &description, Array <Value> &state)
{
	Array <Value> *res[7];
	res[0] = &name;
	res[1] = &version;
	res[2] = &vendor;
	res[3] = &installDate;	
	res[4] = &caption;	
	res[5] = &description;	
	res[6] = &state;	
	Array <String> data;
	data.Add("Name");
	data.Add("Version");
	data.Add("Vendor");
	data.Add("InstallDate2");
	data.Add("Caption");
	data.Add("Description");
	data.Add("InstallState");
	if (!GetWMIInfo("Win32_Product", data, res))
		return false;
	
	for (int i = 0; i < installDate.GetCount(); ++i) {
		String sdate = installDate[i];
		Time t(atoi(sdate.Left(4)), atoi(sdate.Mid(4, 2)), atoi(sdate.Mid(6, 2)), 
			   atoi(sdate.Mid(8, 2)), atoi(sdate.Mid(10, 2)), atoi(sdate.Mid(12, 2)));
		installDate[i] = t;
		int istate = state[i];
		switch (istate) {
		case -6:	state[i] = "Bad Configuration";	break;
		case -2:	state[i] = "Invalid Argument";	break;
		case -1:	state[i] = "Unknown Package";	break;
		case 1:		state[i] = "Advertised";		break;
		case 2:		state[i] = "Absent";			break;
		case 5:		state[i] = "Ok";				break;
		default: 	return false;
		}
	}
	return true;
}

double GetCpuTemperature() {
	Value data;
	if (GetWMIInfo("MSAcpi_ThermalZoneTemperature", "CurrentTemperature", data, "root\\wmi"))
		return (double(data) - 2732.) / 10.;
	if (GetWMIInfo("Win32_TemperatureProbe", "CurrentReading", data))
		return data;
	return Null;
}
#endif


void NetAdapter::Copy(const NetAdapter& src) {
	description <<= src.description;
	fullname <<= src.fullname;
	mac <<= src.mac;
	type <<= src.type;
}

void NetAdapter::Xmlize(XmlIO &xml) {
	xml ("description", description)("fullname", fullname)("mac", mac)("type", type);
}

void NetAdapter::Jsonize(JsonIO &json) {
	json ("description", description)("fullname", fullname)("mac", mac)("type", type);
}

void NetAdapter::Serialize(Stream &stream) {
	stream % description % fullname % mac % type;
}

bool GetNetworkInfo(String &name, String &domain, String &ip4, String &ip6) {
	Buffer<char> sname(255);
	
	bool close = false;
	if (0 != gethostname(sname, 255)) {
#ifdef _WIN32
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
			return false;
		if (0 != gethostname(sname, 255))
			return false;	
		close = true;
#else
		return false;
#endif
	}
	name = sname;
	
	struct hostent *host = gethostbyname(sname);
	domain = host->h_name;
	
#ifdef _WIN32
	ip4 = inet_ntop4((const unsigned char *)((struct in_addr *)*host->h_addr_list));
	ip6 = inet_ntop6((const unsigned char *)((struct in_addr *)*host->h_addr_list));
#else
	Buffer<char> str(max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN) + 1);
	inet_ntop(AF_INET, (void *)(struct in_addr *)*host->h_addr_list, ~str, INET_ADDRSTRLEN);
	ip4 = str;
	inet_ntop(AF_INET6, (void *)(struct in_addr *)*host->h_addr_list, ~str, INET6_ADDRSTRLEN);
	ip6 = str;
#endif
	
#ifdef _WIN32
	if (close)
		WSACleanup();
#endif
	return true;	
}	


#if defined (PLATFORM_POSIX)
void GetSystemInfo(String &manufacturer, String &productName, String &version, int &numberOfProcessors, String &mbSerial)
{
	manufacturer = LoadFile_Safe("/sys/devices/virtual/dmi/id/board_vendor");
	productName = LoadFile_Safe("/sys/devices/virtual/dmi/id/board_name");
	version = LoadFile_Safe("/sys/devices/virtual/dmi/id/product_version");
	mbSerial = LoadFile_Safe("/sys/devices/virtual/dmi/id/board_serial");
	if (mbSerial.IsEmpty()) 
		mbSerial = FormatInt(gethostid());	

	StringParse cpu(LoadFile_Safe("/proc/cpuinfo"));	
	numberOfProcessors = 1;
	while (cpu.GoAfter("processor")) {
		cpu.GoAfter(":");
		numberOfProcessors = atoi(cpu.GetText()) + 1;
	} 
}

void GetBiosInfo(String &biosVersion, Date &biosReleaseDate, String &biosSerial)
{
	String biosVendor = LoadFile_Safe("/sys/devices/virtual/dmi/id/bios_vendor");
	biosVersion = LoadFile_Safe("/sys/devices/virtual/dmi/id/bios_version");
	StrToDate(biosReleaseDate, LoadFile_Safe("/sys/devices/virtual/dmi/id/bios_date"));
	biosSerial = LoadFile_Safe("/sys/devices/virtual/dmi/id/chassis_serial");
}

bool GetProcessorInfo(int number, String &vendor, String &identifier, String &architecture, int &speed)	
{
	StringParse cpu(LoadFile_Safe("/proc/cpuinfo"));
	
	int cpuNumber;
	do {
		if (!cpu.GoAfter("processor", ":"))
			return false;
		String sCpu = cpu.GetText();
		if (sCpu == "")
			return false;
		cpuNumber = atoi(sCpu);
	} while (cpuNumber != number);
		
	cpu.GoAfter("vendor_id", ":");
	vendor = cpu.GetText();
	cpu.GoAfter("cpu family", ":");
	String family = cpu.GetText();		// 6 means 686
	cpu.GoAfter("model", ":");
	String model = cpu.GetText();
	cpu.GoAfter("model name", ":");
	identifier = cpu.GetText("\n");
	cpu.GoAfter("stepping", ":");
	String stepping = cpu.GetText();
	architecture = Sys("uname -m");		// CPU type
	architecture << " Family " << family << " Model " << model << " Stepping " << stepping;		// And 64 bits ?? uname -m
	cpu.GoAfter_Init("cpu MHz", ":");
	speed = cpu.GetInt();
}

static double GetCpuTemperatureACPI() {
	StringParse info = Sys("acpi -t");
	if (info.IsEmpty())
		return Null;
	StringParse line;
	double tempAcc = 0;
	int count = 0;
	while (true) {
		line = info.GetLine();
		if (line.IsEmpty())
			if (info.Eof())
				break;
			else
				continue;
		if (!line.GoAfter("Thermal", "ok,"))
			continue;
		double temp = line.GetDouble();
		if (IsNull(temp))
			continue;
		tempAcc += temp;
		count++;
	}
	if (count == 0)
		return Null;
	return tempAcc/count;	
}

static double GetCpuTemperatureSensors() {
	StringParse info = Sys("sensors");
	if (info.IsEmpty())
		return Null;
	StringParse line;
	double tempAcc = 0;
	int count = 0;
	while (true) {
		if (!info.GoAfter("coretemp"))
			break;
		while(true) {
			line = info.GetLine();
			if (line.IsEmpty())
				break;
			if (!line.GoAfter("Core", ":"))
				continue;
			double temp = line.GetDouble();
			if (IsNull(temp))
				continue;
			tempAcc += temp;
			count++;
		}
		if (info.Eof())
			break;		
	}
	if (count == 0)
		return Null;
	return tempAcc/count;		
}

double GetCpuTemperature() {
	double temp = GetCpuTemperatureACPI();
	if (IsNull(temp)) 
		temp = GetCpuTemperatureSensors();	
	return temp;
}


/*
Array <NetAdapter> GetAdapterInfo() {
	Array <NetAdapter> ret;
	
	StringParse data = Sys("ifconfig -a");
	for (StringParse line = data.GetLine(); !data.Eof(); line = data.GetLine()) {
		NetAdapter &adapter = ret.Add();
		String str = line.GetText();				
		if (str.Find("eth") >= 0)
			adapter.type = "ETHERNET";
		else if (str.Find("lo") >= 0)
			adapter.type = "SOFTWARE_LOOPBACK";
		else if (str.Find("ppp") >= 0)
			adapter.type = "MODEM";		
		else if (str.Find("hci") >= 0)
			adapter.type = "BLUETOOTH";
		else if (str.Find("tr") >= 0)
			adapter.type = "TOKENRING";
		else if (str.Find("vbox") >= 0 || str.Find("wifi") >= 0 || str.Find("ath") >= 0)
			adapter.type = "VIRTUALBOX";
		else if (str.Find("wlan") >= 0)
			adapter.type = "IEEE80211";
		else			
			adapter.type = "OTHER";
		line.GoAfter(":");
		int pos = line.GetPos();
		int npos = line.Find("  ", pos);
		if (npos >= 0) {
		 	adapter.fullname = line.Mid(pos, npos-pos);
		 	line.SetPos(npos+1);
		}
		if (line.GetText() != "") 
			adapter.mac = ToUpper(Trim(line.GetText()));
		do {
			data.GoAfter("\n");
		} while (data[data.GetPos()] == ' ');
	}
	return ret;
}
*/

Array<NetAdapter> GetAdapterInfo() {
	Array<NetAdapter> res;
	
	int sck = socket(PF_INET, SOCK_DGRAM, 0);
	if(sck < 0) 
		return res;
 
 	char buf[8192] = {0};
 	struct ifconf ifc = {0};
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if(ioctl(sck, SIOCGIFCONF, &ifc) < 0) 
		return res;

	struct ifreq *ifr = ifc.ifc_req;
	int nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
	for(int i = 0; i < nInterfaces; i++) {
		String MAC;
      
      	char hostname[NI_MAXHOST];
		bzero(hostname, NI_MAXHOST);
		struct ifreq *item = &ifr[i];
		
		struct sockaddr *addr = &(item->ifr_addr);
		socklen_t salen;
		switch(addr->sa_family) {
		case AF_INET:	salen = sizeof(struct sockaddr_in);		break;
    	case AF_INET6:	salen = sizeof(struct sockaddr_in6);	break;
    	default:		salen = 0;
    	}
   
   		getnameinfo(addr, salen, hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD);

    	if(ioctl(sck, SIOCGIFADDR, item) < 0) 
      		continue;
    
    	NetAdapter &adapter = res.Add();
    	adapter.description = hostname;
    	
    	char ip[INET6_ADDRSTRLEN] = {0};
		switch(addr->sa_family) {
		case AF_INET:	inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), ip, INET6_ADDRSTRLEN);	
						adapter.ip4 = ip;
						break;
		case AF_INET6:	inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)addr)->sin6_addr), ip, INET6_ADDRSTRLEN);	
						adapter.ip6 = ip;
						break;
		}
		adapter.fullname = item->ifr_name;
		
		if(adapter.fullname.StartsWith("eth"))
			adapter.type = "ETHERNET";
		else if(adapter.fullname.StartsWith("lo"))
			adapter.type = "SOFTWARE_LOOPBACK";
		else if(adapter.fullname.StartsWith("ppp"))
			adapter.type = "MODEM";		
		else if(adapter.fullname.StartsWith("hci"))
			adapter.type = "BLUETOOTH";
		else if(adapter.fullname.StartsWith("tr"))
			adapter.type = "TOKENRING";
		else if(adapter.fullname.StartsWith("vbox") || adapter.fullname.StartsWith("wifi") ||
				adapter.fullname.StartsWith("ath"))
			adapter.type = "VIRTUALBOX";
		else if(adapter.fullname.StartsWith("wlan"))
			adapter.type = "IEEE80211";
		else if(adapter.fullname.StartsWith("vmnet"))			
			adapter.type = "VMWARE";
		else
			adapter.type = "OTHER";	
			
#ifdef SIOCGIFHWADDR
    	if(ioctl(sck, SIOCGIFHWADDR, item) < 0) 
      		continue;
    
    	MAC = Format("%02x:%02x:%02x:%02x:%02x:%02x",
					(unsigned char)item->ifr_hwaddr.sa_data[0],
					(unsigned char)item->ifr_hwaddr.sa_data[1],
					(unsigned char)item->ifr_hwaddr.sa_data[2],
					(unsigned char)item->ifr_hwaddr.sa_data[3],
					(unsigned char)item->ifr_hwaddr.sa_data[4],
					(unsigned char)item->ifr_hwaddr.sa_data[5]);
#elif SIOCGENADDR
    	if(ioctl(sck, SIOCGENADDR, item) < 0) 
      		continue;

    	MAC = Format("%02x:%02x:%02x:%02x:%02x:%02x",
					(unsigned char)item->ifr_enaddr[0],
					(unsigned char)item->ifr_enaddr[1],
					(unsigned char)item->ifr_enaddr[2],
					(unsigned char)item->ifr_enaddr[3],
					(unsigned char)item->ifr_enaddr[4],
					(unsigned char)item->ifr_enaddr[5]);
#elif __MACH__ || __NetBSD__ || __OpenBSD__ || __FreeBSD__
		int mib[6] = {0};
		int len = 0;
		char *macbuf;
		struct if_msghdr *ifm;
		struct sockaddr_dl *sdl;
		unsigned char ptr[];
      
		mib[0] = CTL_NET;
		mib[1] = AF_ROUTE;
		mib[2] = 0;
		mib[3] = AF_LINK;
		mib[4] = NET_RT_IFLIST;
		mib[5] = if_nametoindex(item->ifr_name);
		if(mib[5] == 0)
      		continue;

  		if(sysctl(mib, 6, NULL, (size_t*)&len, NULL, 0) != 0) 
      		continue;

 		macbuf = (char *) malloc(len);
    	if(macbuf == NULL) 
     		continue;

    	if(sysctl(mib, 6, macbuf, (size_t*)&len, NULL, 0) != 0) 
      		continue;

		ifm = (struct if_msghdr *)macbuf;
		sdl = (struct sockaddr_dl *)(ifm + 1);
		ptr = (unsigned char *)LLADDR(sdl);

    	MAC = Format("%02x:%02x:%02x:%02x:%02x:%02x",
						ptr[0], ptr[1], ptr[2],
						ptr[3], ptr[4], ptr[5]);

    	free(macbuf);
#else
#error OS Distribution Not Recognized
#endif
		adapter.mac = MAC;
  	}	
	return res;
}


// Not implemented yet in Linux
String GetHDSerial() {
	return Null;
}

#endif

/////////////////////////////////////////////////////////////////////
// Memory Info

#if defined(PLATFORM_WIN32)

bool GetMemoryInfo(
	int &memoryLoad, 			// percent of memory in use		
	uint64 &totalPhys, 			// physical memory				
	uint64 &freePhys, 			// free physical memory			
	uint64 &totalPageFile,		// total paging file			
	uint64 &freePageFile,		// free paging file				
	uint64 &totalVirtual,		// total virtual memory			
	uint64 &freeVirtual)		// free virtual memory			
{
	MEMORYSTATUSEX status;	
	status.dwLength = sizeof (status);
	if (!GlobalMemoryStatusEx(&status))
		return false;
	memoryLoad          = status.dwMemoryLoad;
	totalPhys			= status.ullTotalPhys;
	freePhys			= status.ullAvailPhys;
	totalPageFile		= status.ullTotalPageFile;
	freePageFile		= status.ullAvailPageFile;
	totalVirtual		= status.ullTotalVirtual;
	freeVirtual	    	= status.ullAvailVirtual;
	
	return true;
}

#else

bool GetMemoryInfo(
			int &memoryLoad, 			// percent of memory in use		
			uint64 &totalPhys, 			// physical memory				
			uint64 &freePhys, 			// free physical memory			
			uint64 &totalPageFile,		// total paging file			
			uint64 &freePageFile,		// free paging file				
			uint64 &totalVirtual,		// total virtual memory			
			uint64 &freeVirtual)
{
	StringParse meminfo = LoadFile_Safe("/proc/meminfo");
	if (meminfo == "")
		return false;
	meminfo.GoAfter_Init("MemTotal", ":");	totalPhys = 1024*meminfo.GetUInt64();
	meminfo.GoAfter_Init("MemFree", ":");	freePhys  = 1024*meminfo.GetUInt64();
	memoryLoad = (int)(100.*(totalPhys-freePhys)/totalPhys);
	meminfo.GoAfter_Init("SwapCached", ":");freePageFile = 1024*meminfo.GetUInt64();
	meminfo.GoAfter_Init("Cached", ":");	totalPageFile = 1024*meminfo.GetUInt64() + freePageFile;
	meminfo.GoAfter_Init("SwapTotal", ":");	totalVirtual = 1024*meminfo.GetUInt64();
	meminfo.GoAfter_Init("SwapFree", ":");	freeVirtual = 1024*meminfo.GetUInt64();
	
	return true;
}
#endif


/////////////////////////////////////////////////////////////////////
// Process list

#if defined(PLATFORM_WIN32) 

// Get the list of process identifiers.
bool GetProcessList(Array<int64> &pid, Array<String> &pNames)
{
	PROCESSENTRY32 proc;
	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) 
		return false;
	proc.dwSize = sizeof(proc);
	long f = Process32First(hSnap, &proc);
	while (f) {
		pid.Add(proc.th32ProcessID);
		pNames.Add(proc.szExeFile);
       	f = Process32Next(hSnap, &proc);
	}
	CloseHandle(hSnap);
	return true;	
}

Array<int64> GetProcessList()
{
	PROCESSENTRY32 proc;
	Array<int64> ret;
	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) 
		return ret;
	proc.dwSize = sizeof(proc);
	long f = Process32First(hSnap, &proc);
	while (f) {
		ret.Add(proc.th32ProcessID);
       	f = Process32Next(hSnap, &proc);
	}
	CloseHandle(hSnap);
	return ret;	
}

String GetProcessName(int64 processID)
{
	WCHAR szProcessName[MAX_PATH];
	String ret;
	
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, DWORD(processID));

    // Get the process name.
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseNameW(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(WCHAR));
            ret = FromSystemCharset(WString(szProcessName).ToString());
        }
    }
    CloseHandle(hProcess);

    return ret;
}

String GetProcessFileName(int64 processID)
{
	WCHAR szProcessName[MAX_PATH];
    String ret;
    
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, DWORD(processID));

    // Get the process name.
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleFileNameExW(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(WCHAR));
            ret = FromSystemCharset(WString(szProcessName).ToString());
        }
    }
    CloseHandle(hProcess);

    return ret;
}


ULONGLONG SubtractFILETIME(FILETIME &hasta, FILETIME &desde) {
	__int64 timeDesde = ((__int64)desde.dwHighDateTime << 32) + desde.dwLowDateTime;
	__int64 timeHasta = ((__int64)hasta.dwHighDateTime << 32) + hasta.dwLowDateTime;
	__int64 delta = timeHasta - timeDesde;
	return delta;
}

int GetProcessCPUUsage(int64 pid)
{
	HANDLE hp = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, 0, DWORD(pid));
	if (hp == NULL)
	    return Null;
	
	FILETIME iddleTime, kernelTimeS, userTimeS, kernelTimeS_0, userTimeS_0;
	FILETIME creationTime, exitTime, kernelTimeP, userTimeP, kernelTimeP_0, userTimeP_0;
    GetSystemTimes(&iddleTime, &kernelTimeS_0, &userTimeS_0);
	GetProcessTimes(hp, &creationTime, &exitTime, &kernelTimeP_0, &userTimeP_0);
	Sleep(50);		// It spends some ms elapsing the process used time
	GetSystemTimes(&iddleTime, &kernelTimeS, &userTimeS);
	GetProcessTimes(hp, &creationTime, &exitTime, &kernelTimeP, &userTimeP);
	
	int64 kernelS = SubtractFILETIME(kernelTimeS, kernelTimeS_0);
	int64 userS = SubtractFILETIME(userTimeS, userTimeS_0);
	int64 totalS = kernelS + userS;
	int64 kernelP = SubtractFILETIME(kernelTimeP, kernelTimeP_0);
	int64 userP = SubtractFILETIME(userTimeP, userTimeP_0);
	int64 totalP = kernelP + userP;

	int cpu = int((100*totalP)/totalS);
	CloseHandle(hp);
	return cpu;
}


BOOL CALLBACK EnumGetWindowsList(HWND hWnd, LPARAM lParam) 
{
	if (!hWnd)
		return TRUE;		// Not a window
	if (GetParent(hWnd) != 0)
		return TRUE;		// Child window
	Array<int64> *ret = (Array<int64> *)lParam;
	ret->Add(reinterpret_cast<int64>(hWnd));
	return TRUE;
}

void GetWindowsList(Array<int64> &hWnd, Array<int64> &processId, Array<String> &name, Array<String> &fileName, Array<String> &caption)
{
	HANDLE hProcess;
	DWORD dwThreadId, dwProcessId;
	HINSTANCE hInstance;
	WCHAR str[MAX_PATH];
	int count;
	
	EnumWindows(EnumGetWindowsList, (LPARAM)&hWnd);	
	for (int i = 0; i < hWnd.GetCount(); ++i) {
		String sstr;
		hInstance = (HINSTANCE)GetWindowLongPtr(reinterpret_cast<HWND>(hWnd[i]), GWLP_HINSTANCE);
		dwThreadId = GetWindowThreadProcessId(reinterpret_cast<HWND>(hWnd[i]), &dwProcessId);
		processId.Add(dwProcessId);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
		if (count = GetModuleFileNameExW(hProcess, hInstance, str, sizeof(str)/sizeof(WCHAR))) {
			sstr = WString(str, count).ToString();
			fileName << sstr;
		} else
			fileName << t_("Unknown process");	
		if (count = GetModuleBaseNameW(hProcess, hInstance, str, sizeof(str)/sizeof(WCHAR))) {
			sstr = WString(str, count).ToString();
			name << sstr;
		 } else
			name << t_("Unknown process");		
		CloseHandle(hProcess);
		
		if (sstr == "TPClnt.dll")		// VMWare Thinprint crashes SendMessageW()
			caption << "";	
		else if (IsWindowVisible(reinterpret_cast<HWND>(hWnd[i]))) {
			count = int(SendMessageW(reinterpret_cast<HWND>(hWnd[i]), WM_GETTEXT, sizeof(str)/sizeof(WCHAR), (LPARAM)str));
			caption << WString(str, count).ToString();	
		} else
			caption << "";	
	}
}

Array<int64> GetWindowsList()
{
	Array<int64> ret;
	EnumWindows(EnumGetWindowsList, (LPARAM)&ret);	
	return ret;
}

BOOL CALLBACK TerminateAppEnum(HWND hwnd, LPARAM lParam)
{
	DWORD dwID ;
  	GetWindowThreadProcessId(hwnd, &dwID);
  	if(dwID == (DWORD)lParam)
     	PostMessage(hwnd, WM_CLOSE, 0, 0);
  	return TRUE;
}

bool ProcessTerminate(int64 processId, int timeout)
{
  	HANDLE hProc = ::OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, DWORD(processId));
  	if(hProc == NULL)
    	return false;
  	::EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM)processId) ;

	int ret;
  	int state = ::WaitForSingleObject(hProc, timeout);
  	if ((state == WAIT_TIMEOUT) || (state == WAIT_FAILED))
     	ret = ::TerminateProcess(hProc, 0);
	else
		ret = true;
	CloseHandle(hProc);
	return ret;
}

int GetProcessPriority(int64 pid)
{
	int priority;
	HANDLE hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, DWORD(pid));
  	if(hProc == NULL) 
    	return -1;
	priority = ::GetPriorityClass(hProc);
   	CloseHandle(hProc);
   	
   	switch(priority) {
   	case REALTIME_PRIORITY_CLASS:	priority = 10;	// Process that has the highest possible priority. The threads of a real-time priority class process preempt the threads of all other processes, including operating system processes performing important tasks. For example, a real-time process that executes for more than a very brief interval can cause disk caches not to flush or cause the mouse to be unresponsive.
   									break;
   	case HIGH_PRIORITY_CLASS:		priority = 8;	// Process that performs time-critical tasks that must be executed immediately for it to run correctly. The threads of a high-priority class process preempt the threads of normal or idle priority class processes. An example is the Task List, which must respond quickly when called by the user, regardless of the load on the operating system. Use extreme care when using the high-priority class, because a high-priority class CPU-bound application can use nearly all available cycles.
   									break;
	case ABOVE_NORMAL_PRIORITY_CLASS: priority = 6; // Process that has priority above NORMAL_PRIORITY_CLASS but below HIGH_PRIORITY_CLASS.
   									break;
	case NORMAL_PRIORITY_CLASS:		priority = 5;	// Process with no special scheduling needs.
   									break;	
	case BELOW_NORMAL_PRIORITY_CLASS: priority = 3;	// Process that has priority above IDLE_PRIORITY_CLASS but below NORMAL_PRIORITY_CLASS.
   									break;	
	case IDLE_PRIORITY_CLASS:		priority = 0;	// Process whose threads run only when the system is idle and are preempted by the threads of any process running in a higher priority class. An example is a screen saver. The idle priority class is inherited by child processes.
									break;
	default:						return -1;
   	}
  	return priority;	
}
 
bool SetProcessPriority(int64 pid, int priority)
{
	HANDLE hProc = ::OpenProcess(PROCESS_SET_INFORMATION , FALSE, DWORD(pid));
  	if(hProc == NULL)
    	return false;
  	if (priority == 10)
  		priority = REALTIME_PRIORITY_CLASS;
  	else if (priority >= 8)
		priority = HIGH_PRIORITY_CLASS;
  	else if (priority >= 6)
		priority = ABOVE_NORMAL_PRIORITY_CLASS;
  	else if (priority >= 5)
		priority = NORMAL_PRIORITY_CLASS;	
  	else if (priority >= 3)
   		priority = BELOW_NORMAL_PRIORITY_CLASS;
  	else
  		priority = IDLE_PRIORITY_CLASS;
	int ret = ::SetPriorityClass(hProc, priority);		// SetProcessAffinityMask
   	CloseHandle(hProc) ;
  	return ret;	
}

#endif
#ifdef PLATFORM_POSIX			 //Check with ps

bool IsInteger(String s)
{
	for (int i = 0; i < s.GetCount(); ++i) {
		if (!isdigit(s[i]))
			return false;
	}
	return true;
}

bool GetProcessList(Array<int64> &pid, Array<String> &pNames)
{
	FindFile ff;
	if(ff.Search("/proc/*")) {
		do {
			if (IsInteger(ff.GetName())) {
				String exe = Format("/proc/%s/exe", ff.GetName());
				StringBuffer exeb;
				exeb = exe;
				char procName[2048];
				int procNameLen = readlink(exeb, procName, sizeof(procName)-1);
				if (procNameLen > 0) {
					procName[procNameLen] = 0;
					pNames.Add(procName);
					pid.Add(atoi(ff.GetName()));
				}
			}
		} while(ff.Next());
	}
	return true;
}

Array<int64> GetProcessList()
{
	FindFile ff;
	Array<int64> pid; 
	if(ff.Search("/proc/*")) {
		do {
			if (IsInteger(ff.GetName())) {
				String exe = Format("/proc/%s/exe", ff.GetName());
				StringBuffer exeb;
				exeb = exe;
				char procName[2048];
				int procNameLen = readlink(exeb, procName, sizeof(procName)-1);
				if (procNameLen > 0) 
					pid.Add(atoi(ff.GetName()));
			}
		} while(ff.Next());
	}
	return pid;
}

String GetProcessName(int64 pid)
{
	return GetFileName(GetProcessFileName(pid));
}

// ls -l /proc/%d/fd gets also the files opened by the process
String GetProcessFileName(int64 pid)
{
	String ret = "";
	String exe = Format("/proc/%s/exe", FormatLong(pid));
	StringBuffer exeb;
	exeb = exe;
	char procName[2048];
	int procNameLen = readlink(exeb, procName, sizeof(procName)-1);
	if (procNameLen > 0) {
		procName[procNameLen] = 0;
		ret = procName;
	} 
	return ret;	
}

void GetWindowsList_Rec (_XDisplay *dpy, Window w, int depth, Array<int64> &wid) 
{ 
	if (depth > 3) // 1 is enough for Gnome. 2 is necessary for Xfce and Kde
		return; 

	wid.Add(w);

	Window root, parent; 
	unsigned int nchildren; 
	Window *children = NULL; 
	if (XQueryTree (dpy, w, &root, &parent, &children, &nchildren))  {
		for (int i = 0; i < nchildren; i++) {
			XWindowAttributes windowattr;
			XGetWindowAttributes(dpy, children[i], &windowattr);
			if (windowattr.map_state == IsViewable)
				GetWindowsList_Rec (dpy, children[i], depth + 1, wid); 
		}
	}
	if (children) 
		XFree((char *)children); 
	return; 
}

Array<int64> GetWindowsList()
{
	Array<int64> ret;	
	SetSysInfoX11ErrorHandler();
	
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return ret;
	}
	GetWindowsList_Rec (dpy, RootWindow(dpy, DefaultScreen(dpy)), 0, ret);
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	return ret;
}

void GetWindowsList(Array<int64> &hWnd, Array<int64> &processId, Array<String> &nameL, Array<String> &fileName, Array<String> &caption)
{
	SetSysInfoX11ErrorHandler();
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return;
	}
	GetWindowsList_Rec(dpy, RootWindow (dpy, DefaultScreen (dpy)), 0, hWnd);
	for (int i = 0; i < hWnd.GetCount(); ++i) {
		// Get window name
		XTextProperty tp;
		if (XGetWMName(dpy, hWnd[i], &tp) == 0) 
	    	caption.Add("");
        else {
        	if (tp.nitems > 0) {
           		int count = 0, i, ret;
           		char **list = NULL;
          		ret = XmbTextPropertyToTextList(dpy, &tp, &list, &count);
            	if((ret == Success || ret > 0) && list != NULL) {
                	String sret;
              		for(i = 0; i < count; i++)
              			sret << list[i]; 
              		XFreeStringList(list);
              		caption.Add(FromSystemCharset(sret));
          		} else 
              		caption.Add(FromSystemCharset((char *)tp.value));
      		} else 
      			caption.Add("");
        }
		// Get pid
		Atom atomPID = XInternAtom(dpy, "_NET_WM_PID", true);
		unsigned long pid = 0;
		if (atomPID == None)
			processId.Add(0LL);
		else {
			Atom type;        
			int format;        
			unsigned long nItems;        
			unsigned long bytesAfter;        
			unsigned char *propPID = 0;
			if (0 == XGetWindowProperty(dpy, hWnd[i], atomPID, 0, 1024, false, XA_CARDINAL, &type, &format, &nItems, &bytesAfter, &propPID)) {
				if(propPID != 0) {
					pid = *((unsigned long *)propPID);
					processId.Add(pid);
					XFree(propPID);
				} else
					processId.Add(0LL);
			} else
				processId.Add(0LL);
		}
		if (pid != 0L)
			fileName.Add(GetProcessFileName(pid));
		else
			fileName.Add("");
		// Name and class
		XClassHint ch;
		ch.res_name = ch.res_class = NULL; 	
		Status status = XGetClassHint(dpy, hWnd[i], &ch);
		if (status != BadWindow) {
			if (ch.res_name)
				nameL.Add(ch.res_name);
			else
				nameL.Add("");
		} else
			nameL.Add("");
		if (ch.res_name) 
			XFree (ch.res_name); 
		if (ch.res_class) 
			XFree (ch.res_class); 
	}
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	return;
}    

bool WindowKill(int64 wid)
{
	if (wid == 0)
		return false;
	
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) 
		return false;

	XSync (dpy, 0);			
	XKillClient (dpy, wid);
	XSync (dpy, 0);
	
	XCloseDisplay (dpy);
	return true;	
}

// Also possible to stop or cont
bool ProcessTerminate(int64 pid, int timeout)
{
	if (!ProcessExists(pid))
		return false;
	long wid = GetWindowIdFromProcessId(pid);		// Just in case
		
	// First... SIGTERM
	kill(pid, SIGTERM);	
	Sleep(timeout/3);
	if (!ProcessExists(pid))
		return true;
	// Second... SIGKILL
	kill(pid, SIGKILL);	
	Sleep(timeout/3);
	if (!ProcessExists(pid))
		return true;
	// Third ... WindowKill
	Sleep((int)(timeout/3));
	return WindowKill(wid);
}

int GetProcessPriority(int64 pid)
{
	int priority = getpriority(PRIO_PROCESS, pid);
	return 10 - (priority + 20)/4;		// Rescale -20/20 to 10/0
}

bool SetProcessPriority(int64 pid, int priority)
{
	priority = 20 - 4*priority;
	if (0 == setpriority(PRIO_PROCESS, pid, priority))
 		return true;	
	else
		return false;
}

bool ProcessExists(int64 pid)
{
	return DirectoryExists(Format("/proc/%s", Sprintf("%ld", pid)));
}

#endif

int64 GetWindowIdFromCaption(String windowCaption, bool exactMatch)
{
	Array<int64> wid, pid;
	Array<String> name, fileName, caption;
	GetWindowsList(wid, pid, name, fileName, caption);
	for (int i = 0; i < wid.GetCount(); ++i) {
		if (exactMatch) {
			if (caption[i] == windowCaption)
				return wid[i];
		} else {
			if (caption[i].Find(windowCaption) >= 0)
				return wid[i];
		}
	}
	return -1;
} 

int64 GetProcessIdFromWindowCaption(String windowCaption, bool exactMatch)
{
	Array<int64> wid, pid;
	Array<String> name, fileName, caption;
	GetWindowsList(wid, pid, name, fileName, caption);
	for (int i = 0; i < wid.GetCount(); ++i) {
		if (exactMatch) {
			if (caption[i] == windowCaption)
				return pid[i];
		} else {
			if (caption[i].Find(windowCaption) >= 0)
				return pid[i];
		}
	}
	return -1;
}    

int64 GetProcessIdFromWindowId(int64 _wId)
{
	Array<int64> wId, pid;
	Array<String> name, fileName, caption;
	GetWindowsList(wId, pid, name, fileName, caption);
	for (int i = 0; i < pid.GetCount(); ++i) {
		if (wId[i] == _wId)
			return pid[i];
	}
	return -1;
} 

int64 GetWindowIdFromProcessId(int64 _pid)
{
	Array<int64> wId, pid;
	Array<String> name, fileName, caption;
	GetWindowsList(wId, pid, name, fileName, caption);
	for (int i = 0; i < pid.GetCount(); ++i) {
		if (pid[i] == _pid)
			return wId[i];
	}
	return -1;
} 

int64 GetProcessIdFromName(String name) 
{
	Array<int64> pid;
	Array<String> pNames;
	if (!GetProcessList(pid, pNames))
		return Null;
	name = ToLower(name);
	for (int i = 0; i < pid.GetCount(); ++i) {
		if (ToLower(pNames[i]) == name)
			return pid[i];
	}
	return Null;
}


/////////////////////////////////////////////////////////////////////
// Others

int64    GetProcessId()			{return getpid();}

/////////////////////////////////////////////////////////////////////
// Drives list
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

bool GetDriveSpace(String drive, 
	//uint64 &totalBytes,	// To determine the total number of bytes on a disk or volume, use IOCTL_DISK_GET_LENGTH_INFO.
	uint64 &freeBytesUser, 	// Total number of free bytes on a disk that are available to the user who is associated with the calling thread.
	uint64 &totalBytesUser, // Total number of bytes on a disk that are available to the user who is associated with the calling thread.
	uint64 &totalFreeBytes)	// Total number of free bytes on a disk.
{
	StringBuffer sb(drive);	
	
	if(!GetDiskFreeSpaceEx(sb, (PULARGE_INTEGER)&freeBytesUser, (PULARGE_INTEGER)&totalBytesUser, (PULARGE_INTEGER)&totalFreeBytes))
		return false;
	//totalBytes = 0;
	return true;
}

// return true if mounted
bool GetDriveInformation(String drive, String &type, String &volume, /*uint64 &serial, */int &maxName, String &fileSystem)
{
	StringBuffer sb(drive);	
	
	switch (::GetDriveType(sb)) {
	case DRIVE_UNKNOWN: 	type = "Drive unknown";		break;
   	case DRIVE_NO_ROOT_DIR: type = "The root directory does not exist";	break;
	case DRIVE_REMOVABLE:
          switch (*sb) {
          case 'A':
          case 'B': 		type = "Floppy";		
          					volume = fileSystem = "";
          					/*serial = */maxName = 0;
          					return false;
          default: 			type = "Removable";	break;
          }
          break;
   	case DRIVE_FIXED:   	type = "Hard";		break;
    case DRIVE_REMOTE:  	type = "Network";	break;
    case DRIVE_CDROM:   	type = "Optical";	break;
    case DRIVE_RAMDISK: 	type = "RAM";		break;
   	}
   	char vol[MAX_PATH], fs[MAX_PATH];
   	long flags;
   	uint64 serial;
   	uint64 _maxName;
   	if(!::GetVolumeInformation(sb, vol, MAX_PATH, (LPDWORD)&serial, (LPDWORD)&_maxName, (LPDWORD)&flags, fs, MAX_PATH)) {
   		if (type == "Optical") {
   			volume = "";
   			fileSystem = "";
   			maxName = 0;
   			return true;
   		} else
   			return false;
   	}
   	volume = vol;
   	fileSystem = fs;
   	maxName = (int)_maxName;
   	
   	return true;
}

#elif defined(PLATFORM_POSIX)

bool GetDriveSpace(String drive, 
	//uint64 &totalBytes,	// To determine the total number of bytes on a disk or volume, use IOCTL_DISK_GET_LENGTH_INFO.
	uint64 &freeBytesUser, 	// Total number of free bytes on a disk that are available to the user who is associated with the calling thread.
	uint64 &totalBytesUser, // Total number of bytes on a disk that are available to the user who is associated with the calling thread.
	uint64 &totalFreeBytes)	// Total number of free bytes on a disk.
{
	freeBytesUser = totalBytesUser = totalFreeBytes = 0;
	
	StringParse space = Sys("df -T");
	if (space == "")
		return false;
	
	while (drive != space.GetText())
		;
	space.MoveRel(-10);	space.GoBeginLine(); 
	space.GetText();	space.GetText();	// Jumps over device path and filesystem
	totalBytesUser = 1024*space.GetUInt64();
	space.GetText();						// Jumps over used space
	freeBytesUser  = totalFreeBytes = 1024*space.GetUInt64();
	return true;
}

// return true if mounted
bool GetDriveInformation(String drive, String &type, String &volume, /*uint64 &serial, */int &maxName, String &fileSystem)
{
	StringParse info = Sys("mount -l");
	if (info  == "")
		return false;
	
	String straux;
	while (drive != (straux = info.GetText()))
		if (straux == "")
			return false;
	       
	if("type" != info.GetText())		// Jumps over "type"
		return false;

	fileSystem = info.GetText();
	String details = info.GetText();
	info.GoAfter("[");
	volume = info.GetText("]");
	//serial = 0;			// Unknown		
	if ((fileSystem == "udf" || fileSystem == "iso9660") && details.Find("ro") >=0)
		type = "Optical";
	else if (details.Find("flush") >=0)
		type = "Removable";
	else
		type = "Hard";
		
	struct statfs buf;
	if (0 == statfs(drive, &buf)) 
		//puts(Format("%x", buf.f_type)); // Filesystem type
		maxName = buf.f_namelen; 	
	else 
		maxName = 0;
	
	return true;	
}

#endif

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

#ifndef SHTDN_REASON_MINOR_OTHER
	#define SHTDN_REASON_MINOR_OTHER 0
#endif

bool Shutdown(String action) {
	action = ToLower(action);
	
	bool hibernate, suspend;
	hibernate = suspend = false;
	
	unsigned int flag;
	if (action == "logoff")
		flag = EWX_LOGOFF;
	else if (action == "poweroff")
		flag = EWX_POWEROFF;
	else if (action == "reboot")
		flag = EWX_REBOOT;
	else if (action == "shutdown")
		flag = EWX_SHUTDOWN;
	else if (action == "hibernate") 
		hibernate = true;
	else if (action == "suspend")	
		suspend = true;
	else
		return false;
	
   	HANDLE hToken; 
   	TOKEN_PRIVILEGES tkp; 
 
   	// Get a token for this process. 
   	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
 		return false; 
 
   	// Get the LUID for the shutdown privilege. 
   	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
 
   	tkp.PrivilegeCount = 1;  // one privilege to set    
   	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   	// Get the shutdown privilege for this process. 
   	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES)NULL, 0); 
 
   	if (GetLastError() != ERROR_SUCCESS) 
      	return false; 
 
	if (hibernate || suspend) {
		if (!SetSuspendState(hibernate, TRUE, FALSE))
			return false;
	} else {
	   	// Shut down the system and force all applications to close. 
	   	if (!ExitWindowsEx(flag | EWX_FORCE, SHTDN_REASON_MINOR_OTHER)) 
	      	return false; 
	}
   	return true;
}
#endif

#ifdef PLATFORM_POSIX
bool Shutdown(String action) {
	if (action == "logoff") {
		kill(1, SIGTSTP);
		sync();
		signal(SIGTERM, SIG_IGN);
		setpgrp();
		kill(-1, SIGTERM);
		sleep(1);
		kill(-1, SIGHUP); //* Force PPPD's down, too *
		sleep(1);
		kill(-1, SIGKILL);
		sync();
		sleep(1);
	} else if (action == "shutdown") {
		sync();
		sleep(1);
#if __GNU_LIBRARY__ > 5
		reboot(0xCDEF0123);
#else
		reboot(0xfee1dead, 672274793, 0xCDEF0123);
#endif
		sync();
		sleep(1);
	} else if (action == "reboot") {		// LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2
		sync();
		sleep(1);
#if __GNU_LIBRARY__ > 5
		reboot(0x01234567);
#else
		reboot(0xfee1dead, 672274793, 0x01234567);
#endif
		sync();
		sleep(1);
	} 
	Exit(0);
	return true; 
}
#endif


void GetCompilerInfo(String &name, int &version, Upp::Time &time, String &mode, int &bits) {	
	time = ScanTime("mdy", String(__DATE__) + " " + __TIME__);
	
	name = "";
	version = 0;
	#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
		#if defined(__MINGW32__)
			name = "mingw";
			version = __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__;	// __VERSION__
		#elif defined(COMPILER_MSC) 
			name = "msc";
			version = _MSC_FULL_VER;
		#elif defined (__BORLANDC__)
			name = "borlandc"
			version = __BORLANDC__;
		#elif defined (__WATCOMC__)
			name = "watcomc"
			version = __WATCOMC__;
		#endif
	#elif defined (PLATFORM_POSIX)
	    #if defined(__GNUC__)
	    	name = "gnuc";
	    	version = __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__;
	    #endif
    #elif defined (__APPLE__) 
    	// In a next future?
	#endif
	#ifdef _DEBUG
		mode = "debug";
	#else
		mode = "release";
	#endif
	#if defined(_MSC_VER) && defined(_WIN64)
		bits = 64;
	#elif defined(__MINGW64__)
		bits = 64;
	#elif defined(__LP64__)
		bits = 64;
	#else
		bits = 32;
	#endif	
}


#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

bool PutWindowPlacement(HWND hwnd, RECT rcNormalPosition, POINT ptMinPosition, POINT ptMaxPosition, long showcmd, long flags)
{
    WINDOWPLACEMENT place;
        
    place.ptMinPosition = ptMinPosition;
    place.ptMaxPosition = ptMaxPosition;
    place.rcNormalPosition = rcNormalPosition;
    place.showCmd = showcmd;
    place.flags = flags;
    place.length = sizeof(place);
    return SetWindowPlacement(hwnd, &place);
}

bool TakeWindowPlacement(HWND hwnd, RECT &rcNormalPosition, POINT &ptMinPosition, POINT &ptMaxPosition, long &showcmd)
{
    WINDOWPLACEMENT place;
    
    place.length = sizeof(place);
    bool ret = GetWindowPlacement(hwnd, &place);
    if (!ret)
        return false;
    ptMinPosition = place.ptMinPosition;
    ptMaxPosition = place.ptMaxPosition;
    rcNormalPosition = place.rcNormalPosition;
    showcmd = place.showCmd;     //SW_SHOWMAXIMIZED, SW_SHOWMINIMIZED, SW_SHOWNORMAL
    
    return ret;
}

int Window_GetStatus(int64 windowId) 
{
    WINDOWPLACEMENT place;
    
    place.length = sizeof(place);
    bool ret = GetWindowPlacement((HWND)windowId, &place);
    if (!ret)
        return Null;

	switch(place.showCmd) {
	case SW_SHOWMAXIMIZED:	return WINDOW_MAXIMIZED;
	case SW_SHOWMINIMIZED:	return WINDOW_MINIMIZED;
	case SW_SHOWNORMAL:		return WINDOW_NORMAL;
	}
    return Null;
}

bool Window_GetRect(int64 windowId, long &left, long &top, long &right, long &bottom)
{
	RECT rcNormalPosition;
    POINT ptMinPosition, ptMaxPosition;
    long showcmd;	
    
	TakeWindowPlacement(reinterpret_cast<HWND>(windowId), rcNormalPosition, ptMinPosition, ptMaxPosition, showcmd);
	
	left = rcNormalPosition.left;
	top = rcNormalPosition.top;
	right = rcNormalPosition.right;
	bottom = rcNormalPosition.bottom;
	
	return true;
}

bool Window_SetRect(int64 windowId, long left, long top, long right, long bottom)
{
	RECT rcNormalPosition;
    POINT ptMinPosition, ptMaxPosition;
    long showcmd;	
    
	if (!TakeWindowPlacement(reinterpret_cast<HWND>(windowId), rcNormalPosition, ptMinPosition, ptMaxPosition, showcmd))
		return false;
	
	rcNormalPosition.left = left;
	rcNormalPosition.top = top;
	rcNormalPosition.right = right;
	rcNormalPosition.bottom = bottom;
	return PutWindowPlacement(reinterpret_cast<HWND>(windowId), rcNormalPosition, ptMinPosition, ptMaxPosition, showcmd, 0);
}

void Window_Bottom(int64 windowId)
{
	SetWindowPos(reinterpret_cast<HWND>(windowId), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE || SWP_NOSIZE || SWP_SHOWWINDOW);
}

void Window_Top(int64 windowId)
{
	SetWindowPos(reinterpret_cast<HWND>(windowId), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE || SWP_NOSIZE || SWP_SHOWWINDOW);
}

void Window_TopMost(int64 windowId)
{
	SetWindowPos(reinterpret_cast<HWND>(windowId), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE || SWP_NOSIZE || SWP_SHOWWINDOW);
}

#endif

#ifdef PLATFORM_POSIX

bool Window_GetRect(int64 windowId, long &left, long &top, long &right, long &bottom)
{
	SetSysInfoX11ErrorHandler();
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return false;
	}
	bool ret = false;
	Window rt;
	int x, y, rx, ry; 
	unsigned int width, height, bw, depth_; 
	if (XGetGeometry(dpy, windowId, &rt, &x, &y, &width, &height, &bw, &depth_)) { 
		left = x;
		top = y;
		right = x + width;
		bottom = y + height;
		ret = true;
	}
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	return ret; 
}

bool Window_SetRect(int64 windowId, long left, long top, long right, long bottom)
{
	SetSysInfoX11ErrorHandler();
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return false;
	}
	bool ret = false;

	if (XMoveWindow(dpy, windowId, left, top)) {
		if (!XResizeWindow(dpy, windowId, right-left, bottom-top))
			ret = false;
		else
			ret = true;
	}
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	return ret; 
}

#endif


#ifdef PLATFORM_POSIX

void SetDesktopWallPaper(const char *path)
{
	String desktopManager = GetDesktopManagerNew();
	
	if (desktopManager == "gnome") {
		Sys("gconftool-2 -t str -s /desktop/gnome/background/picture_filename \"" + String(path) + "\"");
		String mode;
		if (*path == '\0')
			mode = "none";		// Values "none", "wallpaper", "centered", "scaled", "stretched"
		else
			mode = "stretched";
		Sys("gconftool-2 -t str -s /desktop/gnome/background/picture_options \"" + mode + "\"");	
	} else if (desktopManager == "kde") {
			// mode values
			// 1: disabled, only background color
			// 2: tiled with first image in top left corner
			// 3: tiled with first image centered
			// 4: centered stretched with proportions kept until one side hits screen, space filled by background color
			// 5: same as 4, though wallpaper aligned to top left and space after stretching filled by tiling
			// 6: stretched to fit screen 
		int mode;
		if (*path == '\0')
			mode = 1;
		else
			mode = 6;
		Sys("dcop kdesktop KBackgroundIface setWallpaper \"" + String(path) + "\" " + AsString(mode));
	} else
		throw Exc(t_("Impossible to change Desktop bitmap"));
}

#endif

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

void SetDesktopWallPaper(const char *path)
{
    if (0 == SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (LPVOID)path, SPIF_UPDATEINIFILE || SPIF_SENDWININICHANGE))
        throw Exc(String(t_("Impossible to change Desktop bitmap")) + ": " + AsString(GetLastError()));
}
#endif



void SystemSignature::Load() {	
	GetSystemInfo(manufacturer, productName, version, numberOfProcessors, mbSerial);
	hdserial = GetHDSerial();
	userName = GetUserName();
	netAdapters = GetAdapterInfo();	
}

bool SystemSignature::operator==(const SystemSignature &other) const {
	if (!(manufacturer == other.manufacturer && productName == other.productName && 
		  version == other.version && mbSerial == other.mbSerial && 
		  numberOfProcessors == other.numberOfProcessors && hdserial == other.hdserial && 
		  userName == other.userName))
		return false;
	for (int i = 0; i < netAdapters.GetCount(); ++i) {
		if (TrimBoth(netAdapters[i].mac).IsEmpty())
			continue;
		for (int j = 0; j < other.netAdapters.GetCount(); ++j)
			if(netAdapters[i].mac == other.netAdapters[j].mac)
				return true;
	}
	return false;
}

void SystemSignature::Copy(const SystemSignature& src) {
	manufacturer <<= src.manufacturer;
	productName <<= src.productName;
	version <<= src.version;
	mbSerial <<= src.mbSerial;
	numberOfProcessors = src.numberOfProcessors;
	hdserial <<= src.hdserial;
	userName <<= src.userName;
	netAdapters.SetCount(src.netAdapters.GetCount());
	for (int i = 0; i < src.netAdapters.GetCount(); ++i)
		netAdapters[i] <<= src.netAdapters[i];
}

void SystemSignature::Xmlize(XmlIO &xml) {
	xml
		("manufacturer", manufacturer)("productName", productName)("version", version)    
		("numberOfProcessors", numberOfProcessors)("mbSerial", mbSerial)("hdserial", hdserial)
		("userName", userName)("netAdapters", netAdapters)
	;
}

void SystemSignature::Jsonize(JsonIO& json) {
	json
		("manufacturer", manufacturer)("productName", productName)("version", version)    
		("numberOfProcessors", numberOfProcessors)("mbSerial", mbSerial)("hdserial", hdserial)
		("userName", userName)("netAdapters", netAdapters)
	;	
}

void SystemSignature::Serialize(Stream& stream) {
	stream % manufacturer % productName % version % numberOfProcessors % mbSerial % hdserial
		   % userName % netAdapters;
}

void SystemOverview::Load() {	
	signature.Load();
	GetBiosInfo(biosVersion, biosReleaseDate, biosSerial);
	computerName = GetComputerName();
	GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion);
	int compilerBits;
	GetCompilerInfo(compilerName, compilerVersion, compilerTime, compilerMode, compilerBits);
}

void SystemOverview::Copy(const SystemOverview& src) {
	signature <<= src.signature;
	biosVersion <<= src.biosVersion;
	biosSerial <<= src.biosSerial;
	biosReleaseDate = src.biosReleaseDate;
	computerName <<= src.computerName;
	kernel <<= src.kernel;
	kerVersion <<= src.kerVersion;
	kerArchitecture <<= src.kerArchitecture;
	distro <<= src.distro;
	distVersion <<= src.distVersion;
	desktop <<= src.desktop;
	deskVersion <<= src.deskVersion;
	compilerName <<= src.compilerName;
	compilerVersion = src.compilerVersion;
	compilerTime = src.compilerTime;
	compilerMode <<= src.compilerMode;	
}

void SystemOverview::Xmlize(XmlIO &xml) {
	xml
		("signature", signature)("biosVersion", biosVersion)("biosSerial", biosSerial)
		("biosReleaseDate", biosReleaseDate)("computerName", computerName)("kernel", kernel)
		("kerVersion", kerVersion)("kerArchitecture", kerArchitecture)("distro", distro)
		("distVersion", distVersion)("desktop", desktop)("deskVersion", deskVersion)
		("compilerName", compilerName)("compilerVersion", compilerVersion)
		("compilerTime", compilerTime)("compilerMode", compilerMode)
	;
}

void SystemOverview::Jsonize(JsonIO& json) {
	json
		("signature", signature)("biosVersion", biosVersion)("biosSerial", biosSerial)
		("biosReleaseDate", biosReleaseDate)("computerName", computerName)("kernel", kernel)
		("kerVersion", kerVersion)("kerArchitecture", kerArchitecture)("distro", distro)
		("distVersion", distVersion)("desktop", desktop)("deskVersion", deskVersion)
		("compilerName", compilerName)("compilerVersion", compilerVersion)
		("compilerTime", compilerTime)("compilerMode", compilerMode)
	;	
}

void SystemOverview::Serialize(Stream& stream) {
	stream % signature % biosVersion % biosSerial % biosReleaseDate % computerName % kernel
		 % kerVersion % kerArchitecture % distro % distVersion % desktop % deskVersion
		 % compilerName % compilerVersion % compilerTime % compilerMode;
}

#ifdef PLATFORM_POSIX
static void GetPorts(SortedIndex<int> &ports, const char *file) {
	StringParse text;
	text = LoadFile_Safe(String("/proc/net/") + file);	
	text.GetLine();
	while (true) {
		StringParse line = text.GetLine();
		if (line.IsEmpty())
			break;
		line.GoAfter(":");
		line.GoAfter(":");
		String sport = line.GetText();
		unsigned int port;
		sscanf(~sport, "%x", &port);
		ports.FindAdd(port);
	}
}
#endif

int GetAvailableSocketPort(int from) {
	SortedIndex<int> ports;	
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	DWORD size = sizeof(MIB_TCPTABLE);
	Buffer<char> table(size);
	MIB_TCPTABLE *pTable = (MIB_TCPTABLE*)~table;
	if (GetTcpTable(pTable, &size, TRUE) == ERROR_INSUFFICIENT_BUFFER) 
		table.Alloc(size);
	pTable = (MIB_TCPTABLE*)~table;	
    if (GetTcpTable(pTable, &size, TRUE) != NO_ERROR)
    	return Null;

  	for (int i = 0; i < (int)pTable->dwNumEntries; i++) 
  		ports.FindAdd(ntohs((u_short)pTable->table[i].dwLocalPort));
#else
	GetPorts(ports, "tcp");
	GetPorts(ports, "raw");
	GetPorts(ports, "udp");
#endif
  	int i;
  	for (i = 0; i < ports.GetCount() && ports[i] < from; ++i)
  		;
  	if (i == ports.GetCount())
  		return from;
  	for (; ports[i] == from; ++i, ++from)
  		;
  	return from;
}

END_UPP_NAMESPACE