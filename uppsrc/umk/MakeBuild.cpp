#include "umake.h"

void Ide::ConsoleShow()
{
}

void Ide::ConsoleSync()
{
}

Vector<String> Ide::PickErrors()
{
	return console.PickErrors();
}

void Ide::DoProcessEvents()
{
}

void Ide::ReQualifyCodeBase()
{
}

String Ide::GetMain()
{
	return main;
}

void Ide::BeginBuilding(bool clear_console)
{
	SetupDefaultMethod();
	SetHdependDirs();
	HdependTimeDirty();
	build_time = msecs();
}

void Ide::EndBuilding(bool ok)
{
	console.EndGroup();
	console.Wait();
	Vector<String> errors = console.PickErrors();
	for(String p: errors)
		DeleteFile(p);
	if(!errors.IsEmpty())
		ok = false;
	PutConsole("");
	PutConsole((ok ? "OK. " : "There were errors. ") + GetPrintTime(build_time));
}

void Ide::ConsoleClear()
{
}

void Ide::SetupDefaultMethod()
{
}

void Ide::ClearErrorEditor()
{
}

void Ide::SetErrorEditor()
{
}

