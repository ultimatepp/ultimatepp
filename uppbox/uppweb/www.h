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

extern String uppsrc;
extern String uppbox;
extern String bazaar;
extern VectorMap<String, String> reflink;

String TopicFileName(const char *dir, const char *topic);
String TopicFileNameHtml(const char *topic);
String GatherTopics(VectorMap<String, Topic>& map, const char *topic, String& title);
String GatherTopics(VectorMap<String, Topic>& map, const char *topic);

String CppAsQtf(const String& s);

void GatherRefLinks(const char *upp);


#endif
