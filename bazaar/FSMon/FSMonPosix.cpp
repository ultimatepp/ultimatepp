#include "FSMon.h"

NAMESPACE_UPP

// code for POSIX platform
#ifdef PLATFORM_POSIX
	
#include <sys/inotify.h>
#include <sys/ioctl.h>

// sets error code message from errno
String FSMon::GetErrorStr(int err)
{
	return strerror(err);
}


// constructor
FSMon::FSMon(bool nOnClose)
{
	errCode = 0;
	errMsg = "";
	
	notifyOnClose = nOnClose;
	
	iNotifyHandle = inotify_init1(IN_NONBLOCK);

	if(iNotifyHandle == -1)
	{
		SetError(errno);
		return;
	}
	
	// start monitor thread
	shutDown = false;
	fsmThread.Start(THISBACK(monitorCb));
}

// destructor
FSMon::~FSMon()
{
	// stops monitoring thread
	shutDown = true;
	while(shutDown)
		;
	
	if(iNotifyHandle >= 0)
	{
		// close notify handle
		// that should un-monitor all paths...
		close(iNotifyHandle);
	}
}

/*
struct inotify_event {
    int      wd;       // Watch descriptor
    uint32_t mask;     // Mask of events
    uint32_t cookie;   // Unique cookie associating related events (for rename(2))
    uint32_t len;      // Size of name field
    char     name[];   // Optional null-terminated name
};
*/

// scans a newly created folder to look for files
// being created BEFORE notify handler was in place
/*
void FSMon::ScanCreatedFolder(String path)
{
	FindFile ff(AppendFileName(path, "*"));
	while(ff)
	{
		if(ff.IsFolder())
			ScanCreatedFolder(ff.GetPath());
		else
		{
			INTERLOCKED_(fsmMutex)
			{
				changed.Add();
				Info &info = changed.Top();
				info.flags = FSM_Created;
				info.path = ff.GetPath();
				info.newPath.Clear();
				callHandlerCb();
			}
		}
		ff.Next();
	}
}
*/

// callback to call event handler in maint thread
// (via PostCallback) when using GUI
void FSMon::callHandlerCb(void)
{
#ifdef flagGUI
	PostCallback(EventHandler);
#else
	EventHandler();
#endif
}

// event handling selector
void FSMon::EventsSelector(uint32 mask, String const &path, String const &newPath)
{
	// flag stating event related to folder, not file
	bool isFolder = mask & IN_ISDIR;

	// mask field is a bitmask with OR-ed values, so we can't use a switch
	// we just check each value in a given order
	if(mask & IN_CLOSE_WRITE)
	{
		INTERLOCKED_(fsmMutex)
		{
			// remove file from opened list
			int iOpened = openedFiles.Find(path);
			if(iOpened >= 0)
				openedFiles.Remove(iOpened);

			// we propagate IN_CLOSE_WRITE event only if
			// we want to be notified just after file closing
			// otherwise, the change is always propagated by IN_MODIFY
			if(notifyOnClose)
			{
				changed.Add();
				Info &info = changed.Top();
				info.flags = FSM_Modified;
				info.path = path;
				info.newPath.Clear();
				callHandlerCb();
			}
		}
		return;
	}
	if(mask & IN_CLOSE_NOWRITE)
	{
		// just update the opened files list
		int iOpened = openedFiles.Find(path);
		if(iOpened >= 0)
		{
			INTERLOCKED_(fsmMutex)
			{
				openedFiles.Remove(iOpened);
			}
		}
		return;
	}
	if(mask & IN_OPEN)
	{
		// just update the opened files list
		if(openedFiles.Find(path) < 0)
		{
			INTERLOCKED_(fsmMutex)
			{
				openedFiles.Add(path);
			}
		}
		return;
	}
	if(mask & IN_CREATE)
	{
		// signal file/path creation
		INTERLOCKED_(fsmMutex)
		{
			changed.Add();
			Info &info = changed.Top();
			info.flags = (isFolder ? FSM_FolderCreated : FSM_Created);
			info.path = path;
			info.newPath.Clear();
			callHandlerCb();
		}
		// if a folder was created, we shall first setup a monitor
		// in it, then ensure that in the meanwhile no subitems have been created
		if(isFolder)
		{
			INTERLOCKED_(fsmMutex2){
				AddWatch(path);
			}
//			ScanCreatedFolder(path);
		}
		return;
	}
	if(mask & IN_DELETE)
	{
		// signal file removal
		INTERLOCKED_(fsmMutex)
		{
			changed.Add();
			Info &info = changed.Top();
			info.flags = isFolder ? FSM_FolderDeleted : FSM_Deleted;
			info.path = path;
			info.newPath.Clear();
			callHandlerCb();
		}
		// for folders, we shall de-monitor all contained ones
		if(isFolder)
		{
			INTERLOCKED_(fsmMutex)
			{
				for(int iFolder = monitoredPaths.GetCount() - 1; iFolder >= 0; iFolder--)
				{
					if(path.EndsWith(monitoredPaths[iFolder]))
					{
						monitoredPaths.Pop();
						int desc = monitoredDescriptors.Pop();
						inotify_rm_watch(iNotifyHandle, desc);
					}
				}
			}
		}
		return;
	}
	if(mask & IN_MODIFY)
	{
		// if we want just notifies on close, do nothing
		if(!notifyOnClose)
		{
			INTERLOCKED_(fsmMutex)
			{
				changed.Add();
				Info &info = changed.Top();
				info.flags = FSM_Modified;
				info.path = path;
				info.newPath.Clear();
				callHandlerCb();
			}
		}
		return;
	}
	// following one was pre-handled by thread to differentiate
	// between true moves inside monitored folders or create/delete
	// if coming/going outside
	if(mask & IN_MOVE)
	{
		INTERLOCKED_(fsmMutex)
		{
			changed.Add();
			Info &info = changed.Top();
			info.flags = isFolder ? FSM_FolderMoved : FSM_Moved;
			info.path = path;
			info.newPath = newPath;
			callHandlerCb();
			
			// if we moved a folder, we shall update stored info
			// with new ones -- we assume that monitors will stay in place...
			if(isFolder)
			{
				INTERLOCKED_(fsmMutex2)
				{
					Index<String> paths;
					for(int i = 0; i < monitoredPaths.GetCount(); i++)
					{
						String oldPth = monitoredPaths[i];
						if(oldPth.StartsWith(path))
							oldPth = newPath + oldPth.Mid(path.GetCount());
						paths.Add(oldPth);
					}
					monitoredPaths = pick(paths);
				}
			}
		}
		return;
	}
	if(mask & IN_ATTRIB)
	{
		// add the attribute-modify event
		INTERLOCKED_(fsmMutex)
		{
			changed.Add();
			Info &info = changed.Top();
			info.flags = FSM_AttribChange;
			info.path = path;
			info.newPath.Clear();
			callHandlerCb();
		}
	}
		return;
}

