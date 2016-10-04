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
 * $Source: /cvsroot/xlslib/xlslib/src/oledoc/olefs.cpp,v $
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


#include <string>
#include <XLS/ole/olefs.h>
#include <XLS/stringtok.h>

using namespace std;
using namespace xlslib_core;
 
/* 
***********************************
COleFileSystem class Implementation
***********************************
*/



COleFileSystem::COleFileSystem()
   :m_RootEntry((signed32_t)0,string("Root Entry")),
    m_nProperty_Count(0)
{

   m_RootEntry.SetType(PTYPE_ROOT);
   m_RootEntry.SetSize(0);
   m_RootEntry.SetPreviousIndex( PLINK_EMPTY);
   m_RootEntry.SetNextIndex( PLINK_EMPTY);
   m_RootEntry.SetChildIndex( PLINK_EMPTY);
   m_RootEntry.SetStartBlock( PBLOCK_END);
   m_RootEntry.SetColor(PROPERTY_COLOR_NODE_BLACK);
   m_RootEntry.SetDataPointer(NULL);


   m_RootEntry.SetCreatedSecs(PPT_DFLT_SECS1);
   m_RootEntry.SetCreatedDays(PPT_DFLT_DAYS1);
   m_RootEntry.SetModifiedSecs(PPT_DFLT_SECS2);
   m_RootEntry.SetModifiedDays(PPT_DFLT_DAYS2);



}

COleFileSystem::~COleFileSystem()
{

}

/* 
***********************************
***********************************
*/

int COleFileSystem::GetNode(string const &path,  Tree_Level_Itor_t& node)
{
   
   StringList_t path_list;
   stringtok(path_list, path, "/");
   
   int retval = SearchNode(&m_RootEntry, path_list, node);

   // Delete dinamically created strings of path_list   
   // anyway the list is deleted iside SearchNode...
   for (StringListItor_t pl = path_list.begin(); pl != path_list.end(); pl++)
      delete *pl;

   return retval;
}

/* 
***********************************
***********************************
*/

int COleFileSystem::SearchNode(COleProp* base_node, StringList_t& path_list, Tree_Level_Itor_t& node_level)
{
   int errcode = FS_INVALID_PATH;
   
   if(!((base_node->m_Child_List).empty()))
   {
      for (StringListItor_t node_name = path_list.begin(); node_name != path_list.end(); ++node_name)
         for(node_level = (base_node->m_Child_List).begin(); node_level != (base_node->m_Child_List).end(); node_level++)
         {
            if (**node_name == (*node_level)->GetName())
            {

               delete path_list.front();
               path_list.pop_front(); // Remove the found element from path
               if(path_list.empty()) // If it was the last element return it
               {
          
                  return FS_NO_ERRORS;
               }
               else
               { // Recursively continue the search
                  return SearchNode(*node_level, path_list, node_level);
               }
            }
         } 
   }
   
   return errcode;



}


/* 
***********************************
***********************************
*/

int COleFileSystem::AddDirectory(string const &dir_path)
{
   int errcode = NO_ERRORS;
   StringList_t path_list;

   stringtok(path_list, dir_path, "/");
   errcode = AddNode(&m_RootEntry, path_list);

   // Delete dinamically created strings of path_list
   for (StringListItor_t pl = path_list.begin(); pl != path_list.end(); pl++)
      delete *pl;

   if(errcode == FS_NO_ERRORS)
   {
      Tree_Level_Itor_t newnode;
      GetNode(dir_path, newnode);
      (*newnode)->SetChildIndex(PLINK_EMPTY);
      (*newnode)->SetType(PTYPE_DIRECTORY);
      (*newnode)->SetSize(0);
      (*newnode)->SetColor(PROPERTY_COLOR_NODE_BLACK);
      (*newnode)->SetDataPointer(NULL);

      (*newnode)->SetCreatedSecs(PPT_DFLT_SECS1);
      (*newnode)->SetCreatedDays(PPT_DFLT_DAYS1);
      (*newnode)->SetModifiedSecs(PPT_DFLT_SECS2);
      (*newnode)->SetModifiedDays(PPT_DFLT_DAYS2);

   }



   return errcode;
}


/* 
***********************************
***********************************
*/
#define MIN_DATA_SIZE (0x1000)

// TODO: Change the DataStorage pass-argument

