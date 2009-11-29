/***************************************************************************
                          main  -  description
                             -------------------
    begin                : Aug. 2, 2007
    copyright            : (C) 2007 by Allen
    email                : bon_ami_@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   Explicit Distribution Limitation                                      *
 *   This rule overrides others below.                                     *
 *   This program may not be modified or used by, or, if possible,         *
 *   redistributed to people described as below,                           *
 *   1.Japanese who hold hostility against Chinese.                        *
 *   2.or, those who discriminate against people based solely on race,     *
 *     gender or sexual orientation.                                       *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
 *	header of EZ Comm Project Ultimate++ functionalities
 */

#ifndef _ezcomm_ezcomm_h
#define _ezcomm_ezcomm_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ezcomm/ezcomm.lay>
#include <CtrlCore/lay.h>

class ezcommwin : public WithezcommwinLayout<TopWindow> {
public:
	ColumnList cl;
	typedef ezcommwin CLASSNAME;
	ezcommwin() { 	Add(cl.LeftPos(10, 100).TopPos(30, 80)); }
};

#endif

