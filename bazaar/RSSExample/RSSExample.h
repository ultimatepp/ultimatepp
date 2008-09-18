#ifndef _RSSExample_RSSExample_h
#define _RSSExample_RSSExample_h

#include <CtrlLib/CtrlLib.h>
#include <Rss/Rss.h>
using namespace Upp;

#define LAYOUTFILE <RSSExample/RSSExample.lay>
#include <CtrlCore/lay.h>



class RSSExample : public WithRSSExampleLayout<TopWindow> {
public:
	typedef RSSExample CLASSNAME;
	RSSExample();
	// Rss class encapsulates RSS web Syndication format feeds. (v2.0).
	// The Rss class can act both as a parser and as a composer. 
	// Also same instance of Rss class can be used as both at once.
	// This example demonstrates the above mentioned behaviour.
	void	ComposeRss();
	void	ParseRss(String& feed);

	// Helper methods.
	void	GetRssTags	(int parent, RssTags& tags);
	void	GetRssNodes	(int parent, RssNodes& nodes);
	String  GetTagAttrs	(RssTag& tag);
	
	Rss		rss;						
};

#endif

