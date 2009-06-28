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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/sheetrec.cpp,v $
 * $Revision: 1.3 $
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


#include <XLS/sheetrec.h>

using namespace std;
using namespace xlslib_core;


/*
 * LOCAL MODULE MACROS
 */


#if STORAGE_CELL==LIST_CONTAINER

#define MARK_CELLS_UNSORTED() {                 \
   m_CellsSorted = false;                       \
   m_SizesCalculated = false;                   \
   m_RBSizes.clear();                           \
}


#elif STORAGE_CELL==SET_CONTAINER

#define MARK_CELLS_UNSORTED() {                 \
   m_CellsSorted = false;                       \
   m_SizesCalculated = false;                   \
   m_RBSizes.clear();                           \
}


#endif

#define MAX_ROWBLOCK_SIZE (32)

#define RB_DBCELL_MINSIZE          (unsigned8_t(8))
#define RB_DBCELL_CELLSIZEOFFSET   (unsigned8_t(2))


using namespace std;

/*
**********************************************************************
worksheet class implementation
**********************************************************************
*/


worksheet::worksheet(CGlobalRecords* pglobalrec)
   : m_DumpState(SHEET_INIT), m_pCurrentData(NULL),
     m_Size(0),
     m_CellsSorted(false),
     m_SizesCalculated(false),
     m_DumpRBState(RB_INIT),
     m_DBCellOffset(0),
     m_CurrentRowBlock(0)
{
   m_pGlobalRecords = pglobalrec;
   
   minRow =  minCol = 0xFFFF;	// UINT16_MAX 
   maxRow = maxCol = 0;
}
#ifdef HAVE_ICONV
worksheet::worksheet(CGlobalRecords* pglobalrec, string& code)
	: m_DumpState(SHEET_INIT), m_pCurrentData(NULL),
     m_Size(0),
     m_CellsSorted(false),
     m_SizesCalculated(false),
     m_DumpRBState(RB_INIT),
     m_DBCellOffset(0),
     m_CurrentRowBlock(0)
{
   m_pGlobalRecords = pglobalrec;
   
   minRow =  minCol = 0xFFFF;	// UINT16_MAX 
   maxRow = maxCol = 0;
   iconv_code = code;
}
#endif


worksheet::~worksheet()
{
   if(!m_RBSizes.empty())
      for(RBSize_List_Itor_t rbs = m_RBSizes.begin(); rbs != m_RBSizes.end(); rbs++)
	 delete *rbs;

   // Delete the dinamically created cell objects (pointers)
   
   if(!m_Cells.empty())
   {
//      cout<<"worksheet::~worksheet(), this = "<<this<<endl;
      for(Cell_List_Itor_t cell = m_Cells.begin(); cell != m_Cells.end(); cell++)
         delete *cell;
      m_Cells.clear();
   }

   // Delete dinamically allocated ranges of merged cells.

   if(!m_MergedRanges.empty())
   {
      do
      {
         delete m_MergedRanges.front();
         m_MergedRanges.pop_front();
      }while(!m_MergedRanges.empty());

   }

   // Delete dinamically allocated Cellinfo record definitions

   if(!m_Colinfos.empty())
   {
      for(Colinfo_List_Itor_t ci = m_Colinfos.begin(); ci != m_Colinfos.end(); ci++)
         delete *ci;
      m_Colinfos.clear();
   }

/*
  if(!m_Colinfos.empty())
  {
  do
  {
  delete m_Colinfos.front();
  m_Colinfos.pop_front();
  }while(!m_Colinfos.empty());

  }
*/


   // Delete dynamically allocated Cellinfo record definitions

   if(!m_RowHeights.empty())
   {
      for(RowHeight_List_Itor_t rh = m_RowHeights.begin(); rh != m_RowHeights.end(); rh++)
         delete *rh;
      m_RowHeights.clear();
   }
/*
  if(!m_RowHeights.empty())
  {
  do
  {
  delete m_RowHeights.front();
  m_RowHeights.pop_front();
  }while(!m_RowHeights.empty());

  }
*/

   // Delete dynamically allocated range definitions
   if(!m_Ranges.empty())
   {
      do
      {
         delete m_Ranges.front();
         m_Ranges.pop_front();
      }while(!m_Ranges.empty());

   }

}

