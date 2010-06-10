#include <CtrlLib/CtrlLib.h>
#include <Controls4U/Controls4U.h>

using namespace Upp;

#define LAYOUTFILE <UltimateBook/book.lay>
#include <CtrlCore/lay.h>

String MakeExamples(const char *webdir, const char *dir, const char *www, String language, GatherTpp &tt);
void SrcDocs(Index<String> &x, String& qtf, String uppsrc, const char *folder, String lang, GatherTpp &tt);

class Main : public Withmain<TopWindow> {
	typedef Main CLASSNAME;

public:
	Main();
	~Main();
	
	void DoGo();
	void Xmlize(XmlIO xml)	{
		xml 
			("HtmlFolder", htmlFolder)
			("PdfFile", pdfFile)
			("DoPdf", doPdf)			
			("DoHtml", doHtml)			
		;
	}
private:
	EditString grFolder;
	String configFile;
	String indexFile;
	
	bool SetProgress(int value, int total);
};

Main::Main() {
	CtrlLayout(*this, "Ultimate Book");
	Sizeable().Zoomable();
	
	go.WhenAction = THISBACK(DoGo);
	progress.Set(0, 1);
	
	configFile = GetHomeDirFile("ultimatebook.xml");
	LoadFromXMLFile(*this, configFile);
}

Main::~Main() {
	StoreAsXMLFile(*this, NULL, configFile);
}

GUI_APP_MAIN {
	Main main;
	
	main.Execute();
}

bool Main::SetProgress(int value, int total) {
	progress.Set(value, total);
	Ctrl::ProcessEvents();
	return false;
}

void Main::DoGo()
{
	GatherTpp tpp;
	
	String uppfolder = "C:/Desarrollo/Upp_Extra/upp-mirror/";
	String uppsrc    = uppfolder + "uppsrc";
	
	tpp.AddFolder(uppfolder + "uppbox/uppweb");
	tpp.AddFolder(uppsrc);
	tpp.AddFolder(uppfolder + "bazaar");	
	
	action = "Loading pages";
	tpp.Load(uppsrc + "uppbox/uppweb/www.tpp/index$en-us.tpp", THISBACK(SetProgress));
	
	{	// Make examples pages
		int id = tpp.FindTopic("topic://uppweb/www/examples$en-us");
		if (id == -1) {
			Exclamation("Error gathering examples");
			return;
		}
		tpp.GetTopic(id).text << MakeExamples(uppfolder + "uppbox/uppweb", uppfolder + "examples", 
											  "examples", "en-us", tpp);
		tpp.GetTopic(id).text << ReadTopic(LoadFile(uppfolder + "uppbox/uppweb/www.tpp/reference$en-us.tpp")).text;
		tpp.GetTopic(id).text << MakeExamples(uppfolder + "uppbox/uppweb", uppfolder + "reference", 
											  "reference", "en-us", tpp);
	}
	
	{	// Make documentation pages
		int id = tpp.FindTopic("topic://uppweb/www/documentation$en-us");
		if (id == -1) {
			Exclamation("Error gathering documentation");
			return;
		}

		Index<String> x;
		x.Clear();
		String qtf;
		SrcDocs(x, qtf, uppsrc, "Core", "en-us", tpp);
		SrcDocs(x, qtf, uppsrc, "Draw", "en-us", tpp);
		SrcDocs(x, qtf, uppsrc, "CtrlCore", "en-us", tpp);
		SrcDocs(x, qtf, uppsrc, "CtrlLib", "en-us", tpp);
		SrcDocs(x, qtf, uppsrc, "RichText", "en-us", tpp);
		SrcDocs(x, qtf, uppsrc, "RichEdit", "en-us", tpp);
		FindFile ff(AppendFileName(uppfolder + "uppsrc", "*.*"));
		while(ff) {
			if(ff.IsFolder())
				SrcDocs(x, qtf, uppsrc, ff.GetName(), "en-us", tpp);
			ff.Next();
		}
		tpp.GetTopic(id).text << qtf;
	}
	
	if (doHtml) {
		action = "Making html";
		tpp.MakeHtml(~htmlFolder, THISBACK(SetProgress));
	}

	if (doPdf) {
		action = "Making pdf";
		tpp.MakePdf(~pdfFile, THISBACK(SetProgress));
	}
	action = "";
}


