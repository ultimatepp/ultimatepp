#include <windows.h>
#include <shellapi.h>
#include <mapix.h>
#include <mapiutil.h>
#include <mspst.h>
#include <string>
#include <list>
#include <map>
using namespace std;
#pragma hdrstop // precompiled headers stop here
#include "WinMapiUtils.h"



list<mapi_TLibraryInfo> mapi_Libraries;  // from the registry, a list of ex-mapi dlls
bool got_libraries=false;                // have we built that list yet?
//
HINSTANCE hmapilib=0;             // for loadlibrary(mapi32.dll). If this is non-null, we must freelibrary it at the end.
string mapi_lib_path;             // this is the pathname of what we've currently loaded.
list<mapi_TStoreInfo> mapi_Stores;
//
MAPIADMINPROFILES *pMAPIAdminProfiles=0;
MAPIUNINITIALIZE *pMAPIUninitialize=0;
MAPILOGONEX *pMAPILogonEx=0;
MAPIFREEBUFFER *pMAPIFreeBuffer=0;
RTFSYNC *pRTFSync=0;
WRAPCOMPRESSEDRTFSTREAM *pWrapCompressedRTFStream=0;
//
IMAPISession *mapi_session=0;     // The session
string mapi_session_profile;      // This is the profile name which session is logged onto.
IMsgStore *mapi_msgstore=0;       // The message store
string mapi_msgstore_name;        // This is the name to which that message-store refers
list<mapi_TFolderInfo> mapi_Folders;  // a list of the folders
bool got_eids = false;             // a shortcut for whether or not all the following have been set
mapi_TEntryid eid_inbox, eid_outbox, eid_sent, eid_deleted;
mapi_TEntryid eid_calendar, eid_contacts, eid_journal, eid_notes, eid_tasks;

void mapi_EnsureCommonEids();
mapi_TFolderType mapi_GetFolderType(mapi_TEntryid &eid, IMAPIFolder *f);
void mapi_RecEnsureFolders(IMAPIFolder *parent, int depth, string prefix, list<mapi_TFolderInfo> *folders);
void mapi_EnsureCrazyProfileDeleted(IProfAdmin *iprofadmin);
list<string> mapi_RegQuerySubkeys(HKEY key);
string mapi_RegQueryString(HKEY key,const string name);

// MAPI_ENSURELIBRARIES -- builds up a list of all the Extended MAPI libraries
// that have been installed on this machine, in the global mapi_Libraries
// list. This is a difficult task...
// In the olden days, there didn't exist such a list anywhere on the system.
// Very recently (with the advent of Outlook XP) it has introduced the
// idea that there should be a list under HKEY_LOCAL_MACHINE\Clients\Mail
// whereby every extended-MAPI is indicated with a "DLLPathEx" value.
// But in the olden days (with Outlook2000 in both IMO and CWG modes, and
// with Outlook97) it merely has a "DLLPath" value.
// So our plan is as follows:
// (1) For any DLLPathEx keys, we'll add them.
// (2) If one of those DLLPathEx happened to be for the "Microsoft Outlook"
// service, then it must have been XP or later, and so we can return immediately.
// (3) If Microsoft Outlook was not even installed (i.e. not in the list), then we
// can also return immediately.
// (4) Otherwise, there must have been Outlook97 or 2000 installed. So we
// will set a key under HKEY_LOCAL_MACHINE\SOFTWARE\Microsft\Windows Messaging Subsystem\MSMapiApps
// to indicate that our app will use the "Microsoft Outlook" mapi. Then, we
// add "mapi32.dll" into the list. On a modern system, when you LoadLibrary(mapi32.dll),
// this is merely a stubb: it actually looks into that list and figures out which mapi
// to use from there. Therefore, this will end up loading Outlook.
//
void mapi_EnsureLibraries()
{ if (got_libraries) return;
  got_libraries=true;
  //
  // First, if the mapi stub has been installed, then we can check in the
  // registry for which MAPI libraries are present on this machine.
  bool uses_stub=false, IsOutlookInstalled=false, IsOutlookExListed=false;
  HKEY key; LONG res;
  res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Clients\\Mail",0,KEY_READ,&key);
  if (res==ERROR_SUCCESS)
  { uses_stub=true;
    string defname = mapi_RegQueryString(key,"");   // Find out which one is the default
    list<string> names = mapi_RegQuerySubkeys(key); // Get the list of child keys
    RegCloseKey(key);
    for (list<string>::const_iterator i=names.begin(); i!=names.end(); i++)
    { string name = *i;
      bool thisisoutlook = (name=="Microsoft Outlook");
      IsOutlookInstalled |= thisisoutlook;
      res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,("SOFTWARE\\Clients\\Mail\\"+name).c_str(),0,KEY_READ,&key);
      if (res==ERROR_SUCCESS)
      { // Get the path, stored in "DLLPathEx"
        string path = mapi_RegQueryString(key,"DLLPathEx");
        if (path!="")
        { if (thisisoutlook) IsOutlookExListed=true;
          mapi_TLibraryInfo lib; lib.name=name; lib.path=path; lib.supported=true;
          if (name==defname) mapi_Libraries.push_front(lib); else mapi_Libraries.push_back(lib);
        }
        RegCloseKey(key);
      }
    }
  }
  if (IsOutlookInstalled && IsOutlookExListed) return; // outlook XP is fine as it is.
  //
  // If it uses the stub technique, and Outlook is installed, we can set the registry
  // key to tell the stub to give us outlook
  if (uses_stub && IsOutlookInstalled)
  { res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows Messaging Subsystem\\MSMapiApps",0,KEY_SET_VALUE,&key);
    if (res!=ERROR_SUCCESS) return;
    char c[MAX_PATH]; GetModuleFileName(NULL,c,MAX_PATH);
    const char *d=c+strlen(c)-1; while (d>c && *d!='/' && *d!='\\' && *d!=':') d--; if (*d=='/' || *d=='\\' || *d==':') d++;
    res = RegSetValueEx(key,d,0,REG_SZ,(LPBYTE)"Microsoft Outlook",18);
    RegCloseKey(key);
    if (res!=ERROR_SUCCESS) return;
    mapi_TLibraryInfo outlib; outlib.name="Microsoft Outlook"; outlib.path="mapi32.dll";
    mapi_Libraries.push_back(outlib);
  }
  //
  // Otherwise, if it doesn't even use the stub, then we'll use the old-fashioned technique.
  if (!uses_stub)
  { res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows Messaging Subsystem",0,KEY_READ,&key);
    if (res!=ERROR_SUCCESS) return;
    DWORD type; DWORD size=10; char c[10];
    res = RegQueryValueEx(key,"MAPIX",NULL,&type,(LPBYTE)c,&size);
    RegCloseKey(key);
    if (res!=ERROR_SUCCESS) return;
    if (strcmp(c,"1")!=0) return;
    mapi_TLibraryInfo deflib; deflib.name="Default"; deflib.path="mapi32.dll";
    mapi_Libraries.push_back(deflib);
  }
}