/*
***********************************
***********************************
*/
void worksheet::SortCells()
{

//
// 
// Set containers don't need sorting
// 
// 
// 

}

/*
***********************************
***********************************
*/

#define RB_INDEX_MINSIZE ((unsigned8_t)16)



CUnit* worksheet::DumpData(unsigned32_t offset)
{


   bool repeat = false;

   
   XTRACE("worksheet::DumpData");
   do
   {

      switch(m_DumpState)
      {
         case SHEET_INIT:
            m_DumpState = SHEET_BOF;
            m_Current_Colinfo = m_Colinfos.begin();
            repeat  = true;

            break;

         case SHEET_BOF:
            XTRACE("\tBOF");

            repeat = false;

            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CBof(BOF_TYPE_WORKSHEET));
            m_DumpState = SHEET_INDEX;
            break;

         case SHEET_INDEX:
         {
            XTRACE("\tINDEX");

            // Get first/last row from the list of row blocks
            unsigned32_t first_row, last_row;
            GetFirstLastRows(&first_row, &last_row);
       
            m_pCurrentData = (CUnit*)(new CIndex(first_row, last_row));

            unsigned32_t numrb = GetNumRowBlocks();
            unsigned32_t rb_size_acc = 0;
            unsigned32_t index_size = RB_INDEX_MINSIZE + 4*numrb;

            for(unsigned32_t rb = 0; rb < numrb; rb++)
            {
               // Get sizes of next RowBlock
               unsigned32_t rowandcell_size, dbcell_size;
               GetRowBlockSizes( &rowandcell_size, &dbcell_size);

               // Update the offset accumulator and cerate the next DBCELL's offset
               rb_size_acc += rowandcell_size;
               unsigned32_t dbcelloffset = offset + BOF_RECORD_SIZE + index_size + rb_size_acc;
               ((CIndex*)m_pCurrentData)->AddDBCellOffset(dbcelloffset);

               // Update the offset for the next DBCELL's offset
               rb_size_acc += dbcell_size;
            }

            m_DumpState = SHEET_DIMENSION; // Change to the next state

            break;
         }

         case SHEET_DIMENSION:
            XTRACE("\tDIMENSION");

            repeat = false;

            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CDimension(minRow, maxRow, minCol, maxCol));
            m_DumpState = SHEET_ROWBLOCKS;
            break;

         case SHEET_ROWBLOCKS:
            XTRACE("\tROWBLOCKS");
            if(GetNumRowBlocks())
            {// First check if the list of RBs is not empty...
       
               m_pCurrentData = RowBlocksDump();

               if(m_pCurrentData == NULL)
               {

                  repeat = true;
                  m_DumpState = SHEET_MERGED;

               }  

            }
            else
            {// if the list is empty, change the dump state.
               repeat = true;
               m_DumpState = SHEET_MERGED;

            }
          
            break;
         case SHEET_MERGED:

            repeat = false;
            XTRACE("\tMERGED");

            if(!m_MergedRanges.empty())
            {
               m_pCurrentData = (CUnit*)(new CMergedCells());
               ((CMergedCells*)m_pCurrentData)->SetNumRanges(m_MergedRanges.size());
               for(Range_List_Itor_t mr = m_MergedRanges.begin(); mr != m_MergedRanges.end(); mr++)
               {
                  ((CMergedCells*)m_pCurrentData)->AddRange(*mr);
               }
          
            }
            else
            {
               repeat = true;
            }

            m_DumpState = SHEET_COLINFO;
            break;

         case SHEET_COLINFO:
            repeat = false;
            XTRACE("\tCOLINFO");

            if(!m_Colinfos.empty())
            {// First check if the list of fonts is not empty...

               //Delete_Pointer(m_pCurrentData);
               m_pCurrentData = (CUnit*)(new CColInfo(*m_Current_Colinfo));

               if(m_Current_Colinfo != (--m_Colinfos.end()))
               {// if it was'nt the last font from the list, increment to get the next one
                  m_Current_Colinfo++;
               }
               else
               {// if it was the last from the list, change the DumpState
                  m_DumpState = SHEET_WINDOW2;
                  m_Current_Colinfo = m_Colinfos.begin();
               }
            }
            else
            {// if the list is empty, change the dump state.
               m_DumpState = SHEET_WINDOW2;
               //font = m_Fonts.begin();
               repeat = true;
            }
            break;

         case SHEET_WINDOW2:
            XTRACE("\tWINDOW2");
            repeat = false;
            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CWindow2());
            m_DumpState = SHEET_EOF;

            break;

         case SHEET_EOF:
            XTRACE("\tEOF");
            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CEof());
            m_DumpState = SHEET_FINISH;
            break;

         case SHEET_FINISH:
            XTRACE("\tFINISH");
            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = NULL;
            m_DumpState = SHEET_INIT;

            break;



      }

   }while(repeat);

   return m_pCurrentData;
}


