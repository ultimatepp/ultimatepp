/* mbpixbuf.c
 *
 * Copyright (C) 2002 Matthew Allum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#define _GNU_SOURCE

#include <strings.h>
#include "mbpixbuf.h"
#ifdef USE_PNG
#include <png.h>
#endif
#define BYTE_ORD_24_RGB  0
#define BYTE_ORD_24_RBG  1
#define BYTE_ORD_24_BRG  2
#define BYTE_ORD_24_BGR  3
#define BYTE_ORD_24_GRB  4
#define BYTE_ORD_24_GBR  5
#define BYTE_ORD_32_ARGB 6

#define alpha_composite(composite, fg, alpha, bg) {               \
    ush temp;                                                     \
    if ((alpha) == 0)                                             \
       (composite) = (bg);                                        \
    else if ((alpha) == 255)                                      \
        (composite) = (fg);                                       \
    else {                                                        \
        temp = ((ush)(fg)*(ush)(alpha) +                          \
                (ush)(bg)*(ush)(255 - (ush)(alpha)) + (ush)128);  \
    (composite) = (ush)((temp + (temp >> 8)) >> 8);             } \
}

#ifdef WORDS_BIGENDIAN
#define  SHORT_FROM_2BYTES(p) ( *(p+1) | (*(p) << 8) )
#define  2BYTES_FROM_SHORT(p,s)                       \
     *((p)+1)   = (unsigned char) s;                  \
     *(p)       = (unsigned char) ((s >> 8) & 0xff);
#else
#define  SHORT_FROM_2BYTES(p) ( *(p) | (*((p)+1) << 8) )
#define  BYTES_FROM_SHORT(p,s)                       \
     *(p)   = (unsigned char) s;                      \
     *((p)+1) = (unsigned char) ((s >> 8) & 0xff);
#endif

#define internal_16bpp_pixel_to_rgb(p,r,g,b)           \
      {                                                \
         unsigned short s = SHORT_FROM_2BYTES(p);      \
         (r) = (( s & 0xf800) >> 8);                   \
         (g) = (( s & 0x07e0) >> 3);                   \
         (b) = (( s & 0x001f) << 3);                   \
      }


#define internal_rgb_to_16bpp_pixel(r,g,b,p)          \
     {                                                \
      unsigned short s = (  (((b) >> 3) & 0x001f) |   \
                            (((g) << 3) & 0x07e0) |   \
                            (((r) << 8) & 0xf800) );  \
      BYTES_FROM_SHORT(p,s)                           \
     }

#define internal_16bpp_pixel_next(p) \
      (p) += 2

#define IN_REGION(x,y,w,h) ( (x) > -1 && (x) < (w) && (y) > -1 && (y) <(h) )

typedef unsigned short ush;

#ifdef USE_PNG
static unsigned char*
_load_png_file( const char *file,
		int *width, int *height, int *has_alpha );
#endif

#ifdef USE_JPG
static unsigned char*
_load_jpg_file( const char *file,
		int *width, int *height, int *has_alpha );
#endif

static int _mbpb_trapped_error_code = 0;
static int (*_mbpb_old_error_handler) (Display *d, XErrorEvent *e);

static int
_mbpb_error_handler(Display     *display,
	      XErrorEvent *error)
{
   _mbpb_trapped_error_code = error->error_code;
   return 0;
}

static void
_mbpb_trap_errors(void)
{
   _mbpb_trapped_error_code = 0;
   _mbpb_old_error_handler = XSetErrorHandler(_mbpb_error_handler);
}

static int
_mbpb_untrap_errors(void)
{
   XSetErrorHandler(_mbpb_old_error_handler);
   return _mbpb_trapped_error_code;
}

static int
_paletteAlloc(MBPixbuf *pb);

#ifdef USE_JPG

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

void
_jpeg_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

static unsigned char*
_load_jpg_file( const char *file,
		int *width, int *height, int *has_alpha )
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  JSAMPLE *buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  unsigned char *data = NULL;

  if ((infile = fopen(file, "rb")) == NULL) {
    if (mb_want_warnings())
      fprintf(stderr, "mbpixbuf: can't open %s\n", file);
    return NULL;
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = _jpeg_error_exit;

  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return NULL;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);

  cinfo.do_fancy_upsampling = FALSE;
  cinfo.do_block_smoothing  = FALSE;
  cinfo.out_color_space     = JCS_RGB;
  cinfo.scale_num           = 1;

  jpeg_start_decompress(&cinfo);

  if( cinfo.output_components != 3 )
    {
      if (mb_want_warnings())
	fprintf( stderr, "mbpixbuf: jpegs with %d channles not supported\n",
		 cinfo.output_components );
      jpeg_finish_decompress(&cinfo);
      jpeg_destroy_decompress(&cinfo);
      return NULL;
  }

  *has_alpha = False;
  *width     = cinfo.output_width;
  *height    = cinfo.output_height;

  data = malloc(*width * *height * 3 );
  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = malloc( sizeof(JSAMPLE)*row_stride );

  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, &buffer, 1);
    memcpy( &data[ ( cinfo.output_scanline - 1 ) * row_stride ],
	    buffer, row_stride );
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  if (buffer) free(buffer);

  return data;
}

#endif

#ifdef USE_PNG

static unsigned char*
_load_png_file( const char *file,
	       int *width, int *height, int *has_alpha ) {
  FILE *fd;
  unsigned char *data;
  unsigned char header[8];
  int  bit_depth, color_type;

  png_uint_32  png_width, png_height, i, rowbytes;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep *row_pointers;

  if ((fd = fopen( file, "rb" )) == NULL) return NULL;

  fread( header, 1, 8, fd );
  if ( ! png_check_sig( header, 8 ) )
    {
      fclose(fd);
      return NULL;
    }

  png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if ( ! png_ptr ) {
    fclose(fd);
    return NULL;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if ( ! info_ptr ) {
    png_destroy_read_struct( &png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    fclose(fd);
    return NULL;
  }

  if ( setjmp( png_ptr->jmpbuf ) ) {
    png_destroy_read_struct( &png_ptr, &info_ptr, NULL);
    fclose(fd);
    return NULL;
  }

  png_init_io( png_ptr, fd );
  png_set_sig_bytes( png_ptr, 8);
  png_read_info( png_ptr, info_ptr);
  png_get_IHDR( png_ptr, info_ptr, &png_width, &png_height, &bit_depth,
		&color_type, NULL, NULL, NULL);
  *width = (int) png_width;
  *height = (int) png_height;

  if (( color_type == PNG_COLOR_TYPE_PALETTE )||
      ( png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS )))
    png_set_expand(png_ptr);

  if (( color_type == PNG_COLOR_TYPE_GRAY )||
      ( color_type == PNG_COLOR_TYPE_GRAY_ALPHA ))
    png_set_gray_to_rgb(png_ptr);

  if ( info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA
       || info_ptr->color_type == PNG_COLOR_TYPE_GRAY_ALPHA
       )
    *has_alpha = 1;
  else
    *has_alpha = 0;

  /* 8 bits */
  if ( bit_depth == 16 )
    png_set_strip_16(png_ptr);

  if (bit_depth < 8)
    png_set_packing(png_ptr);

  /* not needed as data will be RGB not RGBA and have_alpha will reflect this
    png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
  */

  png_read_update_info( png_ptr, info_ptr);

  /* allocate space for data and row pointers */
  rowbytes = png_get_rowbytes( png_ptr, info_ptr);
  data = (unsigned char *) malloc( (rowbytes*(*height + 1)));
  row_pointers = (png_bytep *) malloc( (*height)*sizeof(png_bytep));

  if (( data == NULL )||( row_pointers == NULL )) {
    png_destroy_read_struct( &png_ptr, &info_ptr, NULL);
    free(data);
    free(row_pointers);
    return NULL;
  }

  for ( i = 0;  i < *height; i++ )
    row_pointers[i] = data + i*rowbytes;

  png_read_image( png_ptr, row_pointers );
  png_read_end( png_ptr, NULL);

  free(row_pointers);
  png_destroy_read_struct( &png_ptr, &info_ptr, NULL);
  fclose(fd);

  return data;
}

#endif

