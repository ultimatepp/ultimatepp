#include "FSMonTest.h"

void FSMonTest::Log(String const &s)
{
	logEdit.Set(logEdit.Get() + s + "\n");
	logEdit.SetCursor(INT_MAX);
}

void FSMonTest::startCb(void)
{
	String path = AppendFileName(GetHomeDirectory(), "FSMonTest_A");
	Log("Starting monitor for '" + path + "'");
	if(!DirectoryExists(path))
		RealizeDirectory(path);
	fsmMon.Add(path);
}

void FSMonTest::stopCb(void)
{
	Log("Stopping\n");
	String path = AppendFileName(GetHomeDirectory(), "FSMonTest_A");
	fsmMon.Remove(path);
}

void FSMonTest::quitCb(void)
{
	stopCb();
	Break();
}

void FSMonTest::monitorCb()
{
	Vector<FSMon::Info> infos = fsmMon.GetChanged();
	for(int iChange = 0; iChange < infos.GetCount(); iChange++)
	{
		FSMon::Info const &info = infos[iChange];
		switch(info.flags)
		{
			case FSMon::FSM_NOP :
				Log(String("NO-OP EVENT"));
				break;
			case FSMon::FSM_Created :
				Log(String("Creating file '") + info.path + "'");
				break;
			case FSMon::FSM_Deleted :
				Log(String("Deleting file '") + info.path + "'");
				break;
			case FSMon::FSM_Moved :
				Log(String("Moving file '") + info.path + "' to '" + info.newPath + "'");
				break;
			case FSMon::FSM_FolderCreated :
				Log(String("Creating folder '") + info.path + "'");
				break;
			case FSMon::FSM_FolderDeleted :
				Log(String("Deleting folder '") + info.path + "'");
				break;
			case FSMon::FSM_FolderMoved :
				Log(String("Moving folder '") + info.path + "' to '" + info.newPath + "'");
				break;
			case FSMon::FSM_Modified :
				Log(String("Modifying file '") + info.path + "'");
				break;
			case FSMon::FSM_AttribChange :
				Log(String("Modifying file  attributes for '") + info.path + "'");
				break;
		}
	}
}

FSMonTest::FSMonTest()
{
	CtrlLayout(*this, "Window title");
	fsmMon.EventHandler = THISBACK(monitorCb);
	startBtn <<= THISBACK(startCb);
	stopBtn <<= THISBACK(stopCb);
	quitBtn <<= THISBACK(quitCb);
}

GUI_APP_MAIN
{
	FSMonTest().Run();
}