CUnit* worksheet::RowBlocksDump()
{
   bool repeat = false;
   CUnit* rb_record = NULL;

   do
   {
      switch(m_DumpRBState)
      {
         case RB_INIT:
            m_DumpRBState = RB_ROWS;
            m_CurrentRowBlock = 0;
            m_RowCounter = 0;
            m_CurrentCell = m_Cells.begin();

            // Initialize the row widths

            m_Current_RowHeight = m_RowHeights.begin();
            m_DumpRBState = RB_FIRST_ROW;
            repeat = true;

            break;
         case RB_FIRST_ROW:
            repeat = true;
       
            if( m_Cells.empty() || m_CurrentCell != m_Cells.end()) 
            {
               m_Starting_RBCell = m_CurrentCell;      
               m_CellCounter = 0;
               m_DBCellOffset = 0;
               m_CellOffsets.clear();

               m_DumpRBState = RB_ROWS;

            }
            else
            {
               m_DumpRBState = RB_FINISH;

            }
            break;

         case RB_ROWS:
         {
            repeat = false;

            // Initialize first/last cols to impossible values
            // that are appropriate for the following detection algorithm
            unsigned16_t first_col = (unsigned16_t)(-1);
            unsigned16_t last_col  = 0;
            unsigned16_t row_num = 0;

            // Get the row number for the current row
            row_num = (*m_CurrentCell)->GetRow();

            Cell_List_Itor_t this_cell, next_cell;
            do
            {
          
               // Determine the first/last column of the current row
               if((*m_CurrentCell)->GetCol() > last_col)
                  last_col = (*m_CurrentCell)->GetCol();

               if((*m_CurrentCell)->GetCol() < first_col)
                  first_col = (*m_CurrentCell)->GetCol();


               // To avoid dereference an empty iterator check if this is the only one cell
               // in m_Cells list.
               if(m_Cells.size() > 1)
               {
                  m_CellCounter++;

                  this_cell = m_CurrentCell;
                  next_cell = ++m_CurrentCell;
        
                  // Break the while if there are no more cells
                  if(next_cell == m_Cells.end())
                     break;
               }
               else
               {
                  // Break the loop if this was the only cell.
            

                  if(!m_Cells.empty())

                  {

                     m_CellCounter++;

                     ++m_CurrentCell;

                  }

                  break;
               }


         
               // The order in the following and-statement is important
            }while( m_CurrentCell != (m_Cells.end()) && *(*this_cell) == *(*next_cell ));

            // Check if the current row is in the list of height-set
            // rows.
            if(m_Current_RowHeight != m_RowHeights.end())
            {

               if((*m_Current_RowHeight)->GetRowNum() == row_num)
               {

                  rb_record = (CUnit*) (new CRow(row_num, first_col, 
                                                 last_col, 
                                                 (*m_Current_RowHeight)->GetRowHeight()) );

                  m_Current_RowHeight++;          
               }
               else
               {
                  rb_record = (CUnit*) (new CRow(row_num, first_col, last_col) );
               }
            }
            else
            {
               rb_record = (CUnit*) (new CRow(row_num, first_col, last_col) );
            }



            m_DBCellOffset += ROW_RECORD_SIZE;

            // If the current row-block is full OR there are no more cells
            if(++m_RowCounter >= MAX_ROWBLOCK_SIZE ||  m_CurrentCell == m_Cells.end())
            {
               if (m_CurrentCell == (--m_Cells.end()))
                  m_CellCounter++;
               m_RowCounter = 0;
               m_DumpRBState = RB_FIRSTCELL;        
            }
       

            break;
         }

         case RB_FIRSTCELL:
            rb_record = (*m_Starting_RBCell)->GetData();
       
            // Update the offset to be used in the DBCell Record
            m_DBCellOffset += rb_record->GetDataSize();

            // The first cell of the rowblock has an offset that includes (among others)
            // the rows size (without counting the first row)
            m_CellOffsets.push_back(m_DBCellOffset -ROW_RECORD_SIZE);

            // Update the pointer (iterator) to the next cell
            if(--m_CellCounter == 0)
            {// The RowBlock's cells are done
               m_DumpRBState = RB_DBCELL;
            }
            else
            {

               m_Starting_RBCell++;
               m_DumpRBState = RB_CELLS;
            }


            break;

         case RB_CELLS:
            repeat = false;

            if(m_CellCounter == 0)
            {// The RowBlock's cells are done



               m_DumpRBState = RB_DBCELL;

               repeat = true;
            }
            else
            {


               rb_record = (*m_Starting_RBCell)->GetData();

               m_DBCellOffset += rb_record->GetDataSize();

               m_CellOffsets.push_back(rb_record->GetDataSize());



               m_CellCounter--;



               m_Starting_RBCell++;
            }

          

            break;

         case RB_DBCELL:
         {

            repeat = false;

         
            rb_record = (CUnit*)(new CDBCell(m_DBCellOffset));

            CellOffsets_List_Itor_t celloffset;
            for(celloffset = m_CellOffsets.begin(); celloffset != m_CellOffsets.end(); celloffset++)
               ((CDBCell*)rb_record)->AddRowOffset(*celloffset);

            if(m_CurrentCell == (--m_Cells.end()) )
               m_DumpRBState = RB_FINISH;
            else
               m_DumpRBState =  RB_FIRST_ROW;

            break;
         }
         case RB_FINISH:
            repeat = false;
            rb_record = NULL;
            m_DumpRBState =  RB_INIT;


            break;

         default:
            break;

      }


   }while(repeat);

   return rb_record;

}
/*
***********************************
***********************************
*/

