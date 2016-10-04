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
 * $Source: /cvsroot/xlslib/xlslib/src/oledoc/oleprop.cpp,v $
 * $Revision: 1.3 $
 * $Author: dhoerl $
 * $Date: 2008/10/25 18:39:53 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <XLS/ole/oleprop.h>

/* 
***********************************
COleProp class Implementation
***********************************
*/

using namespace std;
using namespace xlslib_core;
 


COleProp::COleProp()
   : m_pData(NULL)
{
  
}

/* 
***********************************
***********************************
*/

COleProp::COleProp(signed32_t node_index,
                   string   name, 
                   CDataStorage* data)
   : m_sName(name), m_pData(data), m_nIndex(node_index)
{

}

COleProp::~COleProp()
{

   for(Tree_Level_Itor_t chld = m_Child_List.begin();  chld != m_Child_List.end(); chld++)
      delete *chld;


}



/* 
***********************************
***********************************
*/

int COleProp::Init(signed32_t node_index,
                   string name, 
                   CDataStorage*   data)
{
   int errcode = NO_ERRORS;

   SetName(name);
   m_pData = data;
   m_nIndex = node_index;
   
   return errcode;

}

/* 
***********************************
***********************************
*/

int COleProp::SetName(string name)
{
   int errcode = NO_ERRORS;
  
   m_nNameSize = name.size() + 1;
   m_sName = name;

   return errcode;
}

/* 
***********************************
***********************************
*/

string COleProp::GetName(void)
{
   return m_sName;
}

/* 
***********************************
***********************************
*/
int COleProp::SetIndex(signed32_t newindex)
{
   int errcode = NO_ERRORS;
  
   m_nIndex = newindex;

   return errcode;

}

/* 
***********************************
***********************************
*/

signed32_t COleProp::GetIndex(void)
{
   return m_nIndex;
}

/* 
***********************************
***********************************
*/
int COleProp::SetType(unsigned8_t newtype)
{
   int errcode = NO_ERRORS;
  
   m_nPropType = newtype;

   return errcode;
}

/* 
***********************************
***********************************
*/
unsigned8_t COleProp::GetType(void)
{
   return m_nPropType;
}

/* 
***********************************
***********************************
*/
int COleProp::SetColor(unsigned8_t newcolor)
{
   int errcode = NO_ERRORS;

   m_nNodeColor = newcolor;

   return errcode;

}

/* 
***********************************
***********************************
*/
unsigned8_t COleProp::GetColor(void)
{
   return m_nNodeColor;
}

/* 
***********************************
***********************************
*/
int COleProp::SetPreviousIndex(signed32_t prev)
{
   int errcode = NO_ERRORS;

   m_nPrevPropIndex = prev;

   return errcode;

}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetPreviousIndex(void)
{
   return m_nPrevPropIndex;
}

/* 
***********************************
***********************************
*/
int COleProp::SetNextIndex(signed32_t next)
{
   int errcode = NO_ERRORS;

   m_nNextPropIndex = next;

   return errcode;

}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetNextIndex(void)
{
   return m_nNextPropIndex;
}

/* 
***********************************
***********************************
*/
int COleProp::SetChildIndex(signed32_t child)
{
   int errcode = NO_ERRORS;

   m_nChildPropIndex = child;

   return errcode;

}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetChildIndex(void)
{
   return m_nChildPropIndex;
}

int COleProp::SetStartBlock(signed32_t sb)
{
   int errcode = NO_ERRORS;
  
   m_nStartBlock = sb;

   return errcode;

}
signed32_t COleProp::GetStartBlock(void)
{
   return m_nStartBlock;
}

/* 
***********************************
***********************************
*/
int COleProp::SetSize(signed32_t size)
{
   int errcode = NO_ERRORS;
  
   m_nSize = size;

   return errcode;

}

/* 
***********************************
***********************************
*/

signed32_t COleProp::GetSize(void)
{
   return m_nSize;
}

/* 
***********************************
***********************************
*/


void COleProp::SetDataPointer(CDataStorage* pdata)
{

   m_pData = pdata;
}

/* 
***********************************
***********************************
*/

CDataStorage* COleProp::GetDataPointer(void)
{
   return m_pData;
}




/* 
***********************************
***********************************
*/
void COleProp::SetCreatedSecs(signed32_t secs1)
{
   m_nTSCreatedSeconds = secs1;
}
/* 
***********************************
***********************************
*/
signed32_t COleProp::GetCreatedSecs(void)
{
   return m_nTSCreatedSeconds;
}


/* 
***********************************
***********************************
*/
void COleProp::SetCreatedDays(signed32_t days1)
{
   m_nTSCreatedDays = days1;
}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetCreatedDays(void)
{
   return m_nTSCreatedDays;
}


/* 
***********************************
***********************************
*/
void COleProp::SetModifiedSecs(signed32_t secs2)
{
   m_nTSModifiedSeconds = secs2;
}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetModifiedSecs(void)
{
   return m_nTSModifiedSeconds;
}

/* 
***********************************
***********************************
*/

void COleProp::SetModifiedDays(signed32_t days2)
{
   m_nTSModifiedDays = days2;
}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetModifiedDays(void)
{

   return  m_nTSModifiedDays;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: oleprop.cpp,v $
 * Revision 1.3  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:04  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:43  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

