/***************************************************************************
                          ezcommon  -  description
                             -------------------
    begin                : Dec. 26, 2004
    copyright            : (C) 2005 by Allen
    email                : bon_ami_@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   Explicit Distribution Limitation                                      *
 *   This rule overrides others below.                                     *
 *   This program may not be modified or used by, or, if possible,         *
 *   redistributed to people described as below,                           *
 *   1.Japanese who hold hostility against Chinese.                        *
 *   2.or, those who discriminate against people based solely on race,     *
 *     gender or sexual orientation.                                       *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
 *	header of EZ Project common functionalities
 */

#ifndef EZCOMMON_H
#define EZCOMMON_H

#include <string>
#include <fstream>

namespace ezproject {

/*	EZ Project common return type	*/
enum erttp
{
	RTTP_CRER,	//critical error, such as full, destroy with subvectors'
	// existence, request of a level not registered, memory failure
	RTTP_MNER,	//minor error, such as not found, retrieval of a value not set
	// yet
	RTTP_NOER, //no error, such as destroy almost nothing, overwritten,
	// over-registration, destroy of a non-exist vector
	RTTP_RGHT,	//right
	RTTP_NA  //N/A
};

/*	ezoi's private implementation	*/
class ezoip;

/*	EZ Project common I/O functionalities	*/
class ezoi
{
private:
	ezoi &operator=(const ezoi& sth);	//we just don't need this
	ezoi(const ezoi& sth);	//we just don't need this

	ezoip *m_p;	//private implementation detail

public:
	enum eoilvl	//I/O levels
	{
		OILVL_FATAL,	//fatal messages
		OILVL_WARN,	//warnings
		OILVL_INFO,	//information
		OILVL_NA	//messages without decorations
	};
	ezoi(void);
	ezoi(std::istream *StreamIn, std::ostream *StreamOut,
		std::ostream *StreamError);
	~ezoi(void);

	erttp st(void);	//get old and set new standard Input attributes
	void rst(void);	//restore old standard Input attributes
	void dsp(const eoilvl level, const std::string content);	//output something
	void mem(void);	//display memory failure message
	void ind(const std::string &Indication);	//output something almost in raw
	void inpt(const std::string &Indication, std::string &Result);
		//input something
	void inpt(const std::string &Indication, char &Result); //input something
};

}	/*	namespace ezproject	*/

#endif	/*	EZCOMMON_H	*/

