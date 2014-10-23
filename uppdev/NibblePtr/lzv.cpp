#include "Entropy.h"


/***********************************************************************
**
**	lzv.c -- an extremly fast compression/decompression-method
**
**	Written by Hermann Vogt
**
**		v 0.5 -- 00/04/10	fix unaligned access (Marc)
**		v 0.4 -- 00/03/25	adapted for PApp by Marc Lehmann <pcg@goof.com>
**		v 0.3 -- 94/03/08	aCembler version of rLZV built in.
**		v 0.2 -- 94/03/04	Changes for usage with DouBle 0.2 built in.
**		v 0.1 -- 94/03/01	Intensivly tested, removed all known bugs.
**		v 0.0 -- 94/02/21	First Version.
**
**	Copyright (c) 1994 Hermann Vogt. Redistribution of this file is
**	permitted under the GNU Public Licence.
**
**	The method presented here is faster and compresses better
**	than lzrw1 and lzrw1-a. I named it lzv for "Lev-Zimpel-Vogt".
**	It uses ideas introduced by Ross Williams in his algorithm lzrw1
**	[R. N. Williams (1991): "An Extremly Fast ZIV-Lempel Data
**	Compression Algorithm", Proceedings IEEE Data Compression
**	Conference, Snowbird, Utah, 362-371] and by Fiala and Green in their
**	algorithm a1 [E. R. Fiala, D. H. Greene (1989): "Data Compression
**	with Finite Windows", Communications of the ACM, 4, 490-505].
**	Because lzv differs strongly from both, I hope there will be no
**	patent problems. The hashing-method has been stolen from Jean-loup
**	Gailly's (patent free) gzip.
**
**	KNOWN PROBLEMS:
**	-	My english is very bad.
**	-	Badly commented. (I hope this will be better in the next
**		version.)
**	-	I'm not sure if lzv is free from patent problems.
**
***********************************************************************/

#define HSIZE	0x4000
#define HMASK	0x3fff
#define HSHIFT	5

#define	MLL	32		/*        Maximum len of chain of literals        */
#define	MML	(8+256)		/*        Maximum len of match                    */
#define	MOFF	8191		/*        Maximum offset                          */
#define	HSIZ	16384		/*        Size of Hashtable                       */

/* ugly type names */

typedef	byte	uch;
typedef	word	ush;
typedef	dword	uit;

#undef ONLY_64K /* 64k-max encoder is faster */
                /* but only veeeery slightly */

/* unconditionally aligning does not cost much much, so do it if unsure */
#define align_ushort !defined(__i386)

int LZVCompress(byte * in, byte * out, byte * heap, int len, int out_len)
{
  uit hval, op, ip, l_len, m_pos, m_off, m_len, maxlen;
  ush *lzv1_htab = (word *)heap;

  maxlen = out_len;
  hval = ((in[0] << 5) ^ in[1]) & (HSIZ - 1);
  ip = op = l_len = 0;

  do
    {
      hval = ((hval << 5) ^ in[ip + 2]) & (HSIZ - 1);
      m_pos = lzv1_htab[hval];
      lzv1_htab[hval] = ip;

#ifndef ONLY_64K
      /* 
       *    If you want to compress more than 64K, uncomment
       *      the following lines.
       */

      m_pos = (ip & ~0xffff) + m_pos;
      if (m_pos >= ip && m_pos >= 0x10000)
	m_pos -= 0x10000;
#endif

      if (m_pos < ip
	  && in[m_pos    ] == in[ip    ]
	  && (m_off = ip - m_pos - 1) <= MOFF
	  && ip + 4 < len
#if align_ushort
          && in[m_pos + 1] == in[ip + 1]
          && in[m_pos + 2] == in[ip + 2]
#else
	  && *(ush *) (in + m_pos + 1) == *(ush *) (in + ip + 1)
#endif
         )
	{
	  /*      We have found a match   */
	  uit look = len - ip - 2;
	  if (look > MML)
	    look = MML;
	  m_len = 2;

	  do
	    {
	      m_len++;
	    }
	  while (m_len != look && in[ip + m_len] == in[m_pos + m_len]);

	  if (op + 2 + l_len + 3 >= maxlen)
	    return 0;

	  if (l_len != 0)
	    {
	      out[op++] = (l_len - 1) << 3;
	      do
		{
		  out[op++] = in[ip - l_len--];
		}
	      while (l_len != 0);
	    }

	  m_len -= 2;

	  if (m_len <= 6)
	    {
	      out[op++] = m_len | ((m_off >> 5) & 0xf8);
	    }
	  else
	    {
	      out[op++] = 0x07 | ((m_off >> 5) & 0xf8);
	      out[op++] = m_len - 7;
	    }

	  out[op++] = m_off & 0xff;
	  ip++;
	  hval = ((hval << 5) ^ in[ip + 2]) & (HSIZ - 1);
	  lzv1_htab[hval] = ip;
	  ip++;

	  do
	    {
	      hval = ((hval << 5) ^ in[ip + 2]) & (HSIZ - 1);
	      lzv1_htab[hval] = ip;
	      ip++;
	      m_len--;
	    }
	  while (0 != m_len);
	}
      else
	{
	  /*      No match found  */
	  ip++;
	  l_len++;

	  if (MLL == l_len)
	    {
	      if (op + 2 + MLL >= maxlen)
		return 0;

	      out[op++] = 0xf8;

	      do
		{
		  out[op++] = in[ip - l_len--];
		}
	      while (l_len != 0);
	    }
	}
    }
  while (ip < len);

  if (l_len != 0)
    {
      if (op + l_len + 3 >= maxlen)
	return 0;

      out[op++] = (l_len - 1) << 3;

      do
	{
	  out[op++] = in[ip - l_len--];
	}
      while (l_len != 0);
    }
  return op;
}

int LZVDecompress (uch const *const in, uch * const out, int ilen, int len)
{
  register uit tbuf, c_len;
  uch *const out_end = out + len;
  register uch *op = out;
  uch const *const in_end = in + ilen;
  register uch const *ip = in;

  do
    {
      tbuf = *ip++;
      c_len = tbuf & 0x07;

      if (0 == c_len)
	{
	  c_len = (tbuf >> 3) + 1;

          /*if (op + c_len > out_end) /* too many checks... */
          /*  return 0;*/

	  do
	    *op++ = *ip++;
	  while (--c_len);	/* effic: memcpy()? */
	}
      else
	{
	  register uch *m_pos;

	  if (0x07 == c_len)
	    c_len = *ip++ + 7;
	  m_pos = op - 1 - (((uit) (tbuf & 0xf8) << 5) | *ip++);

	  /* If we don't check this then we segfault (if in user
	     space) or leave process in uninteruptible state (if
	     in kernel) if the data is corrupt. */
	  if (m_pos < out)
	    return 0;		/* Compression error. */

          /*if (op + c_len + 2 > out_end) /* too many checks */
          /*  return 0;*/

	  *op++ = *m_pos++;
	  *op++ = *m_pos++;

	  do
	    *op++ = *m_pos++;
	  while (--c_len);
	}
    }
  while (op < out_end && ip < in_end);

  return op - out;
}
