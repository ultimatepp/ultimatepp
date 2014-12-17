#include <SysExec/SysExec.h>

#include <Uniq/Uniq.h>

#include <cups/cups.h>

#include "maxpdf_frontend.h"

#include "PdfPaths.h"

#define IMAGEFILE <maxpdf_frontend/maxpdf_frontend.iml>
#include <Draw/iml_source.h>

// parameters from command line
//  1   2     3      4      5        6       7
// job user title copies options  faxfile printer


/////////////////////////////////////////////////////////////////////////////////////
// close handler
void TPdfFrontend::Close()
{
	onCancel();
	
} // END TPdfFrontend::Close()

/////////////////////////////////////////////////////////////////////////////////////
// reloads document list from data folder
void TPdfFrontend::handleInstance(Vector<String> const &v)
{
	AddDocument(v[5], v[2]);

	// @@@ WORKAROUND TO DETACH BACKEND AND FREE CUPS QUEUE
	// cancels current job
	cupsCancelJob2(CUPS_HTTP_DEFAULT, v[6], atoi(v[0]), 1);

	Hide();
	Show();

} // END TPdfFrontendCtrl::onSIGHUP()

/////////////////////////////////////////////////////////////////////////////////////
// cancel pdf creation handler
void TPdfFrontend::onCancel(void)
{
	if(DocumentList.GetCount() == 0 || PromptYesNo("Annullare la generazione ?"))
	{
		// remove pending poststript documents and closes app
		ClearDocuments();
		Break(0);
	}
	
} // END TPdfFrontend::onCancel()

/////////////////////////////////////////////////////////////////////////////////////
// create handler
void TPdfFrontend::onCreate(void)
{

	FileSel fs;
	
	// if no documents, just do nothing
	if(!Documents.GetCount())
		return;
	
	// first, gets last browsed path (if any)
	String cfgPath = GetPdfConfigPath() + "lastpath";
	if(FileExists(cfgPath))
	{
		FileIn f(cfgPath);
		fs.Set(f.GetLine());
	}
	
	// gets the output file name, returns if cancelled
	fs.DefaultExt(".pdf").Type("documenti pdf", "*.pdf");
	if(!fs.ExecuteSaveAs("Nome del documento pdf:"))
		return;

	// stores last active output path
	RealizeDirectory(GetPdfConfigPath());
	FileOut f(cfgPath);
	f.PutLine(fs.Get());

	// creates the command line and executes ghostscript
	String FileName = fs.Get();
    String args =
		"-q -dBATCH -dAutoFilterColorImages=false -sColorImageFilter=FlateEncode -dNOPAUSE -sDEVICE=pdfwrite -sOutputFile=\"" +
    	FileName + "\" \"" +
    	Documents[0] + "\"";
    for(int i = 1 ; i < Documents.GetCount(); i++)
    	args = args + " \"" + Documents[i] + "\"";
    String OutStr, ErrStr;
	bool res = SysExec("gs", args, Environment(), OutStr, ErrStr);

	// creates progress bar and hooks inside client
//	Progress progress("Creazione pdf in corso....");
//	Client.setProgress(progress);
//	progress.Create();
	
//	Client.setProgress(0);
//	if(progress.Canceled())
//		res = false;
	// if error, does nothing	
	if(!res)
		return;
	
	// clear collected documents
	ClearDocuments();
	
	// closes application
	Break(0);
	
} // END TPdfFrontend::onCreate()

/////////////////////////////////////////////////////////////////////////////////////
// constructor class TPdfFrontend
TPdfFrontend::TPdfFrontend()
{
	// clears document list and documents
	DocumentList.Clear();
	Documents.Clear();
	
	// setup button handlers
	CreateButton	<<= THISBACK(onCreate);
	CancelButton	<<= THISBACK(onCancel);
	
	CtrlLayout(*this, "MaxPdf");
}

/////////////////////////////////////////////////////////////////////////////////////
// destructor class TPdfFrontend
TPdfFrontend::~TPdfFrontend()
{
	
} // END destructor class TPdfFrontend

/////////////////////////////////////////////////////////////////////////////////////
// adds a document
void TPdfFrontend::AddDocument(String const &name, String const &Title)
{
	if(FileExists(name))
	{
		// adds to document lists
		DocumentList.Add(Title);
		Documents.Add(name);
	}
	
} // END TPdfFrontend::AddDocument()

/////////////////////////////////////////////////////////////////////////////////////
// removes a document from document list
bool TPdfFrontend::RemoveDocument(int index)
{
	if(index < 0 || index >= DocumentList.GetCount())
		return false;
	FileDelete(Documents[index]);
	DocumentList.Remove(index);
	Documents.Remove(index);
	return true;
	
} // END TPdfFrontend::RemoveDocument()

/////////////////////////////////////////////////////////////////////////////////////
// clears documents list
void TPdfFrontend::ClearDocuments(void)
{
	for(int i = 0; i < Documents.GetCount(); i++)
		FileDelete(Documents[i]);		
	DocumentList.Clear();
	Documents.Clear();
	
} // END TPdfFrontend::ClearDocuments()


GUI_APP_MAIN
{
	// create Uniq object
	Uniq uniq;

	// if not inside first instance, send commandline
	// to first instance and leave
	if(!uniq)
		return;
	
	// creates FaxFrontend object
	TPdfFrontend PdfFrontend;
	
	// setup callback handling other app's instances
	uniq.WhenInstance = callback(&PdfFrontend, &TPdfFrontend::handleInstance);
		
	// if wrong number of parameters, print error and exit
	if(CommandLine().GetCount() < 7)
	{
		PromptOK("USAGE : maxpdf_frontend job user title copies options  faxfile printer");
		SetExitCode(1);
		return;
	}
	
	// sets up exit code
	SetExitCode(0);
	
	// add the provided document
	PdfFrontend.AddDocument(CommandLine()[5], CommandLine()[2]);

	// @@@ WORKAROUND TO DETACH BACKEND AND FREE CUPS QUEUE
	// cancels current job
	cupsCancelJob2(CUPS_HTTP_DEFAULT, CommandLine()[6], atoi(CommandLine()[0]), 1);
	
	// executes main window
	PdfFrontend.Run();

} // END Constructor class TPdfFrontend
