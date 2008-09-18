//**********************************************************************************************
//* RssView.
//* --------
//* Simple Rss parser example, using Rss class.
//* Author: ismail yilmaz (oblivion)
//*

#include "RssView.h"

#define IMAGEFILE <RssView/RssView.iml>
#define IMAGECLASS RssImg
#include <Draw/iml_source.h>

void RssView::GetRssTags(int parent, RssTags& tags)
{
	
	for(int i = 0; i < tags.GetCount(); i++) {
		tree.Add(parent, Null, 
			Format("Element<%s>: %s %s", 
			ToUpper(tags[i].Tag()), 
			tags[i].Text(), 
			GetTagAttrs(tags[i]))
		);
	}
}

void RssView::GetRssNodes(int parent, RssNodes& nodes)
{
	for(int i = 0; i < nodes.GetCount(); i++) {
			int nid = tree.Add(parent, Null, 
				Format("NODE <%s>", nodes[i].Tag())
			);
			if(nodes[i].HasTag())
				GetRssTags(nid, nodes[i].Tags());
			if(nodes[i].HasNode()) 
				GetRssNodes(nid, nodes[i].Nodes());
		}
}

String RssView::GetTagAttrs(RssTag& tag)
{
	String out = Null;
	if(tag.HasAttr()) {
		RssAttributes& attrs = tag.Attrs();
		for(int i = 0; i < attrs.GetCount(); i++) {
			String key, val;
			val = attrs.Get(key = attrs.GetKey(i));
			out << key << ": (" << val << ") ";		                
		}
	}
	return out;
}


void RssView::Load(const char *filename)
{
	// 	Rss object.
	Rss rss;
	
	int64 l = GetFileLength(filename);
	if(l < 0 || l > 16000000)
		return;
	String feed = LoadFile(filename);
	
	// Clear the TreeCtrl.
	tree.NoRoot();
	tree.Show();
	tree.Clear();

	// Parse the Rss feed.
	if(!rss.Parse(feed)) {
		tree.Add(0, Null, "Parsing Error: " + rss.GetError());
		return;
	}

	// Now the rss is parsed successfully, we can set the correct character set
	// using Rss encoding data (not necessary though).
	SetDefaultCharset(CharsetByName(rss.Encoding()));	
	
	// There are two ways to get datta from a Rss class in parser state.
	// 1) Using element (or if you prefer "tag") specific methods:
	int rssid = tree.Add(0, Null, 
						Format("RSS (v%s) structure, encoding: (%s)",
	                   	rss.Version(),
	                   	rss.Encoding())
	                   );

	// 2) Using vectors to get group of elements.          
	RssChannels& channels = rss.Channels();
					
	// Also, Rss class supports multi-channel feeds.
	for(int i = 0; i < channels.GetCount(); i++) {
		int chid = tree.Add(rssid, Null, Format("CHANNEL (%d)", i + 1));
		GetRssTags(chid, channels[i].Tags());
		GetRssNodes(chid, channels[i].Nodes());
	}
	
	tree.OpenDeep(0);
	
}

void RssView::Enter()
{
	if(!files.IsCursor()) {
		Title(dir);
		return;
	}
	const FileList::File& f = files.Get(files.GetCursor());
	tree.Hide();
	view.Hide();
	if(f.name == "..") {
		Title(dir);
		return;
	}
	String p = AppendFileName(dir, f.name);
	Title(p);
	if(!f.isdir)
		Load(p);
}

void RssView::DoDir()
{
	if(!files.IsCursor())
		return;
	const FileList::File& f = files.Get(files.GetCursor());
	if(!f.isdir)
		return;
	if(f.name == "..") {
		String n = DirectoryUp(dir);
		LoadDir(dir);
		files.FindSetCursor(n);
	}
	else
		LoadDir(AppendFileName(dir, f.name));
}

void RssView::LoadDir(const char *d)
{
	files.Clear();
	dir = d;
	Title(dir);
#ifdef PLATFORM_WIN32
	if(dir.GetLength())
#else
	if(dir.GetLength() > 1)
#endif
		files.Add("..", CtrlImg::DirUp(), StdFont(), SColorText(), true);
	::Load(files, dir, "*.*");
	SortByExt(files);
}

bool RssView::Key(dword key, int)
{
	if(key == K_ENTER) {
		DoDir();
		return true;
	}
	return false;
}


void RssView::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
	s % files;
	s % dir;
	s % splitter;
}

RssView::RssView()
{
	tree.NoRoot();

	error.SetFont(Arial(20)).SetInk(Red);
	errorbg.Height(25).Add(error.SizePos());
	view.AddFrame(errorbg);
	view.SetReadOnly();
	view.SetColor(LineEdit::PAPER_READONLY, SColorPaper());

	tree.Hide();
	view.Hide();

	data.Add(tree.SizePos());
	data.Add(view.SizePos());

	splitter.Horz(files, data.SizePos());
	splitter.SetPos(2700);
	Add(splitter.SizePos());

	files.WhenEnterItem = THISBACK(Enter);
	files.WhenLeftDouble = THISBACK(DoDir);

	Sizeable().Zoomable();

	dir = GetCurrentDirectory();

	Icon(RssImg::RssIcon()).Title("RssView. Rss parser example");
}

GUI_APP_MAIN
{
	RssView x;
	LoadFromFile(x);
	x.LoadDir();
	x.Run();
	StoreToFile(x);	
}