static unsigned char*
_load_xpm_file( MBPixbuf *pb, const char *filename, int *w, int *h, int *has_alpha)
{ 				/* This hell is adapted from imlib ;-) */
  FILE *file;

  struct _cmap
  {
    unsigned char       str[6];
    unsigned char       transp;
    short                 r, g, b;
  } *cmap = NULL;

  unsigned char *data = NULL, *ptr = NULL, *end = NULL;

  int pc, c = ' ', i = 0, j = 0, k = 0, ncolors = 0, cpp = 0,
    comment = 0, transp = 0, quote = 0, context = 0,  len, done = 0;

  char *line, s[257], tok[128], col[256];

  XColor xcol;
  int lsz = 256;

  short lookup[128 - 32][128 - 32];

  if (!filename) return NULL;

  if ((file = fopen( filename, "rb" )) == NULL) return NULL;

  line = malloc(lsz);

  while (!done)
    {
      pc = c;
      c = fgetc(file);
      if (c == EOF)
	break;
      if (!quote)
	{
	  if ((pc == '/') && (c == '*'))
	    comment = 1;
	  else if ((pc == '*') && (c == '/') && (comment))
	    comment = 0;
	}
      if (!comment)
	{
	  if ((!quote) && (c == '"'))
	    {
	      quote = 1;
	      i = 0;
	    }
	  else if ((quote) && (c == '"'))
	    {
	      line[i] = 0;
	      quote = 0;
	      if (context == 0)
		{
		  /* Header */
		  sscanf(line, "%i %i %i %i", w, h, &ncolors, &cpp);
                  if (ncolors > 32766 || cpp > 5 || *w > 32767 || *h > 32767)
		    {
		      if (mb_want_warnings())
			fprintf(stderr, "xpm file invalid");
		      fclose(file);
		      free(line);
		      fclose(file);
		      return NULL;
		    }

		  cmap = malloc(sizeof(struct _cmap) * ncolors);

		  if (!cmap)
		    {
		      free(line);
		      fclose(file);
		      return NULL;
		    }

		  data = malloc(*w ** h * 4);
		  if (!data)
		    {
		      free(cmap);
		      free(line);
		      fclose(file);
		      return NULL;
		    }

		  ptr = data;
		  end = ptr + (*w ** h * 4);
		  j = 0;
		  context++;
		}
	      else if (context == 1)
		{
		  /* Color Table */
		  if (j < ncolors)
		    {
		      int                 slen;
		      int                 hascolor, iscolor;

		      iscolor = 0;
		      hascolor = 0;
		      tok[0] = 0;
		      col[0] = 0;
		      s[0] = 0;
		      len = strlen(line);
		      strncpy((char*)cmap[j].str, line, cpp);
		      cmap[j].str[cpp] = 0;
		      cmap[j].r = -1;
		      cmap[j].transp = 0;
		      for (k = cpp; k < len; k++)
			{
			  if (line[k] != ' ')
			    {
			      s[0] = 0;
			      sscanf(&line[k], "%256s", s);
			      slen = strlen(s);
			      k += slen;
			      if (!strcmp(s, "c"))
				iscolor = 1;
			      if ((!strcmp(s, "m")) || (!strcmp(s, "s")) ||
				  (!strcmp(s, "g4")) || (!strcmp(s, "g")) ||
				  (!strcmp(s, "c")) || (k >= len))
				{
				  if (k >= len)
				    {
				      if (col[0])
					strcat(col, " ");
                                      if (strlen(col) + strlen(s) < sizeof(col))
					strcat(col, s);
				    }
				  if (col[0])
				    {
				      if (!strcasecmp(col, "none"))
					{
					  transp = 1;
					  cmap[j].transp = 1;
					}
				      else
					{
					  if ((((cmap[j].r < 0) ||
						(!strcmp(tok, "c"))) &&
					       (!hascolor)))
					    {
					      XParseColor(pb->dpy,
							  DefaultColormap(pb->dpy,
									  pb->scr),

							  col, &xcol);
					      cmap[j].r = xcol.red >> 8;
					      cmap[j].g = xcol.green >> 8;
					      cmap[j].b = xcol.blue >> 8;
					      if ((cmap[j].r == 255) &&
						  (cmap[j].g == 0) &&
						  (cmap[j].b == 255))
						cmap[j].r = 254;
					      if (iscolor)
						hascolor = 1;
					    }
					}
				    }
				  strcpy(tok, s);
				  col[0] = 0;
				}
			      else
				{
				  if (col[0])
				    strcat(col, " ");
				  strcat(col, s);
				}
			    }
			}
		    }
		  j++;
		  if (j >= ncolors)
		    {
		      if (cpp == 1)
			for (i = 0; i < ncolors; i++)
			  lookup[(int)cmap[i].str[0] - 32][0] = i;
		      if (cpp == 2)
			for (i = 0; i < ncolors; i++)
			  lookup[(int)cmap[i].str[0] - 32][(int)cmap[i].str[1] - 32] = i;
		      context++;
		    }
		}
	      else
		{
		  /* Image Data */
		  i = 0;
		  if (cpp == 1)
		    {
		      for (i = 0;
			   ((i < 65536) && (ptr < end) && (line[i]));
			   i++)
			{
			  col[0] = line[i];
			  if (transp &&
			      cmap[lookup[(int)col[0] - 32][0]].transp)
			    {
			      *ptr++ = 0; *ptr++ = 0; *ptr++ = 0; *ptr++ = 0;
			    }
			  else
			    {
			      int idx = lookup[(int)col[0] - 32][0];
			      *ptr++ = (unsigned char)cmap[idx].r;
			      *ptr++ = (unsigned char)cmap[idx].g;
			      *ptr++ = (unsigned char)cmap[idx].b;
			      if (transp) *ptr++ = 255;
			    }
			}
		    }
		  else
		    {
		      for (i = 0;
			   ((i < 65536) && (ptr < end) && (line[i]));
			   i++)
			{
			  for (j = 0; j < cpp; j++, i++)
			    {
			      col[j] = line[i];
			    }
			  col[j] = 0;
			  i--;
			  for (j = 0; j < ncolors; j++)
			    {
			      if (!strcmp(col, (char*)cmap[j].str))
				{
				  if (transp && cmap[j].transp)
				    {
				      *ptr++ = 0;
				      *ptr++ = 0;
				      *ptr++ = 0;
				      *ptr++ = 0;
				    }
				  else
				    {
				      *ptr++ = (unsigned char)cmap[j].r;
				      *ptr++ = (unsigned char)cmap[j].g;
				      *ptr++ = (unsigned char)cmap[j].b;
				      if (transp) *ptr++ = 255;
				    }
				  j = ncolors;
				}
			    }
			}
		    }
		}
	    }
	}

      /* Scan in line from XPM file */
      if ((!comment) && (quote) && (c != '"'))
	{
	  if (c < 32)
	    c = 32;
	  else if (c > 127)
	    c = 127;
	  line[i++] = c;
	}
      if (i >= lsz)
	{
	  lsz += 256;
	  line = realloc(line, lsz);
	  if(line == NULL)
	    {
	      free(cmap);
	      fclose(file);
	      return NULL;
	    }
	}

      if ((ptr) && ((ptr - data) >= *w ** h * 4))
	done = 1;
    }

  if (transp)
    *has_alpha = 1;
  else
    *has_alpha = 0;

  free(cmap);
  free(line);

  fclose(file);
  return data;
}

