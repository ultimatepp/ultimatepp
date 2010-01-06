#ifndef _SysInfo_SysInfo_h
#define _SysInfo_SysInfo_h

#include "Functions4U/Functions4U.h"

using namespace Upp;


/////////////////////////////////////////////////////////////////////
// Processor Info
void GetSystemInfo(String &manufacturer, String &productName, String &version, int &numberOfProcessors);
void GetBiosInfo(String &biosVersion, Date &biosReleaseDate);
bool GetProcessorInfo(int number, String &vendor, String &identifier, String &architecture, int &speed);		
// Gets the real CPU speed in MHz
int GetCpuSpeed();

#if defined(PLATFORM_WIN32) 
bool GetVideoInfo(Array <Value> &name, Array <Value> &description, Array <Value> &videoProcessor, 
												Array <Value> &ram, Array <Value> &videoMode);
bool GetPackagesInfo(Array <Value> &name, Array <Value> &version, Array <Value> &vendor, 
			Array <Value> &installDate, Array <Value> &caption, Array <Value> &description, Array <Value> &state);
#endif

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
void GetWindowsList(Array<long> &wid, Array<long> &pid, Array<String> &name, Array<String> &fileName, Array<String> &title);
Array<long> GetWindowsList();

/////////////////////////////////////////////////////////////////////
// Process list
// They get arrays with handles to all the opened processes and process names 
bool GetProcessList(Array<long> &pid, Array<String> &pNames);
Array<long> GetProcessList();
String GetProcessName(long pid);
// Gets the program file name of a process
String GetProcessFileName(long processID);

// Gets the process id of a program with a window with certain title
long GetProcessIdFromWindowCaption(String windowCaption, bool exactMatch = false);

long GetWindowIdFromCaption(String windowCaption, bool exactMatch = false);

long GetProcessIdFromWindowId(long wid);
long GetWindowIdFromProcessId(long pid);

// Ends a process by any means
bool ProcessTerminate(long pid, int timeout = 500);

// Gets the process priority as a number from 0 (minimum) to 10 (maximum)
int GetProcessPriority(long pid);
bool SetProcessPriority(long pid, int priority);

// True if a process with handle pid exists
bool ProcessExists(long pid);

/////////////////////////////////////////////////////////////////////
// Os Info
bool GetOsInfo(String &kernel, String &kerVersion, String &kerArchitecture, String &distro, String &distVersion, String &desktop, String &deskVersion);
String GetDesktopManagerNew();

/////////////////////////////////////////////////////////////////////
// Get drives info
// Return false if drive is not mounted or it is not accesible
bool GetDriveSpace(String drive, uint64 &freeBytesUser, uint64 &totalBytesUser, uint64 &totalFreeBytes);
bool GetDriveInformation(String drive, String &type, String &volume, /*uint64 &serial, */int &maxName, String &fileSystem);

/////////////////////////////////////////////////////////////////////
// Others
// Gets process id
long GetProcessId();

// I tries to "logoff", "reboot" or "shutdown"
bool Shutdown(String action);

// It gets compiler info
void GetCompilerInfo(String &name, int &version, String &date);

// It gets info about the battery status
bool GetBatteryStatus(bool &discharging, int &percentage, int &remainingMin);
// Get if there is battery
bool GetBatteryInfo(bool &present/*, int &designCapacity, int &lastFullCapacity, String &vendor, String &type, String &model, String &serial*/);	

bool OpenCDTray(String drive);
bool CloseCDTray(String drive);

/////////////////////////////////////////////////////////////////////
// Key and mouse keys

bool Window_GetRect(long windowId, long &left, long &top, long &right, long &bottom);

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

void Window_SetRect(long windowId, long left, long top, long right, long bottom);

bool Record_Desktop(String fileName, int duration, int secsFrame = 1, bool viewMouse = true);
bool Record_DesktopRectangle(String fileName, int duration, int left, int top, int width, int height, int secsFrame = 1, bool viewMouse = true);
bool Record_Window(String fileName, int duration, long handle, int secsFrame = 1, bool viewMouse = true);

#endif


void SetDesktopWallPaper(const char *path);

#endif


// Known bugs
// GetWindowsList does not get the window title in Kde
// Shutdown in Linux only works with option "logoff", probably because of user permissions
