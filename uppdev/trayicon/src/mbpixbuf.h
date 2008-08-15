#ifndef _HAVE_MBPIXBUF_H
#define _HAVE_MBPIXBUF_H

/* libmb
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

//#include "libmb/mbconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>



#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

/* XXX if have_shm */

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <X11/extensions/XShm.h>
#include <X11/Xmd.h>

#ifdef USE_JPG
#include "jpeglib.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup MBPixbuf MBPixbuf - Simple image manipulation tools
 * @brief mbpixbuf contains simple image manipulation and composition
 * functions for client side images.
 *
 * Example: Simple example to load an image and display it on root window.
 *
 * <pre>
 *
 *
 *
 *  int
 *  main(int argc, char **argv)
 *  {
 *    Display       *dpy;
 *    Window         win_root;
 *    XEvent         xevent;
 *    MBPixbufImage *img, *img_scaled;
 *    MBPixbuf      *pb;
 *
 *    if (argc < 2)
 *      { fprintf(stderr, "usage: %s image file\\n", argv[0]); exit(1); }
 *
 *    dpy = XOpenDisplay(NULL);
 *
 *    pb = mb_pixbuf_new(dpy, DefaultScreen(dpy));
 *
 *    img = mb_pixbuf_img_new_from_file(pb, argv[1]);
 *
 *    if (!img) { fprintf(stderr, "image load failed\\n"); exit(1); }
 *
 *    img_scaled = mb_pixbuf_img_scale(pb, img,
 *                                     DisplayWidth(dpy), DisplayHeight(dpy));
 *
 *    if (!img_scaled) { fprintf(stderr, "image scale failed\\n"); exit(1); }
 *
 *    mb_pixbuf_img_free(pb, img);
 *
 *    mb_pixbuf_img_render_to_drawable(pb, img_scaled,
 *                                     RootWindow(dpy, DefaultScreen(dpy))
 *				     0, 0);
 *    for (;;)
 *       XNextEvent(dpy, &xevent);
 *
 *    exit(0);
 *  }
 *
 * </pre>
 *
 * Notes: if the enviromental varible 'MBPIXBUF_NO_SHM' is set, the MIT-SHM
 * extension will not be used.
 *
 * @{
 */

/**
 * @typedef MBPixbufTransform
 *
 * enumerated types for #mb_pixbuf_img_transform
 */
typedef enum
{
  MBPIXBUF_TRANS_ROTATE_90,
  MBPIXBUF_TRANS_ROTATE_180,
  MBPIXBUF_TRANS_ROTATE_270,
  MBPIXBUF_TRANS_FLIP_VERT,
  MBPIXBUF_TRANS_FLIP_HORIZ
} MBPixbufTransform;


typedef struct _mb_pixbuf_col {
  int                 r, g, b;
  unsigned long       pixel;
} MBPixbufColor;

/**
 * @typedef MBPixbuf
 *
 * Opaque structure used for all operations.
 */
typedef struct MBPixbuf
{
  Display       *dpy;
  int            scr;
  Visual        *vis;
  Window         root;
  int            depth;
  Colormap       root_cmap;
  int            byte_order;
  int            num_of_cols;
  GC             gc;
  MBPixbufColor *palette;
  Bool           have_shm;

  int            internal_bytespp;

} MBPixbuf;

/**
 * @typedef MBPixbufImage
 *
 * Type for representing an mbpixbuf image.
 * Its not recommended you touch this directly.
 */
typedef struct MBPixbufImage
{
  int            width;     /**< width in pixels of image */
  int            height;    /**< height in pixels of image */
  unsigned char *rgba;      /**< rgb(a) raw image data */
  int            has_alpha; /**< has alpha channel flag */
  XImage        *ximg;      /**< ximage, if set */

  int            internal_bytespp;

} MBPixbufImage;

/* macros */

/**
 * @def mb_pixbuf_img_set_pixel(i, x, y, r, g, b)
 *
 * DEPRICIATED. Use #mb_pixbuf_img_plot_pixel instead.
 */
#define mb_pixbuf_img_set_pixel(i, x, y, r, g, b) { \
  (i)->rgba[(((y)*(i)->width*4)+((x)*4))]   = r;    \
  (i)->rgba[(((y)*(i)->width*4)+((x)*4))+1] = g;    \
  (i)->rgba[(((y)*(i)->width*4)+((x)*4))+2] = b;    \
  (i)->rgba[(((y)*(i)->width*4)+((x)*4))+3] = 0;    \
}

/**
 * @def mb_pixbuf_img_set_pixel_alpha
 *
 * sets a pixels alpha value
 */