static int
_paletteAlloc(MBPixbuf *pb)
{
  XColor              xcl;
  int                 colnum, i, j;
  unsigned long       used[256];
  int                 num_used, is_used;

  int num_of_cols = 1 << pb->depth;
  int colors_per_channel = num_of_cols / 3;

  if (pb->palette) free(pb->palette);

  pb->palette = malloc(sizeof(MBPixbufColor) * num_of_cols);

  num_used = 0;
  colnum = 0;

  switch(pb->vis->class)
    {
    case PseudoColor:
    case StaticColor:
      /*
      for (r = 0, i = 0; r < colors_per_channel; r++)
        for (g = 0; g < colors_per_channel; g++)
          for (b = 0; b < colors_per_channel; b++, i++)
	    {
	      xcl.red   = (r * 0xffff) / (colors_per_channel - 1);
	      xcl.green = (g * 0xffff) / (colors_per_channel - 1);
	      xcl.blue  = (b * 0xffff) / (colors_per_channel - 1);
	      xcl.flags = DoRed | DoGreen | DoBlue;

      */

      for (i = 0; i < num_of_cols; i++)
	{			/* RRRGGGBB - TODO check for 4 bit col */
	  int ii = (i * 256)/num_of_cols;
	  xcl.red = (unsigned short)( ( ii & 0xe0 ) << 8 );
	  xcl.green = (unsigned short)(( ii & 0x1c ) << 11 );
	  xcl.blue = (unsigned short)( ( ii & 0x03 ) << 14 );
	  xcl.flags = DoRed | DoGreen | DoBlue;

	  if (!XAllocColor(pb->dpy, pb->root_cmap, &xcl))
	    {
	      if (mb_want_warnings())
		printf("alloc color failed\n");
	    }
	  is_used = 0;
	  for (j = 0; j < num_used; j++)
	    {
	      if (xcl.pixel == used[j])
		{
		  is_used = 1;
		  j = num_used;
		}
	    }
	  if (!is_used)
	    {
	      pb->palette[colnum].r = xcl.red >> 8;
	      pb->palette[colnum].g = xcl.green >> 8;
	      pb->palette[colnum].b = xcl.blue >> 8;
	      pb->palette[colnum].pixel = xcl.pixel;
	      used[num_used++] = xcl.pixel;
	      colnum++;
	    }
	  else
	    xcl.pixel = 0;
	}
      break;
    case GrayScale:
    case StaticGray:
      for(i = 0; i < num_of_cols; i++)
	{
	  xcl.red   = (i * 0xffff) / (colors_per_channel - 1);
	  xcl.green = (i * 0xffff) / (colors_per_channel - 1);
	  xcl.blue  = (i * 0xffff) / (colors_per_channel - 1);
	  xcl.flags = DoRed | DoGreen | DoBlue;

	  if (!XAllocColor(pb->dpy, pb->root_cmap, &xcl))
	    {
	      if (mb_want_warnings())
		fprintf(stderr, " alloc color failed\n");
	    }
	  is_used = 0;
	  for (j = 0; j < num_used; j++)
	    {
	      if (xcl.pixel == used[j])
		{
		  is_used = 1;
		  j = num_used;
		}
	    }
	  if (!is_used)
	    {
	      pb->palette[colnum].r = xcl.red >> 8;
	      pb->palette[colnum].g = xcl.green >> 8;
	      pb->palette[colnum].b = xcl.blue >> 8;
	      pb->palette[colnum].pixel = xcl.pixel;
	      used[num_used++] = xcl.pixel;
	      colnum++;
	    }
	  else
	    xcl.pixel = 0;
	}
    }
  return colnum;
}


static unsigned long
mb_pixbuf_get_pixel(MBPixbuf *pb, int r, int g, int b, int a)
{
  if (pb->depth > 8)
    {
      switch (pb->depth)
	{
	case 15:
	  return ((r & 0xf8) << 7) | ((g & 0xf8) << 2) | ((b & 0xf8) >> 3);
	case 16:
	  return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
	case 24:
	case 32:
	  switch (pb->byte_order)
	    {
	    case BYTE_ORD_24_RGB:
	      return ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
	    case BYTE_ORD_24_RBG:
	      return ((r & 0xff) << 16) | ((b & 0xff) << 8) | (g & 0xff);
	    case BYTE_ORD_24_BRG:
	      return ((b & 0xff) << 16) | ((r & 0xff) << 8) | (g & 0xff);
	    case BYTE_ORD_24_BGR:
	      return ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff);
	    case BYTE_ORD_24_GRB:
	      return ((g & 0xff) << 16) | ((r & 0xff) << 8) | (b & 0xff);
	    case BYTE_ORD_24_GBR:
	      return ((g & 0xff) << 16) | ((b & 0xff) << 8) | (r & 0xff);
	    case BYTE_ORD_32_ARGB:
	      return  (a << 24) | (r << 16) | (g << 8) | b;
	    default:
	      return 0;
	    }
	default:
	  return 0;
	}
      return 0;
    }

  /* pb->depth <= 8 */
  switch(pb->vis->class)
    {
    case PseudoColor:
    case StaticColor:
    {
      int                 i;
      int                 dif;
      int                 dr, dg, db;
      unsigned long       col = 0;
      int                 mindif = 0x7fffffff;

      for (i = 0; i < pb->num_of_cols; i++)
	{
	  dr = r - pb->palette[i].r;
	  if (dr < 0)
	    dr = -dr;
	  dg = g - pb->palette[i].g;
	  if (dg < 0)
	    dg = -dg;
	  db = b - pb->palette[i].b;
	  if (db < 0)
	    db = -db;
	  dif = dr + dg + db;
	  if (dif < mindif)
	    {
	      mindif = dif;
	      col = i;
	    }
	}
      return pb->palette[col].pixel;
    }
    case GrayScale:
    case StaticGray:
      return (((r * 77) + (g * 151) + (b * 28)) >> (16 - pb->depth));
#if 0
      if ((r+g+b) == 765 ) return 0; 	/* HACK */
      return (1 << pb->depth) - ((( ((r * 54) + (g * 183) + (b * 19)) / 256) * 0xffff )/ (1 << pb->depth)); /* TODO should be oxffffffff ?? */
#endif
    default:
      break;
    }
  return 0;
}

unsigned long
mb_pixbuf_lookup_x_pixel(MBPixbuf *pb, int r, int g, int b, int a)
{
  return mb_pixbuf_get_pixel(pb, r, g, b, a);
}


MBPixbuf *
mb_pixbuf_new(Display *dpy, int scr)
{
  return mb_pixbuf_new_extended(dpy,
				scr,
				DefaultVisual(dpy, scr),
				DefaultDepth(dpy, scr));
}


void
mb_pixbuf_destroy(MBPixbuf *pb)
{
  /* XXX Probably needs to free more here */
  XFreeGC(pb->dpy, pb->gc);
  free(pb);
}

MBPixbuf *
mb_pixbuf_new_extended(Display *dpy,
		       int      scr,
		       Visual  *vis,
		       int      depth)
{
  XGCValues gcv;
  unsigned long rmsk, gmsk, bmsk;
  MBPixbuf *pb = malloc(sizeof(MBPixbuf));

  pb->dpy = dpy;
  pb->scr = scr;

  pb->root  = RootWindow(dpy, scr);
  pb->depth = depth;
  pb->vis   = vis;

  pb->palette = NULL;

  rmsk = pb->vis->red_mask;
  gmsk = pb->vis->green_mask;
  bmsk = pb->vis->blue_mask;

  if (depth == 32 && pb->vis->class == TrueColor)
    pb->byte_order = BYTE_ORD_32_ARGB;
  else if ((rmsk > gmsk) && (gmsk > bmsk))
    pb->byte_order = BYTE_ORD_24_RGB;
  else if ((rmsk > bmsk) && (bmsk > gmsk))
    pb->byte_order = BYTE_ORD_24_RBG;
  else if ((bmsk > rmsk) && (rmsk > gmsk))
    pb->byte_order = BYTE_ORD_24_BRG;
  else if ((bmsk > gmsk) && (gmsk > rmsk))
    pb->byte_order = BYTE_ORD_24_BGR;
  else if ((gmsk > rmsk) && (rmsk > bmsk))
    pb->byte_order = BYTE_ORD_24_GRB;
  else if ((gmsk > bmsk) && (bmsk > rmsk))
    pb->byte_order = BYTE_ORD_24_GBR;
  else
    pb->byte_order = 0;

  pb->internal_bytespp = 3;

  if ((pb->depth < 24 && !getenv("MBPIXBUF_FORCE_32BPP_INTERNAL"))
      || getenv("MBPIXBUF_FORCE_16BPP_INTERNAL"))
    pb->internal_bytespp = 2;

  if ((pb->depth <= 8))
    {
      XWindowAttributes   xwa;
      if (XGetWindowAttributes(dpy, pb->root, &xwa))
	{
	  if (xwa.colormap)
	    pb->root_cmap = xwa.colormap;
	  else
	    pb->root_cmap = DefaultColormap(dpy, scr);

	}
      else
	pb->root_cmap = DefaultColormap(dpy, scr);
      pb->num_of_cols = _paletteAlloc(pb);
    }

  /* TODO: No exposes ? */
  gcv.foreground = BlackPixel(dpy, scr);
  gcv.background = WhitePixel(dpy, scr);

  pb->gc = XCreateGC( dpy, pb->root, GCForeground | GCBackground, &gcv);

  if (!XShmQueryExtension(pb->dpy) || getenv("MBPIXBUF_NO_SHM"))
    {
      fprintf(stderr, "mbpixbuf: no shared memory extension\n");
      pb->have_shm = False;
    }
  else 		                /* Really check we have shm */
    {				/* Urg, not nicer way todo this ? */
      XShmSegmentInfo shminfo;

      pb->have_shm = True;

      shminfo.shmid=shmget(IPC_PRIVATE, 1, IPC_CREAT|0777);
      shminfo.shmaddr=shmat(shminfo.shmid,0,0);
      shminfo.readOnly=True;

      _mbpb_trap_errors();

      XShmAttach(pb->dpy, &shminfo);
      XSync(pb->dpy, False);

      if (_mbpb_untrap_errors())
	{
	  fprintf(stderr, "mbpixbuf: unable to use XShm. DISPLAY remote?\n");
	  pb->have_shm = False;
	}
      else XShmDetach(pb->dpy, &shminfo);

      shmdt(shminfo.shmaddr);
      shmctl(shminfo.shmid, IPC_RMID, 0);
    }
  return pb;
}

