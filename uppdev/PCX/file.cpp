/*
 *************************************************************************
 *
 *  PCX_FILE.C - PCX_LIB Library Image File Functions
 *
 *  Version:    1.00B
 *
 *  History:    91/02/14 - Created
 *              91/04/01 - Release 1.00A
 *              91/04/03 - fixed "segread" call.
 *              91/04/07 - Release 1.00B
 *
 *  Compiler:   Microsoft C V6.0
 *
 *  Author:     Ian Ashdown, P.Eng.
 *              byHeart Software
 *              620 Ballantree Road
 *              West Vancouver, B.C.
 *              Canada V7S 1W3
 *              Tel. (604) 922-6148
 *              Fax. (604) 987-7621
 *
 *  Copyright:  Public Domain
 *
 *************************************************************************
 */

/*
 *************************************************************************
 *
 *  PORTABILITY NOTES
 *
 *  1.  While this program is written in ANSI C, it uses a number of 
 *      function calls that are specific to the Microsoft C V6.0 library.
 *      These are documented as follows for the purposes of porting this
 *      program to other compilers and/or processors: 
 *
 *          _ffree      - "free" for small model / far data
 *          _fmalloc    - "malloc" for small model / far data
 *          _fmemcpy    - "memcpy" for small model / far data
 *          int86       - execute 80x86 interrupt routine
 *          int86x      - execute 80x86 interrupt routine (far data)
 *          outpw       - output word to 80x86 I/O port
 *          segread     - get current 80x86 segment register values
 *
 *  2.  When porting this program to other processors, remember that words
 *      are stored by 80x86-based machines in the big-endian format.  That
 *      is, the eight least significant bits (lower byte) are stored
 *      first, followed by the eight most significant bits (upper byte).
 *      If PCX-format files are transferred to little-endian machines
 *      (such as those based on 680x0 and Z8000 processors), the order of
 *      bytes within each word will have to be reversed before they can 
 *      be interpreted.  (This applies to the file header only, since the
 *      encoded image data and optional 256-color palette are stored as
 *      bytes.)
 *
 * 3.   MS-DOS does not recognize the 720 x 348 graphics mode of the
 *      Hercules monochrome display adapter.  Therefore, the constant
 *      PCX_HERC should never be passed as a video mode parameter to any
 *      BIOS service routine.
 *
 *      The Microsoft C compiler includes a "video mode" parameter
 *      definition (_HERCMONO) that is defined as 0x08.  This is a
 *      reserved MS-DOS video mode that is apparently used internally by
 *      the ROM BIOS.  It can, however, be passed to the Microsoft C
 *      library function "_setvideomode" to force the Hercules display
 *      adapter into graphics mode.
 *
 *      Most other MS-DOS C compilers offer similar library functions to
 *      force the Hercules monochrome display adapter into its 720 x 348
 *      graphics mode.
 *
 ************************************************************************* 
 */

/*      INCLUDE FILES                                                   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <malloc.h>
#include <dos.h>
#include <graph.h>
#include "pcx_int.h"

/*      FORWARD REFERENCES                                              */

static BOOL pcx_encode(int, int, FILE *);
static BOOL pcx_init_palette(PCX_PAL *, int);
static BOOL pcx_write_extpal(FILE *);
static BOOL pcx_write_line(unsigned char *, int, FILE *);
static BOOL pcx_write_init(PCX_WORKBLK *, int, int, int, int);

static void pcx_get_cga(PCX_WORKBLK *, unsigned char _far *, int);
static void pcx_get_ega(PCX_WORKBLK *, unsigned char _far *, int);
static void pcx_get_herc(PCX_WORKBLK *, unsigned char _far *, int);
static void pcx_get_vga(PCX_WORKBLK *, unsigned char _far *, int);

/*      GLOBALS                                                         */

/* Default EGA palette register values                                  */

static BYTE pcx_EGA_DefPal_1[16] =      /* Modes 0x0d and 0x0e          */
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13,
  0x14, 0x15, 0x16, 0x17
};

static BYTE pcx_EGA_DefPal_2[16] =      /* Mode 0x0f                    */
{
  0x00, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x18, 0x00, 0x00
};

static BYTE pcx_EGA_DefPal_3[16] =      /* Mode 0x10                    */
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3a, 0x3b, 
  0x3c, 0x3d, 0x3e, 0x3f
};

/*      PUBLIC FUNCTIONS                                                */

/*
 *************************************************************************
 *
 *  PCX_WRITE - Write PCX File
 *
 *  Purpose:    To write a PCX-format image file from an image stored in
 *              a video buffer.  The image is assumed to start in the
 *              upper left corner of the screen.
 *
 *  Setup:      BOOL pcx_write
 *              (
 *                char *fname,
 *                int vmode,
 *                int page,
 *                int width,
 *                int height,
 *              )
 *
 *  Where:      fname is a PCX image file name.
 *              vmode is the MS-DOS video mode.  Valid values are:
 *
 *                PCX_HERC -    720 x 348 Hercules monochrome
 *                0x04 -        320 x 200 4-color CGA
 *                0x05 -        320 x 200 4-color CGA (color burst off)
 *                0x06 -        640 x 200 2-color CGA
 *                0x0d -        320 x 200 16-color EGA/VGA
 *                0x0e -        640 x 200 16-color EGA/VGA
 *                0x0f -        640 x 350 2-color EGA/VGA
 *                0x10 -        640 x 350 16-color EGA/VGA
 *                0x11 -        640 x 480 2-color VGA
 *                0x12 -        640 x 480 16-color VGA
 *                0x13 -        320 x 200 256-color VGA
 *
 *              page is the video display page number.  Valid values are:
 *
 *                Mode PCX_HERC - 0 or 1
 *                Mode 0x0d     - 0 to 7
 *                Mode 0x0e     - 0 to 3
 *                Mode 0x0f     - 0 or 1
 *                Mode 0x10     - 0 or 1
 *                All Other     - 0
 *
 *              width is the image width in pixels.
 *              height is the image height in pixels.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *************************************************************************
 */

