#ifndef _SysInfo_SysInfo_h
#define _SysInfo_SysInfo_h

#include "Functions4U/Functions4U.h"


/////////////////////////////////////////////////////////////////////
// Processor Info
void GetSystemInfo(String &manufacturer, String &productName, String &version, int &numberOfProcessors, String &mbSerial);
void GetBiosInfo(String &biosVersion, Date &biosReleaseDate, String &biosSerial);
bool GetProcessorInfo(int number, String &vendor, String &identifier, String &architecture, int &speed);		
int GetCpuSpeed();
double GetCpuTemperature();

struct NetAdapter : DeepCopyOption<NetAdapter> {
	String description;
	String fullname;
	String mac;
	String type;
	
	void Xmlize(XmlIO xml);
	void Jsonize(JsonIO& json);
	void Serialize(Stream& stream);
	void Copy(const NetAdapter& src);
	NetAdapter(const NetAdapter& src, int) 	{Copy(src);}
	//NetAdapter(const NetAdapter& src) 		{Copy(src);}
	NetAdapter() {}
};

Upp::Array <NetAdapter> GetAdapterInfo();

#if defined(PLATFORM_WIN32) 
bool GetVideoInfo(Upp::Array <Value> &name, Upp::Array <Value> &description, Upp::Array <Value> &videoProcessor, 
				  Upp::Array <Value> &ram, Upp::Array <Value> &videoMode);
bool GetPackagesInfo(Upp::Array <Value> &name, Upp::Array <Value> &version, Upp::Array <Value> &vendor, 
			Upp::Array <Value> &installDate, Upp::Array <Value> &caption, Upp::Array <Value> &description, Upp::Array <Value> &state);
#endif

String GetHDSerial();

/////////////////////////////////////////////////////////////////////
// Memory Info
bool GetMemoryInfo(int &memoryLoad, uint64 &totalPhys, uint64 &freePhys, uint64 &totalPageFile, uint64 &freePageFile, uint64 &totalVirtual, uint64 &freeVirtual);

/////////////////////////////////////////////////////////////////////
// Windows list
// They get arrays with handles to all the opened windows with additional info as 
// pid: Handle to the process that manages the window
// name: Window name
// fileName: Window process program file name
// title: Window title (caption)
void GetWindowsList(Upp::Array<long> &wid, Upp::Array<long> &pid, Upp::Array<String> &name, 
					Upp::Array<String> &fileName, Upp::Array<String> &title);
Upp::Array<long> GetWindowsList();

/////////////////////////////////////////////////////////////////////
// Process list
bool GetProcessList(Upp::Array<long> &pid, Upp::Array<String> &pNames);
Upp::Array<long> GetProcessList();
String GetProcessName(long pid);
String GetProcessFileName(long processID);

long GetProcessIdFromWindowCaption(String windowCaption, bool exactMatch = false);

long GetWindowIdFromCaption(String windowCaption, bool exactMatch = false);

long GetProcessIdFromWindowId(long wid);
long GetWindowIdFromProcessId(long pid);

bool ProcessTerminate(long pid, int timeout = 500);

int GetProcessPriority(long pid);
bool SetProcessPriority(long pid, int priority);

bool ProcessExists(long pid);

/////////////////////////////////////////////////////////////////////
// Os Info
bool GetOsInfo(String &kernel, String &kerVersion, String &kerArchitecture, String &distro, String &distVersion, String &desktop, String &deskVersion);
String GetDesktopManagerNew();

/////////////////////////////////////////////////////////////////////
// Get drives info
bool GetDriveSpace(String drive, uint64 &freeBytesUser, uint64 &totalBytesUser, uint64 &totalFreeBytes);
bool GetDriveInformation(String drive, String &type, String &volume, /*uint64 &serial, */int &maxName, String &fileSystem);

/////////////////////////////////////////////////////////////////////
// Others
long GetProcessId();

bool Shutdown(String action);

void GetCompilerInfo(String &name, int &version, Time &time, String &mode);
void GetCompilerInfo(String &name, int &version, String &time, String &mode);

