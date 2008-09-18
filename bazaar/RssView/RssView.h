//**********************************************************************************************
//* RssView.
//* --------
//* Simple Rss parser example, using Rss class.
//* Author: ismail yilmaz (oblivion)
//*

#ifndef _RssView_RssView_h
#define _RssView_RssView_h

#include <CtrlLib/CtrlLib.h>
#include <Rss/Rss.h>

using namespace Upp;

#define IMAGEFILE <RssView/RssView.iml>
#define IMAGECLASS RssImg
#include <Draw/iml_header.h>

class RssView : public TopWindow {

public:
	virtual bool Key(dword key, int);

private:
	TreeCtrl              tree;
	LineEdit              view;
	FrameTop<StaticRect>  errorbg;
	Label                 error;
	FileList              files;
	ParentCtrl            data;
	Splitter              splitter;
	String                dir;

	void 	Load(const char *filename);
	void	GetRssTags(int parent, RssTags& tags);
	void	GetRssNodes(int parent, RssNodes& nodes);
	String  GetTagAttrs(RssTag& tag);
	void 	Enter();
	void 	DoDir();

public:
	typedef RssView CLASSNAME;

	void Serialize(Stream& s);
	void LoadDir(const char *d);
	void LoadDir()                      { LoadDir(dir); }

	RssView();
};

#endif