cell_t* worksheet::label(unsigned16_t row, unsigned16_t col, 
                         string strlabel, xf_t* pxformat)
{
   label_t* label = new label_t(row, col, strlabel, pxformat);
   AddCell((cell_t*)label);

   return (cell_t*)label;

   
}

#if VERSION_BIFF == VERSION_BIFF8
cell_t* worksheet::label(unsigned16_t row, unsigned16_t col, 
                         ustring strlabel, xf_t* pxformat)
{
	label_t* label = new label_t(row, col, strlabel
#ifdef HAVE_ICONV
	, iconv_code
#endif
	, pxformat);
   AddCell((cell_t*)label);

   return (cell_t*)label;

}
#endif


cell_t* worksheet::number(unsigned16_t row, unsigned16_t col, 
                          double numval, format_number_t fmtval,
                          xf_t* pxformat)
{
   number_t* number = new number_t(row, col, numval, pxformat);
   AddCell((cell_t*)number);
   number->format(fmtval);
   return (cell_t*)number;

}

/*
***********************************
***********************************
*/

cell_t* worksheet::blank(unsigned16_t row, unsigned16_t col, xf_t* pxformat)
{
   blank_t* blank = new blank_t(row, col, pxformat);
   AddCell((cell_t*)blank);

   return (cell_t*)blank;
}



