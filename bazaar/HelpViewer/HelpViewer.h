#ifndef _HelpViewer_HelpViewer_h
#define _HelpViewer_HelpViewer_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

/*
#define IMAGEFILE <HelpViewer/HelpViewer.iml>
#include <Draw/iml_header.h>
*/

class HelpViewer : public TopWindow
{
	private:
		Splitter splitter;
		ParentCtrl tocPane;
		RichTextView contentsPane;
		TreeCtrl mainTocTree;
		
		// reacts on TOC link selection
		void tocLinkCb();
		
		// reacts on link selection inside content pane
		void contentLinkCb(String const &link);
		
		ToolBar        toolbar;
		Zoom           zoom;
	
		struct Pos : Moveable<Pos>
		{
			String link;
			int    scy;
		};
		
		Vector<Pos>    back;
		Vector<Pos>    forward;
		String         current_link;
		
		// appends a treeCtrl to main tocCtrl
		void AppendTOC(TreeCtrl const &t, int curId = 0, int destId = 0);

	public:
		typedef HelpViewer CLASSNAME;
		
		HelpViewer();
		
		// loads TOC contents
		bool LoadTOC(String const &toc);
		
		// clears contents
		void Clear() { mainTocTree.Clear(); }
		
};

#endif