// MAPI_ENSURESTORES -- given a path to a mapi-x DLL, this builds up a list
// of all the profiles and message-stores listed in that mapi implementation.
// The list is heterogenous. The 'type' field in each item in the list
// says whether it's a store, or a profile, or a "secret profile" (one where
// we weren't able to get a list of its stores, presumably because this
// information is protected by a password).
// Note: the PST-file support in the rest of this program uses a temporary
// profile called "Lu's Crazy Profile (democode)". Just out of neatness,
// the list we generate will not include that profile.
//
void mapi_EnsureStores(const string libpath)
{ if (mapi_lib_path==libpath) return;
  mapi_EnsureFinished();
  mapi_lib_path=libpath;
  //
  // Load the library
  hmapilib = LoadLibrary(libpath.c_str()); if (hmapilib==0) return;
  MAPIINITIALIZE *pMAPIInitialize = (MAPIINITIALIZE*)GetProcAddress(hmapilib,"MAPIInitialize");
  pMAPIAdminProfiles = (MAPIADMINPROFILES*)GetProcAddress(hmapilib,"MAPIAdminProfiles");
  pMAPILogonEx = (MAPILOGONEX*)GetProcAddress(hmapilib,"MAPILogonEx");
  pMAPIUninitialize = (MAPIUNINITIALIZE*)GetProcAddress(hmapilib,"MAPIUninitialize");
  pMAPIFreeBuffer = (MAPIFREEBUFFER*)GetProcAddress(hmapilib,"MAPIFreeBuffer");
  pRTFSync = (RTFSYNC*)GetProcAddress(hmapilib,"RTFSync");
  pWrapCompressedRTFStream = (WRAPCOMPRESSEDRTFSTREAM*)GetProcAddress(hmapilib,"WrapCompressedRTFStream");
  if (pMAPIInitialize==0 || pMAPIAdminProfiles==0 || pMAPILogonEx==0 || pMAPIUninitialize==0
     || pMAPIFreeBuffer==0 || pRTFSync==0 || pWrapCompressedRTFStream==0) {FreeLibrary(hmapilib);hmapilib=0;return;}
  HRESULT hr = pMAPIInitialize(NULL); if (hr!=S_OK) {FreeLibrary(hmapilib);hmapilib=0;return;}
  //
  // List all the profiles
  IProfAdmin *iprofadmin;
  hr = pMAPIAdminProfiles(0,&iprofadmin);
  if (hr==S_OK) 
  { list<string> profiles;
    IMAPITable *proftable;
    hr = iprofadmin->GetProfileTable(0, &proftable);
    if (hr==S_OK) 
    { SizedSPropTagArray(2, proftablecols) = { 2, {PR_DISPLAY_NAME,PR_DEFAULT_PROFILE} };
      SRowSet *profrows;
      hr = pHrQueryAllRows(proftable,(SPropTagArray*)&proftablecols,NULL,NULL,0,&profrows);
      if (hr==S_OK)
      { for (unsigned int i=0; i<profrows->cRows; i++)
        { string name=""; bool isdefault=false;
          if (profrows->aRow[i].lpProps[0].ulPropTag==PR_DISPLAY_NAME) name=profrows->aRow[i].lpProps[0].Value.lpszA;
          if (profrows->aRow[i].lpProps[1].ulPropTag==PR_DEFAULT_PROFILE) isdefault=(0!=profrows->aRow[i].lpProps[1].Value.b);
          if (name!="" && name!="Lu's Crazy Profile (democode)") {if (isdefault) profiles.push_front(name); else profiles.push_back(name);}
        }
        pFreeProws(profrows);
      }
      proftable->Release();
    }
    //
    // For each profile we will attempt to log on and list the message-stores it contains.
    // Plan: we will build up a list of stores just for this profile. Then we will splice it
    // onto the main global list of stores -- either at the beginning (if this is the default
    // profile) or at the end.
    for (list<string>::const_iterator profile=profiles.begin(); profile!=profiles.end(); profile++)
    { list<mapi_TStoreInfo> thesestores;
      IMAPISession *isession; bool issecret=true;
      hr = pMAPILogonEx(0,(char*)profile->c_str(),NULL,MAPI_NEW_SESSION|MAPI_EXTENDED|MAPI_NO_MAIL,&isession);
      if (hr==S_OK) 
      { IMAPITable *mstable;
        hr = isession->GetMsgStoresTable(0, &mstable);
        if (hr==S_OK) 
        { issecret=false;
          SizedSPropTagArray(2, mstablecols) = { 2, {PR_RESOURCE_FLAGS,PR_DISPLAY_NAME} };
          SRowSet *msrows;
          hr = pHrQueryAllRows(mstable,(SPropTagArray*)&mstablecols,NULL,NULL,0,&msrows);
          if (hr==S_OK) 
          { for (unsigned int i=0; i<msrows->cRows; i++)
            { string name=""; bool isdefault=false;
              if (msrows->aRow[i].lpProps[0].ulPropTag==PR_RESOURCE_FLAGS) isdefault=(msrows->aRow[i].lpProps[0].Value.ul&STATUS_DEFAULT_STORE)!=0;
              if (msrows->aRow[i].lpProps[1].ulPropTag==PR_DISPLAY_NAME) name=msrows->aRow[i].lpProps[1].Value.lpszA;
              if (name!="")
              { mapi_TStoreInfo sinfo; sinfo.profile=*profile; sinfo.store=name; sinfo.type=mstStore;
                if (isdefault) thesestores.push_front(sinfo); else thesestores.push_back(sinfo);
              }
            }
            pFreeProws(msrows);
          }
          mstable->Release();
        }
        isession->Logoff(0,0,0);
        isession->Release();
      }
      mapi_TStoreInfo sinfo;  sinfo.profile=*profile; sinfo.store="";
      if (issecret) sinfo.type=mstProfileSecret; else sinfo.type=mstProfile;
      thesestores.push_front(sinfo);
      mapi_Stores.splice(mapi_Stores.end(),thesestores);
    }
    iprofadmin->Release();
  }
}

