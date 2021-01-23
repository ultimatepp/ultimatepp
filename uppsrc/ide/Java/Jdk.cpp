#include "Java.h"

namespace Upp {

#define METHOD_NAME "Jdk::" << UPP_FUNCTION_NAME << "(): "

String Jdk::GetDownloadUrl()
{
	return "https://www.azul.com/downloads/zulu-community/?package=jdk";
}

Jdk::Jdk(const String& path, Host* host)
	: path(path)
{
	ASSERT_(host, "Host is null.");
	
	FindVersion(host);
}

bool Jdk::Validate() const
{
	if(!FileExists(GetJavacPath())) return false;
	
	return true;
}

void Jdk::FindVersion(Host* host)
{
	if (!Validate()) {
		Logw() << METHOD_NAME << "Path to JDK is wrong or files are corrupted.";
		return;
	}
	
	StringStream ss;
	if (host->Execute(GetJavacPath() + " -version", ss) != 0) {
		Logw() << METHOD_NAME << "Cannot obtain version due to command execution failure.";
		return;
	}
	
	String output = static_cast<String>(ss);
	output.Replace("\n", "");
	Vector<String> splitedOutput = Split(output, " ");
	if (splitedOutput.GetCount() != 2) {
		Logw() << METHOD_NAME << "Splited output is too short (" + output + ").";
		return;
	}
	
	Vector<String> splitedVersion = Split(splitedOutput[1], ".");
	if (splitedVersion.GetCount() != 3) {
		Logw() << METHOD_NAME << "Splited version is too short (" + output + ").";
		return;
	}
	
	int major = StrInt(splitedVersion[0]);
	if (major == INT_NULL) {
		Logw() << METHOD_NAME << "Major version conversion to int failed (" + splitedVersion[0] + ").";
		return;
	}
	
	int minor = StrInt(splitedVersion[1]);
	if (minor == INT_NULL) {
		Logw() << METHOD_NAME << "Minor version conversion to int failed (" + splitedVersion[1] + ").";
		return;
	}
	
	version = JavaVersion(major, minor);
}

}
