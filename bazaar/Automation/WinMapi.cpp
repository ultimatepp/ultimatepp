#include <windows.h>
#include <list>
#include <vector>
#include "WinMapiUtils.h"
using namespace std;

class Mapi
{
public:
	Mapi();
	~Mapi();
};
Mapi::Mapi()
{
	mapi_EnsureLibraries();
	if (mapi_Libraries.size() > 0) {// we require at least one mapi-x implementation
		mapi_EnsureStores(mapi_Libraries.front().path);
	}
}
Mapi::~Mapi()
{
	mapi_EnsureFinished();
}
class MapiStores
{
public:
	MapiStores(Mapi &mapi);
	bool GetInfo(int i, char *profile, char *store);
	int GetCount() {return (int) profiles.size();};
private:
	vector<string> profiles;
	vector<string> stores;
};
// Load a list of all the stores
MapiStores::MapiStores(Mapi &mapi)		// No hago nada con mapi ya que esta como variables en mapi_utils.h
{
	mapi_EnsureStores(mapi_Libraries.front().path);
	for (list<mapi_TStoreInfo>::const_iterator j = mapi_Stores.begin(); j != mapi_Stores.end(); j++) {
		if (j->type == mstStore) { //... do something with j->profile and j->store
			profiles.push_back(j->profile); 
			stores.push_back(j->store);
		}
	}
}
bool MapiStores::GetInfo(int i, char *profile, char *store)
{
	if (i >= (int) profiles.size()) 
		return false;
	strcpy(profile, profiles[i].c_str());
	strcpy(store, stores[i].c_str());
	return true;
}
class MapiMessages;

class MapiFolders 
{
	friend class MapiMessages;
public:
	MapiFolders(char *profile, char *storename);
	~MapiFolders();
	bool GetInfo(int i, char *folder, unsigned long &num);
	bool GetMessages(int ifolder);
	int GetCount() {return (int) folderNames.size();};
private:
	vector<string> folderNames;
	vector<unsigned long> numMsg;
	vector<IMAPIFolder *> iFolders;
};
MapiFolders::MapiFolders(char *_profile, char *_storename)		// No hago nada con mapi ya que esta como variables en mapi_utils.h
{
	// To iterate over all folders.
	// The following code code, given a profile/storename,
	// retrieves the list of folders in it, and iterates over
	// all the folders. It assumes a variable 'hwnd' which
	// is the window-handle of the application's main window.
	HWND hwnd=0;
	//IMAPIFolder *best_folder=0; 
	//string best_folder_name; // we'll pick a random folder
	//
	string profile = _profile, storename = _storename;
	mapi_EnsureFolders(hwnd, profile, storename);
	for (list<mapi_TFolderInfo>::const_iterator i = mapi_Folders.begin(); i != mapi_Folders.end(); i++) { 
		IMAPIFolder *ifolder; 
		ULONG ftype;
		HRESULT hr = mapi_msgstore->OpenEntry(i->eid.size,i->eid.ab, NULL, 0,& ftype, (IUnknown**)&ifolder);
		if (hr == S_OK) { 
			if (ftype == MAPI_FOLDER) { // ... and do something with the ifolder, like iterating over its messages
				ULONG num_messages=0;
				IMAPITable *contents;
				HRESULT hr = ifolder->GetContentsTable(0,&contents);
				if (hr == S_OK) { 
					contents->GetRowCount(0, &num_messages); 
					contents->Release();
					folderNames.push_back(i->name); 
					numMsg.push_back(num_messages);
					iFolders.push_back(ifolder);
				}
			} 
			//ifolder->Release();
		}
	}
}
bool MapiFolders::GetInfo(int i, char *folderName, unsigned long &num)
{
	if (i >= (int) folderNames.size()) 
		return false;
	strcpy(folderName, folderNames[i].c_str());
	num = numMsg[i];
	return true;
}
MapiFolders::~MapiFolders()
{
	for (int i = 0; i < GetCount(); ++i)
		iFolders[i]->Release();
}