BOOL pcx_write
(
  char *fname,
  int vmode,
  int page,
  int width,
  int height
)
{
  int bpline;                   /* Number of bytes per scan line        */
  int line_num;                 /* Scan line number                     */
  unsigned char *linep;         /* Image scan line buffer pointer       */
  BOOL status = TRUE;           /* Return status                        */
  PCX_WORKBLK *wbp;             /* PCX image file workblock pointer     */

  /* Open a PCX image file workblock                                    */

  if ((wbp = pcx_open(fname, TRUE)) == (PCX_WORKBLK *) NULL)
    return (FALSE);

  /* Initialize the workblock for writing                               */

  if (pcx_write_init(wbp, vmode, page, width, height) == FALSE)
    status = FALSE;

  /* Calculate number of bytes per line (for all color planes)          */

  bpline = wbp->header.bppscan * wbp->header.nplanes;

  /* Allocate a scan line buffer                                        */

  if (status == TRUE)
    if ((linep = (unsigned char *) malloc(bpline)) == (unsigned char *)
        NULL)
      status = FALSE;

  /* Write the file header to the file                                  */

  if (status == TRUE)
    if (fwrite(&(wbp->header), sizeof(PCX_HDR), 1, wbp->fp) != 1)
      status = FALSE;

  /* Write the encoded image data to the file                           */

  if (status == TRUE)
  {
    for (line_num = 0; line_num <= (int) wbp->header.ylr; line_num++)
    {
      /* Get the current video buffer scan line                         */

      wbp->pcx_funcp(wbp, (unsigned char _far *) linep, line_num);

      /* Encode the scan line and write it to the file                  */

      if (pcx_write_line(linep, bpline, wbp->fp) == FALSE)
      {
        status = FALSE;
        break;
      }
    }
  }

  if (vmode == 0x13)    /* Is a 256-color palette supported ?           */
  {
    /* Write the extended palette to the file                           */

    if (status == TRUE)
      if (pcx_write_extpal(wbp->fp) == FALSE)
        status = FALSE;
  }

  if (pcx_close(wbp) == FALSE)  /* Close the PCX workblock              */
    status = FALSE;

  free(linep);          /* Free the scan line buffer                    */

  /* Remove the PCX image file if an error occurred                     */

  if (status == FALSE)
    (void) remove(fname);

  return (status);
}

/*
 *************************************************************************
 *
 *  PCX_INIT_DSA - Initialize Dynamic Save Area
 *
 *  Purpose:    To set up a Video Services Primary Pointer Table and an
 *              associated Dynamic Save Area where BIOS service "Set All
 *              Palette Registers" (function 0x02) can store the EGA color
 *              palette registers settings after it updates them.
 *
 *  Setup:      BOOL pcx_init_dsa
 *              (
 *                PCX_VSB **vsbpp
 *              )
 *
 *  Where:      vsbp is a pointer to where a pointer to an instantiated
 *              PCX_VSB structure is to be returned.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *  Note:       The EGA display adapter color palette registers are
 *              write-only.  In order to save the current color palette
 *              with a PCX-format image file by calling "pcx_write", you
 *              must call this function BEFORE you display the image in
 *              the following MS-DOS video modes:
 *
 *                0x0d  - 320 x 200 16-color EGA
 *                0x0e  - 640 x 200 16-color EGA
 *                0x0f  - 640 x 350 2-color EGA
 *                0x10  - 640 x 350 16-color EGA
 *
 *              You MUST call "pcx_free_dsa" upon completion of your
 *              program.   See the function header of "pcx_init_palette"
 *              for more information.
 *
 *************************************************************************
 */

BOOL pcx_init_dsa
(
  PCX_VSB **vsbpp
)
{
  unsigned char _far *dsap;     /* Dynamic Save Area pointer            */
  PCX_VSB *vsbp;                /* Video services data save buffer ptr  */

  *vsbpp = (PCX_VSB *) NULL;    /* Initialize returned pointer          */

  /* Allocate a Dynamic Save Area buffer                                */

  if ((dsap = (unsigned char _far *) _fmalloc(sizeof(unsigned char) *
      256)) == (unsigned char _far *) NULL)
    return (FALSE);

  /* Allocate a BIOS video services data save buffer                    */

  if ((vsbp = (PCX_VSB *) malloc(sizeof(PCX_VSB))) == (PCX_VSB *) NULL)
  {
    _ffree(dsap);       /* Free the Dynamic Save Area buffer            */
    return (FALSE);
  }

  /* Save the existing Primary Pointer Table pointer                    */

  vsbp->prev_pptp = *((struct pcx_ppt _far * _far *) 0x004000a8L);

  /* Copy the existing Primary Pointer Table into the buffer            */

  (void) _fmemcpy((struct pcx_ppt _far *) &(vsbp->pcx_ppt),
      vsbp->prev_pptp, sizeof(struct pcx_ppt));

  vsbp->pcx_ppt.dsap = dsap;    /* Update the Dynamic Save Area ptr     */

  /* Update the Primary Pointer Table pointer in the Video Save Table   */

  *((struct pcx_ppt _far * _far *) 0x004000a8L) = &(vsbp->pcx_ppt);

  *vsbpp = vsbp;        /* Return Video Services data save buffer ptr   */

  return (TRUE);
}