/*
***********************************
***********************************
*/

void worksheet::AddCell(cell_t* pcell)
{

   Cell_List_Itor_t existing_cell;

   unsigned32_t row, col;
   
   row = pcell->GetRow();
   col = pcell->GetCol();

   if(row < minRow) minRow = row;
   if(row > maxRow) maxRow = row;
   if(col < minCol) minCol = col;
   if(col > maxCol) maxCol = col;
 
   // Pass a pointer to m_GlobalRecords, so the global records
   // can be modified from the cell as needed
   pcell->SetGlobalRecs(m_pGlobalRecords);

   SortCells();
   // lookup the cell
   existing_cell = m_Cells.find(pcell);
      
   if(existing_cell != m_Cells.end())
   {
      //Always overwrite
      delete (*existing_cell);
      m_Cells.erase(existing_cell);
      m_Cells.insert(pcell);
      MARK_CELLS_UNSORTED();
   }
   else
   {
      m_Cells.insert(pcell);
      MARK_CELLS_UNSORTED();
   }

}

cell_t* worksheet::FindCell(unsigned16_t row, unsigned16_t col)
{
   Cell_List_Itor_t existing_cell;
   
   cell_t* cell = new blank_t(row, col);
   existing_cell = m_Cells.find(cell);
   delete cell;

   // The find operation returns the end() itor
   // if the cell wasn't found
   if(existing_cell != m_Cells.end())
      return *existing_cell;
   else
   {
    
      return blank(row,col);
   }
}
/*
  void worksheet::AddCell(cell_t* pcell, bool overwrite)
  {

  #if STORAGE_CELL == LIST_CONTAINER
  m_Cells.push_back(pcell);

  #elif STORAGE_CELL == SET_CONTAINER
  m_Cells.insert(pcell);
  #endif
  MARK_CELLS_UNSORTED();
  }
*/
/*
***********************************
***********************************
*/


unsigned32_t worksheet::GetSize()
{
   m_CurrentSizeCell = m_Cells.begin();
   unsigned32_t numrb = GetNumRowBlocks();
   unsigned16_t merged_size;
   unsigned16_t colinfo_size;


   // The size of the merged cells record (if any) has to be taken in count
   if(!m_MergedRanges.empty())
   {
      //            [HEADER] + [NUMRANGESFIELD] + [RANGES]
      merged_size =  4       +  2               +  m_MergedRanges.size()*8; 
   }
   else
   {
      merged_size = 0;
   }
   
   // The size of the Colinfo records (if any) has to be taken in count
   if(!m_Colinfos.empty())
   {
      colinfo_size =  m_Colinfos.size()*14; 
   }
   else
   {
      colinfo_size = 0;
   }


   unsigned32_t size = BOF_SIZE + 
      RB_INDEX_MINSIZE + 
      4*numrb          + 
      merged_size      +
      colinfo_size     +
      WINDOW2_SIZE     + 
      EOF_SIZE;
   
   for(unsigned32_t rb = 0; rb < numrb; rb++)
   {
      // Get sizes of next RowBlock
      unsigned32_t rowandcell_size, dbcell_size;
      GetRowBlockSizes( &rowandcell_size, &dbcell_size);

      // Update the offset accumulator and cerate the next DBCELL's offset
      size += rowandcell_size;
      size += dbcell_size;
   }

   m_CurrentSizeCell = m_Cells.begin();

   return size;
}

/*
***********************************
***********************************
*/