MBPixbufImage *
mb_pixbuf_img_new(MBPixbuf *pb, int w, int h)
{
  MBPixbufImage *img;

  img = malloc(sizeof(MBPixbufImage));
  img->width = w;
  img->height = h;

  img->rgba = malloc(sizeof(unsigned char)*((w*h*(pb->internal_bytespp+1))));
  memset(img->rgba, 0, sizeof(unsigned char)*((w*h*(pb->internal_bytespp+1))));

  img->ximg = NULL;
  img->has_alpha = 1;
  img->internal_bytespp = pb->internal_bytespp;

  return img;
}

MBPixbufImage *
mb_pixbuf_img_rgba_new(MBPixbuf *pb, int w, int h)
{
  return mb_pixbuf_img_new(pb, w, h);
}


MBPixbufImage *
mb_pixbuf_img_rgb_new(MBPixbuf *pixbuf, int width, int height)
{

 MBPixbufImage *img;

 img = malloc(sizeof(MBPixbufImage));
 img->width = width;
 img->height = height;

 img->rgba
   = malloc(sizeof(unsigned char)*((width*height*pixbuf->internal_bytespp)));
 memset(img->rgba, 0,
	sizeof(unsigned char)*((width*height*pixbuf->internal_bytespp)));

 img->ximg = NULL;
 img->has_alpha = 0;
 img->internal_bytespp = pixbuf->internal_bytespp;

 return img;

}

/* ARGB Data */

MBPixbufImage *
mb_pixbuf_img_new_from_int_data(MBPixbuf            *pixbuf,
				const int           *data,
				int                  width,
				int                  height)
{
  MBPixbufImage *img;
  int            i=0,x,y;

  img = mb_pixbuf_img_rgba_new(pixbuf, width, height);

  if (pixbuf->internal_bytespp == 3)
    {
      unsigned char *p = img->rgba;

      for (y=0; y<height; y++)
	for (x=0; x<width; x++)
	  {
	    *p++ = (data[i] >> 16) & 0xff;
	    *p++ = (data[i] >> 8) & 0xff;
	    *p++ = data[i] & 0xff;
	    *p++ = data[i] >> 24;
	    i++;
	  }
    }
  else
    {
      unsigned char *p = img->rgba, r,g,b,a;

      for (y=0; y<height; y++)
	for (x=0; x<width; x++)
	  {
	    r = ((data[i] >> 16) & 0xff);
	    g = ((data[i] >> 8) & 0xff);
	    b = (data[i] & 0xff);
	    a = (data[i] >> 24);

	    internal_rgb_to_16bpp_pixel(r,g,b,p);
	    internal_16bpp_pixel_next(p);
	    *p++ = a;

	    i++;
	  }
    }

  return img;
}

MBPixbufImage *
mb_pixbuf_img_new_from_data(MBPixbuf            *pixbuf,
			    const unsigned char *data,
			    int                  width,
			    int                  height,
			    Bool                 has_alpha)
{
  MBPixbufImage *img;

  if (has_alpha)
    img = mb_pixbuf_img_rgba_new(pixbuf, width, height);
  else
    img = mb_pixbuf_img_rgb_new(pixbuf, width, height);

  if (pixbuf->internal_bytespp == 3)
    {
      memcpy(img->rgba, data, width*height*(3+has_alpha));
    }
  else
    {
      /* Data is expected as 24/32 RGBA. but internally were
       * 16/24 so we need to scale the data down.
      */
      int x,y;
      unsigned char r,g,b, *p16 = img->rgba;
      const unsigned char *p24 = data;

      for (x=0; x<img->width; x++)
	for (y=0; y<img->height; y++)
      {
	r = *p24++; g = *p24++; b = *p24++;
	internal_rgb_to_16bpp_pixel(r,g,b,p16);
	internal_16bpp_pixel_next(p16);

	if (has_alpha)
	  *p16++ = *p24++;
      }
    }

  return img;
}

MBPixbufImage *
mb_pixbuf_img_new_from_drawable (MBPixbuf *pb,
				 Drawable  drw,
				 Drawable  msk,
				 int       sx,
				 int       sy,
				 int       sw,
				 int       sh  )
{
  return mb_pixbuf_img_new_from_x_drawable(pb, drw, msk, sx, sy, sw, sh, 0);
}

