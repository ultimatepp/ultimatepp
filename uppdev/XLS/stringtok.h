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
 * $Source: /cvsroot/xlslib/xlslib/src/common/stringtok.h,v $
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

/*
 * stringtok.h -- Breaks a string into tokens.  This is an example for lib3.
 *
 * Template function looks like this:
 *
 *    template <typename Container>
 *    void stringtok (Container &l,
 *                    string const &s,
 *                    char const * const ws = " \t\n");
 *
 * A nondestructive version of strtok() that handles its own memory and can
 * be broken up by any character(s).  Does all the work at once rather than
 * in an invocation loop like strtok() requires.
 *
 * Container is any type that supports push_back(a_string), although using
 * list<string> and deque<string> are indicated due to their O(1) push_back.
 * (I prefer deque<> because op[]/at() is available as well.)  The first
 * parameter references an existing Container.
 *
 * s is the string to be tokenized.  From the parameter declaration, it can
 * be seen that s is not affected.  Since references-to-const may refer to
 * temporaries, you could use stringtok(some_container, readline("")) when
 * using the GNU readline library.
 *
 * The final parameter is an array of characters that serve as whitespace.
 * Whitespace characters default to one or more of tab, space, and newline,
 * in any combination.
 *
 * 'l' need not be empty on entry.  On return, 'l' will have the token
 * strings appended.
 *
 *
 * [Example:
 *       list<string>       ls;
 *       stringtok (ls, " this  \t is\t\n  a test  ");
 *       for (list<string>::const_iterator i = ls.begin();
 *            i != ls.end(); ++i)
 *       {
 *            cerr << ':' << (*i) << ":\n";
 *       }
 *
 *  would print
 *       :this:
 *       :is:
 *       :a:
 *       :test:
 * -end example]
 *
 * pedwards@jaj.com  May 1999
 */

#include <string>
#include <cstring>    // for strchr

#include <XLS/config.h>
#include <XLS/systype.h>

/*****************************************************************
 * This is the only part of the implementation that I don't like.
 * It can probably be improved upon by the reader...
 */
namespace {
  inline bool
    isws (char c, char const * const wstr)
    {
      return (strchr(wstr,c) != NULL);
    }
}

/*****************************************************************
 * Simplistic and quite Standard, but a bit slow.  This should be
 * templatized on basic_string instead, or on a more generic StringT
 * that just happens to support ::size_type, .substr(), and so on.
 * I had hoped that "whitespace" would be a trait, but it isn't, so
 * the user must supply it.  Enh, this lets them break up strings on
 * different things easier than traits would anyhow.
 */
template <typename Container>
void
stringtok (Container &l, std::string const &s, char const * const ws = " \t\n")
{
  const std::string::size_type  S = s.size();
  std::string::size_type  i = 0;

  while (i < S) {
    // eat leading whitespace
    while ((i < S) && (isws(s[i],ws)))  ++i;
    if (i == S)  return;  // nothing left but WS

    // find end of word
    std::string::size_type  j = i+1;
    while ((j < S) && (!isws(s[j],ws)))  ++j;

    // add word
    std::string* substr = new std::string;
    *substr = s.substr(i,j-i);
    l.push_back(substr);

    // set up for next loop
    i = j+1;
  }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: stringtok.h,v $
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:32:05  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

