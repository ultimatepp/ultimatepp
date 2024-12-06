#ifndef _uppweb_www_h_
#define _uppweb_www_h_

// upp web forum counter at 11/11 453604

#include <RichText/RichText.h>
#include <plugin/gif/gif.h>
#include <CtrlLib/CtrlLib.h>
#include <ide/Browser/Browser.h>

#include "Htmls.h"

using namespace Upp;

void InitWwwTpp();

extern String uppsrc;
extern String devsrc;
extern String uppbox;
extern String rootdir;
extern VectorMap<String, String> reflink;

String TopicFileName(String dir, String topic);

String TopicFileNameHtml(const char *topic, bool dev);

String GatherTopics(ArrayMap<String, Topic>& map, Vector<String>& ttFullIds, const char *topic, String& title, String parentIds, bool dev);
String GatherTopics(ArrayMap<String, Topic>& map, Vector<String>& ttFullIds, const char *topic, String parentIds, bool dev);

String CppAsQtf(const String& s);

void GatherRefLinks(const char *upp, bool dev);

String BytesToString(uint64 _bytes, bool units = true);

String MakeExamples(const char *dir, const char *www, int language, String parentIds);

#endif
