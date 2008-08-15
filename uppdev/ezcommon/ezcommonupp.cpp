/***************************************************************************
                          ezcommon  -  description
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
 *	source code of EZ Project common Ultimate++ functionalities
 */

#include <windows.h>
#include "ezcommonupp.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason,  LPVOID lpReserved)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