/*
 *************************************************************************
 *
 *  PCX_FREE_DSA - Release Dynamic Save Area
 *
 *  Purpose:    To release memory allocated to the Video Services Primary
 *              Pointer Table and associated Dynamic Save Area and reset
 *              the pointer in the Video Save Table.
 *
 *  Setup:      void pcx_free_dsa
 *              (
 *                PCX_VSB *vsbp
 *              )
 *
 *  Where:      vsbp is a pointer to a PCX_VSB structure that was
 *                previously allocated and initialized by "pcx_init_dsa".
 *
 *  Note:       You MUST call this function on completion of your program
 *              if you previously called "pcx_init_dsa".  Failure to do so
 *              will leave the system in an unstable state.  See the
 *              function header of "pcx_init_palette" for more
 *              information.
 *
 *************************************************************************
 */

void pcx_free_dsa
(
  PCX_VSB *vsbp
)
{
  /* Restore the previous primary pointer table pointer                 */

  *((struct pcx_ppt _far * _far *) 0x004000a8L) = vsbp->prev_pptp;

  _ffree(vsbp->pcx_ppt.dsap);   /* Free the Dynamic Save Area           */

  free(vsbp);   /* Free the Video Services data save buffer             */
}

/*      PRIVATE FUNCTIONS                                               */

/*
 *************************************************************************
 *
 *  PCX_WRITE_INIT - Initialize PCX Workblock For Writing
 *
 *  Purpose:    To initialize a PCX image file workblock for writing.
 *
 *  Setup:      static BOOL pcx_write_init
 *              (
 *                PCX_WORKBLK *wbp,
 *                int vmode,
 *                int page,
 *                int width,
 *                int height
 *              )
 *
 *  Where:      wbp is a PCX workblock pointer.
 *              vmode is the MS-DOS video mode.  Valid values are:
 *
 *                0x04 -        320 x 200 4-color CGA
 *                0x05 -        320 x 200 4-color CGA (color burst off)
 *                0x06 -        640 x 200 2-color CGA
 *                Ox07 -        720 x 348 Hercules monochrome
 *                0x0d -        320 x 200 16-color EGA/VGA
 *                0x0e -        640 x 200 16-color EGA/VGA
 *                0x0f -        640 x 350 2-color EGA/VGA
 *                0x10 -        640 x 350 16-color EGA/VGA
 *                0x11 -        640 x 480 2-color VGA
 *                0x12 -        640 x 480 16-color VGA
 *                0x13 -        320 x 200 256-color VGA
 *
 *              page is the video display page number.  Valid values are:
 *
 *                Mode PCX_HERC - 0 or 1
 *                Mode 0x0d     - 0 to 7
 *                Mode 0x0e     - 0 to 3
 *                Mode 0x0f     - 0 or 1
 *                Mode 0x10     - 0 or 1
 *                All Other     - 0
 *
 *              width is the image width in pixels.
 *              height is the image height in pixels.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *************************************************************************
 */

