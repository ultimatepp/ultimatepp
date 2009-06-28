/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * xlslib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xlslib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with xlslib.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2004 Yeico S. A. de C. V.
 * Copyright 2008 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/summinfo.cpp,v $
 * $Revision: 1.2 $
 * $Author: dhoerl $
 * $Date: 2008/10/25 18:39:54 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <XLS/summinfo.h>

#include <XLS/data/data3.h> // Temporarilly used to add data for summary info

using namespace std;
using namespace xlslib_core;

/*
**********************************************************************
CSummaryInfo class implementation
**********************************************************************
*/

      
CSummaryInfo::CSummaryInfo()
{
	XTRACE("WRITE_SUMMARY");
	
	CUnit* ptraildata = new CUnit;
	ptraildata->AddDataArray(CSummaryInfo::summ_info_data, sizeof(CSummaryInfo::summ_info_data));
	(*this) += ptraildata;
}

CSummaryInfo::~CSummaryInfo()
{


}


/*
***********************************
***********************************
*/

CUnit* CSummaryInfo::DumpData(void)
{
   XTRACE("\tCSummaryInfo::DumpData");
   return (CUnit*)this;

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: summinfo.cpp,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:57  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

