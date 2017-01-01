#ifndef _Java_Java_h_
#define _Java_Java_h_

#include <Core/Core.h>
#include <ide/Core/Core.h>

namespace Upp {

class JavaVersion {
public:
	JavaVersion();
	JavaVersion(int major, int minor);
	
	bool IsGreate(int major) const;
	
	bool IsGreaterOrEqual(int major) const;
	bool IsGreaterOrEqual(int major, int minor) const;
	
	String ToString() { return IntStr(mMajor) + "." + IntStr(mMinor); }
	
private:
	int mMajor;
	int mMinor;
};

class Java {
public:
	Java() = delete;
	Java(const Java&) = delete;
	Java& operator=(const Java&) = delete;
	virtual ~Java() = delete;
	
	static String GetDelimiter();
};

class Jdk {
public:
	static String GetDownloadUrl();
	
public:
	Jdk(const String& path, Host* host);
	virtual ~Jdk() {}
	
	bool Validate() const;
	
public:
	const JavaVersion& GetVersion() const  { return version; }
	
	String GetBinDir() const               { return path + DIR_SEPS + "bin"; }
	String GetIncludeDir() const           { return path + DIR_SEPS + "include"; }
	
	String GetJarPath() const              { return GetBinDir() + DIR_SEPS + "jar" + GetExeExt(); }
	String GetJavacPath() const            { return GetBinDir() + DIR_SEPS + "javac" + GetExeExt(); }
	String GetJavadocPath() const          { return GetBinDir() + DIR_SEPS + "javadoc" + GetExeExt(); }
	String GetJavahPath() const            { return GetBinDir() + DIR_SEPS + "javah" + GetExeExt(); }
	String GetJavapPath() const            { return GetBinDir() + DIR_SEPS + "javap" + GetExeExt(); }
	String GetJdbPath() const              { return GetBinDir() + DIR_SEPS + "jdb" + GetExeExt(); }
	
	String GetJarsignerPath() const        { return GetBinDir() + DIR_SEPS + "jarsigner" + GetExeExt(); }
	String GetKeytoolPath() const          { return GetBinDir() + DIR_SEPS + "keytool" + GetExeExt(); }
	
	String GetPath() const                 { return this->path; }

public:
	void SetPath(const String& path) { this->path = path; }
	void SetVersion(const String& version);
	
private:
	void FindVersion(Host* host);
	
private:
	String       path;
	JavaVersion  version;
};

}

#endif
