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
 * $Source: /cvsroot/xlslib/xlslib/src/oledoc/oledoc.cpp,v $
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

#include <XLS/ole/oledoc.h>
#include <XLS/timespan.h>

using namespace std;
using namespace xlslib_core;

/* 
***********************************
COleDoc class implementation
***********************************
*/
const unsigned8_t  COleDoc::OLE_FILETYPE[] =
{ 0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1}; 


COleDoc::COleDoc()
{

}
/*
  COleDoc::COleDoc(const string& file_name)
  {
  Open(file_name);

  }
*/

COleDoc::~COleDoc()
{

}

/* 
***********************************
***********************************
*/

int  COleDoc::DumpHeader(blocks bks, unsigned32_t total_data_size)
{
    unsigned32_t	i, total_data_blocks, sectorID, msatID;
	int				errcode = NO_ERRORS;
   
	total_data_blocks = total_data_size/BIG_BLOCK_SIZE;
   
#if OLE_DEBUG
	fprintf(stderr, "dataBlocks=%u\n", total_data_blocks), fflush(stderr);
#endif
   // [00]FILETYPE
   WriteByteArray(COleDoc::OLE_FILETYPE, /*(unsigned32_t)*/sizeof(COleDoc::OLE_FILETYPE));
   // [08]UK1
   WriteSigned32(HEADVAL_DFLT_UK1);
   // [0c]UK2
   WriteSigned32(HEADVAL_DFLT_UK2);
   // [10]UK2b
   WriteSigned32(HEADVAL_DFLT_UK2b);
   // [14]UK3
   WriteSigned32(HEADVAL_DFLT_UK3);
   // [18]UK4
   WriteSigned16(HEADVAL_DFLT_UK4);
   // [1a]UK5
   WriteSigned16(HEADVAL_DFLT_UK5);
   // [1c]UK6
   WriteSigned16(HEADVAL_DFLT_UK6);
   // [1e]LOG_2_BIG_BLOCK
   WriteSigned16(HEADVAL_DFLT_LOG2_BIGBLOCK);
   // [20]LOG_2_SMALL_BLOCK
   WriteSigned32(HEADVAL_DFLT_LOG2_SMALLBLOCK);
   // [24]UK7
   WriteSigned32(HEADVAL_DFLT_UK7);
   // [28]UK8
   WriteSigned32(HEADVAL_DFLT_UK8);

   // [2c] BAT_COUNT (BBDEPOT NUM BLOCKS)
   WriteUnsigned32(bks.bat_count);

   //[30] PROPERTIES_START_BLOCK
   // Since the big block depot will go immediately after the data, I need
   // to know the size of the data and the size of the BAT in blocks (prev)
   WriteUnsigned32(bks.msat_count+total_data_blocks+bks.bat_count);
#if OLE_DEBUG
	fprintf(stderr, "HEADER says directory at %d\n", bks.msat_count+total_data_blocks+bks.bat_count);
#endif
   // [34] UK9
   WriteSigned32(HEADVAL_DFLT_UK9);
   // [38] UK10
   WriteSigned32(HEADVAL_DFLT_UK10);

   // [3c] SBAT_START 
   // No small blocks will be used, so this is set to the default empty value
   WriteSigned32(HEADVAL_DFLT_SBAT_START);

   // [40] SBAT_BLOCKCOUNT_NUMBER
   // Use the default value
   WriteSigned32(HEADVAL_DFLT_SBAT_COUNT);

   // [44] XBAT_START
   // we will use first and possibly additional blocks for large files
   WriteSigned32(bks.msat_count ? 0 : HEADVAL_DFLT_XBAT_START);
#if OLE_DEBUG
	fprintf(stderr, "xbatStart=%d\n", bks.msat_count ? 0 : HEADVAL_DFLT_XBAT_START), fflush(stderr);
#endif

   // [48] XBAT_COUNT
   WriteUnsigned32(bks.msat_count);	// was HEADVAL_DFLT_XBAT_COUNT (0)
#if OLE_DEBUG
	fprintf(stderr, "msat_count=%d\n", bks.msat_count), fflush(stderr);
#endif

   // [4C] BAT_ARRAY
   // The BAT_ARRAY shall be calculated from the number of BAT blocks and their position
   
   // The additional blocks, if needed, are directly below the header block, so we can write
   // them out contiguously. The special conditions are:
   //  * for each MSAT block, the last entry needs to be a pointer to the next block
   //  * the fill is -1 for all unused entries
   //  * if there are MSAT blocks, the very last entry in the last block is a special marker
   // first sector ID
   sectorID = bks.msat_count + total_data_blocks;
   for(i=0; i<bks.header_bat_count; i++) {
#if OLE_DEBUG
	  // fprintf(stderr, "sectorID=%d\n", sectorID), fflush(stderr);
#endif
      WriteUnsigned32(sectorID++);
	}
#if OLE_DEBUG
	fprintf(stderr, "Position=0x%lx\n", Position() );
	// fprintf(stderr, "SEC_ID[%u]=%u\n", i, total_data_size/(BIG_BLOCK_SIZE) + i), fflush(stderr);
#endif

   // if we don't fill the header table, zero out unused entries
   for(i = 0; i<bks.header_fill; i++) {
      WriteSigned32(BAT_NOT_USED);
   }

#if OLE_DEBUG
	fprintf(stderr, "Position=0x%lx\n", Position() );
#endif

	// plow ahead, adding up to 127 entries per extra MSAT block
   msatID = 1;	// sector 0 is the first MSAT, if used
   for(i=1; i<=bks.extra_bat_count; i++) {
      WriteUnsigned32(sectorID++);
	  if((i % BAT_BLOCKS_PER_MSAT_BLOCK) == 0) {
		if(i == bks.extra_bat_count) {
			WriteSigned32(BAT_END_CHAIN);	// pointer to next MSAT
		} else {
			WriteUnsigned32(msatID++);		// pointer to next MSAT
		}
	  }
	}
	if(bks.extra_fill) {
		for(i = 0; i<bks.extra_fill; i++) {
		  WriteSigned32(BAT_NOT_USED);
		}
		WriteSigned32(BAT_END_CHAIN);	// pointer to next MSAT
	}
	
#if OLE_DEBUG
	fprintf(stderr, "Position=0x%lx\n", Position() );
#endif

	assert(Position() == (HEAD_SIZE + (bks.msat_count*BIG_BLOCK_SIZE)));

   return errcode;
}