bool GetBatteryStatus(bool &discharging, int &percentage, int &remainingMin);
bool GetBatteryInfo(bool &present/*, int &designCapacity, int &lastFullCapacity, String &vendor, String &type, String &model, String &serial*/);	

bool OpenCDTray(String drive);
bool CloseCDTray(String drive);

/////////////////////////////////////////////////////////////////////
// Key and mouse keys
bool Window_GetRect(long windowId, long &left, long &top, long &right, long &bottom);
bool Window_SetRect(long windowId, long left, long top, long right, long bottom);

bool Mouse_GetPos(long &x, long &y);
bool Mouse_SetPos(long x, long y, long windowId);

void Mouse_LeftClick();
void Mouse_LeftDown();
void Mouse_LeftUp();
void Mouse_MiddleClick(); 
void Mouse_MiddleDown();
void Mouse_MiddleUp();
void Mouse_RightClick();
void Mouse_RightDown();
void Mouse_RightUp();
void Mouse_LeftDblClick();
void Mouse_MiddleDblClick();
void Mouse_RightDblClick();

void Keyb_SendKeys(String text, long finalDelay = 100, long delayBetweenKeys = 50);

bool Window_SaveCapture(long windowId, String fileName, int left = -1, int top = -1, int width = -1, int height = -1);

bool Snap_Desktop(String fileName);
bool Snap_DesktopRectangle(String fileName, int left, int top, int width, int height);
bool Snap_Window(String fileName, long handle);

bool GetKeyLockStatus(bool &caps, bool &num, bool &scroll);
bool SetKeyLockStatus(bool caps, bool num, bool scroll);

#if defined(PLATFORM_WIN32)

bool Record_Desktop(String fileName, int duration, int secsFrame = 1, bool viewMouse = true);
bool Record_DesktopRectangle(String fileName, int duration, int left, int top, int width, int height, int secsFrame = 1, bool viewMouse = true);
bool Record_Window(String fileName, int duration, long handle, int secsFrame = 1, bool viewMouse = true);

#endif

void SetDesktopWallPaper(const char *path);


struct SystemSignature : DeepCopyOption<SystemSignature> {
	String manufacturer, productName, version, mbSerial;
	int numberOfProcessors;	
	String hdserial;
	String userName;
	Array <NetAdapter> netAdapters;
	
	void Load();
	String ToString() const			{return StoreAsJson(*this, true);}
	operator String() const			{return ToString();}
	operator const char *() const	{return ToString();}
	String operator~() const		{return ToString();}
	void Copy(const SystemSignature& src);
	SystemSignature(const SystemSignature& src, int) {Copy(src);}
	SystemSignature()	{}
	void Xmlize(XmlIO xml);
	void Jsonize(JsonIO& json);
	void Serialize(Stream& stream);
	bool operator==(const SystemSignature &other) const;
	bool operator!=(const SystemSignature &other) const {return !(*this == other);}	
};

struct SystemOverview : DeepCopyOption<SystemOverview> {
	SystemSignature signature;
	String biosVersion, biosSerial;
	Date biosReleaseDate;
	String computerName;
	String kernel;
	String kerVersion;
	String kerArchitecture;
	String distro;
	String distVersion;
	String desktop;
	String deskVersion;
	String compilerName;
	int compilerVersion;
	Time compilerTime;
	String compilerMode;
	
	void Load();
	String ToString() const			{return StoreAsJson(*this, true);}
	operator String() const			{return ToString();}
	operator const char *() const	{return ToString();}
	String operator~() const		{return ToString();}
	void Copy(const SystemOverview& src);
	SystemOverview(const SystemOverview& src, int) { Copy(src); }	
	SystemOverview() {}
	void Xmlize(XmlIO xml);
	void Jsonize(JsonIO& json);
	void Serialize(Stream& stream);
};


#endif

// Known bugs
// GetWindowsList does not get the window title in Kde
// Shutdown in Linux only works with option "logoff", probably because of user permissions
