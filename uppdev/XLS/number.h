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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/number.h,v $
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

#ifndef NUMBER_H
#define NUMBER_H

#include <XLS/config.h>
#include <XLS/common.h>
#include <XLS/cell.h>
#include <XLS/record.h>
#include <XLS/unit.h>


namespace xlslib_core
{

  class number_t;
  class CNumber: public CRecord
    {
    private:


    public:
      CNumber(unsigned16_t row,
              unsigned16_t col,
              double num,
              xf_t* pxfval = NULL);

      CNumber(number_t& blankdef);
      ~CNumber();


    };


  class number_t: public cell_t
    {

    private:
      double number;
    public:
      number_t(unsigned16_t rowval, 
               unsigned16_t colval, 
               double numval,
               xf_t* pxfval = NULL);
   
      ~number_t(){};

      unsigned16_t GetSize(){return 18;};
      double GetNumber() {return number;};
      CUnit* GetData();

    };


}

#endif //NUMBER_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: number.h,v $
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

