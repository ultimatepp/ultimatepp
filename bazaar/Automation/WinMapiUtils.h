#ifndef mapi_utilsH
#define mapi_utilsH
#include <windows.h>
#include <mapix.h>
#include <mapiutil.h>
#include <string>
#include <list>

void mapi_EnsureLibraries();
void mapi_EnsureStores(const std::string libpath);
void mapi_EnsureFolders(HWND h, const std::string profile, const std::string store);
void mapi_EnsureFolders(HWND h, const std::string pst_fn);
void mapi_EnsureFinished();

bool isrtfhtml(const char *buf,unsigned int len);
void decodertfhtml(char *buf,unsigned int *len);

class mapi_TEntryid
{ 
public:
	unsigned int size;
	ENTRYID *ab;
	mapi_TEntryid() {ab=0;size=0;}
	mapi_TEntryid(SPropValue *v) 
	{
		ab=0;
		size=0; 
		if (v->ulPropTag!=PR_ENTRYID) 
			return; 
		set(v->Value.bin.cb,(ENTRYID*)v->Value.bin.lpb);
	}
	mapi_TEntryid(const mapi_TEntryid &e) 
	{
		ab=0;
		size=0; 
		set(e.size,e.ab);
	}
	mapi_TEntryid(unsigned int asize,ENTRYID *eid) 
	{
		ab=0;
		size=0; 
		set(asize,eid);
	}
	mapi_TEntryid &operator= (const mapi_TEntryid *e) 
	{
		set(e->size,e->ab); 
		return *this;
	}
	mapi_TEntryid &operator= (const SPropValue *v) 
	{
		set(0,0); 
		if (PROP_TYPE(v->ulPropTag)!=PT_BINARY) 
			return *this; 
		set(v->Value.bin.cb,(ENTRYID*)v->Value.bin.lpb); 
		return *this;
	}
	~mapi_TEntryid() {set(0,0);}
	void set(unsigned int asize, ENTRYID *eid) 
	{
		if (ab!=0) 
			delete[] ((char*)ab); 
		size=asize; 
		if (eid==0) 
			ab = 0; 
		else {
			ab = (ENTRYID*)(new char[size]);
			memcpy(ab,eid,size);
		}
	}
	void clear() {set(0,0);}
	bool isempty() const {return (ab==0 || size==0);}
	bool isequal(IMAPISession *sesh, mapi_TEntryid const &e) const
	{ 
		if (isempty() || e.isempty()) 
			return false;
		ULONG res; 
		HRESULT hr = sesh->CompareEntryIDs(size,ab,e.size,e.ab,0,&res);
		if (hr!=S_OK) 
			return false;
		return (res!=0);
	}
};

typedef struct {
	std::string name, path; 
	bool supported;
} mapi_TLibraryInfo;

enum mapi_TFolderType {mftInbox,mftOutbox,mftSent,mftDeleted,mftCalendar,mftContacts,mftJournal,mftNotes,mftTasks,mftSpecial,mftMail,mftStuff};
typedef struct {int depth; std::string name, path; mapi_TFolderType type; mapi_TEntryid eid;} mapi_TFolderInfo; // nb. path is the complete thing, and name is just the final bit of it

enum mapi_TStoreType {mstProfile, mstProfileSecret, mstStore};
typedef struct {std::string profile, store; mapi_TStoreType type;} mapi_TStoreInfo;

// These are initialized by mapi_EnsureLibraries()
extern std::list<mapi_TLibraryInfo> mapi_Libraries;
// These are initialized by mapi_EnsureStores(lib)
extern std::list<mapi_TStoreInfo> mapi_Stores;
// And so are these mapi functions
typedef HRESULT (STDMETHODCALLTYPE RTFSYNC)(LPMESSAGE lpMessage, ULONG ulFlags, BOOL FAR *lpfMessageUpdated);
typedef HRESULT (STDMETHODCALLTYPE WRAPCOMPRESSEDRTFSTREAM)(LPSTREAM lpCompressedRTFStream, ULONG ulFlags, LPSTREAM FAR *lpUncompressedRTFStream);
extern RTFSYNC *pRTFSync;
extern WRAPCOMPRESSEDRTFSTREAM *pWrapCompressedRTFStream;
extern MAPIFREEBUFFER *pMAPIFreeBuffer;

// These are initialized by mapi_EnsureFolders(storeinfo)
extern IMAPISession *mapi_session;    // The session
extern IMsgStore *mapi_msgstore;      // The message store
extern std::list<mapi_TFolderInfo> mapi_Folders;
// And all are freed, if necessary, by mapi_EnsureFinished.

// I must implement these utility functions myself. That's because
// they're not present in Outlook97's version of mapi32.dll.
HRESULT pHrGetOneProp(LPMAPIPROP lpMapiProp, ULONG ulPropTag, LPSPropValue FAR *lppProp);
void pFreeProws(LPSRowSet lpRows);
HRESULT pHrQueryAllRows(LPMAPITABLE lpTable, LPSPropTagArray lpPropTags, LPSRestriction lpRestriction, LPSSortOrderSet lpSortOrderSet, LONG crowsMax, LPSRowSet FAR *lppRows);

// These were omitted from the standard headers
#ifndef PR_BODY_HTML
#define PR_BODY_HTML (PROP_TAG(PT_TSTRING,0x1013))
#endif

#ifndef PR_ATTACH_CONTENT_ID
#define PR_ATTACH_CONTENT_ID (PROP_TAG(PT_TSTRING,0x3712))
#endif

#ifndef PR_ATTACH_CONTENT_LOCATION
#define PR_ATTACH_CONTENT_LOCATION (PROP_TAG(PT_TSTRING,0x3713))
#endif

#ifndef PR_ATTACH_FLAGS
#define PR_ATTACH_FLAGS (PROP_TAG(PT_LONG,0x3714))
#endif

#ifndef PR_ATTACH_TRANSPORT_NAME
#define PR_ATTACH_TRANSPORT_NAME (PROP_TAG(PT_TSTRING,0x370C))
#endif

#ifndef PR_ATTACH_MIME_SEQUENCE
#define PR_ATTACH_MIME_SEQUENCE (PROP_TAG(PT_LONG,0x3710))
#endif

#ifndef PR_SMTP_MESSAGE_ID
#define PR_SMTP_MESSAGE_ID (PROP_TAG(PT_TSTRING,0x1035))
#endif

HRESULT AutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs...);

#endif