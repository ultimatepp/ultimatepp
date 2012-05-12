#include "FSMonSync.h"
#include "SyncUtils.h"

void FSMonSync::Log(String const &s)
{
	logEdit.Set(logEdit.Get() + s);
	logEdit.SetCursor(INT_MAX);
}

// gets destination path given source one
String FSMonSync::GetDestPath(String const &srcPath)
{
	ASSERT(srcPath.StartsWith(srcFolder));
	String res = srcPath.Mid(srcFolder.GetCount());
	return AppendFileName(dstFolder, res);
}

void FSMonSync::startCb(void)
{
	srcFolder = AppendFileName(GetHomeDirectory(), "FSMonTest_A");
	if(!DirectoryExists(srcFolder))
		RealizeDirectory(srcFolder);
	dstFolder = AppendFileName(GetHomeDirectory(), "FSMonTest_B");
	if(!DirectoryExists(dstFolder))
		RealizeDirectory(dstFolder);
	Log("Starting syncinc '" + srcFolder + "' to '" + dstFolder + "'\n");
	Log("Initial sync.....");
	ProcessEvents();
	SyncFolder(srcFolder, dstFolder);
	Log("DONE\n");
	ProcessEvents();
	fsmMon.Add(srcFolder);

	started = true;
	stopBtn.Enable();
	startBtn.Disable();
}

void FSMonSync::stopCb(void)
{
	Log("Stopping\n\n");
	fsmMon.Remove(srcFolder);
	started = false;
	stopBtn.Disable();
	startBtn.Enable();
}

void FSMonSync::quitCb(void)
{
	if(started)
		stopCb();
	Break();
}

void FSMonSync::monitorCb()
{
	Vector<FSMon::Info> infos = fsmMon.GetChanged();
	for(int iChange = 0; iChange < infos.GetCount(); iChange++)
	{
		FSMon::Info const &info = infos[iChange];
		switch(info.flags)
		{
			case FSMon::FSM_NOP :
				Log("NO-OP EVENT\n");
				break;
			case FSMon::FSM_Created :
				Log("Syncing created file '" + info.path + "'.....");
				if(SyncFile(info.path, GetDestPath(info.path)))
					Log("DONE\n");
				else
					Log("FAILED\n");
				break;
			case FSMon::FSM_Deleted :
				Log("Syncing deleted file '" + info.path + "'.....");
				if(SyncFile(info.path, GetDestPath(info.path)))
					Log("DONE\n");
				else
					Log("FAILED\n");
				break;
			case FSMon::FSM_Moved :
				Log("Syncing file move from '" + info.path + "' to '" + info.newPath + "'.....");
				if(FileMove(GetDestPath(info.path), GetDestPath(info.newPath)))
					Log("DONE\n");
				else
					Log("FAILED\n");
				break;
			case FSMon::FSM_FolderCreated :
				Log("Syncing created folder '" + info.path + "'.....");
				if(SyncFolder(info.path, GetDestPath(info.path)))
					Log("DONE\n");
				else
					Log("FAILED\n");
				break;
			case FSMon::FSM_FolderDeleted :
				Log("Syncing deleted file '" + info.path + "'.....");
				if(DeleteFolderDeep(GetDestPath(info.path)))
					Log("DONE\n");
				else
					Log("FAILED\n");
				break;
			case FSMon::FSM_FolderMoved :
				Log("Syncing folder move from '" + info.path + "' to '" + info.newPath + "'.....");
				if(FileMove(GetDestPath(info.path), GetDestPath(info.newPath)))
					Log("DONE\n");
				else
					Log("FAILED\n");
				break;
			case FSMon::FSM_Modified :
				Log("Syncing modified file '" + info.path + "'.....");
				if(SyncFile(info.path, GetDestPath(info.path)))
					Log("DONE\n");
				else
					Log("FAILED\n");
				break;
			case FSMon::FSM_AttribChange :
				Log("Attribute syncing not supported\n");
				break;
		}
	}
}

FSMonSync::FSMonSync()
{
	CtrlLayout(*this, "Window title");
	fsmMon.EventHandler = THISBACK(monitorCb);
	startBtn <<= THISBACK(startCb);
	stopBtn <<= THISBACK(stopCb);
	quitBtn <<= THISBACK(quitCb);

	started = false;
	stopBtn.Disable();
}

GUI_APP_MAIN
{
	FSMonSync().Run();
}