/* 
***********************************
***********************************
*/
int  COleDoc::DumpData(void)
{
   int errcode = NO_ERRORS;

   NodeList_t node_list;
   GetAllNodes(node_list);
   for(NodeList_Itor_t i = node_list.begin(); i != node_list.end(); i++)
   {
      if((*i)->GetType() == PTYPE_FILE)
      {
         for(DataList_Itor_t j = (*i)->GetDataPointer()->begin(); 
             j != (*i)->GetDataPointer()->end(); j++)
         {
            WriteByteArray((*j)->GetBuffer(), (*j)->GetDataSize());
         }
      }
   } 

   return errcode;
}

/* 
***********************************
***********************************
*/
int  COleDoc::DumpDepots(blocks bks)
{
	int errcode = NO_ERRORS;

	NodeList_t node_list;
	GetAllNodes(node_list);
	unsigned32_t bat_index;

	bat_index = 0;
	
	// tells Excel that these are used by the MSAT
	for(unsigned32_t i=0; i<bks.msat_count; ++i) {
		WriteSigned32(BAT_MSAT_PLACE);
		++bat_index;
		++bks._bat_entries;
	}

#if OLE_DEBUG
   int foo=0;
#endif
   for(NodeList_Itor_t node = node_list.begin(); node != node_list.end(); node++)
   {
		unsigned32_t chain_len, data_size;
      // The start block is set in the node.
      (*node)->SetStartBlock(bat_index);
      // Write the chain for this node element
	  data_size	= (*node)->GetDataPointer()->GetDataSize();
	  chain_len	= data_size/BIG_BLOCK_SIZE - 1;
#if OLE_DEBUG
	fprintf(stderr, "NODE[%d]: start_block=%d data=%d Sectors= %d\n", foo, bat_index, chain_len + 1 /* directory_terminator */);
#endif
      for(unsigned32_t i = 0; i < chain_len; i++)
      {
		WriteSigned32(++bat_index);
		++bks._bat_entries;
      }

      // Set the terminator number
      WriteSigned32(BAT_END_CHAIN);
      ++bat_index;
      ++bks._bat_entries;
   } 

#if OLE_DEBUG
   fprintf(stderr, "BAT_SELF_PLACE=%d -> %d TOTAL=%d\n", bat_index+1, bat_index+1+bks.bat_count+1, bks.bat_count);
#endif
   // Write the -3 number for every index in the BAT that references to some BAT block (uh!?)
   for(unsigned32_t i=0; i<bks.bat_count;i++)
   {
		WriteSigned32(BAT_SELF_PLACE);
		++bat_index;
		++bks._bat_entries;
   }
#if OLE_DEBUG
   fprintf(stderr, "last write: left = %d\n", Position() % HEAD_SIZE);
#endif

	// This is the entry for the directory chain, the very last block, saying directory is just one sector
	WriteSigned32(BAT_END_CHAIN);
	++bat_index;
	++bks._bat_entries;

   //Fill the rest of the _LAST_ BAT block, code appears to handle the 0 case
   unsigned32_t num_indexes = bat_index;
   unsigned32_t to_fill_size = BIG_BLOCK_SIZE - ((4*num_indexes) % BIG_BLOCK_SIZE);
   SerializeFixedArray(BAT_NOT_USED_BYTE, to_fill_size);

#if OLE_DEBUG
   fprintf(stderr, "last write: left = %d\n", Position() % HEAD_SIZE);
#endif

#if OLE_DEBUG
   fprintf(stderr, "Position=0x%lx\n", Position() );
   fprintf(stderr, "bat_entries=%d actual=%d\n", bks.bat_entries, bks._bat_entries );
#endif

   assert(bks.bat_entries == bks._bat_entries);

   return errcode;
}