bool worksheet::GetRowBlockSizes(unsigned32_t* rowandcell_size, 
                                 unsigned32_t* dbcell_size,
                                 unsigned32_t* num_rows)
{

   SortCells();
   
   unsigned32_t row_counter = 0;
   unsigned32_t cell_counter = 0;

   Cell_List_Itor_t beginning_cell = m_CurrentSizeCell;

   // Initialize the size values (since they work as accumulators)
   *rowandcell_size = 0;
   *dbcell_size = 0;


   if(!m_SizesCalculated)
   {

      // Check if there are no cells
      if(!m_Cells.empty())
      {
         // The first cell is inside a row that has to be counted
         // row_counter = 1;
    
         do
         {
            cell_counter++; // There's at least  one cell on each loop.. that's for sure!

            // Since the list of cells is sorted by rows, continuouslly equal cells (compared by row)
            // conform one row... if the next one is different, increment the row counter
            Cell_List_Itor_t this_cell = m_CurrentSizeCell;
            Cell_List_Itor_t next_cell = ++ m_CurrentSizeCell;
       
            // To avoid dereferencing an empty iterator check if this is the only
            // one cell in m_Cells list.
            if( m_Cells.size()>1)
            {
               if( *(*this_cell) != *(*(next_cell))  ) 
                  row_counter++;
            }
            else
            {
               // Break the loop if this was the only one cell in the list
               // .. also set the only one row
               cell_counter--;
               m_CurrentSizeCell = (--m_Cells.end());
               break;
          
            }
    
         }while(row_counter < MAX_ROWBLOCK_SIZE &&
                m_CurrentSizeCell != (--m_Cells.end())); // Check also if the currentcell isn't the last one

         // If the cells run out before the row counter changes, the last row (and last cell) isn't counted
         // in the previous control structre.
         if(m_CurrentSizeCell == (--m_Cells.end()))
         {
            row_counter++;
            cell_counter++;
         }


         if(num_rows != NULL)
            *num_rows += row_counter;
         // Get the size of the rows
         *rowandcell_size += ROW_RECORD_SIZE*row_counter;
      
         // Get the size of the cells using the saved iterator pointing to the beginning of this block
         for(unsigned32_t count_blockcells = 0; count_blockcells <cell_counter; count_blockcells++)
         {
            *rowandcell_size += (*beginning_cell)->GetSize();
            beginning_cell++;
         }

         // Now get the size of the DBCELL
         *dbcell_size += RB_DBCELL_MINSIZE;
         *dbcell_size += RB_DBCELL_CELLSIZEOFFSET*cell_counter;


         // Check the size of the data int the DBCELL record (without the header)
         // to take in count the overhead of the CONTINUE record (4bytes/CONTrec)
         if((*dbcell_size-4) > MAX_RECORD_SIZE)
         {
            unsigned32_t cont_overhead = (*dbcell_size / MAX_RECORD_SIZE);
            if(*dbcell_size % MAX_RECORD_SIZE)
               cont_overhead++;

            *dbcell_size += (cont_overhead-1)*4;
       
         }


         rowblocksize_t*  rbsize = new rowblocksize_t;

         rbsize->rowandcell_size = *rowandcell_size;
         rbsize->dbcell_size = *dbcell_size;
         rbsize->rows_sofar = row_counter;
         m_RBSizes.push_back(rbsize);
    
         // If it was the last block, reset the current-label pointer
         if(m_CurrentSizeCell == (--m_Cells.end()))
         {
            m_CurrentSizeCell = m_Cells.begin();
            m_Current_RBSize = m_RBSizes.begin();
            m_SizesCalculated = true;
       
            return false;
         }


      }   
      
      // If there are no cells in the sheet, return sizes = 0.
      if(m_Cells.empty())
         return false;

      else

         return true;
      
   }
   else
   {
      *rowandcell_size = (*m_Current_RBSize)->rowandcell_size;
      *dbcell_size = (*m_Current_RBSize)->dbcell_size;
      if(num_rows != NULL) 
         *num_rows += (*m_Current_RBSize)->rows_sofar;

      m_Current_RBSize++;

      // Resett the current RBSize
      if(m_Current_RBSize == m_RBSizes.end())
      {
         m_Current_RBSize = m_RBSizes.begin();
         return false;
      }
   }
   return true;
}