// MAPI_ENSUREFOLDERS -- given a profile name and a store name, this builds
// up a list of all the folders in that store. Note that, in truth, the folders
// form a tree. We will flatten the tree to obtain our list. For each item
// we will record its depth, and its complete path, as well as just its name.
// To open a message-store might involve displaying a password dialog.
// For this reason, once we have opened a message-store and a profile, we
// leave both of them open, in global static variables mapi_session and mapi_msgstore.
// That means that subsequent calls for the same profile/store will not
// require another login.
//
void mapi_EnsureFolders(HWND hwnd, const string profile, const string store)
{ HRESULT hr;
  if (mapi_session_profile==profile && mapi_msgstore_name==store) return;
  //
  // First thing we do is clean out anything we've allocated before.
  mapi_Folders.clear();
  if (mapi_session_profile!=profile || mapi_msgstore_name!=store)
  { mapi_msgstore_name=""; got_eids=false; if (mapi_msgstore!=0) mapi_msgstore->Release(); mapi_msgstore=0;
  }
  if (mapi_session_profile!=profile)
  { mapi_session_profile=""; if (mapi_session!=0) {mapi_session->Logoff(0,0,0); mapi_session->Release();}
  }
  if (hmapilib==0 || pMAPILogonEx==0) return;
  //
  // Now we can log on to the specified profile...
  if (mapi_session_profile!=profile)
  { hr = pMAPILogonEx(PtrToUlong(hwnd),(char*)profile.c_str(),NULL,MAPI_NEW_SESSION|MAPI_EXTENDED|MAPI_NO_MAIL|MAPI_PASSWORD_UI,&mapi_session);
    if (hr!=S_OK) return;
    mapi_session_profile=profile;
  }
  // ... and find the specified store.
  if (mapi_msgstore_name!=store)
  { // task is to get the store we know by name. We do this by enumerating
    // all the message-stores in the table, and picking out the one with the right name.
    IMAPITable *mstable=0;
    hr = mapi_session->GetMsgStoresTable(0, &mstable);
    if (hr==S_OK)
    { SizedSPropTagArray(2, mstablecols) = { 2, {PR_ENTRYID,PR_DISPLAY_NAME} };
      SRowSet *msrows;
      hr = pHrQueryAllRows(mstable,(SPropTagArray*)&mstablecols,NULL,NULL,0,&msrows);
      if (hr==S_OK)
      { for (unsigned int i=0; i<msrows->cRows && mapi_msgstore==0; i++)
        { string name=""; mapi_TEntryid eid;
          if (msrows->aRow[i].lpProps[0].ulPropTag==PR_ENTRYID) eid=&msrows->aRow[i].lpProps[0];
          if (msrows->aRow[i].lpProps[1].ulPropTag==PR_DISPLAY_NAME) name=msrows->aRow[i].lpProps[1].Value.lpszA;
          if (name==store && !eid.isempty())
          { hr = mapi_session->OpenMsgStore(PtrToUlong(hwnd),msrows->aRow[i].lpProps[0].Value.bin.cb,
                 (LPENTRYID)msrows->aRow[i].lpProps[0].Value.bin.lpb, NULL, MDB_NO_MAIL, &mapi_msgstore);
            if (hr!=S_OK) mapi_msgstore=0;
          }

        }
        pFreeProws(msrows);
      }
      mstable->Release();
    }
    if (mapi_msgstore==0) {mapi_session->Logoff(0,0,0);mapi_session->Release();mapi_session=0;mapi_session_profile="";return;}
  }
  //
  // Now we have the msgstore. Let's get the human (intepersonal) subtree.
  // All the email folders are children of the human subtree.
  IMAPIFolder *ipmroot=0; mapi_TEntryid eid;
  SPropValue *ipm_eid;
  hr = pHrGetOneProp(mapi_msgstore, PR_IPM_SUBTREE_ENTRYID, &ipm_eid);
  if (hr==S_OK)
  { eid = ipm_eid;
    pMAPIFreeBuffer(ipm_eid);
  }
  if (!eid.isempty())
  { ULONG ipmroottype;
    hr = mapi_msgstore->OpenEntry(eid.size,eid.ab,NULL,0,&ipmroottype,(IUnknown**)&ipmroot);
    if (hr==S_OK)
    { if (ipmroottype!=MAPI_FOLDER) {ipmroot->Release(); ipmroot=0;}
    }
  }
  if (ipmroot==0) 
	  return;
  // the following recursive call does the work! puts the tree under "ipmroot" into mapi_Folders.
  mapi_RecEnsureFolders(ipmroot,0,"",&mapi_Folders);
  ipmroot->Release();
}