static BOOL pcx_write_init
(
  PCX_WORKBLK *wbp,
  int vmode,
  int page,
  int width,
  int height
)
{
  int max_width;        /* Maximum image width                          */
  int max_height;       /* Maximum image height                         */
  int shift;            /* Mask shift value                             */
  BOOL status = TRUE;   /* Return status                                */
  PCX_HDR *hdrp;        /* File header buffer pointer                   */

  /* Initialize the display page address offset                         */

  wbp->page_offset = (unsigned long) 0L;

  hdrp = &(wbp->header);        /* Initialize the file header pointer   */

  /* Initialize the header constants                                    */

  hdrp->pcx_id = 0x0a;          /* PCX format identifier                */
  hdrp->version = 5;            /* Version number                       */
  hdrp->encoding = 1;           /* Encoding format (run-length)         */
  hdrp->xul = 0;                /* Upper left x-position                */
  hdrp->yul = 0;                /* Upper left y-position                */
  hdrp->reserved = 0;           /* (Used to be video mode)              */
  hdrp->palette_type = 1;       /* Color or b&w palette type            */

  memset(hdrp->filler, 0, sizeof(hdrp->filler));        /* Padding      */

  /* Initialize the video mode-dependent parameters                     */

  switch (vmode)
  {
    case PCX_HERC:      /* 720 x 348 Hercules monochrome                */

      max_width = min(width, 720);      /* Maximum image width          */
      max_height = min(height, 348);    /* Maximum image height         */

      hdrp->bppixel = 1;                /* Bits per pixel               */
      hdrp->horz_res = 720;             /* Horizontal resolution        */
      hdrp->vert_res = 348;             /* Vertical resolution          */
      hdrp->nplanes = 1;                /* Number of color planes       */

      /* Maximum two pages supported                                    */

      wbp->page_offset = 0x08000000L * (unsigned long) page;

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 7) >> 3;

      shift = (max_width & 7);  /* Calculate mask shift value           */

      wbp->pcx_funcp = pcx_get_herc;    /* Set display capture fcn ptr  */

      break;

    case 0x04:          /* 320 x 200 4-color CGA                        */
    case 0x05:          /* 320 x 200 4-color CGA (color burst off)      */

      max_width = min(width, 320);      /* Maximum image width          */
      max_height = min(height, 200);    /* Maximum image height         */

      hdrp->bppixel = 2;                /* Bits per pixel               */
      hdrp->horz_res = 320;             /* Horizontal resolution        */
      hdrp->vert_res = 200;             /* Vertical resolution          */
      hdrp->nplanes = 1;                /* Number of color planes       */

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 3) >> 2;

      shift = (max_width & 3) << 1;     /* Calculate mask shift value   */

      wbp->pcx_funcp = pcx_get_cga;     /* Set display capture fcn ptr  */

      break;

    case 0x06:          /* 640 x 200 2-color CGA                        */

      max_width = min(width, 640);      /* Maximum image width          */
      max_height = min(height, 200);    /* Maximum image height         */

      hdrp->bppixel = 1;                /* Bits per pixel               */
      hdrp->horz_res = 640;             /* Horizontal resolution        */
      hdrp->vert_res = 200;             /* Vertical resolution          */
      hdrp->nplanes = 1;                /* Number of color planes       */

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 7) >> 3;

      shift = (max_width & 7);  /* Calculate mask shift value           */

      wbp->pcx_funcp = pcx_get_cga;     /* Set display capture fcn ptr  */

      break;

    case 0x0d:          /* 320 x 200 16-color EGA/VGA                   */

      max_width = min(width, 320);      /* Maximum image width          */
      max_height = min(height, 200);    /* Maximum image height         */

      hdrp->bppixel = 1;                /* Bits per pixel               */
      hdrp->horz_res = 320;             /* Horizontal resolution        */
      hdrp->vert_res = 200;             /* Vertical resolution          */
      hdrp->nplanes = 4;                /* Number of color planes       */

      /* Maximum eight display pages supported                          */

      wbp->page_offset = 0x02000000L * (unsigned long) page;

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 7) >> 3;

      shift = (max_width & 7);  /* Calculate mask shift value           */

      wbp->pcx_funcp = pcx_get_ega;     /* Set display capture fcn ptr  */

      break;

    case 0x0e:          /* 640 x 200 16-color EGA/VGA                   */

      max_width = min(width, 640);      /* Maximum image width          */
      max_height = min(height, 200);    /* Maximum image height         */

      hdrp->bppixel = 1;                /* Bits per pixel               */
      hdrp->horz_res = 640;             /* Horizontal resolution        */
      hdrp->vert_res = 200;             /* Vertical resolution          */
      hdrp->nplanes = 4;                /* Number of color planes       */

      /* Maximum four display pages supported                           */

      wbp->page_offset = 0x04000000L * (unsigned long) page;

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 7) >> 3;

      shift = (max_width & 7);  /* Calculate mask shift value           */

      wbp->pcx_funcp = pcx_get_ega;     /* Set display capture fcn ptr  */

      break;

    case 0x0f:          /* 640 x 350 2-color EGA/VGA                    */

      max_width = min(width, 640);      /* Maximum image width          */
      max_height = min(height, 350);    /* Maximum image height         */

      hdrp->bppixel = 1;                /* Bits per pixel               */
      hdrp->horz_res = 640;             /* Horizontal resolution        */
      hdrp->vert_res = 350;             /* Vertical resolution          */
      hdrp->nplanes = 2;                /* Number of color planes       */

      /* Maximum two display pages supported                            */

      wbp->page_offset = 0x08000000L * (unsigned long) page;

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 7) >> 3;

      shift = (max_width & 7);  /* Calculate mask shift value           */

      wbp->pcx_funcp = pcx_get_ega;     /* Set display capture fcn ptr  */

      break;

    case 0x10:          /* 640 x 350 16-color EGA/VGA                   */

      max_width = min(width, 640);      /* Maximum image width          */
      max_height = min(height, 350);    /* Maximum image height         */

      hdrp->bppixel = 1;                /* Bits per pixel               */
      hdrp->horz_res = 640;             /* Horizontal resolution        */
      hdrp->vert_res = 350;             /* Vertical resolution          */
      hdrp->nplanes = 4;                /* Number of color planes       */

      /* Maximum two display pages supported                            */

      wbp->page_offset = 0x08000000L * (unsigned long) page;

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 7) >> 3;

      shift = (max_width & 7);  /* Calculate mask shift value           */

      wbp->pcx_funcp = pcx_get_ega;     /* Set display capture fcn ptr  */

      break;

    case 0x11:          /* 640 x 480 2-color VGA                        */

      max_width = min(width, 640);      /* Maximum image width          */
      max_height = min(height, 480);    /* Maximum image height         */

      hdrp->bppixel = 1;                /* Bits per pixel               */
      hdrp->horz_res = 640;             /* Horizontal resolution        */
      hdrp->vert_res = 480;             /* Vertical resolution          */
      hdrp->nplanes = 1;                /* Number of color planes       */

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 7) >> 3;

      shift = (max_width & 7);  /* Calculate mask shift value           */

      wbp->pcx_funcp = pcx_get_ega;     /* Set display capture fcn ptr  */

      break;

    case 0x12:          /* 640 x 480 16-color VGA                       */

      max_width = min(width, 640);      /* Maximum image width          */
      max_height = min(height, 480);    /* Maximum image height         */

      hdrp->bppixel = 1;                /* Bits per pixel               */
      hdrp->horz_res = 640;             /* Horizontal resolution        */
      hdrp->vert_res = 480;             /* Vertical resolution          */
      hdrp->nplanes = 4;                /* Number of color planes       */

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = (max_width + 7) >> 3;

      shift = (max_width & 7);  /* Calculate mask shift value           */

      wbp->pcx_funcp = pcx_get_ega;     /* Set display capture fcn ptr  */

      break;

    case 0x13:          /* 320 x 200 256-color VGA                      */

      max_width = min(width, 320);      /* Maximum image width          */
      max_height = min(height, 200);    /* Maximum image height         */

      hdrp->bppixel = 8;                /* Bits per pixel               */
      hdrp->horz_res = 320;             /* Horizontal resolution        */
      hdrp->vert_res = 200;             /* Vertical resolution          */
      hdrp->nplanes = 1;                /* Number of color planes       */

      /* Calculate number of bytes to copy                              */

      wbp->num_bytes = max_width;

      shift = 0;        /* Dummy parameter                              */

      wbp->pcx_funcp = pcx_get_vga;     /* Set display capture fcn ptr  */

      break;

    default:            /* Other modes not supported                    */

      status = FALSE;

      break;
  }

  /* Initialize common video mode-dependent parameters                  */

  hdrp->xlr = max_width - 1;            /* Lower right x-position       */
  hdrp->ylr = max_height - 1;           /* Lower right y-position       */
  hdrp->scrn_width = hdrp->horz_res;    /* Screen width                 */
  hdrp->scrn_height = hdrp->vert_res;   /* Screen height                */

  /* Calculate mask for "white" data                                    */

  if (shift != 0)
    wbp->mask = 0xff >> shift;
  else
    wbp->mask = 0x00;

  /* Initialize the file header palette                                 */

  status = pcx_init_palette(hdrp->palette, vmode);

  /* Calculate number of bytes per color plane scan line (must be an    */
  /* even number of bytes)                                              */

  hdrp->bppscan = 2 * (((((hdrp->xlr * hdrp->bppixel) + 7) / 8) + 1) / 2);

  return (status);
}