class MapiMessages
{
public:
	MapiMessages(MapiFolders *folders, int _ifolder);
	~MapiMessages() ;
	bool GetInfo(int i, char *sender, char *subject, char *date);
	int GetCount() {return (int) senders.size();};
private:
	IMAPIFolder *ifolder;
	vector<string> senders;
	vector<string> subjects;
	vector<string> dates;
};
MapiMessages::MapiMessages(MapiFolders *folders, int _ifolder)
{
	// To iterate over messages in a folder.
	// The following code, given a pointer 'ifolder' to
	// an IMAPIFolder, iterates over all the messages in it.

	if (_ifolder > folders->GetCount()) {
		printf("\nFolder no existe");
		return;
	}

	ifolder = folders->iFolders[_ifolder];
	//
	IMAPITable *contents;
	HRESULT hr = ifolder->GetContentsTable(0, &contents);
	if (hr != S_OK) { 
		printf ("\nFallo GetContentsTable");
		return;
	}
	SizedSPropTagArray(4, foldcols) = {4, {PR_ENTRYID, PR_SUBJECT, PR_SENT_REPRESENTING_NAME, PR_MESSAGE_DELIVERY_TIME} };
	SRowSet *frows;
	hr = pHrQueryAllRows(contents, (SPropTagArray*)&foldcols,NULL,NULL,0, &frows);
	if (hr != S_OK) { 
		printf ("\nFallo pHrQueryAllRows");
		ifolder->Release();
		return;
	}
	for (unsigned int m = 0; m < frows->cRows; m++) { 
		// We'll first retrieve the basic header information of the message.
		// We must do this now, from the contents-table, because if the
		// message is offline and we're using Outlook2000/IMO then it's not
		// possible to IMessage::GetProps.
		// NB. In these headers, "sent-representing" is available, but none of the
		// other sender properties are.
		mapi_TEntryid eid; 
		string subj, sndr, date;
		if (frows->aRow[m].lpProps[0].ulPropTag == PR_ENTRYID) 
			eid = &frows->aRow[m].lpProps[0];
		if (frows->aRow[m].lpProps[1].ulPropTag == PR_SUBJECT) 
			subj = frows->aRow[m].lpProps[1].Value.lpszA;
		if (frows->aRow[m].lpProps[2].ulPropTag == PR_SENT_REPRESENTING_NAME) 
			sndr = frows->aRow[m].lpProps[2].Value.lpszA;
		if (frows->aRow[m].lpProps[3].ulPropTag == PR_MESSAGE_DELIVERY_TIME) { 
			FILETIME ft; 
			FileTimeToLocalFileTime(&frows->aRow[m].lpProps[3].Value.ft, &ft); // Translate from UTC to current timezone
			SYSTEMTIME st; 
			FileTimeToSystemTime(&ft,&st);
			const char *days[] = {"Sun ","Mon ","Tue ","Wed ","Thu ","Fri ","Sat "};
			const char *day=0; 
			if (st.wDayOfWeek<=6) 
				day = days[st.wDayOfWeek];
			const char *months[] = {"","Jan ","Feb ","Mar ","Apr ","May ","Jun ","Jul ","Aug ","Sep ","Oct ","Nov ","Dec "};
			const char *month = 0; 
			if (st.wMonth >= 1 && st.wMonth <= 12) 
				month = months[st.wMonth];
			char c[20]; 
			wsprintf(c, "%s%i %s%i", day, st.wDay, month,st.wYear); 
			date = c;
		}
		subjects.push_back(subj);
		senders.push_back(sndr);
		dates.push_back(date);
		// Now, 'message_details' shows the important stuff about the message,
		// so we could use it in (e.g.) error reports if some other bit of code fails.
		if (!eid.isempty()) { 
			IMessage *imsg; 
			ULONG msgtype;
			hr = ifolder->OpenEntry(eid.size, eid.ab, NULL, 0, &msgtype, (IUnknown**)&imsg);
			if (hr == S_OK) { 
				if (msgtype == MAPI_MESSAGE) { 
					bool ShowMessage(IMessage *imsg);
					bool showed = ShowMessage(imsg);
				}
				imsg->Release();
			}
		}
	}
	pFreeProws(frows);
	contents->Release();
}
bool MapiMessages::GetInfo(int i, char *sender, char *subject, char *date)
{
	if (i >= (int) senders.size()) 
		return false;
	strcpy(sender,	senders[i].c_str());
	strcpy(subject, subjects[i].c_str());
	strcpy(date,	dates[i].c_str());
	return true;
}
MapiMessages::~MapiMessages()
{
}

