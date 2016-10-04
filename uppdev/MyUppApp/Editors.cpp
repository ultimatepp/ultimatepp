////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "Editors.h"

#define IMAGECLASS EditorsImg
#define IMAGEFILE <MyUppApp/Editors.iml>
#include <Draw/iml_source.h>


// when DocEdit "Load" button is clicked, call the exploreEditors function
void
Editors::onbtnLoad()
{
	String texttoload = openTextFile(filename_);
	mydocedit.Set(texttoload);
	if (filename_ != "") {
		int linecount = mydocedit.GetLineCount();       // new in 0.0.3
		String s = Format("Loaded %d lines.",linecount); // new in 0.0.3
		PromptOK(s);
	}
}

// new in version 0.0.3
// when DocEdit "Clear" button is clicked, clear the DocEdit control.
void
Editors::onbtnClear()
{
	mydocedit.Clear();
	filename_ = "";
}

//
void
Editors::onbtnSave()
{
	String s = mydocedit.Get();                   //
	if (s == "") {                                //
		Exclamation("There is nothing to save!"); // if failed show error dialog
	    return;                                   //
	}

	if (filename_ == "")                          //
		saveasTextFile();                         //
	else {                                        //
		if( !SaveFile(filename_,s) )              //
			Exclamation("Error saving file!");    // if failed show error dialog
		else                                      //
	    	PromptOK("File saved!");              // show success dialog
	}
}


// display the File Open dialog and get user's choice
String
Editors::openTextFile(String& filename)
{
	FileSel f;                             // create a FileSel object
	String curdir = GetCurrentDirectory(); // get the current folder
	f.ActiveDir(curdir);                   // set folder to look in
	f.Type("Text file", "*.txt;*.log");    // set file types to filter
	f.ExecuteOpen("Select a text file");   // display the File Selection dialog
	String fs = f.Get();                   // get a file name
	filename = fs;
	if (fs != "") {                        // if there is a file name
		String s = LoadFile(fs);           // load the file into a string
		return s;                          // return the String to its caller
	} else {
		Exclamation("No text file selected.");
	}
	return "";
}

// display the File Save As dialog and get user's choice
void
Editors::saveasTextFile()
{
	FileSel f;                                // create a FileSel object
	String curdir = GetCurrentDirectory();    // get the current folder
	f.ActiveDir(curdir);                      // set folder to look in
	f.Type("Text file", "*.txt;*.log");       // set file types to filter
	f.ExecuteSaveAs("Save as a text file");   // display the File Selection dialog
	String fs = f.Get();                      // get a file name

	if ( fs.Find(".txt") < 0 )                // if user enters a file name without an extension
		fs = fs + ".txt";                     // appended .txt to the name.

	if (fs != "") {                           // if there is a file name
		String s = mydocedit.Get();           // get the contents of the DocEdit control
		if( !SaveFile(fs,s) )                 // save it.
			Exclamation("Error saving file!");// if it fails show error dialog
		else
	    	PromptOK("File saved!");          // otherwise show success dialog.
	} else {
		Exclamation("No text file selected.");// if no file name, display error dialog.
	}
}


// do the necessary setup work in the constructor
Editors::Editors()
{
	CtrlLayout(*this,"Editors");
	Icon(EditorsImg::Editors()); // Set an icon for the window

	// set callback for the Load buttons; new in version 0.0.3
	btnLoad <<= THISBACK(onbtnLoad);
	btnClear <<= THISBACK(onbtnClear);
	btnSave <<= THISBACK(onbtnSave);

	filename_ = "";    //
}
// end of file