/*
 *************************************************************************
 *
 *  PCX_INIT_PALETTE - Initialize File Header Palette
 *
 *  Purpose:    To initialize the file header 16-color palette.
 *
 *  Setup:      static BOOL pcx_init_palette
 *              (
 *                PCX_PAL *palettep,
 *                int vmode
 *              )
 *
 *  Where:      palettep is a pointer to the PCX file header buffer
 *                "palette" member.
 *              vmode is the MS-DOS video mode.  Valid values are:
 *
 *                0x04 -        320 x 200 4-color CGA
 *                0x05 -        320 x 200 4-color CGA (color burst off)
 *                0x06 -        640 x 200 2-color CGA
 *                Ox07 -        720 x 348 Hercules monochrome
 *                0x0d -        320 x 200 16-color EGA/VGA
 *                0x0e -        640 x 200 16-color EGA/VGA
 *                0x0f -        640 x 350 2-color EGA/VGA
 *                0x10 -        640 x 350 16-color EGA/VGA
 *                0x11 -        640 x 480 2-color VGA
 *                0x12 -        640 x 480 16-color VGA
 *                0x13 -        320 x 200 256-color VGA
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *  Note:       The CGA color palette is not supported.
 *
 *              If a VGA display adapter is present, the color palette
 *              registers can be read directly from the adapter using the
 *              BIOS routine "Read All Palette Registers" (function 0x09).
 *
 *              Unfortunately, the EGA display adapter color palette
 *              registers are write-only.  This means that the current
 *              color palette for EGA displays cannot in general be read.
 *
 *              The BIOS routine "Set All Palette Registers" (function
 *              0x02) will write the current palette register values to a
 *              buffer called the Dynamic Save Area.  The EGA color
 *              palette can be read from the first 16 bytes of this 256-
 *              byte RAM block.
 *
 *              The Dynamic Save Area is not statically allocated; it must
 *              be supplied by the user.  The BIOS video services data in
 *              segment 0x40 includes a pointer at address 0040:00a8 that
 *              references the Video Services Primary Pointer Table in the
 *              EGA/VGA BIOS.  This table contains seven pointers, the
 *              second of which is used by the "Set All Palette Registers"
 *              routine to reference the Dynamic Save Area.  Since the
 *              Dynamic Save Area does not exist at system initialization,
 *              the value of this pointer is 0000:0000 (in which case the
 *              the updated palette register values are not saved to RAM
 *              when they are updated).
 *
 *              To utilize the EGA palette register save feature, the
 *              user must perform the following:
 *
 *                1.  Save a copy of the pointer at address 0040:00a8.
 *                2.  Allocate a buffer for a new Primary Pointer Table.
 *                3.  Copy the existing Primary Pointer Table to the
 *                    allocated buffer.
 *                4.  Allocate a 256-byte buffer for a Dynamic Save Area.
 *                5.  Initialize the second pointer of the Primary Pointer
 *                    Table to point to the Dynamic Save Area buffer.
 *
 *              Before the program finishes, the user must also restore
 *              the saved Primary Pointer Table pointer to address
 *              0040:00a8.  Failure to do so will mean that subsequent
 *              calls by other programs to the "Set All Palette
 *              Registers" routine will result in the color palette
 *              registers values being written to unallocated memory (or
 *              memory that has been allocated for another purpose).
 *
 *              The function "pcx_init_dsa" performs the five steps
 *              outlined above, while the function "pcx_free_dsa" restores
 *              the saved pointer on completion of your program.
 *
 *              If the Dynamic Save Area pointer is 0000:0000 (the default
 *              value at system initialization), the BIOS default color
 *              palette settings for the appropriate mode are stored in
 *              the file header color palette.
 *
 *************************************************************************
 */

