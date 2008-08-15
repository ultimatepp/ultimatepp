////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_Editors_h_
#define _MyUppApp_Editors_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/Editors.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS EditorsImg
#define IMAGEFILE <MyUppApp/Editors.iml>
#include <Draw/iml_header.h>


class Editors : public WithEditorsLayout<TopWindow> {
	String filename_;    //

	void onbtnLoad();    // new in version 0.0.3
	void onbtnClear();   // new in version 0.0.3
	void onbtnSave();    // new in version 0.0.3
	String openTextFile(String& filename);
	void Editors::saveasTextFile();
public:
	typedef Editors CLASSNAME;
	Editors();
};
#endif