/* 
***********************************
***********************************
*/
int  COleDoc::DumpFileSystem(void)
{
   int errcode = NO_ERRORS;


   NodeList_t node_list;
   GetAllNodes(node_list);

#if OLE_DEBUG
   fprintf(stderr, "FILESYSTEM directory at SecID=%d (remain=%d)\n", (Position() - HEAD_SIZE)/BIG_BLOCK_SIZE, Position() % BIG_BLOCK_SIZE);
#endif
   DumpNode(GetRootEntry());

   for(NodeList_Itor_t node  = node_list.begin(); node != node_list.end(); node++)
      DumpNode(*(*node));
   
   return errcode;
}

/* 
***********************************
***********************************
*/
int  COleDoc::DumpOleFile(void)
{
	blocks bks;
	
   int errcode = NO_ERRORS;

   bks = GetBATCount();
   unsigned32_t total_data_size = GetTotalDataSize();

   errcode |=  DumpHeader(bks, total_data_size);
   assert((Position() % 512) == 0 /*1*/);

   errcode |=  DumpData();
   assert((Position() % 512) == 0 /*2*/);

   errcode |=  DumpDepots(bks);
   assert((Position() % 512) == 0 /*3*/);

   errcode |=  DumpFileSystem();
   assert((Position() % 512) == 0 /*3*/);

   return errcode;
}

/***************************************************************
 *****************************************************************/

blocks COleDoc::GetBATCount()
{
	blocks			bks;
	unsigned32_t	bat_num_entries, data_bat_entries, bat_num_blocks, dir_bat_entries, bat_blocks_needed, bat_block_capacity;
	unsigned32_t	extra_bats, msat_blocks, msat_bats, last_block_extras;

	memset(&bks, 0, sizeof(bks));

	data_bat_entries = GetTotalDataSize()/BIG_BLOCK_SIZE; //  + GetNumDataFiles(); //terminator???
	assert(GetTotalDataSize() == (data_bat_entries * BIG_BLOCK_SIZE) );

	// Block allocation strategy. Within the OLE header are 109 slots for BAT Sectors.
	// But, when the file gets big, you run out (127 sectors in each BAT Sector). So,
	// the 110th BAT has to go into a special block dedicated to hold these. One additional
	// block gets you 127 more BAT entries, and so forth.

	dir_bat_entries		= 1;	// Last block is directory, this is the terminator that says dir uses only one sector
	bat_num_blocks		= 1;	// seed to enter loop below
	bat_block_capacity	= 0;
	msat_blocks			= 0;
	msat_bats			= 0;
	bat_num_entries		= 0;	// kch
	bat_blocks_needed	= data_bat_entries/BAT_ENTRIES_PER_BLOCK;		// minimum
	bat_blocks_needed	+= bat_blocks_needed/BAT_ENTRIES_PER_BLOCK;		// for the BAT itself
	
	// first loop assumes no MSATs involved
	while(bat_num_blocks > bat_block_capacity || bat_num_blocks != bat_blocks_needed) {
		bat_num_blocks = bat_blocks_needed;
		if(bat_num_blocks > HEADER_SAT_SIZE) {
			msat_bats = bat_num_blocks - HEADER_SAT_SIZE;
			msat_blocks = msat_bats/BAT_BLOCKS_PER_MSAT_BLOCK;
			if(msat_bats % BAT_BLOCKS_PER_MSAT_BLOCK) ++msat_blocks;
		}
		
		bat_num_entries = msat_blocks + data_bat_entries + bat_num_blocks + dir_bat_entries;	// bat_bat_entries
		
		// based on what we know now, this is what we need
		bat_blocks_needed	= bat_num_entries/BAT_ENTRIES_PER_BLOCK;
		if(bat_num_entries % BAT_ENTRIES_PER_BLOCK) ++bat_blocks_needed;

		// number of slots available
		bat_block_capacity = HEADER_SAT_SIZE + BAT_BLOCKS_PER_MSAT_BLOCK*msat_blocks;
#if OLE_DEBUG
		fprintf(stderr, "bat_blocks=%d capacity=%d needed=%d\n", bat_num_blocks, bat_block_capacity, bat_blocks_needed);
#endif
	}
	
	if(bat_num_blocks > HEADER_SAT_SIZE) {
		extra_bats				= bat_num_blocks - HEADER_SAT_SIZE;

		bks.msat_count			= msat_blocks;
		bks.header_bat_count	= HEADER_SAT_SIZE;
		bks.extra_bat_count		= extra_bats;
		
		last_block_extras = extra_bats % BAT_BLOCKS_PER_MSAT_BLOCK;
		if(last_block_extras) {
			bks.extra_fill	= BAT_BLOCKS_PER_MSAT_BLOCK - last_block_extras;
		}
	} else {
		bks.header_bat_count	= bat_num_blocks;
		bks.header_fill			= HEADER_SAT_SIZE - bat_num_blocks;
	}
	bks.bat_entries			= bat_num_entries;
	bks.bat_count			= bat_num_blocks;

#if OLE_DEBUG
	fprintf(stderr, "entries=%u bats=%d msats=%d headerBats=%d extraBats=%d headFill=%d extraFill=%d\n", bks.bat_entries,
		bks.bat_count, bks.msat_count, bks.header_bat_count, bks.extra_bat_count, bks.header_fill, bks.extra_fill);
#endif

	return bks;
}


