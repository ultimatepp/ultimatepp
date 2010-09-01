#include "HelpViewer.h"

/*
#define IMAGEFILE <HelpViewer/HelpViewer.iml>
#include <Draw/iml_source.h>
*/

const char *TOPICLINK = "^topic`:`/`/";

HelpViewer::HelpViewer()
{
	// setups splitter and its contents
	Add(splitter);
	splitter.Horz(tocPane, contentsPane).SetPos(2500);
	contentsPane.AutoHideSb();
	contentsPane.SetZoom(Zoom(1,1));
	
	tocPane.Add(mainTocTree.NoRoot().SizePos());
	
	// setup TOC link callback
	mainTocTree.WhenSel = THISBACK(tocLinkCb);
	contentsPane.WhenLink << THISBACK(contentLinkCb);
	
	zoom.m = 160;
	zoom.d = 1000;
	
	Sizeable().Zoomable();
}

// appends a treeCtrl to main tocCtrl
void HelpViewer::AppendTOC(TreeCtrl const &t, int curId, int destId)
{
	// adds root node of current level
	int destChild = mainTocTree.Add(destId, t.GetNode(curId));
	
	// recursively add all children
	for(int i = 0; i < t.GetChildCount(curId); i++)
		AppendTOC(t, t.GetChild(curId, i), destChild);
}

// Parses TOC and fills tocTree control
bool HelpViewer::LoadTOC(String const &tocLink)
{
	// TOC is composed by QTF lines with a TOC link and label text
	// lines not containing the TOC link are simply ignored
	// TOC level is represented by number of TABS contained
	// inside a line; zero tabs means toplevel, id 0 of tocTree
	Vector<int>ids;
	ids.Add(0);
	int curLevel = 0;
	int curId = 0;
	TreeCtrl tocTree;
	
	Topic t = GetTopic(tocLink);
	if(IsNull(t.text))
		return false;

	String label = t.label;
	String topic = t.link;

	// sets toc root string from topic ticle
	tocTree.SetRoot(Null, t.title);
	
	// converts QTF to RichTxt, easier to analyze
	RichText txt = ParseQTF(t.text);

	// extract all paragraphs from RichTxt, skip other objects
	for(int iPart = 0; iPart < txt.GetPartCount(); iPart++)
	{
		String tocLine;
		String lineLink;
		if(!txt.IsPara(iPart))
			continue;
		RichPara p = txt.Get(iPart);
		for(int iParaPart = 0; iParaPart < p.GetCount(); iParaPart++)
		{
			RichPara::Part &paraPart = p.part[iParaPart];
			if(paraPart.format.link != "")
				lineLink = paraPart.format.link;
			if(paraPart.NonText())
				continue;
			tocLine += paraPart.text.ToString();
		}
		// if part contains no text nor link to topic stuffs, simply skip it
		if(tocLine == "" || lineLink == "" || !lineLink.StartsWith("topic:"))
			continue;
		
		// now we should count tabls to get TOC indent level
		int level = 0;
		int j;
		while( (j = tocLine.Find(0x09)) >= 0)
		{
			level++;
			tocLine.Remove(j);
		}
		// allows just SINGLE UPPER level change, i.e., deeper ONE level from current
		// for each step; backleveling is possible at any depth
		if(level > curLevel+1)
			return false;
		if(level > curLevel)
		{
			ids.Add(curId);
			curLevel++;
		}
		else while(curLevel > level)
		{
			curLevel--;
			ids.Pop();
		}

		// add the line to correct tree node
		curId = tocTree.Add(ids.Top(), Null, Value(lineLink), Value(tocLine));
	}
	
	// if the tree control is non empty, appends it to the main TOC TreeCtrl
	if(tocTree.GetChildCount(0))
		AppendTOC(tocTree);
	return true;
}
		
// reacts on link selection inside content pane
void HelpViewer::contentLinkCb(String const &link)
{
	Topic t = GetTopic(link);
	if(!IsNull(t.text))
	{
		String label = t.label;
		RichText txt = ParseQTF(t.text);
		contentsPane.Pick(txt, zoom);
		contentsPane.GotoLabel(label, true);
		current_link = link;
	}
}


void HelpViewer::tocLinkCb()
{
	int id = mainTocTree.GetCursor();
	if(id < 0)
		return;
	
	// gets link
	String link = mainTocTree.Get(id);
	
	contentLinkCb(link);
}
