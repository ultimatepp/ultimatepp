/*-
 * Copyright 2003-2005 Colin Percival
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions 
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if 0
__FBSDID("$FreeBSD: src/usr.bin/bsdiff/bspatch/bspatch.c,v 1.1 2005/08/06 01:59:06 cperciva Exp $");
#endif

#include <Core/Core.h>

#include "bzlib.h"
#include <fcntl.h>

#include "../bsdiff.h"

NAMESPACE_UPP

static off_t offtin(u_char *buf)
{
	off_t y;

	y=buf[7]&0x7F;
	y=y*256;y+=buf[6];
	y=y*256;y+=buf[5];
	y=y*256;y+=buf[4];
	y=y*256;y+=buf[3];
	y=y*256;y+=buf[2];
	y=y*256;y+=buf[1];
	y=y*256;y+=buf[0];

	if(buf[7]&0x80) y=-y;

	return y;
}

bool BSPatch(String oldfile, String newfile, String patchfile)
{
	if (!FileExists(oldfile))
		return Err(Format(t_("File %s does not exist"), oldfile));
	if (!FileExists(patchfile))
		return Err(Format(t_("File %s does not exist"), oldfile));	
	if (newfile.IsEmpty())
		return Err(t_("New file is empty"));	
	
	FILE * f, * cpf, * dpf, * epf;
	BZFILE * cpfbz2, * dpfbz2, * epfbz2;
	int cbz2err, dbz2err, ebz2err;
	int fd;
	ssize_t oldsize,newsize;
	ssize_t bzctrllen,bzdatalen;
	u_char header[32],buf[8];
	u_char *old, *nnew;
	off_t oldpos,newpos;
	off_t ctrl[3];
	off_t lenread;
	off_t i;
	
	//if(argc!=4) errx(1,"usage: %s oldfile newfile patchfile\n",argv[0]);

	/* Open patch file */
	if (
#ifdef PLATFORM_POSIX			
	(f = fopen(patchfile, "r")) == NULL)
#else
	(f = _wfopen(patchfile.ToWString(), L"rb")) == NULL)	
#endif
		return Err(Format(t_("fopen(%s)"), patchfile));

	/*
	File format:
		0	8	"BSDIFF40"
		8	8	X
		16	8	Y
		24	8	sizeof(newfile)
		32	X	bzip2(control block)
		32+X	Y	bzip2(diff block)
		32+X+Y	???	bzip2(extra block)
	with control block a set of triples (x,y,z) meaning "add x bytes
	from oldfile to x bytes from the diff block; copy y bytes from the
	extra block; seek forwards in oldfile by z bytes".
	*/

	/* Read header */
	if (fread(header, 1, 32, f) < 32) {
		if (feof(f))
			return Err(t_("Corrupt patch"));
		return Err(Format(t_("fread(%s)"), patchfile));
	}

	/* Check for appropriate magic */
	if (memcmp(header, "BSDIFF40", 8) != 0)
		return Err(t_("Corrupt patch"));

	/* Read lengths from header */
	bzctrllen=offtin(header+8);
	bzdatalen=offtin(header+16);
	newsize=offtin(header+24);
	if((bzctrllen<0) || (bzdatalen<0) || (newsize<0))
		return Err(t_("Corrupt patch"));

	/* Close patch file and re-open it via libbzip2 at the right places */
	if (fclose(f))
		return Err(Format(t_("fclose(%s)"), /*argv[3]*/patchfile));
	if (
#ifdef PLATFORM_POSIX			
	(cpf = fopen(patchfile, "r")) == NULL)
#else
	(cpf = _wfopen(patchfile.ToWString(), L"rb")) == NULL)	
#endif	    
		return Err(Format(t_("fopen(%s)"), patchfile));
	if (fseeko(cpf, 32, SEEK_SET))
		return Err(Format(t_("fseeko(%s, %lld)"), patchfile,
		    (long long)32));
	if ((cpfbz2 = BZ2_bzReadOpen(&cbz2err, cpf, 0, 0, NULL, 0)) == NULL)
		return Err(Format(t_("BZ2_bzReadOpen, bz2err = %d"), cbz2err));
	if (
#ifdef PLATFORM_POSIX			
	(dpf = fopen(patchfile, "r")) == NULL)
#else
	(dpf = _wfopen(patchfile.ToWString(), L"rb")) == NULL)	
