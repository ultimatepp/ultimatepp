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
 * $Source: /cvsroot/xlslib/xlslib/src/common/timespan.h,v $
 * $Revision: 1.2 $
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

#ifndef TIMESPAN_H
#define TIMESPAN_H

#include <time.h>
#include <assert.h>

#include <XLS/config.h>
#include <XLS/systype.h>

class CCpuClockTimespan
{
  enum
    { INVALID_VALUE = -1, };
 
 public:
  CCpuClockTimespan()
    : m_clockStart(  INVALID_VALUE),
    m_nUsedClockTicks( INVALID_VALUE)
    {}
  ~CCpuClockTimespan()
    {}

  void StartClock()
    {
      // the clock is already started !!!
      // stop it first !!!
      assert( m_clockStart == (clock_t)INVALID_VALUE);
      m_nUsedClockTicks = (clock_t)INVALID_VALUE;
      m_clockStart = clock();
    }

  void StopClock()
    {
      const clock_t clockStop = clock();

      // start the clock first !!!
      assert( m_clockStart != (clock_t)INVALID_VALUE);
      m_nUsedClockTicks = clockStop - m_clockStart;

      // after this, we can start it again !!!
      m_clockStart = INVALID_VALUE;
    }

  unsigned long GetUsedMilliseconds() const
    {
      const int MILLISECONDS_PER_SECOND = 1000;

      // the clock was never started,
      // or it's started, but it has not been stopped yet
      assert( m_nUsedClockTicks != (clock_t)INVALID_VALUE);

      double nSeconds =	( ( double)m_nUsedClockTicks) / CLOCKS_PER_SEC;

      const unsigned long nMilliseconds = (unsigned long)(nSeconds * MILLISECONDS_PER_SECOND);

      return nMilliseconds;
    }

 private:

  // when did we Start to measure
  // clock time?
  clock_t m_clockStart;

  // the used clock ticks, from Start to Stop
  clock_t m_nUsedClockTicks;
};

#endif //TIMESPAN_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: timespan.h,v $
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:32:05  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