int mapimain()
{
	// Get a list of all the stores
	Mapi mapiSystem;
	MapiStores stores(mapiSystem);

	printf("\nStores disponibles:");
	for (int i = 0; i < stores.GetCount(); ++i) {
		char profil[255], store[255];
		stores.GetInfo(i, profil, store);
		printf("\n >Profile=%s Store=%s", profil, store);
	}
	char *profile = "Outlook", *storename = "Correo";
	printf("\nFolders en Store: profile=%s store=%s", profile, storename);
	MapiFolders folders(profile, storename);
	for (int i = 0; i < folders.GetCount(); ++i) {
		char folder[255]; unsigned long numMessages;
		folders.GetInfo(i, folder, numMessages);
		printf("\n >Folder=%s Num mensajes=%lu", folder, numMessages);
	}
	MapiMessages messages(&folders, 0);
	printf("\nPulsa para continuar");	getchar();
	for (int i = 0; i < messages.GetCount(); ++i) {
		char sender[255], subject[1024], date[50]; 
		messages.GetInfo(i, sender, subject, date);
		printf("\n >Mensage=%d Date: %s\nSender: %s\nSubject: %s", i, date, sender, subject);
	}
	printf("\nPrograma finalizado -");	getchar();
	return 0;
}



bool ShowMessage(IMessage *imsg)
{
	// To retrieve message text and properties.
	// The following code, given a pointer 'imsg' to an IMessage,
	// retrieves the body of the message in both text and
	// (if present) html, and also retrieves its 'from' and 'to'
	// information, and determines whether the message is available offline.
	bool AvailableOffline=false;
	SizedSPropTagArray(8, mcols) = {8,
        {PR_MESSAGE_CLASS, PR_SENDER_NAME, PR_SENDER_EMAIL_ADDRESS, PR_RTF_IN_SYNC,
         PR_RECEIVED_BY_EMAIL_ADDRESS, PR_RECEIVED_BY_NAME, PR_RECEIVED_BY_ENTRYID, PR_TRANSPORT_MESSAGE_HEADERS}};
	ULONG pcount; 
	SPropValue *props=0; 
	HRESULT hr;
	hr = imsg->GetProps((SPropTagArray*)&mcols, 0, &pcount, &props);
	// Outlook2000 in IMO gives this NO_ACCESS error messages that are header-only.
	// But Outlook97, and Outlook2000/CWG, and OutlookXP, don't give the error...
	// we'll test for them later on.
	if (hr == MAPI_E_NO_ACCESS) 
		AvailableOffline = false;
	bool okay = true;
	if (hr!=S_OK && hr != MAPI_W_ERRORS_RETURNED) 
		okay=false;
	if (okay && props[0].ulPropTag != PR_MESSAGE_CLASS) 
		okay=false;
	if (okay && (strncmp(props[0].Value.lpszA, "IPM.Note", 8) != 0 && strncmp(props[0].Value.lpszA, "IPF.Note",8)!= 0)) 
		okay=false;
	if (!okay) {
		if (props != 0) 
			pMAPIFreeBuffer(props); 
		return false; 
	}
	// Was it a received message, or one that the user sent?
	// (This isn't recorded explicitly. We'll use a heurstic instead:
	// if there's any sign that this message has been over the internet,
	// then presumably it's received. If there's no such sign, then
	// presumably it's just the local copy put into the 'Sent' folder)
	bool isrcvd = false;
	if (props[4].ulPropTag == PR_RECEIVED_BY_EMAIL_ADDRESS) 
		isrcvd = true;
	if (props[5].ulPropTag == PR_RECEIVED_BY_NAME) 
		isrcvd = true;
	if (props[6].ulPropTag == PR_RECEIVED_BY_ENTRYID) 
		isrcvd = true;
	if (props[7].ulPropTag == PR_TRANSPORT_MESSAGE_HEADERS) 
		isrcvd = true;

	// Retrieve the 'from' property
	// in the form Name <email@addr>
	string from;
	if (props[1].ulPropTag == PR_SENDER_NAME) 
		from = props[1].Value.lpszA;
	if (props[2].ulPropTag == PR_SENDER_EMAIL_ADDRESS) { 
		if (from != "") 
			from += " ";
		from += string("<") + props[2].Value.lpszA + ">";
	}
	if (from != "") 
		AvailableOffline = true;

	// Retrieve the 'to' property
	// in the form Name <email@addr>, AnotherName <email@addr>
	string to;
	IMAPITable *rtable;
	hr = imsg->GetRecipientTable(0,&rtable);
	if (hr == S_OK) { 
		SizedSPropTagArray(3,rcols) = {3, {PR_DISPLAY_NAME, PR_EMAIL_ADDRESS, PR_RECIPIENT_TYPE} };
		SRowSet *rrows;
		hr = pHrQueryAllRows(rtable,(SPropTagArray*)&rcols,NULL,NULL,0,&rrows);
		if (hr == S_OK) { 
			for (unsigned int r = 0; r < rrows->cRows; r++) { 
				string recipient;
				if (rrows->aRow[r].lpProps[0].ulPropTag == PR_DISPLAY_NAME) 
					recipient = rrows->aRow[r].lpProps[0].Value.lpszA;
				if (rrows->aRow[r].lpProps[1].ulPropTag == PR_EMAIL_ADDRESS) { 
					if (recipient != "") 
						recipient += " ";
					recipient += string("<") + rrows->aRow[r].lpProps[1].Value.lpszA + ">";
				}
				if (recipient != "") { 
					if (to != "") 
						to += ", ";
					to += recipient;
				}
			}
			pFreeProws(rrows);
		}
		rtable->Release();
	}
	if (to != "") 
		AvailableOffline = true;

	// Get the body of the message as plain text
	// into the buffer 'bodybuf'
	char *bodybuf = 0; 
	unsigned int bodysize = 0;
	IStream *istream;
	hr = imsg->OpenProperty(PR_BODY, &IID_IStream, STGM_READ, 0, (IUnknown**)&istream);
	if (hr == S_OK) { 
		AvailableOffline = true;
		STATSTG stg = {0};
		hr = istream->Stat(&stg,STATFLAG_NONAME);
		if (hr==S_OK) { 
			bodysize = stg.cbSize.LowPart; // won't bother checking for >2gb messages!
			bodybuf = new char[bodysize+1];
			ULONG red; 
			hr = istream->Read(bodybuf, bodysize, &red);
			if (hr!=S_OK) 
				bodysize = 0;
			else if (red < bodysize) 
				bodysize = red;
			bodybuf[bodysize] = 0;
		}
		istream->Release();
	}

	// Get the body of the message if it was in HTML
	// into the buffer 'htmlbuf'
	char *htmlbuf=0; 
	unsigned int htmlsize=0;
	hr = imsg->OpenProperty(PR_BODY_HTML, &IID_IStream, STGM_READ, 0, (IUnknown**)&istream);
	if (hr==S_OK) { 
		STATSTG stg = {0};
		hr = istream->Stat(&stg,STATFLAG_NONAME);
		if (hr==S_OK) { 
			htmlsize = stg.cbSize.LowPart;
			htmlbuf = new char[htmlsize+1];
			ULONG red; 
			hr = istream->Read(htmlbuf, htmlsize, &red);
			if (hr!=S_OK) 
				htmlsize = 0;
			else if (red < htmlsize) 
				htmlsize = red;
			htmlbuf[htmlsize] = 0;
		}
		istream->Release();
	}
	// In actual fact, the PR_HTML_BODY that we just tested is rarely used.
	// More frequently, Microsoft encode the html message source into the RTF...
	// First we have to sync the rtf with the body, in case the messsagestore hasn't already.
	// (nb. it would have made more sense to query the support mask just once, rather than
	// for each message as we're doing here)
	SPropValue *svmask; 
	bool mustsync = true;
	hr = pHrGetOneProp(mapi_msgstore, PR_STORE_SUPPORT_MASK, &svmask);
	if (hr==S_OK) { 
		if ((svmask->Value.ul&STORE_RTF_OK) != 0) 
			mustsync = false;
		else if (props[3].ulPropTag == PR_RTF_IN_SYNC && props[3].Value.b != 0) 
			mustsync = false;
		pMAPIFreeBuffer(svmask);
	}
	if (mustsync) { 
		BOOL isupdated; 
		pRTFSync(imsg,RTF_SYNC_BODY_CHANGED,&isupdated);
		if (isupdated) 
		  imsg->SaveChanges(0);
	}
	// Now it's synced if necessary. We can retrieve the RTF property
	hr = S_FALSE; 
	if (htmlbuf==0) 
		hr = imsg->OpenProperty(PR_RTF_COMPRESSED, &IID_IStream, STGM_READ, 0, (IUnknown**)&istream);
	if (hr==S_OK) { 
		AvailableOffline = true;
		IStream *iunstream; // for the uncompressed stream
		pWrapCompressedRTFStream(istream,0,&iunstream);
		// bufsize is the size of the buffer we've allocated, and htmlsize is the
		// amount of text we've read in so far. If our buffer wasn't big enough,
		// we enlarge it and continue. We have to do this, instead of allocating
		// it up front, because Stream::Stat() doesn't work for the unc.stream
		unsigned int bufsize=10240; 
		htmlbuf = new char[bufsize];
		htmlsize=0; 
		bool done = false;
		while (!done) { 
			ULONG red; 
			hr = iunstream->Read(htmlbuf+htmlsize, bufsize-htmlsize, &red);
			if (hr != S_OK) {
				htmlbuf[htmlsize] = 0; 
				done = true;
			} else { 
				htmlsize += red; 
				done = (red < bufsize-htmlsize);
				if (!done) { 
					unsigned int newsize=2*htmlsize; 
					char *newbuf = new char[newsize];
					memcpy(newbuf,htmlbuf,htmlsize); 
					delete[] htmlbuf;
					htmlbuf = newbuf; 
					bufsize=newsize;
				}
			}
		}
		htmlbuf[htmlsize]=0;
		iunstream->Release();
		istream->Release();
		// Now, assuming that this thing was an encoded RTF/HTML hybrid,
		// we can extract the original html.
		if (htmlbuf != 0) { 
			if (!isrtfhtml(htmlbuf,htmlsize)) {
				delete[] htmlbuf; 
				htmlbuf=0;
			} else 
				decodertfhtml(htmlbuf,&htmlsize);
		}
	}

	// Check if there are any attachments
	IMAPITable *atable;
	int numAttachments;
	hr = imsg->GetAttachmentTable(0, &atable);
	if (hr == S_OK) { 
		SizedSPropTagArray(3,acols) = {3, {PR_ATTACH_SIZE, PR_ATTACH_NUM, PR_ATTACH_METHOD }};
		SRowSet *arows;
		hr = pHrQueryAllRows(atable, (SPropTagArray*)&acols, NULL, NULL, 0, &arows);
		if (hr == S_OK) { 
			numAttachments = arows->cRows;
			if (arows->cRows > 0) 
				AvailableOffline = true;
			pFreeProws(arows);
		}
		atable->Release();
	}
	// AvailableOffline? We have set this boolean variable through the preceeding
	// code, using the following heuristic: if there is any content to the message
	// at all be it To: or From: lines, or body, or attachments), then it must have
	// been downloaded. The only way this could fail, is if the user composed a
	// draft message but then closed it while it was still completely empty.
	// Why do we resort to such a grubby heuristic? Because Microsoft do not define
	// any other way to tell whether a message is offline. OutlookXP uses some
	// named properties that seem to contain the information, but they're not
	// documented and therefore can't be trusted.

	if (AvailableOffline) { 
	  // Now, do something with the message contents!
	  //
		printf("\n-------------------------------------------------------------------");
		printf("\n>MESSAGE... %s", isrcvd ? "Received" : "Sent");
		printf("\n-------------------------------------------------------------------");
		printf("\n>FROM %s", from.c_str());
		printf("\n-------------------------------------------------------------------");
		printf("\n>TO %s", to.c_str());
		printf("\n-------------------------------------------------------------------");
		printf("\n>ATTACHMENTS %d", numAttachments);
		printf("\n-------------------------------------------------------------------");
		printf("\n>BODY\n%s", bodybuf);
		printf("\n-------------------------------------------------------------------");
		printf("\n>HTML\n%s", htmlbuf);
	}

	if (bodybuf != 0) 
		delete[] bodybuf;
	if (htmlbuf != 0) 
		delete[] htmlbuf;
	pMAPIFreeBuffer(props);

	return AvailableOffline;
}