MBPixbufImage *
mb_pixbuf_img_new_from_x_drawable (MBPixbuf *pb,
				   Drawable  drw,
				   Drawable  msk,
				   int       sx,
				   int       sy,
				   int       sw,
				   int       sh,
				   Bool      want_alpha)
{
  int i,x,y,br,bg,bb,mg,mb,mr,lr,lg,lb;
  unsigned long xpixel;
  unsigned char *p;
  MBPixbufImage *img;

  XImage *ximg, *xmskimg = NULL;
  int num_of_cols = 1 << pb->depth;

  Window chld;
  int          rx;
  unsigned int rw, rh, rb, rdepth;

  XShmSegmentInfo shminfo;

  /* XXX should probably tray an X error here. */
  XGetGeometry(pb->dpy, (Window)drw, &chld, &rx, &rx,
	       (unsigned int *)&rw, (unsigned int *)&rh,
	       (unsigned int *)&rb, (unsigned int *)&rdepth);

  if (rdepth != pb->depth) return NULL;

  if ( (sx + sw) > rw || (sy + sh) > rh )
    {
      /* area wanted is bigger than pixmap - should probably trim it down */
      return NULL;
    }

  XGrabServer(pb->dpy);

  if (1) /* (!pb->have_shm) BROKE */
    {
      ximg = XGetImage(pb->dpy, drw, sx, sy, sw, sh, -1, ZPixmap);
    }
  else
    {
      ximg = XShmCreateImage(pb->dpy, pb->vis, pb->depth,
			      ZPixmap, NULL, &shminfo,
			      sw, sh );
      XSync(pb->dpy, False);

      shminfo.shmid=shmget(IPC_PRIVATE,
			   ximg->bytes_per_line*ximg->height,
			   IPC_CREAT|0777);
      shminfo.shmaddr=img->ximg->data=shmat(shminfo.shmid,0,0);
      shminfo.readOnly=True;

      XShmAttach(pb->dpy, &shminfo);

      XSync(pb->dpy, False);

      if (!XShmGetImage(pb->dpy, drw, ximg, sx, sy, -1))
	{
	  return NULL;
	}

      XSync(pb->dpy, False);

    }

  if (msk != None)
    xmskimg = XGetImage(pb->dpy, msk, sx, sy, sw, sh, -1, ZPixmap);

  XUngrabServer(pb->dpy);
  XFlush(pb->dpy);

  if (ximg == NULL) return NULL;

  if (msk || want_alpha)
    img = mb_pixbuf_img_rgba_new(pb, sw, sh);
  else
    img = mb_pixbuf_img_rgb_new(pb, sw, sh);

  p = img->rgba;

  if (pb->depth > 8)
    {
      switch (pb->depth) {
      case 15:
        br = 7;
        bg = 2;
        bb = 3;
        mr = mg = mb = 0xf8;
	lr = lg = lb = 0;
        break;
      case 16:
	br = 8;
	bg = 3;
	lb = 3;
	bb = lr = lg = 0;
        mr = mb = 0xf8;
        mg = 0xfc;
        break;
      case 24:
      case 32:
        br = 16;
        bg = 8;
        bb = 0;
	lr = lg = lb = 0;
        mr = mg = mb = 0xff;
        break;
      default:
        return NULL;
      }

      if (pb->internal_bytespp == 2)
	{
	  for (y = 0; y < sh; y++)
	    for (x = 0; x < sw; x++)
	      {
		xpixel = XGetPixel(ximg, x, y);
		internal_rgb_to_16bpp_pixel((((xpixel >> br) << lr) & mr),
					    (((xpixel >> bg) << lg) & mg),
					    (((xpixel >> bb) << lb) & mb), p);

		internal_16bpp_pixel_next(p);

		if (msk)
		  {
		    if (xmskimg && XGetPixel(xmskimg, x, y))
		      {
			*p++ = 255;
		      }
		    else *p++ = 0;
		  }
		else if (want_alpha) p++;
	      }
	}
      else
	{
	  for (y = 0; y < sh; y++)
	    for (x = 0; x < sw; x++)
	      {
		xpixel = XGetPixel(ximg, x, y);
		*p++ = (((xpixel >> br) << lr) & mr);      /* r */
		*p++ = (((xpixel >> bg) << lg) & mg);      /* g */
		*p++ = (((xpixel >> bb) << lb) & mb);      /* b */
		if (msk)
		  {
		    if (xmskimg && XGetPixel(xmskimg, x, y))
		      {
			*p++ = 255;
		      }
		    else *p++ = 0;
		  }
		else if (want_alpha) p++;
	      }
	}
    }
  else
    {
      XColor cols[256];
      MBPixbufColor mbcols[256];

      for (i = 0; i < num_of_cols; i++) {
	cols[i].pixel = i;
	cols[i].flags = DoRed | DoGreen | DoBlue;
      }
      XQueryColors(pb->dpy, pb->root_cmap, cols, num_of_cols);
      for (i = 0; i < num_of_cols; i++) {
	mbcols[i].r = cols[i].red >> 8;
	mbcols[i].g = cols[i].green >> 8;
	mbcols[i].b = cols[i].blue >> 8;
	mbcols[i].pixel = cols[i].pixel;
      }
      for (x = 0; x < sw; x++)
	for (y = 0; y < sh; y++)
	  {
	    xpixel = XGetPixel(ximg, x, y);
	    *p++ = mbcols[xpixel & 0xff].r;
	    *p++ = mbcols[xpixel & 0xff].g;
	    *p++ = mbcols[xpixel & 0xff].b;
	    if (msk)
	      {
		if (xmskimg && XGetPixel(xmskimg, x, y))
		  {
		    *p++ = 255;
		  }
		else *p++ = 0;
	      }
	    else if (want_alpha) p++;
	  }
    }

  if (0) /* (pb->have_shm) BROKE */
    {
      XShmDetach(pb->dpy, &shminfo);
      XDestroyImage (ximg);
      shmdt(shminfo.shmaddr);
      shmctl(shminfo.shmid, IPC_RMID, 0);
    }
  else XDestroyImage (ximg);

  ximg = NULL;

  return img;
}

MBPixbufImage *
mb_pixbuf_img_clone(MBPixbuf *pb, MBPixbufImage *img)
{
  MBPixbufImage *img_new;

  if (img->has_alpha)
    img_new = mb_pixbuf_img_rgba_new(pb, img->width, img->height);
  else
    img_new = mb_pixbuf_img_rgb_new(pb, img->width, img->height);

  memcpy(img_new->rgba, img->rgba,
	 sizeof(unsigned char)*((img->width * img->height * (pb->internal_bytespp + img->has_alpha))));
  return img_new;
}

void
mb_pixbuf_img_free(MBPixbuf *pb, MBPixbufImage *img)
{
  if (img->rgba) free(img->rgba);
  free(img);
}

static void
_mb_convert_24bpp_to_16bpp(MBPixbuf *pb, MBPixbufImage **img)
{
  MBPixbufImage *img16, *img24 = *img;
  int            x,y;
  unsigned char  r,g,b, *p16 = NULL, *p24 = NULL;

  if (img24->has_alpha)
    img16 = mb_pixbuf_img_rgba_new(pb, img24->width, img24->height);
  else
    img16 = mb_pixbuf_img_rgb_new(pb, img24->width, img24->height);

  p16 = img16->rgba; p24 = img24->rgba;

  for (x=0; x<img24->width; x++)
    for (y=0; y<img24->height; y++)
      {
	r = *p24++;
	g = *p24++;
	b = *p24++;

	internal_rgb_to_16bpp_pixel(r,g,b,p16);
	internal_16bpp_pixel_next(p16);

	if (img24->has_alpha)
	  *p16++ = *p24++;
      }

  mb_pixbuf_img_free(pb, img24);

  *img = img16;
}

MBPixbufImage *
mb_pixbuf_img_new_from_file(MBPixbuf *pb, const char *filename)
{
  MBPixbufImage *img;

  img = malloc(sizeof(MBPixbufImage));

#ifdef USE_PNG
  if (!strcasecmp(&filename[strlen(filename)-4], ".png"))
    img->rgba = _load_png_file( filename, &img->width,
				&img->height, &img->has_alpha );
  else
#endif
#ifdef USE_JPG
  if (!strcasecmp(&filename[strlen(filename)-4], ".jpg")
      || !strcasecmp(&filename[strlen(filename)-5], ".jpeg"))
    img->rgba = _load_jpg_file( filename, &img->width,
				&img->height, &img->has_alpha );
  else
#endif
if (!strcasecmp(&filename[strlen(filename)-4], ".xpm"))
    img->rgba = _load_xpm_file( pb, filename, &img->width,
				&img->height, &img->has_alpha );
  else img->rgba = NULL;

  if (img->rgba == NULL)
    {
      /* Load failed */
      free(img);
      return NULL;
    }

  if (pb->internal_bytespp == 2)
    {
      /* Need to convert the data down, always comes as 24 rgb :/ */
      _mb_convert_24bpp_to_16bpp(pb, &img);
    }

  img->ximg = NULL;

  return img;
}

void
mb_pixbuf_img_fill(MBPixbuf *pb,
		   MBPixbufImage *img,
		   int r,
		   int g,
		   int b,
		   int a)
{
  unsigned char *p = img->rgba;
  int x,y;

  if (pb->internal_bytespp == 2)
    {
      for(y=0; y<img->height; y++)
	for(x=0; x<img->width; x++)
	  {
	    internal_rgb_to_16bpp_pixel(r,g,b,p);
	    internal_16bpp_pixel_next(p);
	    if (img->has_alpha)
	      { *p = a; *p++; }
	  }
    }
  else
    {
      for(y=0; y<img->height; y++)
	for(x=0; x<img->width; x++)
	  {
	    *p++ = r;
	    *p++ = g;
	    *p++ = b;
	    if (img->has_alpha) *p++ = a;
	  }
    }
}