// MAPI_ENSUREFOLDERS(pst) -- Given the filename of a pst file, we create our
// own temporary profile, and add an MS-PST store to this profile, and configure
// the store to point to the specified PST file. Then, given our own profile-name
// and the name of this store, we get the previous EnsureFolders() call to build
// up a list of all the folders.
// Although we call it a temporary profile, there's not anything intrinsically
// temporary about it. Its temporariness substists in the fact that a call to
// EnsureFinished will delete it. Also, I've given it a silly name, so that
// it doesn't get mistaken for anything important!
//
void mapi_EnsureFolders(HWND hwnd, const string pst_fn)
{ HRESULT hr;
  if (mapi_session_profile=="Lu's Crazy Profile (democode)" && mapi_msgstore_name==pst_fn) return;
  //
  // First thing we do is clean out anything we've allocated before.
  mapi_Folders.clear();
  mapi_msgstore_name=""; got_eids=false; if (mapi_msgstore!=0) mapi_msgstore->Release(); mapi_msgstore=0;
  mapi_session_profile=""; if (mapi_session!=0) {mapi_session->Logoff(0,0,0); mapi_session->Release();}
  if (hmapilib==0 || pMAPIAdminProfiles==0) return;
  //
  // Plan: create a temporary profile, and add a PST service to it, configured for that filename.
  IProfAdmin *iprofadmin;
  hr = pMAPIAdminProfiles(0,&iprofadmin);
  if (hr!=S_OK) return;
  mapi_EnsureCrazyProfileDeleted(iprofadmin);
  hr = iprofadmin->CreateProfile("Lu's Crazy Profile (democode)",NULL,PtrToUlong(hwnd),0);
  if (hr!=S_OK) {iprofadmin->Release(); return;}
  IMsgServiceAdmin *imsadmin;
  hr = iprofadmin->AdminServices("Lu's Crazy Profile (democode)",NULL,PtrToUlong(hwnd),0,&imsadmin);
  if (hr!=S_OK) {iprofadmin->Release(); return;}
  // Now we create the message-store-service. Read <mspst.h> for more details.
  hr = imsadmin->CreateMsgService("MSPST MS","Lu's Zany Message Store",PtrToUlong(hwnd),SERVICE_UI_ALLOWED);
  if (hr==MAPI_E_UNKNOWN_FLAGS) // Outlook97 doesn't understand those two flags at the end...
  { hr = imsadmin->CreateMsgService("MSPST MS","Lu's Zany Message Store",0,0);
  }
  if (hr!=S_OK) {imsadmin->Release(); mapi_EnsureCrazyProfileDeleted(iprofadmin); iprofadmin->Release(); return;}
  // We need to get hold of the MAPIUID for this message-service. We do this
  // by enumerating the message-stores (there will be only one!) and picking it up.
  // Actually, we set up 'mscols' to retrieve the name as well as the MAPIUID, for
  // reasons that will become apparent in just a moment.
  IMAPITable *mstable;
  hr = imsadmin->GetMsgServiceTable(0,&mstable);
  if (hr!=S_OK) {imsadmin->Release(); mapi_EnsureCrazyProfileDeleted(iprofadmin); iprofadmin->Release(); return;}
  SizedSPropTagArray(2, mscols) = { 2, {PR_SERVICE_UID,PR_DISPLAY_NAME} };
  mstable->SetColumns((SPropTagArray*)&mscols,0);
  SRowSet *msrows;
  hr = mstable->QueryRows(1,0,&msrows);
  mstable->Release();
  if (hr!=S_OK) {imsadmin->Release(); mapi_EnsureCrazyProfileDeleted(iprofadmin); iprofadmin->Release(); return;}
  MAPIUID msuid = *((MAPIUID*)msrows->aRow[0].lpProps[0].Value.bin.lpb);
  pFreeProws(msrows);
  // Now we wish to configure our message-store to use the PST filename.
  SPropValue msprops[1];
  msprops[0].ulPropTag=PR_PST_PATH; msprops[0].Value.lpszA=(char*)pst_fn.c_str();
  imsadmin->ConfigureMsgService(&msuid,PtrToUlong(hwnd),SERVICE_UI_ALLOWED,1,msprops);
  // That will have changed the message-store's display-name. Let's get it again.
  // The 'mscols' was already set up to retrieve names. How fortunate! We won't
  // bother checking for errors here: if we could query the table before, then
  // certainly we'll be able to query it again.
  imsadmin->GetMsgServiceTable(0,&mstable);
  mstable->SetColumns((SPropTagArray*)&mscols,0);
  mstable->QueryRows(1,0,&msrows);
  mstable->Release();
  string name="";
  if (msrows->aRow[0].lpProps[1].ulPropTag==PR_DISPLAY_NAME) name=msrows->aRow[0].lpProps[1].Value.lpszA;
  pFreeProws(msrows);
  // all done!
  imsadmin->Release();
  iprofadmin->Release();
  //
  // Finally, retrieve its folders, just as normal
  mapi_EnsureFolders(hwnd,"Lu's Crazy Profile (democode)",name);
}