#define mb_pixbuf_img_set_pixel_alpha(i, x, y, a) { \
  if ((i)->has_alpha) (i)->rgba[(((y)*(i)->width*(i->internal_bytespp+1))+((x)*(i->internal_bytespp+1)))+i->internal_bytespp] = a;    \
}



/**
 * @def mb_pixbuf_img_get_width
 *
 * returns image width
 */
#define mb_pixbuf_img_get_width(image)  (image)->width

/**
 * @def mb_pixbuf_img_get_height
 *
 * returns image height
 */
#define mb_pixbuf_img_get_height(image) (image)->height

/**
 * @def mb_pixbuf_get_depth
 *
 * returns pixbuf instance depth
 */
#define mb_pixbuf_get_depth(pb) (pb)->depth

/**
 * @def mb_pixbuf_img_has_alpha
 *
 * Returns True if image has an alpha channel.
 */
#define mb_pixbuf_img_has_alpha(image)  (image)->has_alpha

/**
 * Constructs a new MBPixbuf instance
 *
 * @param display
 * @param screen
 * @returns a #MBPixbuf object
 */
MBPixbuf*
mb_pixbuf_new (Display *display,
	       int      screen);


/**
 * Constructs a new MBPixbuf instance with non default depth and visual
 *
 * @param dpy X display
 * @param scr X Screen
 * @param vis X Visual to use
 * @param depth Depth to use
 * @returns a #MBPixbuf object
 */
MBPixbuf *
mb_pixbuf_new_extended(Display *dpy,
		       int      scr,
		       Visual  *vis,
		       int      depth);

/**
 * Destroys a new MBPixbuf instance
 *
 * @param pixbuf MBPixbuf object
 */
void
mb_pixbuf_destroy(MBPixbuf *pixbuf);

/**
 * Get the X pixel representation for a given color
 *
 * @param pixbuf mbpixbuf object
 * @param r red component of color
 * @param g green component of color
 * @param b blue component of color
 * @param a alpha component
 *
 * @returns x pixel value
 */
unsigned long
mb_pixbuf_lookup_x_pixel(MBPixbuf *pixbuf, int r, int g, int b, int a);


/**
 * DEPRECIATED. Use #mb_pixbuf_img_rgb_new, #mb_pixbuf_img_rgba_new instead.
 *
 */
MBPixbufImage*
mb_pixbuf_img_new (MBPixbuf  *pb,
		   int        width,
		   int        height);

/**
 * Constructs a new blank mbpixbuf image with an alpha channel.
 *
 * @param pixbuf mbpixbuf object
 * @param width  width in pixels of new image
 * @param height  height in pixels of new image
 * @returns a MBPixbufImage object
 */
MBPixbufImage *
mb_pixbuf_img_rgba_new(MBPixbuf *pixbuf,
		       int       width,
		       int       height);

/**
 * Constructs a new blank mbpixbuf image without an alpha channel.
 *
 * @param pixbuf mbpixbuf object
 * @param width  width in pixels of new image
 * @param height height in pixels of new image
 * @returns a MBPixbufImage object
 */
MBPixbufImage *
mb_pixbuf_img_rgb_new (MBPixbuf *pixbuf,
		       int       width,
		       int       height);

/**
 *  Depreicated. use #mb_pixbuf_img_new_from_x_drawable instead.
 */
MBPixbufImage *
mb_pixbuf_img_new_from_drawable (MBPixbuf *pixbuf,
				 Drawable  drawable,
				 Drawable  mask,
				 int       source_x,
				 int       source_y,
				 int       source_w,
				 int       source_h);

/**
 * Constructs a new blank mbpixbuf image with/without an alpha channel.
 *
 * @param pixbuf   mbpixbuf object
 * @param drawable an X drawable ( window or pixmap )
 * @param mask     set to none if alpha channel not required
 * @param source_x x co-ord of X drawable
 * @param source_y y co-ord of X drawable
 * @param source_w width of X drawable
 * @param source_h height of X drawable
 * @param want_alpha force created image to have an ( empty ) alpha channel
 *        even if no mask is supplied.
 * @returns a MBPixbufImage object, NULL on faliure
 */
MBPixbufImage *
mb_pixbuf_img_new_from_x_drawable (MBPixbuf *pixbuf,
				   Drawable  drawable,
				   Drawable  mask,
				   int       source_x,
				   int       source_y,
				   int       source_w,
				   int       source_h,
				   Bool      want_alpha);


/**
 * Creates an mbpixbuf image from a file on disk.
 * Supports PNG, JPEGS and XPMS.
 *
 * @param pixbuf mbpixbuf object
 * @param filename full filename of image to be loaded
 * @returns a MBPixbufImage object, NULL on faliure
 */
MBPixbufImage *
mb_pixbuf_img_new_from_file (MBPixbuf   *pixbuf,
			     const char *filename);