static BOOL pcx_init_palette
(
  PCX_PAL *palettep,
  int vmode
)
{
  int i;                        /* Scratch counter                      */
  int val;                      /* Current palette register value       */
  int red;                      /* Temporary value                      */
  int green;                    /* Temporary value                      */
  int blue;                     /* Temporary value                      */
  unsigned char *ega_palp;      /* EGA/VGA palette buffer pointer       */
  unsigned char _far *dsap;     /* Dynamic Save Area pointer            */
  union REGS regs;              /* 80x86 register values                */
  struct SREGS sregs;           /* 80x86 segment register values        */

  if (vmode < 0x0d || vmode > 0x12)
  {
    /* Clear the file header palette                                    */

    memset(palettep, 0, sizeof(PCX_PAL) * PCX_PAL_SIZE);

    return (TRUE);
  }

  /* Allocate a 16-color (plus border color) EGA/VGA palette buffer     */

  if ((ega_palp = (unsigned char *) calloc(sizeof(unsigned char), 
      (PCX_PAL_SIZE + 1))) == (unsigned char *) NULL)
    return (FALSE);

  if (pcx_isvga() == TRUE)      /* Check for VGA display adapter        */
  {
    /* Read the EGA/VGA palette registers                               */

    regs.h.ah = 0x10;   /* Select "Read All Palette Registers" routine  */
    regs.h.al = 0x09;

    /* Get the EGA/VGA palette buffer offset value                      */

    regs.x.dx = (unsigned int) ega_palp;

    segread(&sregs);    /* Get the current DS segment register value    */

    sregs.es = sregs.ds;

    int86x(0x10, &regs, &regs, &sregs);  /* Call BIOS video service     */
  }
  else
  {
    /* Check for a Dynamic Save Area buffer                             */

    dsap = *(*((unsigned char _far * _far * _far *) 0x004000a8L) + 1);

    if (dsap != (unsigned char _far *) NULL)
    {
      /* Copy the current palette into the local EGA/VGA palette buffer */

      (void) _fmemcpy((unsigned char _far *) ega_palp, dsap,
          PCX_PAL_SIZE);
    }
    else
    {
      /* Copy the appropriate default palette settings                  */

      switch (vmode)
      {
        case 0x0d:      /* 320 x 200 16-color EGA                       */
        case 0x0e:      /* 640 x 200 16-color EGA                       */

          memcpy(ega_palp, pcx_EGA_DefPal_1, PCX_PAL_SIZE);

          break;

        case 0x0f:      /* 640 x 350 2-color EGA                        */

          memcpy(ega_palp, pcx_EGA_DefPal_2, PCX_PAL_SIZE);

          break;

        case 0x10:      /* 640 x 350 16-color EGA                       */

          memcpy(ega_palp, pcx_EGA_DefPal_3, PCX_PAL_SIZE);

          break;

        default:        /* (Should never reach here)                    */

          break;
      }
    }

    /* Map the EGA/VGA palette to the PCX file header palette           */

    for (i = 0; i < PCX_PAL_SIZE; i++)
    {
      val = (int) ega_palp[i];  /* Get current color palette value      */

      /* Extract color values                                           */

      red = ((val & 0x20) >> 5) | ((val & 0x04) >> 1);
      green = ((val & 0x10) >> 4) | (val & 0x02);
      blue = ((val & 0x08) >> 3) | ((val & 0x01) << 1);

      /* Map each color to a 6-bit value.  Only the top two bits are    */
      /* significant for EGA displays.  The lower four bits (which      */
      /* repeat the top two bits) are significant when the image is     */
      /* presented on a VGA display emulating an EGA display.           */

      palettep[i].red = (BYTE) ((red << 6) | (red << 4) | (red << 2));
      palettep[i].green = (BYTE) ((green << 6) | (green << 4) | (green <<
          2));
      palettep[i].blue = (BYTE) ((blue << 6) | (blue << 4) | (blue << 2));
    }
  }

  free(ega_palp);       /* Free the EGA/VGA palette buffer              */

  return (TRUE);
}

/*
 *************************************************************************
 *
 *  PCX_WRITE_LINE - Write PCX Line
 *
 *  Purpose:    To write an image scan line to a PCX-format image file.
 *
 *  Setup:      static BOOL pcx_write_line
 *              (
 *                unsigned char *linep,
 *                int buflen,
 *                FILE *fp
 *              )
 *
 *  Where:      linep is a PCX scan line buffer pointer.
 *              buflen is the length of the image scan line buffer in
 *                bytes.
 *              fp is a file pointer.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *  Note:       The PCX scan line buffer is assumed to contain the color
 *              plane scan lines in sequence, with padding for an even
 *              number of bytes and trailing "white" data for each line as
 *              appropriate.
 *
 *************************************************************************
 */