// REC-ENSURE-FOLDERS -- Given an IMAPIFolder, we recursively retrieve
// all the folders it contains, and stick them into the 'folders' list.
// Here, "depth" and "prefix" are straightforward recursive counters
// of how deep we are in the tree.
//
void mapi_RecEnsureFolders(IMAPIFolder *parent, int depth, string prefix, list<mapi_TFolderInfo> *folders)
{ 
	//printf("\rCarpeta: %s", prefix.c_str());
	
	if (folders==0) 
		return; 
	if (parent==0) 
		return;

	// NB. We cannot call parent->GetHierarchyTable. That's because GetHierarchyTable
	// tries to open it will full (read/write) access, but Outlook2000/IMO only supports
	// readonly access, hence giving an MAPI_E_NO_ACCESS error. Therefore, we get
	// the hierarchy in this roundabout way, in readonly mode.
	IMAPITable *hierarchy; 
	HRESULT hr;
	const GUID local_IID_IMAPITable = {0x00020301,0,0, {0xC0,0,0,0,0,0,0,0x46}};
	hr = parent->OpenProperty(PR_CONTAINER_HIERARCHY,&local_IID_IMAPITable,0,0,(IUnknown**)&hierarchy);
	if (hr != S_OK) 
		return;
	// and query for all the rows
	SizedSPropTagArray(3, cols) = {3, {PR_ENTRYID,PR_DISPLAY_NAME,PR_SUBFOLDERS} };
	SRowSet *rows;
	hr = pHrQueryAllRows(hierarchy,(SPropTagArray*)&cols, NULL, NULL, 0, &rows);
	hierarchy->Release();
	if (hr!=S_OK) {
		pFreeProws(rows); 
		return;
	}
	// Note: the entry-ids returned by the list are just short-term list-specific
	// entry-ids. But we want to put long-term entry-ids in our 'folder' list.
	// That's why it's necessary to open the folder...

	// Go through all the rows. For each entry, if it is a message-folder add it, and potentially recurse
	for (unsigned int i=0; i<rows->cRows; i++) { 
		BOOL subfolders = rows->aRow[i].lpProps[2].Value.b;
		string name(rows->aRow[i].lpProps[1].Value.lpszA);
		IMAPIFolder *subf; 
		ULONG subftype;
		hr = parent->OpenEntry(rows->aRow[i].lpProps[0].Value.bin.cb, 
					(LPENTRYID)rows->aRow[i].lpProps[0].Value.bin.lpb, NULL, 0, &subftype, (IUnknown**)&subf);
		if (hr==S_OK) { 
			if (subftype == MAPI_FOLDER) { 
				SPropValue *veid=0;
				hr = pHrGetOneProp(subf, PR_ENTRYID, &veid); // get its long-term eid
				if (hr==S_OK) { 
					mapi_TFolderInfo f;
					f.depth=depth; 
					f.name=name; 
					f.path=prefix+name;
					f.eid = veid;
					f.type = mapi_GetFolderType(f.eid,subf);
					bool usefolder = (f.type==mftInbox||f.type==mftSent||f.type==mftMail||f.type==mftStuff);
					if (usefolder) 
						folders->push_back(f);
					pMAPIFreeBuffer(veid);
					if (usefolder && subfolders) 
						mapi_RecEnsureFolders(subf, depth+1, prefix+name+"\\", folders);
				}
			}
			subf->Release();
		}
	}
	pFreeProws(rows);
}

// GET-FOLDER-TYPE -- given a folder and its long-term entry-id,
// returns its type (inbox/outbox/calendar/...). There are three
// techniques for doing this; we do them all, in order of preference.
//
mapi_TFolderType mapi_GetFolderType(mapi_TEntryid &eid, IMAPIFolder *f)
{ // 1. Most assured way to get the type of a folder is to check
  // whether it's long-term ENTRYID is the same as one of the
  // standard ones. See the EnsureCommonEids() routine for an
  // explanation of how we retrieve the standard ones.
  mapi_EnsureCommonEids();
  if (eid.isequal(mapi_session,eid_inbox)) return mftInbox;
  if (eid.isequal(mapi_session,eid_outbox)) return mftOutbox;
  if (eid.isequal(mapi_session,eid_sent)) return mftSent;
  if (eid.isequal(mapi_session,eid_deleted)) return mftDeleted;
  // 2. Second best way, specific to Outlook, is to see if
  // it's equal to one of the Outlook specific ones.
  if (eid.isequal(mapi_session,eid_calendar)) return mftCalendar;
  if (eid.isequal(mapi_session,eid_contacts)) return mftContacts;
  if (eid.isequal(mapi_session,eid_journal)) return mftJournal;
  if (eid.isequal(mapi_session,eid_notes)) return mftNotes;
  if (eid.isequal(mapi_session,eid_tasks)) return mftTasks;
  // 3. Third best way (and the only other way) is to check
  // it's PR_CONTAINER_CLASS property. The documentation says
  // that this shouldn't be used, but apparently the documentation
  // is out of date. The documentation says that everything
  // begins with IPM, but I'm sure I've seen an IPF somewhere.
  SPropValue *sp;
  HRESULT hr = pHrGetOneProp(f, PR_CONTAINER_CLASS, &sp);
  if (hr!=S_OK) return mftStuff;
  string s(sp->Value.lpszA);
  pMAPIFreeBuffer(sp);
  if (s.length()>3 && s[0]=='I' && s[1]=='P') s[2]='.'; // not sure IPM or IPF.
  const char *c=s.c_str();
  if (strncmp(c,"IP..Note",8)==0) return mftMail;
  else if (strncmp(c,"IP..Imap",9)==0) return mftMail;
  else if (strncmp(c,"IP..Appointment",15)==0) return mftCalendar;
  else if (strncmp(c,"IP..Contact",11)==0) return mftContacts;
  else if (strncmp(c,"IP..Journal",11)==0) return mftJournal;
  else if (strncmp(c,"IP..StickyNote",14)==0) return mftNotes;
  else if (strncmp(c,"IP..Task",8)==0) return mftTasks;
  else if (strncmp(c,"IP..",4)==0) return mftSpecial;
  else return mftStuff;
}