#endif			
		return Err(Format(t_("fopen(%s)"), patchfile));
	if (fseeko(dpf, 32 + bzctrllen, SEEK_SET))
		return Err(Format(t_("fseeko(%s, %lld)"), patchfile,
		    (long long)(32 + bzctrllen)));
	if ((dpfbz2 = BZ2_bzReadOpen(&dbz2err, dpf, 0, 0, NULL, 0)) == NULL)
		return Err(Format(t_("BZ2_bzReadOpen, bz2err = %d"), dbz2err));
	if (
#ifdef PLATFORM_POSIX			
	(epf = fopen(patchfile, "r")) == NULL)
#else
	(epf = _wfopen(patchfile.ToWString(), L"rb")) == NULL)	
#endif			
		return Err(Format(t_("fopen(%s)"), patchfile));
	if (fseeko(epf, 32 + bzctrllen + bzdatalen, SEEK_SET))
		return Err(Format(t_("fseeko(%s, %lld)"), patchfile,
		    (long long)(32 + bzctrllen + bzdatalen)));
	if ((epfbz2 = BZ2_bzReadOpen(&ebz2err, epf, 0, 0, NULL, 0)) == NULL)
		return Err(Format(t_("BZ2_bzReadOpen, bz2err = %d"), ebz2err));

	if(
#ifdef PLATFORM_POSIX		
		((fd = open(oldfile, O_RDONLY, 0)) < 0) ||
#else
		((fd = _wsopen(oldfile.ToWString(), O_RDONLY|O_BINARY, _SH_DENYNO, 0)) < 0) ||
#endif		
		((oldsize=lseek(fd,0,SEEK_END))==-1) ||
		((old=(u_char *)malloc(oldsize+1))==NULL) ||
		(lseek(fd,0,SEEK_SET)!=0))
		Err(Format(t_("Error opening %s"), oldfile));
	int r = oldsize;
	while(r>0 && (i=read(fd,old+oldsize-r,r))>0) 
		r -= i;
	if (r>0 || close(fd)==-1) 
		Err(Format(t_("Error opening %s"), oldfile));
	
	if((nnew=(u_char *)malloc(newsize+1))==NULL) 
		return Err(t_("Not enough memory"));

	oldpos=0;newpos=0;
	while(newpos<newsize) {
		/* Read control data */
		for(i=0;i<=2;i++) {
			lenread = BZ2_bzRead(&cbz2err, cpfbz2, buf, 8);
			if ((lenread < 8) || ((cbz2err != BZ_OK) &&
			    (cbz2err != BZ_STREAM_END)))
				return Err(t_("Corrupt patch"));
			ctrl[i]=offtin(buf);
		};

		/* Sanity-check */
		if(newpos+ctrl[0]>newsize)
			return Err(t_("Corrupt patch"));

		/* Read diff string */
		lenread = BZ2_bzRead(&dbz2err, dpfbz2, nnew + newpos, ctrl[0]);
		if ((lenread < ctrl[0]) ||
		    ((dbz2err != BZ_OK) && (dbz2err != BZ_STREAM_END)))
			return Err(t_("Corrupt patch"));

		/* Add old data to diff string */
		for(i=0;i<ctrl[0];i++)
			if((oldpos+i>=0) && (oldpos+i<oldsize))
				nnew[newpos+i]+=old[oldpos+i];

		/* Adjust pointers */
		newpos+=ctrl[0];
		oldpos+=ctrl[0];

		/* Sanity-check */
		if(newpos+ctrl[1]>newsize)
			return Err(t_("Corrupt patch"));

		/* Read extra string */
		lenread = BZ2_bzRead(&ebz2err, epfbz2, nnew + newpos, ctrl[1]);
		if ((lenread < ctrl[1]) ||
		    ((ebz2err != BZ_OK) && (ebz2err != BZ_STREAM_END)))
			return Err(t_("Corrupt patch"));

		/* Adjust pointers */
		newpos+=ctrl[1];
		oldpos+=ctrl[2];
	};

	/* Clean up the bzip2 reads */
	BZ2_bzReadClose(&cbz2err, cpfbz2);
	BZ2_bzReadClose(&dbz2err, dpfbz2);
	BZ2_bzReadClose(&ebz2err, epfbz2);
	if (fclose(cpf) || fclose(dpf) || fclose(epf))
		return Err(Format(t_("fclose(%s)"), patchfile));

	/* Write the new file */
	if(
#ifdef PLATFORM_POSIX		
		((fd = open(newfile, O_CREAT|O_TRUNC|O_WRONLY, 0666)) < 0) ||
#else
		((fd = _wsopen(newfile.ToWString(), O_CREAT|O_TRUNC|O_WRONLY|O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE)) < 0) ||
#endif			
		(write(fd,nnew,newsize)!=newsize) || (close(fd)==-1))
		return Err(Format(t_("Impossible to open %s"), newfile));

	free(nnew);
	free(old);

	return 1;
}

END_UPP_NAMESPACE