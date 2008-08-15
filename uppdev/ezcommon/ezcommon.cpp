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
 *	source code of EZ Project common functionalities
 */

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif
#include <unistd.h>
#include <stdio.h>
#include <iostream>

#include "ezcommon.h"

namespace ezproject {

/*	ezoi's private implementation	*/
class ezoip
{
private:
	std::istream *is;	//input file stream
	std::ostream *os;	//output file stream
	std::ostream *es;	//error-output file stream
#ifdef HAVE_TERMIOS_H
	struct termios *tm;	//terminal attributes for standard input
#endif

public:
	ezoip(void) : is(NULL), os(NULL), es(NULL)
#ifdef HAVE_TERMIOS_H
		, tm(NULL)
#endif
		{};	//use standard I/O
	ezoip(std::istream *StreamIn, std::ostream *StreamOut,
		std::ostream *StreamError)
		: is(StreamIn), os(StreamOut), es(StreamError)
#ifdef HAVE_TERMIOS_H
		, tm(NULL)
#endif
		{};	//use file(s) as I/O
	~ezoip(void) {
#ifdef HAVE_TERMIOS_H
	       free(tm);
#endif
	};

#ifdef HAVE_TERMIOS_H
	erttp st(void);	//get old and set new standard Input attributes
	void rst(void);	//restore old standard Input attributes
#endif
	void dsp(const ezoi::eoilvl level, const std::string content);	//output something
	void ind(const std::string &Indication);	//output something almost in raw
	void inpt(const std::string &Indication, std::string &Result);	//input something
	void inpt(const std::string &Indication, char &Result); //input something
};

}	/*	namespace ezproject	*/


using namespace ezproject;

/*	ezoip	*/
#ifdef HAVE_TERMIOS_H
erttp ezoip::st(void)
{
	erttp ret = RTTP_CRER;

	if ((NULL == is) && (NULL == tm))
	{
		if (NULL != (tm = (struct termios *)malloc(sizeof(struct termios))))
		{
			struct termios t;

			if (0 == tcgetattr(STDIN_FILENO, &t))
			{
				if (NULL != memcpy(tm, &t, sizeof(t)))
				{
					ret = RTTP_RGHT;
					if (0 == (tm->c_lflag & ICANON))
					{
						tm->c_lflag &= ~ICANON;
						tcsetattr(STDIN_FILENO, 0, tm);
					}
					else
					{
						free(tm); tm = NULL;
					}
				}
			}
		}
	}
	else
		ret = RTTP_NOER;
	if (RTTP_CRER == ret)
	{
		free(tm);
		tm = NULL;
	}

	return ret;
}

void ezoip::rst(void)
{
	if ((tm != NULL) && (NULL == is))
		tcsetattr(STDIN_FILENO, 0, tm);
}
#endif	/* HAVE_TERMIOS_H */

void ezoip::dsp(const ezoi::eoilvl lvl, const std::string cnt)
{
	switch (lvl)
	{
		case ezoi::OILVL_FATAL:
			((NULL == es) ? std::cerr : *es)
				<< "FATAL: " << cnt << "!" << std::endl;
			break;
		case ezoi::OILVL_WARN:
			((NULL == es) ? std::cerr : *es)
				<< "Warning! " << cnt << std::endl;
			break;
		case ezoi::OILVL_INFO:
			((NULL == os) ? std::cout : *os)
				<< "Info: " << cnt << std::endl;
			break;
		case ezoi::OILVL_NA:
			((NULL == os) ? std::cout : *os) << cnt;
			break;
	}
}

void ezoip::ind(const std::string &ind)
{
	if(NULL == os)
	{
		std::cout << ind << std::flush;
	}
	else
		dsp(ezoi::OILVL_NA, ind);
}

void ezoip::inpt(const std::string &indi, std::string &rst)
{
	ind(indi);
	((NULL == is) ? std::cin : *is) >> rst;
}

void ezoip::inpt(const std::string &indi, char &rst)
{
	ind(indi);
	rst = getchar();
}


/*	ezoi	*/
/*	-structors	*/
ezoi::ezoi(void)
{
	m_p = new ezoip;
}

ezoi::ezoi(std::istream *si, std::ostream *so,
		std::ostream *se)
{
	m_p = new ezoip(si, so, se);
}

ezoi::~ezoi(void)
{
	delete m_p;
}

/*	other interfaces	*/
erttp ezoi::st(void)
{
#ifdef HAVE_TERMIOS_H
	return m_p->st();
#else
	return RTTP_RGHT;
#endif
}

void ezoi::rst(void)
{
#ifdef HAVE_TERMIOS_H
	m_p->rst();
#endif
}

void ezoi::dsp(const ezoi::eoilvl lvl, const std::string cnt)
{
	m_p->dsp(lvl, cnt);
}

void ezoi::mem(void)
{
	m_p->dsp(ezoi::OILVL_FATAL, "error in allocating memory");
}

void ezoi::ind(const std::string &ind)
{
	m_p->ind(ind);
}

void ezoi::inpt(const std::string &ind, std::string &rst)
{
	m_p->inpt(ind, rst);
}

void ezoi::inpt(const std::string &ind, char &rst)
{
	m_p->inpt(ind, rst);
}