void
mb_pixbuf_img_composite(MBPixbuf *pb, MBPixbufImage *dest,
			MBPixbufImage *src, int dx, int dy)
{
  /* XXX depreictaed, should really now use copy_composite */
  int x, y, r, g, b, a;
  unsigned char *sp, *dp;
  int dbc = 0;

  if (src->has_alpha == False)
    return mb_pixbuf_img_copy(pb, dest, src, 0, 0,
			      src->width, src->height, dx, dy);
  sp = src->rgba;
  dp = dest->rgba;

  dbc = (pb->internal_bytespp + dest->has_alpha);

  dp += ((dest->width*dbc)*dy) + (dx*dbc);

  if (pb->internal_bytespp == 2)
    {
      for(y=0; y<src->height; y++)
	{
	  for(x=0; x<src->width; x++)
	    {
	      unsigned char dr,dg,db;

	      /* TODO, This needs optimising */

	      internal_16bpp_pixel_to_rgb(sp,r,g,b);
	      internal_16bpp_pixel_next(sp);
	      a = *sp++;

	      internal_16bpp_pixel_to_rgb(dp,dr,dg,db);
	      alpha_composite(dr, r, a, dr);
	      alpha_composite(dg, g, a, dg);
	      alpha_composite(db, b, a, db);
	      internal_rgb_to_16bpp_pixel(dr,dg,db,dp)
	      internal_16bpp_pixel_next(dp);

	      dp += dest->has_alpha;
	    }
	  dp += (dest->width-src->width)*dbc;
	}


    }
  else
    {
      for(y=0; y<src->height; y++)
	{
	  for(x=0; x<src->width; x++)
	    {
	      r = *sp++;
	      g = *sp++;
	      b = *sp++;
	      a = *sp++;

	      alpha_composite(*dp, r, a, *dp);
	      dp++;
	      alpha_composite(*dp, g, a, *dp);
	      dp++;
	      alpha_composite(*dp, b, a, *dp);
	      dp++;
	      dp += dest->has_alpha;
	    }
	  dp += (dest->width-src->width)*dbc;
	}
    }
}

void
mb_pixbuf_img_copy_composite_with_alpha (MBPixbuf      *pb,
					 MBPixbufImage *dest,
					 MBPixbufImage *src,
					 int sx, int sy,
					 int sw, int sh,
					 int dx, int dy,
					 int alpha_level )
{
  int x, y, r, g, b, a;
  unsigned char *sp, *dp;
  int dbc = 0;

  if (!src->has_alpha)
    return mb_pixbuf_img_copy(pb, dest, src, sx, sy, sw, sh, dx, dy);

  sp = src->rgba;
  dp = dest->rgba;

  dbc = (pb->internal_bytespp + dest->has_alpha);

  dp += ((dest->width*dbc)*dy) + (dx*dbc);
  sp += ((src->width*(pb->internal_bytespp+1))*sy)  + (sx*(pb->internal_bytespp+1));

  if (pb->internal_bytespp == 2)
    {
      for(y=0; y<sh; y++)
	{
	  for(x=0; x < sw; x++)
	    {
	      unsigned char dr,dg,db;

	      /* TODO, This needs optimising */

	      internal_16bpp_pixel_to_rgb(sp,r,g,b);
	      internal_16bpp_pixel_next(sp);
	      a = *sp; sp++;

	      if (alpha_level)
		{
		  a += alpha_level;
		  if (a < 0) a = 0;
		  if (a > 255) a = 255;
		}

	      internal_16bpp_pixel_to_rgb(dp,dr,dg,db);
	      alpha_composite(dr, r, a, dr);
	      alpha_composite(dg, g, a, dg);
	      alpha_composite(db, b, a, db);
	      internal_rgb_to_16bpp_pixel(dr,dg,db,dp);
	      internal_16bpp_pixel_next(dp);

	      if (dest->has_alpha)
		*dp++ =  a;
	    }
	  dp += (dest->width-sw)*dbc;
	  sp += (src->width-sw)*3;
	}
    }
  else
    {
      for(y=0; y<sh; y++)
	{
	  for(x=0; x < sw; x++)
	    {
	      r = *sp++;
	      g = *sp++;
	      b = *sp++;
	      a = *sp++;

	      if (alpha_level)
		{
		  a += alpha_level;
		  if (a < 0) a = 0;
		  if (a > 255) a = 255;
		}

	      alpha_composite(*dp, r, a, *dp);
	      dp++;
	      alpha_composite(*dp, g, a, *dp);
	      dp++;
	      alpha_composite(*dp, b, a, *dp);
	      dp++;
	      if (dest->has_alpha) *dp++ = a;
	    }
	  dp += (dest->width-sw)*dbc;
	  sp += (src->width-sw)*4;
	}
    }
}


void
mb_pixbuf_img_copy_composite(MBPixbuf *pb, MBPixbufImage *dest,
			     MBPixbufImage *src, int sx, int sy,
			     int sw, int sh, int dx, int dy)
{
  mb_pixbuf_img_copy_composite_with_alpha(pb, dest,src, sx, sy, sw, sh, dx, dy, 0);
}


void
mb_pixbuf_img_copy(MBPixbuf *pb, MBPixbufImage *dest,
		   MBPixbufImage *src, int sx, int sy, int sw, int sh,
		   int dx, int dy)
{
  int x, y, dbc, sbc;
  unsigned char *sp, *dp;

  sp = src->rgba;
  dp = dest->rgba;
  dbc = (pb->internal_bytespp + dest->has_alpha);
  sbc = (pb->internal_bytespp + src->has_alpha);

  dp += ((dest->width*dbc)*dy) + (dx*dbc);
  sp += ((src->width*sbc)*sy)  + (sx*sbc);

  for(y=0; y<sh; y++)
    {
      for(x=0; x < sw; x++)
	{
	  *dp++ = *sp++;
	  *dp++ = *sp++;
	  if (pb->internal_bytespp > 2)
	    *dp++ = *sp++;
	  /* XXX below to optimise */
	  if (dest->has_alpha && src->has_alpha)
	    *dp++ = *sp++;
	  else
	    {
	      if (dest->has_alpha) *dp++ = 0xff;
	      // dp += dest->has_alpha;
	      sp += src->has_alpha;
	    }
	}
      dp += (dest->width-sw)*dbc;
      sp += (src->width-sw)*sbc;
    }
}

MBPixbufImage *
mb_pixbuf_img_scale_down(MBPixbuf *pb, MBPixbufImage *img,
			 int new_width, int new_height)
{
  MBPixbufImage *img_scaled;
  unsigned char *dest, *src, *srcy;
  int *xsample, *ysample;
  int bytes_per_line, i, x, y,  r, g, b, a, nb_samples, xrange, yrange, rx, ry;

  if ( new_width > img->width || new_height > img->height)
    return NULL;

  if (img->has_alpha)
    {
      img_scaled = mb_pixbuf_img_rgba_new(pb, new_width, new_height);
      bytes_per_line = (img->width * (pb->internal_bytespp+1));
    }
  else
    {
      img_scaled = mb_pixbuf_img_rgb_new(pb, new_width, new_height);
      bytes_per_line = (img->width * pb->internal_bytespp);
    }

  xsample = malloc( (new_width+1) * sizeof(int));
  ysample = malloc( (new_height+1) * sizeof(int));

  for ( i = 0; i <= new_width; i++ )
    xsample[i] = i*img->width/new_width;
  for ( i = 0; i <= new_height; i++ )
    ysample[i] = i*img->height/new_height * img->width;

  dest = img_scaled->rgba;

  /* scan output image */
  for ( y = 0; y < new_height; y++ )
    {
      yrange = ( ysample[y+1] - ysample[y] )/img->width;
      for ( x = 0; x < new_width; x++)
	{
	  xrange = xsample[x+1] - xsample[x];
	  srcy = img->rgba + (( ysample[y] + xsample[x] )
			      * ( (img->has_alpha) ?  (pb->internal_bytespp+1) : pb->internal_bytespp ) );

      /* average R,G,B,A values on sub-rectangle of source image */
	  nb_samples = xrange * yrange;
	  if ( nb_samples > 1 )
	    {
	      r = 0;
	      g = 0;
	      b = 0;
	      a = 0;
	      for ( ry = 0; ry < yrange; ry++ )
		{
		  src = srcy;
		  for ( rx = 0; rx < xrange; rx++ )
		    {
		      /* average R,G,B,A values */
		      if (pb->internal_bytespp == 2)
			{
			  unsigned char rr,gg,bb;
			  internal_16bpp_pixel_to_rgb(src,rr,gg,bb);
			  r += rr; g += gg; b += bb;
			  internal_16bpp_pixel_next(src);
			}
		      else
			{
			  r += *src++;
			  g += *src++;
			  b += *src++;
			}
		      if (img->has_alpha) a += *src++;
		    }
		  srcy += bytes_per_line;
		}

	      if (pb->internal_bytespp == 2)
		{
		  unsigned char rr,gg,bb;
		  rr = (r/nb_samples);
		  gg = (g/nb_samples);
		  bb = (b/nb_samples);

		  internal_rgb_to_16bpp_pixel(rr, gg, bb, dest);
		  internal_16bpp_pixel_next(dest);
		}
	      else
		{
		  *dest++ = r/nb_samples;
		  *dest++ = g/nb_samples;
		  *dest++ = b/nb_samples;
		}
	      if (img_scaled->has_alpha) *dest++ = a/nb_samples;
	    }
	  else
	    {
	      int i;
	      for (i=0; i<(pb->internal_bytespp + img_scaled->has_alpha); i++)
		*dest++ = *srcy++;
	    }
	}
    }

  /* cleanup */
  free( xsample );
  free( ysample );

  return img_scaled;
}

