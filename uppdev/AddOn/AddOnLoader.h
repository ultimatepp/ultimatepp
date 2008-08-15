#ifndef _AddOn_AddOnLoader_h_
#define _AddOn_AddOnLoader_h_

#include <Core/Core.h>

using namespace Upp;

#include <AddOn/AddOn.h>

#ifdef PLATFORM_POSIX
#include <dlfcn.h>
#endif


template <class T>
class AddOnLoader
{
public:
	AddOnLoader(){};
	~AddOnLoader();

	int LoadFile(String name);
	int LoadFolder(String name);

	int GetCount();
	T* At(int idx);
	T* operator[](int idx) {return At(idx);}

private:
	AddOnLoader(const AddOnLoader&);

	bool Load(String name);

	struct Item {
		DLLHANDLE hmod;
		T* address;
	};

	Array<Item> addOns;

	typedef IAddOn*(*Start)();
	typedef void(*Stop)();

#if defined PLATFORM_WIN32
	typedef HMODULE DLLHANDLE;
#else
	typedef void* DLLHANDLE;
#endif

	static const String sharedLibExt;
};

#if defined PLATFORM_WIN32  || defined PLATFORM_WINCE
	template <class T> const String AddOnLoader<T>::sharedLibExt = ".dll";
#elif defined PLATFORM_LINUX || defined PLATFORM_FREEBSD || defined PLATFORM_SOLARIS
	template <class T> const String AddOnLoader<T>::sharedLibExt = ".so";
#elif defined PLATFORM_OSX11
	template <class T> const String AddOnLoader<T>::sharedLibExt = ".dylib";
#endif

template <class T>
AddOnLoader<T>::~AddOnLoader()
{
	for (int i = 0; i < addOns.GetCount(); i++) {
		#if defined PLATFORM_WIN32 || defined PLATFORM_WINCE
		Stop stop = (Stop)GetProcAddress(addOns[i].hmod, "Stop");
		#else
		Stop stop = (Stop)dlsym(addOns[i].hmod, "Stop");
		#endif
		if (stop) {
			stop();
		}
		#if defined PLATFORM_WIN32 || defined PLATFORM_WINCE
    	FreeLibrary(addOns[i].hmod);
    	#else
    	dlclose(addOns[i].hmod);
    	#endif
	}
	addOns.Clear();
}

template <class T>
int AddOnLoader<T>::LoadFile(String name)
{
	if (FileExists(name)) {
		return Load(name)?1:0;
	} else {
		return 0;
	}
}

template <class T>
int AddOnLoader<T>::LoadFolder(String name)
{
	int num = 0;
	for(FindFile ff(AppendFileName(name, "*.*")); ff; ff.Next()) {
		if (ff.IsFolder()) {
			num += LoadFolder(AppendFileName(name, ff.GetName()));
		} else if (ff.IsFile() && ToLower(GetFileExt(ff.GetName()) == sharedLibExt)) {
			num += Load(AppendFileName(name, ff.GetName()));
		}
	}
	return num;
}

template <class T>
bool AddOnLoader<T>::Load(String name)
{
	bool status = false;
	#if defined PLATFORM_WIN32 || defined PLATFORM_WINCE
	DLLHANDLE hmod = LoadLibrary(NormalizePath(name));
	#else
	DLLHANDLE hmod = dlopen(NormalizePath(name), RTLD_LAZY);
	#endif

	if (hmod) {
		#if defined PLATFORM_WIN32 || defined PLATFORM_WINCE
		Start start = (Start)GetProcAddress(hmod, "Start");
		#else
		Start start = (Start)dlsym(hmod, "Start");
		#endif
		T* plugin;
		if (start && (plugin = dynamic_cast<T*>(start()))) {
			Item& addOn = addOns.Add();
			addOn.hmod = hmod;
			addOn.address = plugin;
			status = true;
		} else {
			#if defined PLATFORM_WIN32 || defined PLATFORM_WINCE
			FreeLibrary(hmod);
			#else
			dlclose(hmod);
			#endif
		}
	}
	return status;
}

template <class T>
int AddOnLoader<T>::GetCount()
{
	return addOns.GetCount();
}

template <class T>
T* AddOnLoader<T>::At(int idx)
{
	if (idx < addOns.GetCount()) {
		return addOns[idx].address;
	} else {
		return NULL;
	}
}

#endif
