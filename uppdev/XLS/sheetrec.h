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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/sheetrec.h,v $
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


#ifndef SHEETREC_H
#define SHEETREC_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/unit.h>
#include <XLS/biffsection.h>
#include <XLS/recdef.h>
#include <XLS/index.h>
#include <XLS/extformat.h>
#include <XLS/label.h>
#include <XLS/blank.h>
#include <XLS/number.h>
#include <XLS/row.h>
#include <XLS/merged.h>
#include <XLS/colinfo.h>
#include <XLS/globalrec.h>
#include <XLS/range.h>

namespace xlslib_core
{

  /*
***********************************
worksheet class declaration
***********************************
*/

  typedef enum
    {
      SHEET_INIT,
      SHEET_BOF,
      SHEET_INDEX,
      SHEET_DIMENSION,
      SHEET_ROWBLOCKS,
      SHEET_MERGED,
      SHEET_COLINFO,
      SHEET_WINDOW2,
      SHEET_EOF,
      SHEET_FINISH
    } SheetRecordDumpState_t;


#define BOF_SIZE ((unsigned8_t)12)
#define EOF_SIZE ((unsigned8_t)4)
#define WINDOW2_SIZE ((unsigned8_t)14)

  typedef struct
  {
    unsigned32_t rowandcell_size;
    unsigned32_t dbcell_size;
    unsigned32_t rows_sofar;

  } rowblocksize_t;

  typedef std::list<xlslib_core::rowblocksize_t* XLSLIB_DFLT_ALLOCATOR> RBSize_List_t;
  typedef RBSize_List_t::iterator RBSize_List_Itor_t;

  typedef enum
    {
      RB_INIT,
      RB_FIRST_ROW,
      RB_ROWS,
      RB_FIRSTCELL,
      RB_CELLS,
      RB_DBCELL,
      RB_FINISH

    } DumpRowBlocksState_t;

  typedef std::list<unsigned16_t XLSLIB_DFLT_ALLOCATOR> CellOffsets_List_t;
  typedef CellOffsets_List_t::iterator CellOffsets_List_Itor_t;

  //class range;
  class worksheet

    : public CBiffSection

    {

    private:
		CGlobalRecords* m_pGlobalRecords;
#ifdef HAVE_ICONV
		std::string	iconv_code;
#endif
		SheetRecordDumpState_t m_DumpState;
		CUnit* m_pCurrentData;
		//      std::string m_Name; 10-1-08 not sure why here

		bool m_SizeCalculated;
		size_t m_Size;

		Range_List_t m_MergedRanges;

		Colinfo_List_t m_Colinfos;
		Colinfo_List_Itor_t m_Current_Colinfo;

		RowHeight_List_t m_RowHeights;
		RowHeight_List_Itor_t m_Current_RowHeight;

		unsigned16_t  minRow, minCol, maxRow, maxCol;
		// Label related additions

		/*   void AddLabel(unsigned16_t row, unsigned16_t col, 
		   std::string& strlabel, unsigned16_t& xformat);
		*/

    private:
		Cell_List_t m_Cells;
		Cell_List_Itor_t m_CurrentCell; // Init this one in the INIT state
		Cell_List_Itor_t m_CurrentSizeCell; // Init this one in the INIT state
		bool m_CellsSorted;

		RangeObj_List_t m_Ranges;

		RBSize_List_t m_RBSizes;
		RBSize_List_Itor_t m_Current_RBSize;
		bool m_SizesCalculated;

		DumpRowBlocksState_t m_DumpRBState;
		unsigned8_t m_RowCounter;
		unsigned32_t m_CellCounter;
		unsigned32_t m_DBCellOffset;
		CellOffsets_List_t m_CellOffsets;

		unsigned32_t m_CurrentRowBlock;
		Cell_List_Itor_t m_Starting_RBCell;

		void GetFirstLastRows(unsigned32_t* first_row, unsigned32_t* last_row);
		unsigned32_t GetNumRowBlocks();
		bool GetRowBlockSizes(unsigned32_t* rowandcell_size, 
							unsigned32_t* dbcell_size,
							unsigned32_t* num_rows = NULL);
		CUnit* RowBlocksDump();

		void AddCell(cell_t* pcell);

		void SortCells();

    public:

		worksheet(CGlobalRecords* pglobalrec);
#if HAVE_ICONV
		worksheet(CGlobalRecords* pglobalrec, std::string& code);
#endif
		~worksheet();

		// The offset defines the amount
		// of data form the beginning of the BIFF records to the first

		CUnit* DumpData(unsigned32_t offset); 
		unsigned32_t GetSize();
		cell_t* FindCell(unsigned16_t row, unsigned16_t col);

		// Cell operations
		void merge(unsigned16_t first_row, unsigned16_t first_col, 
				 unsigned16_t last_row, unsigned16_t last_col);
		void colwidth(unsigned16_t col, unsigned16_t width);
		void rowheight(unsigned16_t row, unsigned16_t height);
		// Ranges
		range* rangegroup(unsigned16_t row1, unsigned16_t col1,
						unsigned16_t row2, unsigned16_t col2);

		// Cells
		cell_t* label(unsigned16_t row, unsigned16_t col, 
					std::string strlabel, xf_t* pxformat = NULL);

#if VERSION_BIFF == VERSION_BIFF8
		cell_t* label(unsigned16_t row, unsigned16_t col, 
					std::ustring strlabel, xf_t* pxformat = NULL);
#endif   
		cell_t* blank(unsigned16_t row, unsigned16_t col, 
					xf_t* pxformat = NULL);

		cell_t* number(unsigned16_t row, unsigned16_t col, 
					 double numval,  format_number_t fmtval,
					 xf_t* pxformat = NULL);
    };

  typedef std::vector<xlslib_core::worksheet*> Sheets_Vector_t;
  typedef Sheets_Vector_t::iterator Sheets_Vector_Itor_t;

}

#endif // SHEETREC_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: sheetrec.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:44  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



