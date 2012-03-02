#ifndef _HelpViewer_HelpViewer_h
#define _HelpViewer_HelpViewer_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS HelpImg
#define IMAGEFILE <HelpViewer/HelpViewer.iml>
#include <Draw/iml_header.h>

class HelpViewer : public TopWindow
{
	private:
		Splitter splitter;
		ParentCtrl tocPane;
		RichTextView contentsPane;
		TreeCtrl mainTocTree;
		
		// go to a selected link
		void showLink(String const &link);

		// reacts on TOC link selection
		void tocLinkCb();
		
		// reacts on link selection inside content pane
		void contentLinkCb(String const &link);
		
		ToolBar toolBar;
	
		Vector<String> stack;
		int tos;
		
		// go previous/next
		void backCb(void);
		void fwCb(void);
		
		// toolbar construction
		void toolBarCb(Bar &bar);
		
		// appends a treeCtrl to main tocCtrl
		void AppendTOC(TreeCtrl const &t, int curId = 0, int destId = 0);

	public:
		typedef HelpViewer CLASSNAME;
		
		HelpViewer();
		
		// loads TOC contents
		bool LoadTOC(String const &toc);
		
		// follows a link
		void FollowLink(String const &link) { showLink(link); }
		
		// clears contents
		void Clear() { mainTocTree.Clear(); }
		
};

#endif