// monitoring callback (runs in a separate thread)
void FSMon::monitorCb(void)
{
	const size_t BASE_BUFSIZE = offsetof(struct inotify_event, name);

	byte *bigBuf;
	while(!shutDown)
	{
		// check if data is available from inotify
		size_t nBytes;
		if(ioctl(iNotifyHandle, FIONREAD, &nBytes) < 0)
		{
			Sleep(100);
			continue;
		}
		if(nBytes == 0 || (int)nBytes == -1)
		{
			Sleep(100);
			continue;
		}
		// buffer can be filled by many events at once; the variable-lenght
		// name overcomplicates stuffs, as usual
		// we can't also read partial data, because read returns error if buffer is too small
		// so, we have to read full available data and split records later
		bigBuf = (byte *)malloc(nBytes);
		size_t res = read(iNotifyHandle, bigBuf, nBytes);
		if(res != nBytes)
		{
			free(bigBuf);
			Sleep(100);
			continue;
		}
		struct inotify_event *buf = (struct inotify_event *)bigBuf;
		while((byte *)buf - bigBuf < (int)nBytes)
		{
			int wd			= buf->wd;
			uint32_t mask	= buf->mask;
			String name = (buf->len ? buf->name : "");
			buf = (struct inotify_event *)((byte *)buf + BASE_BUFSIZE + buf->len);
	
			// skip ignored events
			if(mask & IN_IGNORED)
				continue;
			
			// get path from descriptor
			int idx = monitoredDescriptors.Find(wd);
			if(idx < 0)
				continue;
			String path = AppendFileName(monitoredPaths[idx], name);
	
			// special handling for IN_MOVED events
			// an IN_MOVED_FROM must be followed by an IN_MOVE__TO
			// if moving between monitored folders. Well, I hope nothing
			// happens between them..... So, if we don't find the IN_MOVED_TO
			// we assume it's a move outside monitored folders, so a IN_DELETE
			// OTOH, if we find an IN_MOVED_TO without corresponding IN_MOVED_FROM
			// we assume e move from outside, so an IN_CREATE
			if(mask & IN_MOVED_FROM && (byte *)buf - bigBuf < (int)nBytes && buf->mask & IN_MOVED_TO)
			{
				// it's a true move
				String newName = (buf->len ? buf->name : "");
				EventsSelector(IN_MOVE | (mask & IN_ISDIR ? IN_ISDIR : 0), path, AppendFileName(monitoredPaths[idx], newName));
				
				// skip the moveto
				buf = (struct inotify_event *)((byte *)buf + BASE_BUFSIZE + buf->len);
			}
			else if(mask & IN_MOVED_FROM)
			{
				// it's a delete
				EventsSelector(IN_DELETE | (mask & IN_ISDIR ? IN_ISDIR : 0), path, "");
			}
			else if(mask & IN_MOVED_TO)
			{
				// it's a create
				EventsSelector(IN_CREATE | (mask & IN_ISDIR ? IN_ISDIR : 0), path, "");
			}
			else
			{
				// normal event handling
				EventsSelector(mask, path, "");
			}
		}
		free(bigBuf);
	}
	shutDown = false;
}
		