/**
 * Creates an mbpixbuf image from arbituary supplied rgb(a) data
 *
 * @param pixbuf mbpixbuf object
 * @param data rgb(a) data
 * @param width image width
 * @param height image height
 * @param has_alpha True if data has alpha component
 * @returns a MBPixbufImage object, NULL on faliure
 */
MBPixbufImage *
mb_pixbuf_img_new_from_data(MBPixbuf            *pixbuf,
			    const unsigned char *data,
			    int                  width,
			    int                  height,
			    Bool                 has_alpha);


/**
 * Creates an mbpixbuf image from arbituary supplied INT ARGB data
 *
 * @param pixbuf mbpixbuf object
 * @param data argb data
 * @param width image width
 * @param height image height
 * @returns a MBPixbufImage object, NULL on faliure
 */

MBPixbufImage *
mb_pixbuf_img_new_from_int_data(MBPixbuf            *pixbuf,
				const int           *data,
				int                  width,
				int                  height);

/**
 * Frees up  a mbpixbuf image.
 *
 * @param pixbuf mbpixbuf object
 * @param image image to free
 */
void
mb_pixbuf_img_free (MBPixbuf      *pixbuf,
		    MBPixbufImage *image);

/**
 * Renders a mbpixbuf image to an X Drawable.
 *
 * @param pixbuf mbpixbuf object
 * @param image mbpixbuf image to render
 * @param drw X11 drawable ( window or pixmap ) to render roo.
 * @param drw_x X co-ord on drawable to render too.
 * @param drw_y Y co-ord on drawable to render too.
 */
void  mb_pixbuf_img_render_to_drawable (MBPixbuf      *pixbuf,
					MBPixbufImage *image,
					Drawable       drw,
					int            drw_x,
					int            drw_y);

/**
 * Renders a mbpixbuf image to an X Drawable with a specified.
 * X Graphics Context ( GC ).
 *
 * @param pixbuf mbpixbuf object
 * @param image mbpixbuf image to render
 * @param drw X11 drawable ( window or pixmap ) to render roo.
 * @param drw_x X co-ord on drawable to render too.
 * @param drw_y Y co-ord on drawable to render too.
 * @param gc X11 GC to use on drawable.
 */
void
mb_pixbuf_img_render_to_drawable_with_gc(MBPixbuf    *pixbuf,
					 MBPixbufImage *image,
					 Drawable     drw,
					 int drw_x,
					 int drw_y,
					 GC gc);


/**
 * Renders alpha component  mbpixbuf image to an X Bitmap.
 *
 * @param pixbuf mbpixbuf object
 * @param image mbpixbuf image to render
 * @param mask X11 bitmap.
 * @param mask_x X co-ord on drawable to render too.
 * @param mask_y Y co-ord on drawable to render too.
 */
void mb_pixbuf_img_render_to_mask (MBPixbuf      *pixbuf,
				   MBPixbufImage *image,
				   Drawable       mask,
				   int            mask_x,
				   int            mask_y);

/**
 * Clones a exisiting mbpixbuf image
 *
 * @param pixbuf mbpixbuf object
 * @param image image to clone
 * @returns a new cloned image
 */
MBPixbufImage *mb_pixbuf_img_clone (MBPixbuf      *pixbuf,
				    MBPixbufImage *image);

/**
 * Fills an image with specified color / alpha level.
 *
 * @param pixbuf mbpixbuf object
 * @param image image to fill.
 * @param r red component of color
 * @param g green component of color
 * @param b blue component of color
 * @param a alpha component
 */
void
mb_pixbuf_img_fill(MBPixbuf *pixbuf,
		   MBPixbufImage *image,
		   int r,
		   int g,
		   int b,
		   int a);

/**
 * Plots a pixel on specified image. Note: on a RGBA image the alpha channel is
 * left as-is.
 *
 * @param pixbuf mbpixbuf object
 * @param image destination image
 * @param x X co-ord on destination image
 * @param y Y co-ord on destination image
 * @param r red component of color
 * @param g green component of color
 * @param b blue component of color
 */
void
mb_pixbuf_img_plot_pixel (MBPixbuf      *pixbuf,
			  MBPixbufImage *image,
			  int            x,
			  int            y,
			  unsigned char  r,
			  unsigned char  g,
			  unsigned char  b);


/**
 * Gets the component values for a specified pixel
 *
 * @param pixbuf mbpixbuf object
 * @param image destination image
 * @param x X co-ord on destination image
 * @param y Y co-ord on destination image
 * @param r red component of color
 * @param g green component of color
 * @param b blue component of color
 * @param a alpha component of pixel
 */