MBPixbufImage *
mb_pixbuf_img_scale_up(MBPixbuf *pb, MBPixbufImage *img,
		       int new_width, int new_height)
{
  MBPixbufImage *img_scaled;
  unsigned char *dest, *src;
  int x, y, xx, yy, bytes_per_line;

  if ( new_width < img->width || new_height < img->height)
    return NULL;

  if (img->has_alpha)
    {
      img_scaled = mb_pixbuf_img_rgba_new(pb, new_width, new_height);
      bytes_per_line = (img->width * (pb->internal_bytespp+1));
    }
  else
    {
      img_scaled = mb_pixbuf_img_rgb_new(pb, new_width, new_height);
      bytes_per_line = (img->width * pb->internal_bytespp);
    }


  dest = img_scaled->rgba;

  for (y = 0; y < new_height; y++)
    {
      yy = (y * img->height) / new_height;
      for (x = 0; x < new_width; x++)
      {
	 xx = (x * img->width) / new_width;
	 src = img->rgba + ((yy * bytes_per_line))
	   + ( (img->has_alpha) ? (xx * (pb->internal_bytespp+1)) : (xx * pb->internal_bytespp) );
	 *dest++ = *src++;
	 *dest++ = *src++;
	 if (pb->internal_bytespp > 2)
	   *dest++ = *src++;

	 if (img->has_alpha)
	   *dest++ = *src++;
      }
   }

  return img_scaled;
}

MBPixbufImage *
mb_pixbuf_img_scale(MBPixbuf *pb, MBPixbufImage *img,
		    int new_width, int new_height)
{
  if (new_width >= img->width && new_height >= img->height)
    return mb_pixbuf_img_scale_up(pb, img, new_width, new_height);

  if (new_width <= img->width && new_height <= img->height)
    return mb_pixbuf_img_scale_down(pb, img, new_width, new_height);

  /* TODO: all scale functions should check for a dimention change
           being zero and act accordingly - ie faster.
  */
  if (new_width >= img->width && new_height <= img->height)
    {
      MBPixbufImage *tmp=NULL, *tmp2 = NULL;
      tmp = mb_pixbuf_img_scale_up(pb, img, new_width, img->height);

      tmp2 = mb_pixbuf_img_scale_down(pb, tmp, new_width, new_height);
      mb_pixbuf_img_free(pb, tmp);
      return tmp2;
    }

  if (new_width <= img->width && new_height >= img->height)
    {
      MBPixbufImage *tmp, *tmp2;
      tmp = mb_pixbuf_img_scale_down(pb, img, new_width, img->height);

      tmp2 = mb_pixbuf_img_scale_up(pb, tmp, new_width, new_height);
      mb_pixbuf_img_free(pb, tmp);
      return tmp2;
    }

  return NULL;
}

void
mb_pixbuf_img_render_to_drawable(MBPixbuf    *pb,
				 MBPixbufImage *img,
				 Drawable     drw,
				 int drw_x,
				 int drw_y)
{
  mb_pixbuf_img_render_to_drawable_with_gc(pb, img, drw, drw_x, drw_y, pb->gc);
}


void
mb_pixbuf_img_render_to_drawable_with_gc(MBPixbuf    *pb,
					 MBPixbufImage *img,
					 Drawable     drw,
					 int drw_x,
					 int drw_y,
					 GC gc)
{
      int bitmap_pad;
      unsigned char *p;
      unsigned long pixel;
      int x,y;
      int a, r, g, b;

      XShmSegmentInfo shminfo;
      Bool shm_success = False;

      if (pb->have_shm)
	{
	  img->ximg = XShmCreateImage(pb->dpy, pb->vis, pb->depth,
				      ZPixmap, NULL, &shminfo,
				      img->width, img->height );

	  shminfo.shmid=shmget(IPC_PRIVATE,
			       img->ximg->bytes_per_line*img->ximg->height,
			       IPC_CREAT|0777);
	  shminfo.shmaddr = img->ximg->data = shmat(shminfo.shmid,0,0);

	  if (img->ximg->data == (char *)-1)
	    {
	      fprintf(stderr, "mbpixbuf: SHM can't attach SHM Segment for Shared XImage, falling back to XImages\n");
	      XDestroyImage(img->ximg);
	      shmctl(shminfo.shmid, IPC_RMID, 0);
	    }
	  else
	    {
	      shminfo.readOnly=True;
	      XShmAttach(pb->dpy, &shminfo);
	      shm_success = True;
	    }
	}

      if (!shm_success)
	{
	  bitmap_pad = ( pb->depth > 16 )? 32 : (( pb->depth > 8 )? 16 : 8 );

	  img->ximg = XCreateImage( pb->dpy, pb->vis, pb->depth,
				    ZPixmap, 0, 0,
				    img->width, img->height, bitmap_pad, 0);

	  img->ximg->data = malloc( img->ximg->bytes_per_line*img->height );
	}

      p = img->rgba;

      if (pb->internal_bytespp == 2)
	{
	  for(y=0; y<img->height; y++)
	    for(x=0; x<img->width; x++)
	      {
		/* Below is potentially dangerous.
		 */
		pixel =  ( *p | (*(p+1) << 8));

		p +=  ((img->has_alpha) ?  3 : 2);

		XPutPixel(img->ximg, x, y, pixel);
	      }
	}
      else
	{
	  for(y=0; y<img->height; y++)
	    {
	      for(x=0; x<img->width; x++)
		{
		  r = ( *p++ );
		  g = ( *p++ );
		  b = ( *p++ );
		  a = ((img->has_alpha) ?  *p++ : 0xff);

		  pixel = mb_pixbuf_get_pixel(pb, r, g, b, a);
		  XPutPixel(img->ximg, x, y, pixel);
		}
	    }
	}

      if (!shm_success)
	{
	  XPutImage( pb->dpy, drw, gc, img->ximg, 0, 0,
		     drw_x, drw_y, img->width, img->height);
	  XDestroyImage (img->ximg);
	}
      else
	{
	  XShmPutImage(pb->dpy, drw, gc, img->ximg, 0, 0,
		       drw_x, drw_y, img->width, img->height, False);

	  XSync(pb->dpy, False);
	  XShmDetach(pb->dpy, &shminfo);
	  XDestroyImage (img->ximg);
	  shmdt(shminfo.shmaddr);
	  shmctl(shminfo.shmid, IPC_RMID, 0);
	}

      img->ximg = NULL;		/* Safety On */
}