// ENSURE-COMMON-EIDS -- There are some standard ENTRYIDs for some standard
// folders. This function sets up these in global variables. Note that they
// are specific to the current library, profile and message-store. For an
// explanation of how to retrieve each type, see inside the procedure.
//
void mapi_EnsureCommonEids()
{ if (got_eids) return;
  eid_inbox.clear(); eid_outbox.clear(); eid_sent.clear(); eid_deleted.clear();
  eid_calendar.clear(); eid_contacts.clear(); eid_journal.clear(); eid_notes.clear(); eid_tasks.clear();
  got_eids=true;
  if (mapi_msgstore==0) return;
  DWORD size; ENTRYID *eid; HRESULT hr;
  // 1. INBOX special folder -- in fact, the user can designate any folder as
  // an inbox. All we can do is check where incoming IPM.Note messages (i.e. emails)
  // are placed.
  hr = mapi_msgstore->GetReceiveFolder("IPM.Note",0,&size,&eid,NULL);
  if (hr==S_OK) eid_inbox.set(size,eid);
  // 2. Other special folders. The message-store has properties for these.
  SizedSPropTagArray(4, cols) = { 4, {PR_VALID_FOLDER_MASK, PR_IPM_OUTBOX_ENTRYID, PR_IPM_SENTMAIL_ENTRYID, PR_IPM_WASTEBASKET_ENTRYID} };
  ULONG pcount; SPropValue *props;
  hr = mapi_msgstore->GetProps((SPropTagArray*)&cols,0,&pcount,&props);
  if (hr==S_OK || hr==MAPI_W_ERRORS_RETURNED)
  { LONG mask; if (props[0].ulPropTag==PR_VALID_FOLDER_MASK) mask=props[0].Value.ul; else mask=0;
    if ((mask&FOLDER_IPM_OUTBOX_VALID) && props[1].ulPropTag==PR_IPM_OUTBOX_ENTRYID) eid_outbox.set(props[1].Value.bin.cb, (ENTRYID*)props[1].Value.bin.lpb);
    if ((mask&FOLDER_IPM_SENTMAIL_VALID) && props[2].ulPropTag==PR_IPM_SENTMAIL_ENTRYID) eid_sent.set(props[2].Value.bin.cb, (ENTRYID*)props[2].Value.bin.lpb);
    if ((mask&FOLDER_IPM_WASTEBASKET_VALID) && props[3].ulPropTag==PR_IPM_WASTEBASKET_ENTRYID) eid_deleted.set(props[3].Value.bin.cb, (ENTRYID*)props[3].Value.bin.lpb);
    pMAPIFreeBuffer(props);
  }
  // 3. The outlook specials. The inbox has properties for these.
  if (eid_inbox.isempty()) return;
  ULONG intype; IMAPIFolder *infolder;
  hr = mapi_msgstore->OpenEntry(eid_inbox.size, eid_inbox.ab, NULL, 0, &intype, (IUnknown**)&infolder);
  if (hr!=S_OK) return;
  if (intype!=MAPI_FOLDER) {infolder->Release();return;}
  SizedSPropTagArray(5, spec) = {5, {0x36D00102, 0x36D10102, 0x36D20102, 0x36D30102, 0x36D40102}};
  hr = infolder->GetProps((SPropTagArray*)&spec,0,&pcount,&props);
  if (hr==S_OK || hr==MAPI_W_ERRORS_RETURNED)
  { if (props[0].ulPropTag!=PT_ERROR) eid_calendar.set(props[0].Value.bin.cb, (ENTRYID*)props[0].Value.bin.lpb);
    if (props[1].ulPropTag!=PT_ERROR) eid_contacts.set(props[1].Value.bin.cb, (ENTRYID*)props[1].Value.bin.lpb);
    if (props[2].ulPropTag!=PT_ERROR) eid_journal.set(props[2].Value.bin.cb, (ENTRYID*)props[2].Value.bin.lpb);
    if (props[3].ulPropTag!=PT_ERROR) eid_notes.set(props[3].Value.bin.cb, (ENTRYID*)props[3].Value.bin.lpb);
    if (props[4].ulPropTag!=PT_ERROR) eid_tasks.set(props[4].Value.bin.cb, (ENTRYID*)props[4].Value.bin.lpb);
    pMAPIFreeBuffer(props);
  }
}