static BOOL pcx_write_line
(
  unsigned char *linep,
  int buflen,
  FILE *fp
)
{
  int curr_data;        /* Current data byte                            */
  int prev_data;        /* Previous data byte                           */
  int data_count;       /* Data repeat count                            */
  int line_count;       /* Scan line byte count                         */

  prev_data = *linep++;         /* Initialize the previous data byte    */
  data_count = 1;
  line_count = 1;

  while (line_count < buflen)   /* Encode scan line                     */
  {
    curr_data = *linep++;       /* Get the current data byte            */
    line_count++;               /* Increment the scan line counter      */

    if (curr_data == prev_data)         /* Repeating data bytes ?       */
    {
      data_count++;     /* Increment the data repeat count              */

      /* Check for maximum allowable repeat count                       */

      if (data_count == PCX_COMP_MASK)
      {
        /* Encode the data                                              */

        if (pcx_encode(prev_data, data_count, fp) == FALSE)
          return (FALSE);

        data_count = 0;         /* Reset the data repeat count          */
      }
    }
    else    /* End of repeating data bytes                              */
    {
      if (data_count > 0)
      {
        /* Encode the data                                              */

        if (pcx_encode(prev_data, data_count, fp) == FALSE)
          return (FALSE);
      }

      prev_data = curr_data;    /* Current data byte now previous       */
      data_count = 1;
    }
  }

  if (data_count > 0)   /* Any remaining data ?                         */
  {
    /* Encode the data                                                  */

    if (pcx_encode(prev_data, data_count, fp) == FALSE)
      return (FALSE);
  }

  return (TRUE);
}

/*
 *************************************************************************
 *
 *  PCX_ENCODE - Encode Byte Pair
 *
 *  Purpose:    To write an encoded byte pair (or a single unencoded
 *              byte) to a PCX-format image file.
 *
 *  Setup:      static BOOL pcx_encode
 *              (
 *                int data,
 *                int count,
 *                FILE *fp
 *              )
 *
 *  Where:      data is the data byte to be encoded (if necessary).
 *              count is the number of times the data byte is repeated in
 *                the image data.
 *              fp is a pointer to the PCX-format file the encoded byte
 *                pair or single byte is to be written to.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *************************************************************************
 */

static BOOL pcx_encode
(
  int data,
  int count,
  FILE *fp
)
{
  if (((data & PCX_COMP_FLAG) == PCX_COMP_FLAG) || count > 1)
  {
    /* Write the count byte                                             */

    if (putc(PCX_COMP_FLAG | count, fp) == EOF)
      return (FALSE);
  }

  /* Write the data byte                                                */

  if (putc(data, fp) == EOF)
    return (FALSE);

  return (TRUE);
}

/*
 *************************************************************************
 *
 *  PCX_WRITE_EXTPAL - Write Extended Palette
 *
 *  Purpose:    To read the current 256-color VGA palette and write an
 *              equivalent extended PCX palette to a PCX-format image
 *              file.
 *
 *  Setup:      static BOOL pcx_write_extpal
 *              (
 *                FILE *fp
 *              )
 *
 *  Where:      fp is a file pointer.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *************************************************************************
 */

static BOOL pcx_write_extpal
(
  FILE *fp
)
{
  int i;                        /* Scratch counter                      */
  BOOL status = TRUE;           /* Return status                        */
  PCX_PAL *palettep;            /* Extended PCX palette buffer pointer  */
  unsigned char *vga_palp;      /* 256-color VGA palette buffer pointer */
  union REGS regs;              /* 80x86 register values                */
  struct SREGS sregs;           /* 80x86 segment register values        */

  /* Allocate an extended PCX palette buffer                            */

  if ((palettep = (PCX_PAL *) calloc(sizeof(PCX_PAL), PCX_EPAL_SIZE)) ==
      (PCX_PAL *) NULL)
    return (FALSE);

  /* Allocate a 256-color VGA palette buffer                            */

  if ((vga_palp = (unsigned char *) calloc(sizeof(unsigned char), 768))
      == (unsigned char *) NULL)
  {
    free(palettep);     /* Free the extended PCX palette buffer         */
    return (FALSE);
  }

  /* Read the current VGA palette (DAC registers)                       */

  regs.h.ah = 0x10;     /* Select "Read DAC Registers" BIOS routine     */
  regs.h.al = 0x17;
  regs.x.bx = 0;        /* Read all 256 DAC registers                   */
  regs.x.cx = 256;
  
  /* Get the VGA palette buffer offset value                            */

  regs.x.dx = (unsigned int) vga_palp;

  segread(&sregs);      /* Get the current DS segment register value    */

  sregs.es = sregs.ds;

  int86x(0x10, &regs, &regs, &sregs);   /* Call BIOS video service      */

  /* Map the VGA palette to an extended PCX palette                     */

  for (i = 0; i < PCX_EPAL_SIZE; i++)
  {
    palettep[i].red = (BYTE) (vga_palp[i * 3] << 2);
    palettep[i].green = (BYTE) (vga_palp[i * 3 + 1] << 2);
    palettep[i].blue = (BYTE) (vga_palp[i * 3 + 2] << 2);
  }

  /* Write the extended PCX palette indicator byte to the file          */

  if (status == TRUE)
    if (fputc(PCX_EPAL_FLAG, fp) == EOF)
      status = FALSE;

  /* Write the extended PCX palette to the file                         */

  if (status == TRUE)
    if (fwrite(palettep, sizeof(PCX_PAL), PCX_EPAL_SIZE, fp) !=
        PCX_EPAL_SIZE)
      status = FALSE;

  free(palettep);       /* Free the extended PCX palette buffer         */

  free(vga_palp);       /* Free the VGA palette buffer                  */

  return (status);
}