int COleFileSystem::AddFile(string const &dir_path, CDataStorage* pdata)
{
   int errcode = NO_ERRORS;
   StringList_t path_list;


   // NOTE: Token analysis can be done inside of AddNode:
   stringtok(path_list, dir_path, "/");
   errcode = AddNode(&m_RootEntry, path_list);

   // Delete dinamically created strings of path_list
   for (StringListItor_t pl = path_list.begin(); pl != path_list.end(); pl++)
      delete *pl;


   if(errcode == FS_NO_ERRORS)
   {
      Tree_Level_Itor_t newnode;
      GetNode(dir_path, newnode);
      (*newnode)->SetType(PTYPE_FILE);
      (*newnode)->SetChildIndex(PLINK_EMPTY);

      
      // Calculate the trail data size needed to complete a BIG_BLOCK
      unsigned32_t trail_size = (pdata->GetDataSize() % BIG_BLOCK_SIZE)?
         (BIG_BLOCK_SIZE - (pdata->GetDataSize() % BIG_BLOCK_SIZE)):0;

      // This library won't use Small Blocks, so all data elements shall be >0x1000
      if ((trail_size + pdata->GetDataSize()) < MIN_DATA_SIZE)
      {
         trail_size = MIN_DATA_SIZE  - pdata->GetDataSize();
         (*newnode)->SetSize(MIN_DATA_SIZE);
      }
      else
      {
         // The size stored in the node is the actual size of the data (not the filled-to-bigblock one)
         (*newnode)->SetSize(pdata->GetDataSize() + trail_size);
      }

      // create the trail data unit.
      CUnit* ptraildata = new CUnit;
      ptraildata->AddFixedDataArray(0x00, trail_size);
      (*pdata) += ptraildata;

      (*newnode)->SetDataPointer(pdata);

      (*newnode)->SetColor(PROPERTY_COLOR_NODE_BLACK);
      (*newnode)->SetCreatedSecs(PPT_DFLT_SECS1);
      (*newnode)->SetCreatedDays(PPT_DFLT_DAYS1);
      (*newnode)->SetModifiedSecs(PPT_DFLT_SECS2);
      (*newnode)->SetModifiedDays(PPT_DFLT_DAYS2);

      
   }



   return errcode;
}

/* 
***********************************
***********************************
*/

/*

Some special adding-node cases:

- The path to reach the node doesn't exist
- The the node already exists

*/

int COleFileSystem::AddNode(COleProp* base_node, StringList_t& path_list)
{

   int errcode;
   
   if(!(base_node->m_Child_List).empty())
   {
      for(Tree_Level_Itor_t node_child = (base_node->m_Child_List).begin();
          node_child != (base_node->m_Child_List).end(); node_child++)
      {
         if((*node_child)->GetName() == **path_list.begin())
         {
            delete path_list.front();
            path_list.pop_front(); // Remove the found element from path
            return AddNode(*node_child, path_list);
         }
      }
   }
   //if the path element wasn't found, the program gets here
   //... and we have two possible reasons:
   // 1. The path isn't correct, so no node is created and an error msg have to be returned
   //    (one of the intermediate nodes is empty or doesn't exist: there are more than one path elements left)
   // 2. The node is the new one we want to create.


   // if the size of the path is 0, the node already existed
   if(path_list.size() != 1)
   {
      if (path_list.size() == 0)
         errcode = FS_NODE_ALREADY_EXISTS;
      else
         errcode = FS_INVALID_PATH;
   }
   else
   {
      unsigned8_t base_node_type = base_node->GetType();
      if(base_node_type == PTYPE_DIRECTORY || base_node_type == PTYPE_ROOT)
      {

#if 0
         COleProp newnode(++m_nProperty_Count, *path_list.begin());

         bool was_firstborn = false;

         if((base_node->m_Child_List).empty()) //if this is the first child of the node...
         {
            base_node->SetChildIndex(m_nProperty_Count); // the new one is the child
            newnode.SetPreviousIndex(PLINK_EMPTY); // there is no previous
            was_firstborn = true;// mark for later
         }
    
         // The next-index of the new node shall to point to nowhere
         newnode.SetNextIndex(PLINK_EMPTY);
         (base_node->m_Child_List).push_back(newnode);       
      
         // Get the just added node
         Tree_Level_Itor_t nodeadded = (base_node->m_Child_List).end();
         nodeadded--;
    
         // Get the previous to the just added node
         Tree_Level_Itor_t prev_to_nodeadded = (base_node->m_Child_List).end();
         prev_to_nodeadded--;prev_to_nodeadded--;
    

         if(!was_firstborn)
            nodeadded->SetPreviousIndex(prev_to_nodeadded->GetIndex());

         // Set the next-index of the previous node to point to the newone
         prev_to_nodeadded->SetNextIndex(nodeadded->GetIndex());

#else
////////////
         COleProp* newnode = new COleProp(++m_nProperty_Count, **path_list.begin());
         size_t childnum = (base_node->m_Child_List).size();

         if(!(base_node->m_Child_List).empty()) //if this is the first child of the node...
         {
            Tree_Level_Itor_t lastnode = (base_node->m_Child_List).end();
            lastnode--;

            if(1 == childnum)
            {
               base_node->SetChildIndex(newnode->GetIndex()); // The 2nd child is the entry always
               newnode->SetPreviousIndex((*lastnode)->GetIndex()); // The previous is the only element of the list
            }
            else
            {
               (*lastnode)->SetNextIndex(newnode->GetIndex());
               newnode->SetPreviousIndex(PLINK_EMPTY); // there is no previous
            }
            newnode->SetNextIndex(PLINK_EMPTY); // there is no next
            newnode->SetChildIndex(PLINK_EMPTY); // there is no child

         }
         else
         {
            base_node->SetChildIndex(m_nProperty_Count); // the new one is the child
            newnode->SetPreviousIndex(PLINK_EMPTY); // there is no previous
            newnode->SetNextIndex(PLINK_EMPTY); // there is no next
            newnode->SetChildIndex(PLINK_EMPTY); // there is no child
         }


         (base_node->m_Child_List).push_back(newnode);

#endif
         errcode = FS_NO_ERRORS;
      }
      else
      {
         errcode = FS_NODE_NOT_A_DIRECTORY;
      }
   }
   
   return errcode;

}