/* 
***********************************
***********************************
*/
// NOTE: name_unicode has to be deleted after this function finishes.
// Ideally, this function should be implemented as part of a std::string 
// derived class, so the array would be deleted automatically


signed16_t COleDoc::GetUnicodeName(const char* name, char** ppname_unicode)
{
   unsigned16_t name_size = strlen(name);
   if(name_size > PROPERTY_MAX_NAME_LENGTH)
      name_size = PROPERTY_MAX_NAME_LENGTH;

   unsigned8_t size_unicode = (name_size+1)*2;
  
   if(*ppname_unicode != NULL) delete[] *ppname_unicode;
   *ppname_unicode = (char*)new unsigned8_t[size_unicode];
   memset(*ppname_unicode, 0x00, size_unicode);

   for(int i=0; i<(size_unicode/2-1); i++)
      (*ppname_unicode)[2*i] = name[i];

   return size_unicode;
}



/* 
***********************************
***********************************
*/
int COleDoc::DumpNode(COleProp& node)
{

   int errcode = NO_ERRORS;
   char* name_unicode = NULL;

   // Get the unicode name and its size
   signed16_t size_name = GetUnicodeName(node.GetName().c_str(), &name_unicode);

   // [00] PROPERTY_NAME
   WriteByteArray((const unsigned8_t*)name_unicode, size_name);

   // Fill the rest of the name field with 0x00
   SerializeFixedArray(PROPERTY_DFLT_NOTUSED, PPTPOS_NAMELENGTH - size_name);

   // [40] NAME_SIZE
   WriteSigned16(size_name);
      
   // [42] PROPERTY_TYPE
   WriteByte(node.GetType());

   // [43] NODE_COLOR
   WriteByte(node.GetColor());

   // [44] PREVIOUS_PROP
   WriteSigned32(node.GetPreviousIndex());
   // [48] NEXT_PROP
   WriteSigned32(node.GetNextIndex());
   // [4c] CHILD_PROP
   WriteSigned32(node.GetChildIndex());

   // Fill empty block
   SerializeFixedArray(PROPERTY_DFLT_NOTUSED, (PPTPOS_SECS1 - PPTPOS_UNUSED_EMPTY0));

   //[64]...[70] 
   // SECONDS_1, DAYS_2, SECONDS_2, DAYS_2
   WriteSigned32(node.GetCreatedSecs());
   WriteSigned32(node.GetCreatedDays());
   WriteSigned32(node.GetModifiedDays());
   WriteSigned32(node.GetModifiedSecs());

   // [74] START_BLOCK
#if OLE_DEBUG
   fprintf(stderr, "START_BLOCK_1=%d\n", node.GetStartBlock() );
#endif
   WriteSigned32(node.GetStartBlock());

   // [78] SIZE
   if(node.GetType() == PTYPE_FILE)
      WriteSigned32(node.GetSize());
   else
      WriteSigned32(0);

   // A unused space:
   WriteSigned32(PROPERTY_DFLT_NOTUSED);

   delete[] name_unicode;
   name_unicode = NULL;

   return errcode;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: oledoc.cpp,v $
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