/* inofify MASKS
IN_ACCESS			File was accessed (read) (*).
IN_ATTRIB			Metadata changed, e.g., permissions, timestamps, extended attributes, link count (since Linux 2.6.25), UID, GID, etc. (*).
IN_CLOSE_WRITE		File opened for writing was closed (*).
IN_CLOSE_NOWRITE	File not opened for writing was closed (*).
IN_CREATE			File/directory created in watched directory (*).
IN_DELETE			File/directory deleted from watched directory (*).
IN_DELETE_SELF		Watched file/directory was itself deleted.
IN_MODIFY			File was modified (*).
IN_MOVE_SELF		Watched file/directory was itself moved.
IN_MOVED_FROM		File moved out of watched directory (*).
IN_MOVED_TO			File moved into watched directory (*).
IN_OPEN				File was opened (*). 
*/
// recursively add or remove monitors for paths
// try to monitor all he can, even if there are some errors
// returns false if some error is found
bool FSMon::AddWatch(String const &path)
{
	bool res = true;
	
	INTERLOCKED_(fsmMutex2)
	{
		// add a monitor to current path if not already there
		if(monitoredPaths.Find(path) >= 0)
			return true;
		
		int desc = inotify_add_watch(
			iNotifyHandle,
			path,
			IN_ATTRIB |
			IN_OPEN |				// this one just to see if file is busy
			IN_CLOSE_WRITE |
			IN_CLOSE_NOWRITE |
			IN_CREATE |
			IN_DELETE |
			// IN_DELETE_SELF |		// we handle this one in IN_DELETE anyways
			IN_MODIFY |
			// IN_MOVE_SELF |		// we handle this one in IN_MOVE anyways
			IN_MOVED_FROM |			// without IN_MOVE_TO, is a DELETE from watched folders
			IN_MOVED_TO
		);
		
		// error ?
		if(desc < 0)
		{
			errMap.Add(path, errno);
			SetError(errno);
			res = false;
		}
		else
		{
			monitoredPaths.Add(path);
			monitoredDescriptors.Add(desc);
		}
	
		// look for all subfolders
		FindFile ff(AppendFileName(path, "*"));
		while(ff)
		{
			if(ff.IsFolder())
				res &= AddWatch(ff.GetPath());
			ff.Next();
		}
	}
	
	return res;
}

bool FSMon::RemoveWatch(String const &path)
{
	bool res = true;
	
	int pDescIdx = monitoredPaths.Find(path);
	if(pDescIdx >= 0)
	{
		if(inotify_rm_watch(iNotifyHandle, monitoredDescriptors[pDescIdx]) < 0)
		{
			errMap.Add(path, errno);
			SetError(errno);
			res = false;
		}
		// look for all subfolders
		FindFile ff(AppendFileName(path, "*"));
		while(ff)
		{
			if(ff.IsFolder())
				res &= RemoveWatch(ff.GetPath());
			ff.Next();
		}
		
		// remove from list of monitored paths
		monitoredPaths.Remove(pDescIdx);
		monitoredDescriptors.Remove(pDescIdx);
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////

// add a monitored path
bool FSMon::Add(String const &path)
{
	if(IsError())
		return false;
	
	// check wether folder exists
	if(!DirectoryExists(path))
		return false;
	
	// clears map of errors
	errMap.Clear();
	
	// we don't want monitor again an already monitored path
	// so we do a quick check about it
	if(monitoredPaths.Find(path) >= 0)
		return true;
	
	// if we're monitoring an external path, we should unmonitor
	// it and remonitor the external one; as doing so we could loose
	// events, we just monitor the external path, that do not harm
	return AddWatch(path);
}

// remove a monitored path
bool FSMon::Remove(String const &path)
{
	if(IsError())
		return false;
	
	// check wether folder exists
	// if not, return success anyways
	if(!DirectoryExists(path))
		return true;
	
	// clears map of errors
	errMap.Clear();
	
	// do the recursive un-monitoring
	return RemoveWatch(path);
	
}

// query for changed files/folders
Vector<FSMon::Info> FSMon::GetChanged(void)
{
	Vector<Info> info;
	INTERLOCKED_(fsmMutex)
	{
		info = pick(changed);
		changed.Clear();
	}
	return info;
}

// gets actually opened files
Index<String> FSMon::GetOpenedFiles(void)
{
	Index<String> of;
	INTERLOCKED_(fsmMutex)
	{
		of <<= openedFiles;
	}
	return of;
}
		
VectorMap<String, int> FSMon::GetErrorMap(void)
{
	VectorMap<String, int> res = pick(errMap);
	errMap.Clear();
	return res;
}

// query IF some changes happened
bool FSMon::HasChanges(void)
{
	bool res;
	INTERLOCKED_(fsmMutex){
		res = changed.GetCount();
	}
	return res;
}

#endif

END_UPP_NAMESPACE