// MAPI_ENSUREFINISHED -- releases any interfaces that we might have obtained
// through the other functions.
//
void mapi_EnsureFinished()
{ 
	got_eids=false;
	mapi_Folders.clear();
	mapi_msgstore_name=""; 
	if (mapi_msgstore!=0) 
		mapi_msgstore->Release(); 
	mapi_msgstore=0;
	mapi_session_profile=""; 
	if (mapi_session!=0) {
		mapi_session->Logoff(0,0,0);
		mapi_session->Release();
		mapi_session=0;
	}
	mapi_Stores.clear();
	if (hmapilib!=0) { 
		IProfAdmin *iprofadmin;
		HRESULT hr = pMAPIAdminProfiles(0,&iprofadmin);
		if (hr==S_OK) { 
			mapi_EnsureCrazyProfileDeleted(iprofadmin);
			iprofadmin->Release();
		}
	}
	mapi_lib_path=""; if (hmapilib!=0) {pMAPIUninitialize();FreeLibrary(hmapilib);hmapilib=0;}
}
// MAPI_ENSURECRAZYPROFILEDELETED -- In case we had created a temporary profile,
// this function ensures that it is deleted.
// Note: it is a *SEVERE* error if you try to delete a profile that's not there.
// Under Outlook2000 in IMO, attempting to do this will delete every message-store
// of the main profile. Gulp! Hence, our care in this function...
//
void mapi_EnsureCrazyProfileDeleted(IProfAdmin *iprofadmin)
{ 
	bool gotcrazy=false;
	IMAPITable *proftable;
	HRESULT hr = iprofadmin->GetProfileTable(0, &proftable);
	if (hr==S_OK) { 
		SizedSPropTagArray(2, proftablecols) = { 2, {PR_DISPLAY_NAME,PR_DEFAULT_PROFILE} };
		SRowSet *profrows;
		hr = pHrQueryAllRows(proftable,(SPropTagArray*)&proftablecols,NULL,NULL,0,&profrows);
		if (hr==S_OK) { 
			for (unsigned int i=0; i<profrows->cRows; i++) { 
				string name="";
				if (profrows->aRow[i].lpProps[0].ulPropTag==PR_DISPLAY_NAME) 
					name=profrows->aRow[i].lpProps[0].Value.lpszA;
				if (name=="Lu's Crazy Profile (democode)") 
					gotcrazy=true;
			}
			pFreeProws(profrows);
		}
		proftable->Release();
	}
	if (gotcrazy) 
		iprofadmin->DeleteProfile("Lu's Crazy Profile (democode)",0);
}

// DECODERTFHTML -- Given an uncompressed RTF body of the message,
// and assuming that it contains encoded-html, this function
// turns it onto regular html.
// [in] (buf,*len) indicate the start and length of the uncompressed RTF body.
// [out] the buffer is overwritten with the HTML version, null-terminated,
// and *len indicates the length of this HTML.
//
// Notes: (1) because of how the encoding works, the HTML version is necessarily
// shorter than the encoded version. That's why it's safe for the function to
// place the decoded html in the same buffer that formerly held the encoded stuff.
// (2) Some messages include characters \'XX, where XX is a hexedecimal number.
// This function simply converts this into ASCII. The conversion will only make
// sense if the right code-page is being used. I don't know how rtf specifies which
// code page it wants.
// (3) By experiment, I discovered that \pntext{..} and \liN and \fi-N are RTF
// markup that should be removed. There might be other RTF markup that should
// also be removed. But I don't know what else.
//
void decodertfhtml(char *buf,unsigned int *len)
{ // c -- pointer to where we're reading from
  // d -- pointer to where we're writing to. Invariant: d<c
  // max -- how far we can read from (i.e. to the end of the original rtf)
  // ignore_tag -- stores 'N': after \mhtmlN, we will ignore the subsequent \htmlN.
  char *c=buf, *max=buf+*len, *d=buf; int ignore_tag=-1;
  // First, we skip forwards to the first \htmltag.
  while (c<max && strncmp(c,"{\\*\\htmltag",11)!=0) c++;
  //
  // Now work through the document. Our plan is as follows:
  // * Ignore { and }. These are part of RTF markup.
  // * Ignore \htmlrtf...\htmlrtf0. This is how RTF keeps its equivalent markup separate from the html.
  // * Ignore \r and \n. The real carriage returns are stored in \par tags.
  // * Ignore \pntext{..} and \liN and \fi-N. These are RTF junk.
  // * Convert \par and \tab into \r\n and \t
  // * Convert \'XX into the ascii character indicated by the hex number XX
  // * Convert \{ and \} into { and }. This is how RTF escapes its curly braces.
  // * When we get \*\mhtmltagN, keep the tag, but ignore the subsequent \*\htmltagN
  // * When we get \*\htmltagN, keep the tag as long as it isn't subsequent to a \*\mhtmltagN
  // * All other text should be kept as it is.
  while (c<max)
  { if (*c=='{') c++;
    else if (*c=='}') c++;
    else if (strncmp(c,"\\*\\htmltag",10)==0)
    { c+=10; int tag=0; while (*c>='0' && *c<='9') {tag=tag*10+*c-'0'; c++;}
      if (*c==' ') c++;
      if (tag==ignore_tag) {while (c<max && *c!='}') c++; if (*c=='}') c++;}
      ignore_tag=-1;
    }
    else if (strncmp(c,"\\*\\mhtmltag",11)==0)
    { c+=11; int tag=0; while (*c>='0' && *c<='9') {tag=tag*10+*c-'0'; c++;}
      if (*c==' ') c++;
      ignore_tag=tag;
    }
    else if (strncmp(c,"\\par",4)==0) {strcpy(d,"\r\n"); d+=2; c+=4; if (*c==' ') c++;}
    else if (strncmp(c,"\\tab",4)==0) {strcpy(d,"   "); d+=3; c+=4; if (*c==' ') c++;}
    else if (strncmp(c,"\\li",3)==0)
    { c+=3; while (*c>='0' && *c<='9') c++; if (*c==' ') c++;
    }
    else if (strncmp(c,"\\fi-",4)==0)
    { c+=4; while (*c>='0' && *c<='9') c++; if (*c==' ') c++;
    }
    else if (strncmp(c,"\\'",2)==0)
    { unsigned int hi=c[2], lo=c[3];
      if (hi>='0' && hi<='9') hi-='0'; else if (hi>='A' && hi<='Z') hi=hi-'A'+10; else if (hi>='a' && hi<='z') hi=hi-'a'+10;
      if (lo>='0' && lo<='9') lo-='0'; else if (lo>='A' && lo<='Z') lo=lo-'A'+10; else if (lo>='a' && lo<='z') lo=lo-'a'+10;
      *((unsigned char*)d) = (unsigned char)(hi*16+lo);
      c+=4; d++;
    }
    else if (strncmp(c,"\\pntext",7)==0) {c+=7; while (c<max && *c!='}') c++;}
    else if (strncmp(c,"\\htmlrtf",8)==0)
    { c++; while (c<max && strncmp(c,"\\htmlrtf0",9)!=0) c++;
      if (c<max) c+=9; if (*c==' ') c++;
    }
    else if (*c=='\r' || *c=='\n') c++;
    else if (strncmp(c,"\\{",2)==0) {*d='{'; d++; c+=2;}
    else if (strncmp(c,"\\}",2)==0) {*d='}'; d++; c+=2;}
    else {*d=*c; c++; d++;}
  }
  *d=0; d++;
  *len = (unsigned int)(d-buf);
}