/* 
***********************************
***********************************
*/

void COleFileSystem::GetAllNodes(NodeList_t& node_list)
{

   GetAllNodesList(node_list, &m_RootEntry);
   SortList(node_list);

}
/* 
***********************************
***********************************
*/


void COleFileSystem::GetAllNodesList(NodeList_t& node_list, COleProp* base_node)
{

   if(!(base_node->m_Child_List).empty())
   {
      for(Tree_Level_Itor_t child_node = (base_node->m_Child_List).begin();
          child_node != (base_node->m_Child_List).end(); child_node++ )
      {
         GetAllNodesList(node_list, (*child_node));
         node_list.push_back((*child_node));
      }

   }
   else
   {
      return;
   }
}

    
/* 
***********************************
An horribly inefficient sorting routine....
... until I find the way of using the list<T>::sort() function so
I can specify the sorting criteria (for this case, a member variable
of the elements of the list.
***********************************
*/

void COleFileSystem::SortList(NodeList_t& node_list)
{
   NodeList_t node_list_shadow = node_list;

   node_list.erase(node_list.begin(), node_list.end());

   NodeList_Itor_t i;
   int index = 1;


   do{
      for(i = node_list_shadow.begin();  i != node_list_shadow.end(); i++)
      {
         if((*i)->GetIndex() == index)
         {
            node_list.push_back(*i);
            node_list_shadow.erase(i);
            index++;
            break;
         }
      }

   }while(!node_list_shadow.empty());
}


/* 
***********************************
***********************************
*/

unsigned32_t COleFileSystem::GetTotalDataSize()
{

   NodeList_t node_list;
   GetAllNodes(node_list);
   unsigned32_t total_size = 0;

   for(NodeList_Itor_t i = node_list.begin(); i != node_list.end(); i++)
   {
      if((*i)->GetType() == PTYPE_FILE)
         total_size += (*i)->GetDataPointer()->GetDataSize();

   } 

   return total_size;

}


/* 
***********************************
***********************************
*/

unsigned32_t COleFileSystem::GetNumDataFiles()
{
   NodeList_t node_list;
   GetAllNodes(node_list);
   unsigned32_t total_files = 0;

   for(NodeList_Itor_t i = node_list.begin(); i != node_list.end(); i++)
   {
      if((*i)->GetType() == PTYPE_FILE)
         total_files ++;

   } 

   return total_files;



}

/* 
***********************************
***********************************
*/

COleProp& COleFileSystem::GetRootEntry()
{
   return m_RootEntry;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: olefs.cpp,v $
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

