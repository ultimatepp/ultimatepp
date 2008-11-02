#include <SysExec/SysExec.h>
#include <Signals/Signals.h>

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
void TPdfFrontend::onSIGHUP(void)
{
	// clears old document lists
	DocumentList.Clear();
	Documents.Clear();
	
	String DataPath = GetPdfDataPath();
	FindFile ff(DataPath + "*-*");
	while(ff)
	{
		if(ff.GetName() != "seqf")
			Documents.Add(DataPath + ff.GetName());
		ff.Next();
	}
	Sort(Documents);
	for(int i = 0; i < Documents.GetCount(); i++)
	{
		// inside list puts just the document name, skipping
		// the sequence number (4 digits)
		// nnnn-s-name
		DocumentList.Add(GetFileName(Documents[i]).Mid(5));
	}
	
	// brings main dialog on top
	Hide();Show(); // sigh... any other way to bring window on top ???
	
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
	
	// install the SIGHUP handler
	Signals().Handle(SIGHUP, THISBACK(onSIGHUP));

	// restores fax documents left from previous run, if any
	String DataPath = GetPdfDataPath();
	FindFile ff(DataPath + "*-*");
	while(ff)
	{
		if(ff.GetName() != "seqf")
			Documents.Add(DataPath + ff.GetName());
		ff.Next();
	}
	Sort(Documents);
	for(int i = 0; i < Documents.GetCount(); i++)
	{
		// inside list puts just the document name, skipping
		// the sequence number (4 digits)
		// nnnn-s-name
		DocumentList.Add(GetFileName(Documents[i]).Mid(5));
	}

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
		// create the dest path, if needed
		RealizeDirectory(GetPdfDataPath());
		
		// opens sequence file, if any end read sequence number
		int Seq;
		String SeqPath = GetPdfDataPath() + "seqf";
		if(FileExists(SeqPath))
		{
			FileIn SeqFile(SeqPath);
			Seq = atoi(SeqFile.GetLine());
		}
		else
			Seq = 1;
		
		// creates destination file name
		String DestName = GetPdfDataPath() + FormatIntDec(Seq, 4, '0') + "-" + Title;
		
		// copies the file to data folder
		FileCopy(name, DestName);
		
		// adds to document lists
		DocumentList.Add(Title);
		Documents.Add(DestName);
		
		// stores the next sequence number
		Seq++;
		FileOut SeqFile(SeqPath);
		SeqFile.PutLine(FormatInt(Seq));
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
	
} // END TPdfFrontend::RemoveDocument()

/////////////////////////////////////////////////////////////////////////////////////
// clears documents list
void TPdfFrontend::ClearDocuments(void)
{
	for(int i = 0; i < Documents.GetCount(); i++)
		FileDelete(Documents[i]);		
	DocumentList.Clear();
	Documents.Clear();
	FileDelete(GetPdfDataPath()+"seqf");
	
} // END TPdfFrontend::ClearDocuments()


GUI_APP_MAIN
{
	// if wrong number of parameters, print error and exit
	if(CommandLine().GetCount() < 7)
	{
		PromptOK("USAGE : maxpdf_frontend job user title copies options  faxfile printer");
		SetExitCode(1);
		return;
	}
	
	// sets up exit code
	SetExitCode(0);
	
	// creates FaxFrontend object
	TPdfFrontend PdfFrontend;
	
	// add the provided document and removes the temp file
	PdfFrontend.AddDocument(CommandLine()[5], CommandLine()[2]);
	FileDelete(CommandLine()[5]);
	
	// @@@ WORKAROUND TO DETACH BACKEND AND FREE CUPS QUEUE
	// cancels current job
	cupsCancelJob(CommandLine()[6], atoi(CommandLine()[1]));

	// checks whether another instance is running
	if(Signals().IsOtherInstanceRunning())
	{
		// yep, just send it a SIGHUP signal to running app and terminate
		// this tells to running app to update document list
		Signals().Send(SIGHUP);
		
		return;
	}
	
	// here we are in child app, we must store the instance
	Signals().StoreInstance();
	
	// executes main window
	PdfFrontend.Run();

} // END Constructor class TPdfFrontend