/*
 *************************************************************************
 *
 *  PCX_GET_HERC - Get Hercules Scan Line
 *
 *  Purpose:    To read a Hercules monochrome graphics display adapter
 *              scan line into a buffer.
 *
 *  Setup:      static void pcx_get_herc
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char _far *linep,
 *                int line_num
 *              )
 *
 *  Where:      wbp is a PCX workblock pointer.
 *              linep is a pointer to where the scan line is to be
 *                returned.
 *              line_num is the scan line number.
 *
 *************************************************************************
 */

static void pcx_get_herc
(
  PCX_WORKBLK *wbp,
  unsigned char _far *linep,
  int line_num
)
{
  unsigned char _far *displayp;         /* Display buffer pointer       */

  /* Calculate Hercules display buffer pointer                          */

  displayp = (unsigned char _far *) (0xb0000000L + wbp->page_offset) +
      ((line_num >> 2) * 90) + 0x2000 * (line_num & 3);

  /* Copy data from the Hercules display buffer to the scan line buffer */

  (void) _fmemcpy(linep, displayp, wbp->num_bytes);

  /* Mask off unseen pixels                                             */

  linep[wbp->num_bytes - 1] |= wbp->mask;

  /* Pad scan line with "white" data byte (if necessary)                */

  if (wbp->num_bytes & 1)
    linep[wbp->num_bytes] = 0xff;
}

/*
 *************************************************************************
 *
 *  PCX_GET_CGA - Get CGA Scan Line
 *
 *  Purpose:    To read a CGA display adapter scan line into a buffer.
 *
 *  Setup:      static void pcx_get_cga
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char _far *linep,
 *                int line_num
 *              )
 *
 *  Where:      wbp is a PCX workblock pointer.
 *              linep is a pointer to where the scan line is to be
 *                returned.
 *              line_num is the scan line number.
 *
 *************************************************************************
 */

static void pcx_get_cga
(
  PCX_WORKBLK *wbp,
  unsigned char _far *linep,
  int line_num
)
{
  unsigned char _far *displayp;         /* Display buffer pointer       */

  /* Calculate CGA display buffer pointer                               */

  displayp = (unsigned char _far *) 0xb8000000L + ((line_num >> 1) * 80)
      + 0x2000 * (line_num & 1);

  /* Copy data from the CGA display buffer to the scan line buffer      */

  (void) _fmemcpy(linep, displayp, wbp->num_bytes);

  /* Mask off unseen pixels                                             */

  linep[wbp->num_bytes - 1] |= wbp->mask;

  /* Pad scan line with "white" data byte (if necessary)                */

  if (wbp->num_bytes & 1)
    linep[wbp->num_bytes] = 0xff;
}

/*
 *************************************************************************
 *
 *  PCX_GET_EGA - Get EGA/VGA Scan Line
 *
 *  Purpose:    To read an EGA/VGA display adapter scan line into a
 *              buffer.
 *
 *  Setup:      static void pcx_get_ega
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char _far *linep,
 *                int line_num
 *              )
 *
 *  Where:      wbp is a PCX workblock pointer.
 *              linep is a pointer to where the scan line is to be
 *                returned.
 *              line_num is the scan line number.
 *
 *************************************************************************
 */

static void pcx_get_ega
(
  PCX_WORKBLK *wbp,
  unsigned char _far *linep,
  int line_num
)
{
  int plane_num;                /* EGA/VGA color plane number           */
  unsigned char _far *displayp; /* Display buffer pointer               */

  /* Calculate buffer pointer                                           */

  displayp = (unsigned char _far *) (0xa0000000L + wbp->page_offset) +
      line_num * 80;

  /* Copy PCX scan line data from each color plane                      */

  for (plane_num = 0; plane_num < (int) wbp->header.nplanes; plane_num++)
  {
    /* Select the current color plane in EGA/VGA Read Mode 0            */

    outpw(0x03ce, (plane_num << 8) | 0x04);

    /* Copy data from the EGA/VGA display to the scan line buffer       */

    (void) _fmemcpy(linep, displayp, wbp->num_bytes);

    /* Mask off unseen pixels                                           */

    linep[wbp->num_bytes - 1] |= wbp->mask;

    /* Pad plane scan line with "white" data byte (if necessary)        */

    if (wbp->num_bytes & 1)
      linep[wbp->num_bytes] = 0xff;

    linep += wbp->header.bppscan;       /* Increment plane offset       */
  }

  /* Select EGA/VGA Write Mode 0 with all color planes enabled          */

  outpw(0x03c4, 0x0f02);
}

/*
 *************************************************************************
 *
 *  PCX_GET_VGA - Get VGA Scan Line
 *
 *  Purpose:    To read a VGA display adapter scan line into a buffer.
 *
 *  Setup:      static void pcx_get_vga
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char _far *linep,
 *                int line_num
 *              )
 *
 *  Where:      wbp is a PCX workblock pointer.
 *              linep is a pointer to where the scan line is to be
 *                returned.
 *              line_num is the scan line number.
 *
 *************************************************************************
 */

static void pcx_get_vga
(
  PCX_WORKBLK *wbp,
  unsigned char _far *linep,
  int line_num
)
{
  unsigned char _far *displayp;         /* Display buffer pointer       */

  /* Calculate buffer pointer                                           */

  displayp = (unsigned char _far *) 0xa0000000L + line_num * 320;

  /* Copy data from the VGA display buffer to the scan line buffer      */

  (void) _fmemcpy(linep, displayp, wbp->num_bytes);

  /* Pad scan line with "white" data byte (if necessary)                */

  if (wbp->num_bytes & 1)
    linep[wbp->num_bytes] = 0xff;
}

