#ifndef _uppweb_www_h_
#define _uppweb_www_h_


#include <Web/Web.h>
#include <RichText/RichText.h>
#include <plugin/gif/gif.h>
#include <CtrlLib/CtrlLib.h>
#include <ide/Browser/Browser.h>

using namespace Upp;

#ifdef _MULTITHREADED
#define MTC
#endif

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

extern String uppsrc;
extern String uppbox;
extern String bazaar;
extern String rootdir;
extern VectorMap<String, String> reflink;
extern VectorMap<String, SvnListRev> svndata;
extern Vector<SvnLogRev> svnlog;

String TopicFileName(const char *dir, const char *topic);
String TopicFileNameHtml(const char *topic);
String GatherTopics(VectorMap<String, Topic>& map, const char *topic, String& title);
String GatherTopics(VectorMap<String, Topic>& map, const char *topic);

String CppAsQtf(const String& s);

void GatherRefLinks(const char *upp);

void GetSvnList(VectorMap<String, SvnListRev> &data, const String &);
void GetSvnLog(Vector<SvnLogRev> &log);
String SvnChanges(Vector<SvnLogRev> &log, int limit, String filter, bool major = false);

#endif
