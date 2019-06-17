#ifndef _ide_Android_Devices_h_
#define _ide_Android_Devices_h_

#include <Core/Core.h>

namespace Upp {

class AndroidDevice : public Moveable<AndroidDevice> {
public:
	String GetSerial() const { return serial; }
	String GetState() const  { return state; }
	String GetUsb() const    { return usb; }
	String GetModel() const  { return model; }
	
	void SetSerial(const String& serial) { this->serial = serial; }
	void SetState(const String& state)   { this->state = state; }
	void SetUsb(const String& usb)       { this->usb = usb; }
	void SetModel(const String& model)   { this->model = model; }
	
public:
	bool IsEmulator() const       { return !usb.IsEmpty(); }
	bool IsPhysicalDevice() const { return !IsEmulator(); }
	
private:
	String serial;
	String state;
	String usb;
	String model;
};

class AndroidVirtualDevice : public Moveable<AndroidVirtualDevice> {
public:
	String GetName() const       { return name; }
	String GetDeviceType() const { return deviceType; }
	String GetPath() const       { return path; }
	String GetTarget() const     { return target; }
	String GetAbi() const        { return abi; }
	Size   GetSize() const       { return size; }
	
	void SetName(const String& name)             { this->name = name; }
	void SetDeviceType(const String& deviceType) { this->deviceType = deviceType; }
	void SetPath(const String& path)             { this->path = path; }
	void SetTarget(const String& target)         { this->target = target; }
	void SetAbi(const String& abi)               { this->abi = abi; }
	void SetSize(const Size& size)               { this->size = size; }
	
private:
	String name;
	String deviceType;
	String path;
	String target;
	String abi;
	Size   size;
};

}

#endif
