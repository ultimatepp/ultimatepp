#include "umake.h"

Ide::Ide()
{
	verbose = false;
}

bool Ide::IsVerbose() const
{
	return verbose;
}

void Ide::PutConsole(const char *s)
{
	console << s << "\n";
}

void Ide::PutVerbose(const char *s)
{
	if(console.verbosebuild)
		PutConsole(s);
}

const Workspace& Ide::IdeWorkspace() const
{
	return wspc;
}

bool Ide::IdeIsBuilding() const
{
	return true;
}

String Ide::IdeGetOneFile() const
{
	return onefile;
}

int Ide::IdeConsoleExecute(const char *cmdline, Stream *out, const char *envptr, bool quiet, bool)
{
	return console.Execute(cmdline, out, envptr, quiet);
}

int Ide::IdeConsoleExecuteWithInput(const char *cmdline, Stream *out, const char *envptr, bool quiet, bool noconvert)
{
	return 0;
}

int Ide::IdeConsoleExecute(One<AProcess> process, const char *cmdline, Stream *out, bool quiet)
{
	return console.Execute(pick(process), cmdline, out, quiet);
}

int Ide::IdeConsoleAllocSlot()
{
	return console.AllocSlot();
}

bool Ide::IdeConsoleRun(const char *cmdline, Stream *out, const char *envptr, bool quiet, int slot, String key, int blitz_count)
{
	return console.Run(cmdline, out, envptr, quiet, slot, key, blitz_count);
}

bool Ide::IdeConsoleRun(One<AProcess> process, const char *cmdline, Stream *out, bool quiet, int slot, String key, int blitz_count)
{
	return console.Run(pick(process), cmdline, out, quiet, slot, key, blitz_count);
}

void Ide::IdeConsoleFlush()
{
	console.Flush();
}

void Ide::IdeConsoleBeginGroup(String group)
{
	console.BeginGroup(group);
}

void Ide::IdeConsoleEndGroup()
{
	console.EndGroup();
}

bool Ide::IdeConsoleWait()
{
	return console.Wait();
}

bool Ide::IdeConsoleWait(int slot)
{
	console.Wait(0);
	return true;
}

void Ide::IdeConsoleOnFinish(Event<> cb)
{
	console.OnFinish(cb);
}

bool Ide::IdeIsDebug() const
{
	return false;
}

void Ide::IdeEndDebug()
{
}

void Ide::IdeSetBottom(Ctrl& ctrl)
{
}

void Ide::IdeActivateBottom()
{
}

void Ide::IdeRemoveBottom(Ctrl& ctrl)
{
}

void Ide::IdeSetRight(Ctrl& ctrl)
{
}

void Ide::IdeRemoveRight(Ctrl& ctrl)
{
}

String Ide::IdeGetFileName() const
{
	return Null;
}

String Ide::IdeGetFileName()
{
	return Null;
}

int Ide::IdeGetFileLine()
{
	return 0;
}

String Ide::IdeGetLine(int i) const
{
	return Null;
}

void Ide::IdeSetDebugPos(const String& fn, int line, const Image& img, int i)
{
}

void Ide::IdeHidePtr()
{
}

bool Ide::IdeDebugLock()
{
	return false;
}

bool Ide::IdeDebugUnLock()
{
	return false;
}

bool Ide::IdeIsDebugLock() const
{
	return false;
}

void Ide::IdeSetBar()
{
}

void Ide::IdeGotoCodeRef(String link)
{
}

void Ide::IdeOpenTopicFile(const String& file)
{
}

void Ide::IdeFlushFile()
{
}

String Ide::IdeGetNestFolder()
{
	return Null;
}

#if defined(__APPLE__) && defined(flagLINUX) // we are on macos building using make

#include <Draw/Draw.h>

namespace Upp {

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;
	return fi;
}

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	return GlyphInfo();
}

Vector<FaceInfo> GetAllFacesSys()
{
	Vector<FaceInfo> r;
	return r;
}

String GetFontDataSys(Font font)
{
	return String();
}

void RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int chr, Font font) {}

};

#endif