/*
***********************************
***********************************
*/

void  worksheet::GetFirstLastRows(unsigned32_t* first_row, unsigned32_t* last_row)
{

   // First check that the m_Cells list is not empty, so we won't dereference
   // empty anr iterator.
   if(!m_Cells.empty())
   {
      SortCells();

      cell_t* pcell;
      pcell = *(m_Cells.begin());
      *first_row = pcell->GetRow();

      pcell = *(--m_Cells.end());
      *last_row = pcell->GetRow();


   }
   else
   {
      // If there is no cells in the list the first/last rows
      // are defaulted to zero.
      *first_row = 0;
      *last_row = 0;
   }

}

/*
***********************************
***********************************
*/


unsigned32_t worksheet::GetNumRowBlocks()
{

   unsigned32_t numrb;

   // First check that the m_Cells list is not empty, so we won't dereference
   // empty anr iterator.

   bool cont = false;
   unsigned32_t num_rows = 0;
   do
   {
      unsigned32_t dummy1, dummy2;
      cont = GetRowBlockSizes(&dummy1, &dummy2, &num_rows);
   }while(cont);
/*
  Cell_List_t temp_cell_list = m_Cells;
  temp_cell_list.sort();
  temp_cell_list.unique();
*/

   if(!m_Cells.empty())
   {

      numrb = num_rows/MAX_ROWBLOCK_SIZE;
      if(num_rows%MAX_ROWBLOCK_SIZE)
         numrb++;
   }
   else
   {
      // If the m_Cell list is empty, there are no rowblocks in the sheet.
      numrb = 0;
   }

   return numrb;
}

/*
***********************************
***********************************
*/

void worksheet::merge(unsigned16_t first_row, unsigned16_t first_col, 
                      unsigned16_t last_row, unsigned16_t last_col)
{
   range_t* newrange = new range_t;

   newrange->first_row = first_row;
   newrange->last_row  = last_row;
   newrange->first_col = first_col;
   newrange->last_col  = last_col;

   m_MergedRanges.push_back(newrange);


}

/*
***********************************
***********************************
*/

void worksheet::colwidth(unsigned16_t col, unsigned16_t width)
{
   colinfo_t* newci = new colinfo_t;
   Colinfo_List_Itor_t existing_ci;

   newci->colfirst = col;
   newci->collast = col;
   newci->flags = 0x00;
   newci->xformat = NULL;
   newci->width = width*256;	//sets column widths to 1/256 x width of "0"

   // m_Colinfos.push_back(newci);
   existing_ci = m_Colinfos.find(newci);
      
   if(existing_ci != m_Colinfos.end())
   {
      //Always overwrite
      delete (*existing_ci);
      m_Colinfos.erase(existing_ci);
      m_Colinfos.insert(newci);
   }
   else
   {
      m_Colinfos.insert(newci);
   }
}


/*
***********************************
***********************************
*/

void worksheet::rowheight(unsigned16_t row, unsigned16_t height)
{
   rowheight_t* newrh = new rowheight_t(row,height*20);
   RowHeight_List_Itor_t existing_rh;

   //m_RowHeights.insert(newrh);

   existing_rh = m_RowHeights.find(newrh);
      
   if(existing_rh != m_RowHeights.end())
   {
      //Always overwrite
      delete (*existing_rh);
      m_RowHeights.erase(existing_rh);
      m_RowHeights.insert(newrh);
   }
   else
   {
      m_RowHeights.insert(newrh);
   }
   
   

}


range* worksheet::rangegroup(unsigned16_t row1, unsigned16_t col1,
                             unsigned16_t row2, unsigned16_t col2)
{
   
   range* newrange = new range(row1, col1, row2, col2, this);
   m_Ranges.push_back(newrange);

   return newrange;

}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: sheetrec.cpp,v $
 * Revision 1.3  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:04  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:51  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