void
mb_pixbuf_img_get_pixel (MBPixbuf      *pixbuf,
			 MBPixbufImage *image,
			 int            x,
			 int            y,
			 unsigned char *r,
			 unsigned char *g,
			 unsigned char *b,
			 unsigned char *a
			 );

/**
 * Gets rgb(a) internal data representation of an image
 *
 * @param pixbuf mbpixbuf object
 * @param image destination image
 * @returns rgb(a) data
 */
unsigned char *
mb_pixbuf_img_data (MBPixbuf      *pixbuf,
		    MBPixbufImage *image);



/**
 * Plots a pixel on specified image with alpha channel value.
 *
 * @param pixbuf mbpixbuf object
 * @param image destination image
 * @param x X co-ord on destination image
 * @param y Y co-ord on destination image
 * @param r red component of color
 * @param g green component of color
 * @param b blue component of color
 * @param a alpha component
 */
void
mb_pixbuf_img_plot_pixel_with_alpha (MBPixbuf      *pixbuf,
				     MBPixbufImage *image,
				     int            x,
				     int            y,
				     unsigned char  r,
				     unsigned char  g,
				     unsigned char  b,
				     unsigned char  a);

/**
 * Copys an specified area of an image to another.
 * No Alpha composition is performed.
 *
 * @param pixbuf mbpixbuf object
 * @param dest destination image
 * @param src  source image
 * @param sx   source area X co-ord
 * @param sy   source area Y co-ord
 * @param sw   source area width.
 * @param sh   source area height.
 * @param dx   destination image X co-ord.
 * @param dy   destination image Y co-ord.
 */
void mb_pixbuf_img_copy (MBPixbuf      *pixbuf,
			 MBPixbufImage *dest,
			 MBPixbufImage *src,
			 int            sx,
			 int            sy,
			 int            sw,
			 int            sh,
			 int            dx,
			 int            dy);


/**
 * Alpha composites an specified area of an image with another.
 *
 * @param pixbuf mbpixbuf object
 * @param dest destination image
 * @param src  source image
 * @param sx   source area X co-ord
 * @param sy   source area Y co-ord
 * @param sw   source area width.
 * @param sh   source area height.
 * @param dx   destination image X co-ord.
 * @param dy   destination image Y co-ord.
 */
void mb_pixbuf_img_copy_composite (MBPixbuf      *pixbuf,
				   MBPixbufImage *dest,
				   MBPixbufImage *src,
				   int            sx,
				   int            sy,
				   int            sw,
				   int            sh,
				   int            dx,
				   int            dy);

/**
 * Like #mb_pixbuf_img_copy_composite but sets an overall alpha value
 *
 * @param pixbuf mbpixbuf object
 * @param dest destination image
 * @param src  source image
 * @param sx   source area X co-ord
 * @param sy   source area Y co-ord
 * @param sw   source area width.
 * @param sh   source area height.
 * @param dx   destination image X co-ord.
 * @param dy   destination image Y co-ord.
 * @param overall_alpha The overall alpha value to set after composition.
 */
void mb_pixbuf_img_copy_composite_with_alpha (MBPixbuf      *pixbuf,
					      MBPixbufImage *dest,
					      MBPixbufImage *src,
					      int sx, int sy,
					      int sw, int sh,
					      int dx, int dy,
					      int overall_alpha );

/**
 * DEPRECATED. Use #mb_pixbuf_img_copy_composite instead.
 *
 */
void mb_pixbuf_img_composite (MBPixbuf      *pb,
			      MBPixbufImage *dest,
			      MBPixbufImage *src,
			      int            dx,
			      int            dy);

/**
 * Scales an image arbitually.
 *
 * @param pixbuf mbpixbuf object
 * @param image mbpixbuf image to scale
 * @param new_width new image width
 * @param new_height new image height
 * @returns a new scaled image
 */
MBPixbufImage *mb_pixbuf_img_scale (MBPixbuf      *pixbuf,
				    MBPixbufImage *image,
				    int            new_width,
				    int            new_height);


MBPixbufImage *mb_pixbuf_img_scale_down (MBPixbuf      *pixbuf,
					 MBPixbufImage *image,
					 int            new_width,
					 int            new_height);


MBPixbufImage *mb_pixbuf_img_scale_up (MBPixbuf      *pixbuf,
				       MBPixbufImage *image,
				       int            new_width,
				       int            new_height);

/**
 * Performs a basic transform on an image.
 *
 * @param pixbuf mbpixbuf object
 * @param image mbpixbuf image to scale
 * @param transform the type of transform to perform
 * @returns a new transformed image
 */
MBPixbufImage *
mb_pixbuf_img_transform (MBPixbuf          *pixbuf,
			 MBPixbufImage     *image,
			 MBPixbufTransform  transform);


/** @} */

#ifdef __cplusplus
}
#endif


#endif