bool isrtfhtml(const char *buf,unsigned int len)
{ // We look for the words "\fromhtml" somewhere in the file.
  // If the rtf encodes text rather than html, then instead
  // it will only find "\fromtext".
  for (const char *c=buf; c<buf+len; c++)
  { if (strncmp(c,"\\from",5)==0) return strncmp(c,"\\fromhtml",9)==0;
  } return false;
}

// These three utility functions are not provided with Outlook97. That's
// why I'm reimplementing them here.
HRESULT pHrGetOneProp(IMAPIProp *obj, ULONG tag, SPropValue **pProp)
{ if (pProp==0) return E_POINTER;
  SizedSPropTagArray(1, cols) = { 1, {tag}};
  ULONG pcount; HRESULT hr;
  hr = obj->GetProps((SPropTagArray*)&cols,0,&pcount,pProp);
  if (hr==S_OK) return S_OK;
  if (hr==MAPI_W_ERRORS_RETURNED) {pMAPIFreeBuffer(*pProp); return MAPI_E_NOT_FOUND;}
  return hr;
}

void pFreeProws(SRowSet *r)
{ 
	if (r==0) 
		return;
	for (unsigned int i=0; i<r->cRows; i++) { 
		SPropValue *pv = r->aRow[i].lpProps;
		pMAPIFreeBuffer(pv);
	}
	pMAPIFreeBuffer(r);
}
HRESULT pHrQueryAllRows(IMAPITable *table, SPropTagArray *tags, SRestriction *res,
			SSortOrderSet *sort, LONG crowsMax, SRowSet **rows)
{ 
	if (tags!=0) 
		table->SetColumns(tags,0);
	if (res!=0) 
		table->Restrict(res,0);
	if (sort!=0) 
		table->SortTable(sort,0);
	if (crowsMax==0) 
		crowsMax=0x0FFFFFFF;
	HRESULT hr = MAPI_E_BUSY;
	while (hr==MAPI_E_BUSY) { 
		hr = table->QueryRows(crowsMax,TBL_NOADVANCE,rows);
		if (hr==MAPI_E_BUSY) { 
			hr=table->WaitForCompletion(0,1000,NULL);
			if (hr!=MAPI_E_NO_SUPPORT) 
				hr=MAPI_E_BUSY;
		}
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////
// Here are just some internal functions for accessing the registry...
list<string> mapi_RegQuerySubkeys(HKEY key)
{ 
	list<string> ss; 
	LONG res; 
	DWORD index = 0; 
	char buf[1024]; 
	FILETIME ft;

	while (true) { 
		DWORD size = 1024;
		res = RegEnumKeyEx(key,index,buf,&size,NULL,NULL,NULL,&ft);
		if (res!=ERROR_SUCCESS) 
			return ss;
		ss.push_back(buf);
		index++;
	}
}
string mapi_RegQueryString(HKEY key,const string name)
{ 
	DWORD type, size=0; 
	LONG res;

	res = RegQueryValueEx(key,name.c_str(),NULL,&type,NULL,&size);
	if (res!=ERROR_SUCCESS) 
		return "";
	if (type!=REG_SZ && type!=REG_EXPAND_SZ) 
		return "";
	char *c=new char[size+1];
	res = RegQueryValueEx(key,name.c_str(),NULL,&type,(BYTE*)c,&size);
	if (res!=ERROR_SUCCESS) {
		delete[] c; 
		return "";
	}
	if (type==REG_EXPAND_SZ) { 
		char dummy[1]; 
		DWORD esize=ExpandEnvironmentStrings(c,dummy,0);
		if (esize!=0) { 
			char *d=new char[esize+1];
			ExpandEnvironmentStrings(c,d,esize+1);
			delete[] c; 
			c=d;
		}
	}
	string r(c);
	delete[] c;
	return r;
}

