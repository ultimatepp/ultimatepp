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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/tostr.h,v $
 * $Revision: 1.2 $
 * $Author: dhoerl $
 * $Date: 2008/10/25 19:08:52 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef TOSTR_H
#define TOSTR_H

#include <string>
#include <sstream>
#include <iostream>
 
#include <XLS/config.h>

// forward declaration
struct str_stream;

// ... helper - allow explicit conversion to string
class as_string {};
inline std::ostream & operator<< ( std::ostream & streamOut, const as_string &)
{
  return streamOut;
}

namespace Private
{
  // what should we return when calling write_to_stream ?
  template< class type>
	 struct return_from_write_to_stream
	 {
		typedef const str_stream & return_type;
	 };
 
  template<>
	 struct return_from_write_to_stream< as_string>
	 {
		typedef std::string return_type;
	 };

  // forward declaration
  template< class type>
	 inline typename return_from_write_to_stream< type>::return_type
	 write_to_stream ( const str_stream & streamOut, const type & value);
}
 

// forward declaration
template< class type>
inline typename Private::return_from_write_to_stream< type>::return_type operator<< ( const str_stream & streamOut, const type & value);

// str_stream - allow stream usage, and then conversion to string
struct str_stream
{
  // default construction
  str_stream(){}
 
  // allow to_string like usage
  template< class type>
  str_stream( const type & value)
  {
	 *this << value;
  }

  std::stringstream & underlying_stream() const
  { return m_streamOut; }
 
  operator std::string() const
  {
	 return m_streamOut.str();
  }

  private:
  mutable std::stringstream m_streamOut;

#ifndef NDEBUG
  public:
  void recalculate_string() const
  { m_string = m_streamOut.str(); }
  private:
  mutable std::string m_string;
#endif

};

namespace Private
{
  template< class type>
	 inline typename return_from_write_to_stream< type>::return_type
	 write_to_stream ( const str_stream & streamOut, const type & value)
  {
	 streamOut.underlying_stream() << value;
#ifndef NDEBUG
	 streamOut.recalculate_string();
#endif
	 return streamOut;
  }
}

template< class type>
inline typename Private::return_from_write_to_stream< type>::return_type operator<< ( const str_stream & streamOut, const type & value)
{
  return Private::write_to_stream( streamOut, value) ;
}
 

// allow function IO manipulators
inline const str_stream & operator<< ( const str_stream & streamOut, std::ios_base & (*func)( std::ios_base&) )
{
  func( streamOut.underlying_stream());
  return streamOut;
}

inline const str_stream & operator<< ( const str_stream & streamOut, std::basic_ios< char> & (*func)( std::basic_ios< char> &) )
{
  func( streamOut.underlying_stream());
  return streamOut;
}

inline const str_stream & operator<< ( const str_stream & streamOut, std::ostream & (*func)( std::ostream &) )
{
  func( streamOut.underlying_stream());
  return streamOut;
}



#endif //TOSTR_H
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: tostr.h,v $
 * Revision 1.2  2008/10/25 19:08:52  dhoerl
 * Updated header
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * Revision 1.2  2004/01/29 03:18:55  dgonzalez
 * + Using the config.h file
 *
 * Revision 1.1  2003/12/02 18:15:06  dgonzalez
 * + Initial version. Added to handle signatures of objects
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