void
mb_pixbuf_img_render_to_mask(MBPixbuf    *pb,
			     MBPixbufImage *img,
			     Drawable  mask,
			     int drw_x,
			     int drw_y)
{
      unsigned char *p;
      int x,y;
      GC gc1;
      XShmSegmentInfo shminfo;
      Bool shm_success = False;

      if (!img->has_alpha) return;

      gc1 = XCreateGC( pb->dpy, mask, 0, 0 );
      XSetForeground(pb->dpy, gc1, WhitePixel( pb->dpy, pb->scr ));

      if (pb->have_shm)
	{
	  img->ximg = XShmCreateImage(pb->dpy, pb->vis, 1,
				      XYPixmap, NULL, &shminfo,
				      img->width, img->height );

	  shminfo.shmid=shmget(IPC_PRIVATE,
			       img->ximg->bytes_per_line*img->ximg->height,
			       IPC_CREAT|0777);
	  shminfo.shmaddr=img->ximg->data=shmat(shminfo.shmid,0,0);

	  if (img->ximg->data == (char *)-1)
	    {
	      if (mb_want_warnings())
		fprintf(stderr, "mbpixbuf: SHM can't attach SHM Segment for Shared XImage, falling back to XImages\n");
	      XDestroyImage(img->ximg);
	      shmctl(shminfo.shmid, IPC_RMID, 0);
	    }
	  else
	    {
	      shminfo.readOnly=True;
	      XShmAttach(pb->dpy, &shminfo);
	      shm_success = True;
	    }
	}

      if (!shm_success)
	{
	  img->ximg = XCreateImage( pb->dpy, pb->vis, 1, XYPixmap,
				    0, 0, img->width, img->height, 8, 0);

	  img->ximg->data = malloc( img->ximg->bytes_per_line*img->height );
	}

      p = img->rgba;

      for(y=0; y<img->height; y++)
	for(x=0; x<img->width; x++)
	    {
	      p += pb->internal_bytespp;
	      XPutPixel(img->ximg, x, y, (*p < 127) ? 0 : 1);
	      p++;
 	    }

      if (!shm_success)
	{
	  XPutImage( pb->dpy, mask, gc1, img->ximg, 0, 0,
		     drw_x, drw_y, img->width, img->height);
	  XDestroyImage (img->ximg);
	}
      else
	{
	  XShmPutImage(pb->dpy, mask, gc1, img->ximg, 0, 0,
		       drw_x, drw_y, img->width, img->height, False);
	  XSync(pb->dpy, False);
	  XShmDetach(pb->dpy, &shminfo);
	  XDestroyImage (img->ximg);
	  shmdt(shminfo.shmaddr);
	  shmctl(shminfo.shmid, IPC_RMID, 0);
	}

      XFreeGC( pb->dpy, gc1 );
      img->ximg = NULL;		/* Safety On */
}

unsigned char *
mb_pixbuf_img_data (MBPixbuf      *pixbuf,
		    MBPixbufImage *image)
{
  return image->rgba;
}

void
mb_pixbuf_img_get_pixel (MBPixbuf      *pixbuf,
			 MBPixbufImage *img,
			 int            x,
			 int            y,
			 unsigned char *r,
			 unsigned char *g,
			 unsigned char *b,
			 unsigned char *a
			 )
{
  int idx;

  idx = pixbuf->internal_bytespp + img->has_alpha;

  if (pixbuf->internal_bytespp == 2)
    {
      int offset = ( (y * img->width * idx) + ( x * idx ) );
      internal_16bpp_pixel_to_rgb(img->rgba+offset, *r, *g, *b);

      if (img->has_alpha)
	*a = *(img->rgba+offset+2);
      else
	*a = 255;
    }
  else
    {
      *r = img->rgba[(((y)*img->width*idx)+((x)*idx))];
      *g = img->rgba[(((y)*img->width*idx)+((x)*idx))+1];
      *b = img->rgba[(((y)*img->width*idx)+((x)*idx))+2];

      if (img->has_alpha)
	*a = img->rgba[(((y)*img->width*idx)+((x)*idx))+3];
      else
	*a = 255;
    }
}


void
mb_pixbuf_img_plot_pixel (MBPixbuf      *pb,
			  MBPixbufImage *img,
			  int            x,
			  int            y,
			  unsigned char  r,
			  unsigned char  g,
			  unsigned char  b)
{
  int idx;
  if (x >= img->width || y >= img->height) return;

  idx = pb->internal_bytespp + img->has_alpha;

  if (pb->internal_bytespp == 2)
    {
      int offset = (((y)*img->width*idx)+((x)*idx));
      internal_rgb_to_16bpp_pixel(r,g,b, (img->rgba+offset));
    }
  else
    {
      img->rgba[(((y)*img->width*idx)+((x)*idx))]   = r;
      img->rgba[(((y)*img->width*idx)+((x)*idx))+1] = g;
      img->rgba[(((y)*img->width*idx)+((x)*idx))+2] = b;
    }
}

void
mb_pixbuf_img_plot_pixel_with_alpha (MBPixbuf      *pb,
				     MBPixbufImage *img,
				     int            x,
				     int            y,
				     unsigned char  r,
				     unsigned char  g,
				     unsigned char  b,
				     unsigned char  a)
{
  int idx = (((y)*img->width*(pb->internal_bytespp+1))+((x)*(pb->internal_bytespp+1)));

  if (!img->has_alpha)
    {
      mb_pixbuf_img_plot_pixel (pb, img, x, y, r, g, b );
      return;
    }

  if (x >= img->width || y >= img->height) return;

  if (pb->internal_bytespp == 2)
    {
      unsigned char rr,gg, bb;
      internal_16bpp_pixel_to_rgb((img->rgba+idx), rr,gg,bb);

      alpha_composite(rr, (r), (a), rr);
      alpha_composite(gg, (g), (a), gg);
      alpha_composite(bb, (b), (a), bb);

      internal_rgb_to_16bpp_pixel(rr,gg,bb, (img->rgba+idx));
    }
  else
    {
      alpha_composite(img->rgba[idx],   (r), (a), img->rgba[idx]);
      alpha_composite(img->rgba[idx+1], (g), (a), img->rgba[idx+1]);
      alpha_composite(img->rgba[idx+2], (b), (a), img->rgba[idx+2]);
    }
}

MBPixbufImage *
mb_pixbuf_img_transform (MBPixbuf          *pb,
			 MBPixbufImage     *img,
			 MBPixbufTransform  transform)
{
  MBPixbufImage *img_trans;
  int            new_width = 0, new_height = 0, new_x = 0, new_y = 0, idx;
  int            bytes_per_line, x, y;
  int            byte_offset = 0, new_byte_offset = 0;

  switch (transform)
    {
    case MBPIXBUF_TRANS_ROTATE_90:
    case MBPIXBUF_TRANS_ROTATE_270:
      new_width = mb_pixbuf_img_get_height(img);
      new_height = mb_pixbuf_img_get_width(img);
      break;
    case MBPIXBUF_TRANS_ROTATE_180:
    case MBPIXBUF_TRANS_FLIP_VERT:
    case MBPIXBUF_TRANS_FLIP_HORIZ:
      new_width = mb_pixbuf_img_get_width(img);
      new_height = mb_pixbuf_img_get_height(img);

      break;
    }

  if (img->has_alpha)
    {
      img_trans = mb_pixbuf_img_rgba_new(pb, new_width, new_height);
      bytes_per_line = (img->width * (pb->internal_bytespp+1));
      idx = pb->internal_bytespp +1 ;
    }
  else
    {
      img_trans = mb_pixbuf_img_rgb_new(pb, new_width, new_height);
      bytes_per_line = (img->width * pb->internal_bytespp);
      idx = pb->internal_bytespp;
    }

  for( y = 0; y < img->height; y++ )
    {
      for( x = 0; x < img->width; x++ )
	{


	  /* XXX This could all be heavily optimised */
	  switch (transform)
	    {
	    case MBPIXBUF_TRANS_ROTATE_180:
	      new_x = new_width  - 1 - x;
	      new_y = new_height - 1 - y;
	      break;
	    case MBPIXBUF_TRANS_ROTATE_270:
	      new_x = y;
	      new_y = img->width - x - 1;
	      break;
	    case MBPIXBUF_TRANS_ROTATE_90:
	      new_x = img->height - y - 1;
	      new_y = x;
	      break;
	    case MBPIXBUF_TRANS_FLIP_VERT:
	      new_x = x; new_y = img->height - y - 1;
	      break;
	    case MBPIXBUF_TRANS_FLIP_HORIZ:
	      new_y = y; new_x = img->width - x - 1;
	      break;
	    }

	  byte_offset     = (y * bytes_per_line) + ( x * idx );
	  new_byte_offset = (new_y * (new_width) * idx) + ( new_x * idx );

	  img_trans->rgba[new_byte_offset]   = img->rgba[byte_offset];
	  img_trans->rgba[new_byte_offset+1] = img->rgba[byte_offset+1];

	  if (pb->internal_bytespp > 2)
	    img_trans->rgba[new_byte_offset+2] = img->rgba[byte_offset+2];

	  if (img->has_alpha)
	    img_trans->rgba[new_byte_offset+pb->internal_bytespp] = img->rgba[byte_offset+pb->internal_bytespp];
	}
    }

  return img_trans;
}
