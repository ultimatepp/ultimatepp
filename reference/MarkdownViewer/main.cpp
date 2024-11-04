#include <CtrlLib/CtrlLib.h>

#include <plugin/md/Markdown.h>
#include <plugin/bmp/bmp.h>
#include <plugin/jpg/jpg.h>

using namespace Upp;

struct MarkdownViewer : TopWindow {
	RichTextView view;
	MenuBar menubar;
	Array<QtfRichObject> images;
	
	MarkdownViewer()
	{
		Title(t_("A simple markdown viewer"));
		Sizeable().Zoomable().SetRect(0, 0, 800, 600);
		Add(view.SizePos());
		AddFrame(menubar);
		menubar.Set([this](Bar& menu)
		{
			menu.Sub(t_("File"), [this](Bar& menu)
			{
					menu.Add(t_("Open"), [this] { OpenFile(); })
						.Key(K_CTRL_O);
					menu.Add(t_("Copy to clipboard (as qtf)"),[this] { CopyAsQtf(); })
						.Key(K_CTRL_C)
						.Enable(!IsNull(~view));
					menu.Separator();
					menu.Add(t_("Exit"),   THISFN(Close))
						.Key(K_CTRL_Q);
			});
		});
		ParseFile(GetDataFile("example.md"));
	}
	
	void OpenFile()
	{
		if(String f = SelectFileOpen("*.md"); !f.IsEmpty())
			ParseFile(f);
	}
	
	void ParseFile(const String& fname)
	{
		images.Clear();
		MarkdownConverter mdc;
		mdc.WhenImages = [&](VectorMap<String, String>& imglist)
		{
			String dir = GetFileDirectory(fname);
			Progress pi(t_("Loading images..."), imglist.GetCount());
			pi.pi.Percent();
			for(int i = 0; i < imglist.GetCount(); i++) {
				if(String path = NormalizePath(imglist.GetKey(i), dir); FileExists(path)) { // Image resource paths are expected to be relative to the main markdown file's.
					if(Image img = StreamRaster::LoadFileAny(path); !IsNull(img))
						imglist[i] << images.Add(CreatePNGObject(img));
					if(pi.StepCanceled())
						break;
				}
			}
		};
		view.SetQTF(mdc.Tables().NoHtml().ToQtf(LoadFile(fname)));
	}
	
	void CopyAsQtf()
	{
		AppendClipboardUnicodeText(~view);
	}
};

GUI_APP_MAIN
{
	MarkdownViewer().Run();
}
