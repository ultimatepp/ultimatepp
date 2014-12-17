#ifndef _maxfax_frontend_maxfax_frontend_h
#define _maxfax_frontend_maxfax_frontend_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// includes windows layouts
#ifndef __LAYOUT_INCLUDED_
#define __LAYOUT_INCLUDED_

#define LAYOUTFILE <maxpdf_frontend/maxpdf_frontend.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <maxpdf_frontend/maxpdf_frontend.iml>
#include <Draw/iml_header.h>

#endif

// finestra principale
class TPdfFrontend : public WithMainWindowLayout<TopWindow>
{
	private:
	
		// documents file list
		Array<String> Documents;
	
		// close handler
		// button handlers
		void onCreate(void);
		void onCancel(void);
		
		void Close(void);

	public:
		typedef TPdfFrontend CLASSNAME;
		
		TPdfFrontend();
		~TPdfFrontend();
		
		// handle other running instances
		void handleInstance(Vector<String> const &v);
		
		// adds a document
		void AddDocument(String const &FileName, String const &Title);

		// removes a document from document list
		bool RemoveDocument(int index);
		
		// clears documents list
		void ClearDocuments(void);
};


#endif

