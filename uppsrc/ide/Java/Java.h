#ifndef JAVA_H
#define JAVA_H

#include <Core/Core.h>

#include <ide/Core/Core.h>

NAMESPACE_UPP

class Java {
public:
	static String GetDelimiter();

private:
	Java();
	Java(const Java&);
	Java& operator=(const Java&);
	virtual ~Java();
};

class Jdk {
public:
	static String GetDownloadUrl();
	
public:
	Jdk();
	Jdk(const String& path);
	virtual ~Jdk();
	
	bool Validate() const;
	
public:
	String GetBinDir() const        { return path + DIR_SEPS + "bin"; }
	
	String GetJarPath() const       { return GetBinDir() + DIR_SEPS + "jar" + GetExeExt(); }
	String GetJavacPath() const     { return GetBinDir() + DIR_SEPS + "javac" + GetExeExt(); }
	String GetJavadocPath() const   { return GetBinDir() + DIR_SEPS + "javadoc" + GetExeExt(); }
	String GetJavahPath() const     { return GetBinDir() + DIR_SEPS + "javah" + GetExeExt(); }
	String GetJavapPath() const     { return GetBinDir() + DIR_SEPS + "javap" + GetExeExt(); }
	String GetJdbPath() const       { return GetBinDir() + DIR_SEPS + "jdb" + GetExeExt(); }
	
	String GetJarsignerPath() const { return GetBinDir() + DIR_SEPS + "jarsigner" + GetExeExt(); }
	String GetKeytoolPath() const   { return GetBinDir() + DIR_SEPS + "keytool" + GetExeExt(); }
	
public:
	String GetPath() { return this->path; }
	
	void SetPath(const String& path) { this->path = path; }
	
private:
	String path;
};

END_UPP_NAMESPACE

#endif
