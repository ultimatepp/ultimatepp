#ifndef _uppweb_www_h_
#define _uppweb_www_h_

// upp web forum counter at 11/11 453604

#include <Web/Web.h>
#include <RichText/RichText.h>
#include <plugin/gif/gif.h>
#include <CtrlLib/CtrlLib.h>
#include <ide/Browser/Browser.h>

using namespace Upp;

#ifdef _MULTITHREADED
#define MTC
#endif

void InitWwwTpp();

struct SvnListRev : Moveable <SvnListRev> {
        String author;
        int revision;
        Time time;
        String fullPath;
};

struct SvnLogRev : Moveable <SvnLogRev> {
        String revision;
        String author;
        Time time;
        struct SvnChange : Moveable <SvnChange> {
                String kind;
                String action;
                String path;
        };
        Vector<SvnChange> changes;
        String msg;
        Index<String> tags;
        bool major;
};

struct SvnBazaarItems : Moveable <SvnBazaarItems> {
	String name;
	int64 size;
	Time lastChange;
	String description;
	String authors;
	String status;
	String externalDependencies;
	String supportedOS;
	String imagePath;
};

extern String uppsrc;
extern String uppbox;
extern String bazaar;
extern String rootdir;
extern VectorMap<String, String> reflink;
extern VectorMap<String, SvnListRev> svndata;
extern Vector<SvnLogRev> svnlog;

String TopicFileName(const char *dir, const char *topic);
String TopicFileNameHtml(const char *topic);
String GatherTopics(VectorMap<String, Topic>& map, Vector<String>& ttFullIds, const char *topic, String& title, String parentIds);
String GatherTopics(VectorMap<String, Topic>& map, Vector<String>& ttFullIds, const char *topic, String parentIds);

String CppAsQtf(const String& s);

void GatherRefLinks(const char *upp);

void GetSvnList(VectorMap<String, SvnListRev> &data, const String &);
void GetSvnLog(Vector<SvnLogRev> &log, int limit = -1);
String SvnChanges(Vector<SvnLogRev> &log, int limit, String filter, bool major = false);
Vector <SvnBazaarItems> SvnBazaarList(String bazaarPath, Vector<SvnLogRev> &log);
String BytesToString(uint64 _bytes, bool units = true);

String MakeExamples(const char *dir, const char *www, int language, String parentIds);

#endif
