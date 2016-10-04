/*
 *************************************************************************
 *
 *  PCX_DISP.C - PCX_LIB Library Image Display Functions
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
 *          _fmemcpy            - "memcpy" for small model / far data
 *          int86               - execute 80x86 interrupt routine
 *          int86x              - execute 80x86 interrupt routine (far
 *                                data)
 *          _remapallpalette    - remap entire video display color palette
 *          _selectpalette      - select CGA color palette
 *          outpw               - output word to 80x86 I/O port
 *          segread             - get current 80x86 segment register
 *                                values
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
#include <dos.h>
#include <malloc.h>
#include <graph.h>
#include "pcx_int.h"

/*      FORWARD REFERENCES                                              */

static BOOL pcx_read_init(PCX_WORKBLK *, int, int);
static BOOL pcx_read_extpal(PCX_WORKBLK *);
static BOOL pcx_read_header(PCX_WORKBLK *);
static BOOL pcx_read_line(PCX_WORKBLK *, unsigned char *, int);
static BOOL pcx_set_palette(PCX_PAL *, int);

static void pcx_cga_palette(PCX_PAL *, int);
static void pcx_put_cga(PCX_WORKBLK *, unsigned char _far *, int);
static void pcx_put_ega(PCX_WORKBLK *, unsigned char _far *, int);
static void pcx_put_herc(PCX_WORKBLK *, unsigned char _far *, int);
static void pcx_put_vga(PCX_WORKBLK *, unsigned char _far *, int);

/*      GLOBALS                                                         */

/*      PUBLIC FUNCTIONS                                                */

/*
 *************************************************************************
 *
 *  PCX_READ - Read PCX Image File
 *
 *  Purpose:    To read and display a PCX-format image file.
 *
 *  Setup:      BOOL pcx_read
 *              (
 *                char *fname,
 *                int vmode,
 *                int page
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
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *  Note:       The video display adapter must be in the appropriate mode
 *              and active page for the image to be displayed.
 *
 *************************************************************************
 */

BOOL pcx_read
(
  char *fname,
  int vmode,
  int page
)
{
  int bpline;                   /* Number of bytes per scan line        */
  int line_num;                 /* Scan line number                     */
  int max_lines;                /* Maximum number of scan lines         */
  unsigned char *linep;         /* PCX scan line buffer pointer         */
  BOOL status = TRUE;           /* Return status                        */
  PCX_WORKBLK *wbp;             /* PCX image file workblock pointer     */

  /* Open a PCX image file workblock                                    */

  if ((wbp = pcx_open(fname, FALSE)) == (PCX_WORKBLK *) NULL)
    return (FALSE);

  /* Initialize the workblock for reading                               */

  if (pcx_read_init(wbp, vmode, page) == FALSE)
  {
    (void) pcx_close(wbp);      /* Close the PCX workblock              */
    return (FALSE);
  }

  /* Calculate the image height                                         */

  max_lines = wbp->header.yul + wbp->header.ylr + 1;

  /* Calculate number of bytes per line (for all color planes)          */

  bpline = wbp->header.bppscan * wbp->header.nplanes;

  /* Allocate the PCX scan line buffer                                  */

  if ((linep = (unsigned char *) malloc(bpline)) != (unsigned char *)
      NULL)
  {
    /* Set the file pointer to the beginning of the encoded image data  */

    if (status == TRUE)
      if (fseek(wbp->fp, (long) (sizeof(PCX_HDR)), SEEK_SET) != 0)
        status = FALSE;

    /* Set the video display adapter color palette unless the PCX file  */
    /* is Version 3.0 (i.e. - PC Paintbrush Version 2.8 w/o palette)    */

    if (status == TRUE)
      if (wbp->header.version != 3)
        if (pcx_set_palette(wbp->palettep, vmode) == FALSE)
          status = FALSE;

    /* Read the image line by line                                      */

    if (status == TRUE)
    {
      for (line_num = 0; line_num < max_lines; line_num++)
      {
        /* Read the current scan line                                   */

        if ((status = pcx_read_line(wbp, linep, bpline)) == FALSE)
        {
          status = FALSE;
          break;
        }

        /* Display the current scan line                                */

        wbp->pcx_funcp(wbp, (unsigned char _far *) linep, line_num);
      }
    }

    free(linep);        /* Free the PCX scan line buffer                */
  }
  else
    status = FALSE;
                         
  if (pcx_close(wbp) == FALSE)  /* Close the PCX workblock              */
    status = FALSE;

  return (status);
}

/*      PRIVATE FUNCTIONS                                               */

/*
 *************************************************************************
 *
 *  PCX_READ_INIT - Initialize PCX Workblock For Reading
 *
 *  Purpose:    To initialize a PCX image file workblock for reading.
 *
 *  Setup:      static BOOL pcx_read_init
 *              (
 *                PCX_WORKBLK *wbp,
 *                int vmode,
 *                int page
 *              )
 *
 *  Where:      wbp is a PCX workblock pointer.
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
 *                All Other      - 0
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *************************************************************************
 */

static BOOL pcx_read_init
(
  PCX_WORKBLK *wbp,
  int vmode,
  int page
)
{
  int width;                    /* Display width                        */
  int leftover;                 /* Number of unseen bits                */
  BOOL status = TRUE;           /* Return status                        */

  /* Read the file header                                               */

  if ((pcx_read_header(wbp)) == FALSE)
    return (FALSE);

  /* Initialize the workblock color palette pointer                     */

  wbp->palettep = wbp->header.palette;
  wbp->epal_flag = FALSE;

  /* Read the extended palette (if any)                                 */

  if (vmode == 0x13 && wbp->header.version == 5)
    if (pcx_read_extpal(wbp) == FALSE)
      return (FALSE);

  /* Initialize the display page address offset                         */

  wbp->page_offset = (unsigned long) 0L;

  switch (vmode)        /* Select PCX line display function             */
  {
    case PCX_HERC:      /* 720 x 348 Hercules monochrome                */

      /* Hercules monochrome display adapter supports 2 pages           */

      wbp->page_offset = 0x08000000L * (unsigned long) page;

      /* Calculate display width in pixels                              */

      width = min((wbp->header.xlr - wbp->header.xul + 1), 720);

      /* Calculate number of bytes to display                           */

      wbp->num_bytes = (width + 7) >> 3;

      /* Calculate mask for leftover bits                               */

      if ((leftover = width & 7) != 0)
        wbp->mask = (0xff << (8 - leftover)) & 0xff;
      else
        wbp->mask = 0xff;

      wbp->pcx_funcp = pcx_put_herc;    /* Set the display function ptr */

      break;

    case 0x04:          /* 320 x 200 4-color CGA                        */
    case 0x05:          /* 320 x 200 4-color CGA (color burst off)      */
  
      /* Calculate display width in pixels                              */

      width = min((wbp->header.xlr - wbp->header.xul + 1), 320);

      /* Calculate number of bytes to display                           */

      wbp->num_bytes = (width + 3) >> 2;

      /* Calculate mask for leftover bits                               */

      if ((leftover = (width & 3) << 1) != 0)
        wbp->mask = (0xff << (8 - leftover)) & 0xff;
      else
        wbp->mask = 0xff;

      wbp->pcx_funcp = pcx_put_cga;     /* Set the display function ptr */

      break;

    case 0x06:          /* 640 x 200 2-color CGA                        */

      /* Calculate display width in pixels                              */

      width = min((wbp->header.xlr - wbp->header.xul + 1), 640);

      /* Calculate number of bytes to display                           */

      wbp->num_bytes = (width + 7) >> 3;

      /* Calculate mask for leftover bits                               */

      if ((leftover = width & 7) != 0)
        wbp->mask = (0xff << (8 - leftover)) & 0xff;
      else
        wbp->mask = 0xff;

      wbp->pcx_funcp = pcx_put_cga;     /* Set the display function ptr */

      break;

    case 0x0d:          /* 320 x 200 16-color EGA/VGA                   */
    case 0x0e:          /* 640 x 200 16-color EGA/VGA                   */
    case 0x0f:          /* 640 x 350 2-color EGA/VGA                    */
    case 0x10:          /* 640 x 350 16-color EGA/VGA                   */
    case 0x11:          /* 640 x 480 2-color VGA                        */
    case 0x12:          /* 640 x 480 16-color VGA                       */

      switch (vmode)    /* Initialize the display adapter page offset   */
      {
        case 0x0d:      /* 320 x 200 16-color EGA/VGA (8 pages maximum) */

          wbp->page_offset = 0x02000000L * (unsigned long) page;

          break;

        case 0x0e:      /* 640 x 200 16-color EGA/VGA (4 pages maximum) */

          wbp->page_offset = 0x04000000L * (unsigned long) page;

          break;

        case 0x0f:      /* 640 x 350 2-color EGA/VGA (2 pages maximum)  */
        case 0x10:      /* 640 x 350 16-color EGA/VGA (2 pages maximum) */

          wbp->page_offset = 0x08000000L * (unsigned long) page;

          break;

        default:        /* All other modes support only one page        */

          break;
      }

      /* Calculate display width in pixels                              */

      width = min((wbp->header.xlr - wbp->header.xul + 1), 640);

      /* Calculate number of bytes to display                           */

      wbp->num_bytes = (width + 7) >> 3;

      /* Calculate mask for leftover bits                               */

      if ((leftover = width & 7) != 0)
        wbp->mask = (0xff << (8 - leftover)) & 0xff;
      else
        wbp->mask = 0xff;

      wbp->pcx_funcp = pcx_put_ega;     /* Set the display function ptr */

      break;

    case 0x13:          /* 320 x 200 256-color VGA                      */

      /* Calculate number of bytes to display                           */

      wbp->num_bytes = min((wbp->header.xlr - wbp->header.xul + 1), 320);

      wbp->mask = 0;  /* Dummy parameter                                */

      wbp->pcx_funcp = pcx_put_vga;     /* Set the display function ptr */

      break;

    default:            /* Other display adapters not supported         */

      status = FALSE;

      break;
  }

  return (status);
}

/*
 *************************************************************************
 *
 *  PCX_READ_HEADER - Read PCX File Header
 *
 *  Purpose:    To read and validate a PCX file header.
 *
 *  Setup:      static BOOL pcx_read_header
 *              (
 *                PCX_WORKBLK *wbp
 *              )
 *
 *  Where:      wbp is a PCX image file workblock pointer.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *  Result:     The file header is read into the "header" member of the
 *              PCX workblock.
 *
 *************************************************************************
 */

static BOOL pcx_read_header
(
  PCX_WORKBLK *wbp
)
{
  BOOL status = TRUE;   /* Status flag                                  */
  PCX_HDR *hdrp;        /* PCX file header buffer pointer               */

  hdrp = &(wbp->header);        /* Initialize the file header pointer   */

  /* Read the file header                                               */

  if (fseek(wbp->fp, 0L, SEEK_SET) != 0)
    status = FALSE;

  if (status == TRUE)
    if (fread(hdrp, sizeof(PCX_HDR), 1, wbp->fp) != 1)
      status = FALSE;

  /* Validate the PCX file format                                       */

  if (status == TRUE)
    if ((hdrp->pcx_id != 0x0a) || (hdrp->encoding != 1))
      status = FALSE;

  return (status);
}

/*
 *************************************************************************
 *
 *  PCX_READ_EXTPAL - Read Extended Palette
 *
 *  Purpose:    To read an extended (256-color) palette (if it exists).
 *
 *  Setup:      static BOOL pcx_read_extpal
 *              (
 *                PCX_WORKBLK *wbp
 *              )
 *
 *  Where:      wbp is a PCX image file workblock pointer.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *  Note:       It is possible for a PCX image file without an appended
 *              256-color palette to have the value 0x0c as the 769th byte
 *              (the location of the extended palette indicator byte) from 
 *              the end of the file (i.e. - in the encoded image data 
 *              section).  This function will misinterpret the following
 *              768 bytes of encoded image data as an extended palette.
 *
 *              This problem will only occur if an attempt is made to
 *              display a PCX image using the wrong MS-DOS video mode.  It
 *              can be detected by decoding the image data and using 
 *              "ftell" to note the file position of the end of the 
 *              encoded image data section, then comparing it to the file
 *              position of the indicator byte.  If the supposed indicator
 *              byte is located within the encoded image data section, the
 *              indicator byte is invalid and so the file header palette
 *              should be used instead.
 *
 *************************************************************************
 */

static BOOL pcx_read_extpal
(
  PCX_WORKBLK *wbp
)
{
  int indicator;        /* PCX extended palette indicator               */

  /* Position the file pointer to the extended palette indicator byte   */

  if (fseek(wbp->fp, -769L, SEEK_END) != 0)
    return (FALSE);

  /* Read the (assumed) extended palette indicator byte                 */

  if ((indicator = getc(wbp->fp)) == EOF)
    return (FALSE);

  if (indicator == PCX_EPAL_FLAG)       /* Check for indicator byte     */
  {
    /* Allocate an extended palette buffer                              */

    if ((wbp->palettep = (PCX_PAL *) calloc(sizeof(PCX_PAL),
        PCX_EPAL_SIZE)) == (PCX_PAL *) NULL)
      return (FALSE);

    /* Read the extended palette                                        */

    if (fread(wbp->palettep, sizeof(PCX_PAL), PCX_EPAL_SIZE, wbp->fp) !=
        PCX_EPAL_SIZE)
    {
      free(wbp->palettep);      /* Free the extended palette buffer     */
      return (FALSE);
    }

    wbp->epal_flag = TRUE;      /* Indicate extended palette present    */
  }

  return (TRUE);
}

/*
 *************************************************************************
 *
 *  PCX_READ_LINE - Read PCX Line
 *
 *  Purpose:    To read an encoded line (all color planes) from a PCX-
 *              format image file and write the decoded data to a line
 *              buffer.
 *
 *  Setup:      static BOOL pcx_read_line
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char *linep,
 *                int bpline
 *              )
 *
 *  Where:      wbp is a PCX image file workblock pointer.
 *              linep is a PCX scan line buffer pointer.
 *              bpline is the number of bytes per scan line (all color
 *                planes).
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *************************************************************************
 */

static BOOL pcx_read_line
(
  PCX_WORKBLK *wbp,
  unsigned char *linep,
  int bpline
)
{
  int data;             /* Image data byte                              */
  int count;            /* Image data byte repeat count                 */
  int offset = 0;       /* Scan line buffer offset                      */

  while (offset < bpline)       /* Decode current scan line             */
  {
    if ((data = getc(wbp->fp)) == EOF)  /* Get next byte                */
      return (FALSE);

    /* If top two bits of byte are set, lower six bits show how         */
    /* many times to duplicate next byte                                */

    if ((data & PCX_COMP_FLAG) == PCX_COMP_FLAG)
    {
      count = data & PCX_COMP_MASK;     /* Mask off repeat count        */

      if ((data = getc(wbp->fp)) == EOF)        /* Get next byte        */
        return (FALSE);

      memset(linep, data, count);       /* Duplicate byte               */
      linep += count;
      offset += count;
    }
    else
    {
      *linep++ = (unsigned char) data;  /* Copy byte                    */
      offset++;
    }
  }

  return (TRUE);
}

/*
 *************************************************************************
 *
 *  PCX_SET_PALETTE - Set Palette
 *
 *  Purpose:    To set the display palette according to a PCX file
 *              palette.
 *
 *  Setup:      static BOOL pcx_set_palette
 *              (
 *                PCX_PAL *palettep,
 *                int vmode
 *              )
 *
 *  Where:      palettep is a pointer to a PCX file palette.
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
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *************************************************************************
 */

static BOOL pcx_set_palette
(
  PCX_PAL *palettep,
  int vmode
)
{
  int i;                        /* Scratch counter                      */
  int red_lo;                   /* Low red intensity                    */
  int red_hi;                   /* High red intensity                   */
  int green_lo;                 /* Green low intensity                  */
  int green_hi;                 /* Green high intensity                 */
  int blue_lo;                  /* Blue low intensity                   */
  int blue_hi;                  /* Blue high intensity                  */
  unsigned char *ega_palp;      /* EGA 16-color palette buffer pointer  */
  unsigned long *vga_palp;      /* VGA 256-color palette buffer pointer */
  BOOL status = TRUE;           /* Return status                        */
  union REGS regs;              /* 80x86 register values                */
  struct SREGS sregs;           /* 80x86 segment register values        */

  switch (vmode)
  {
    case PCX_HERC:      /* 720 x 348 Hercules monochrome                */

      break;

    case 0x04:          /* 320 x 200 4-color CGA display                */
    case 0x05:          /* 320 x 200 monochrome CGA display (burst off) */
    case 0x06:          /* 640 x 200 2-color CGA display                */

      /* Set the CGA color palette                                      */

      pcx_cga_palette(palettep, vmode);

      break;

    case 0x0d:          /* 320 x 200 16-color EGA/VGA                   */
    case 0x0e:          /* 640 x 200 16-color EGA/VGA                   */
    case 0x0f:          /* 640 x 350 2-color EGA/VGA                    */
    case 0x10:          /* 640 x 350 16-color EGA/VGA                   */
    case 0x11:          /* 640 x 480 2-color VGA                        */
    case 0x12:          /* 640 x 480 16-color VGA                       */

      if (pcx_isvga() == TRUE)  /* Check for VGA display adapter        */
      {
        /* Allocate a 16-color VGA display adapter palette buffer       */

        if ((vga_palp = (unsigned long *) calloc(sizeof(unsigned long),
            PCX_PAL_SIZE)) == (unsigned long *) NULL)
        {
          status = FALSE;
          break;
        }

        /* Map PCX hardware palette to 16-color VGA palette (each color */
        /* value is a "long" with the form:                             */
        /*                                                              */
        /*      00000000-00BBBBBB-00GGGGGG-00RRRRRR                     */
        /*                                                              */
        /* where each color is a 6-bit value.                           */

        for (i = 0; i < PCX_PAL_SIZE; i++)
          vga_palp[i] = (long) (palettep[i].red >> 2) |
              ((long) (palettep[i].green >> 2)) << 8 |
              ((long) (palettep[i].blue >> 2)) << 16;

        (void) _remapallpalette(vga_palp);      /* Remap entire palette */

        free(vga_palp);         /* Free the VGA palette buffer          */
      }
      else      /* EGA display adapter                                  */
      {
        /* Allocate an EGA display adapter palette buffer               */

        if ((ega_palp = (unsigned char *) calloc(sizeof(unsigned char),
            PCX_PAL_SIZE + 1)) == (unsigned char *) NULL)
        {
          status = FALSE;
          break;
        }

        /* Map PCX hardware palette to 16-color EGA palette (each EGA   */
        /* color value is an "unsigned char" with the form:             */
        /*                                                              */
        /*        0  0  R' G' B' R  G  B                                */
        /*                                                              */
        /* where X' is the low-intensity value and X is the high        */
        /* intensity value for the color X.)                            */
        /*                                                              */
        /* NOTE: the "_remapallpalette" function could be used to set   */
        /*       the palette for EGA display adapters.  However, this   */
        /*       function does not appear to update the palette         */
        /*       register values in the Dynamic Save Area (see the      */
        /*       function header for "pcx_init_palette" in PCX_FILE.C)  */
        /*       for a detailed explanation).                           */

        for (i = 0; i < PCX_PAL_SIZE; i++)
        {
          /* Extract low and high intensity bits for each color         */

          red_lo = (palettep[i].red >> 6) & 0x01;
          red_hi = (palettep[i].red >> 6) & 0x02;
          green_lo = (palettep[i].green >> 6) & 0x01;
          green_hi = (palettep[i].green >> 6) & 0x02;
          blue_lo = (palettep[i].blue >> 6) & 0x01;
          blue_hi = (palettep[i].blue >> 6) & 0x02;

          /* Combine color intensity bits for EGA palette value         */

          ega_palp[i] = (unsigned char) ((red_lo << 5) | (green_lo << 4) |
              (blue_lo << 3) | (red_hi << 1) | green_hi | (blue_hi >>
              1));
        }

        /* Set the border (overscan) color to black (BIOS default)      */

        ega_palp[16] = (unsigned char) 0;

        regs.h.ah = 0x10;       /* Select "Set All Palette Registers"   */
        regs.h.al = 0x02;

        /* Get the EGA palette registers buffer offset value            */

        regs.x.dx = (unsigned int) ega_palp;

        segread(&sregs);  /* Get the current DS segment register value  */

        sregs.es = sregs.ds;

        int86x(0x10, &regs, &regs, &sregs);  /* Call BIOS video service */

        free(ega_palp);         /* Free the EGA palette buffer          */
      }

      break;

    case 0x13:          /* 320 x 200 256-color VGA display              */

      /* Allocate a 256-color VGA display adapter palette buffer        */

      if ((vga_palp = (unsigned long *) calloc(sizeof(unsigned long),
          PCX_EPAL_SIZE)) == (unsigned long *) NULL)
      {
        status = FALSE;
        break;
      }

      /* Map PCX extended palette to 256-color VGA palette              */

      for (i = 0; i < PCX_EPAL_SIZE; i++)
        vga_palp[i] = (long) (palettep[i].red >> 2) |
            ((long) (palettep[i].green >> 2)) << 8 |
            ((long) (palettep[i].blue >> 2)) << 16;

      (void) _remapallpalette(vga_palp);        /* Remap entire palette */

      free(vga_palp);   /* Free the VGA palette buffer                  */

      break;

    default:            /* Other modes not supported                    */

      status = FALSE;

      break;
  }

  return (status);
}

/*
 *************************************************************************
 *
 *  PCX_CGA_PALETTE - Select CGA Palette
 *
 *  Purpose:    To set the Color Graphics Adapter (CGA) display palette
 *              according to a PCX file palette.
 *
 *  Setup:      static void pcx_cga_palette
 *              (
 *                PCX_PAL *palettep,
 *                int vmode
 *              )
 *
 *  Where:      palettep is a pointer to a PCX file palette.
 *              vmode is the MS-DOS video mode.  Valid values are:
 *
 *                0x04 -        320 x 200 4-color CGA
 *                0x05 -        320 x 200 4-color CGA (color burst off)
 *                0x06 -        640 x 200 2-color CGA
 *
 *  Note:       ZSoft's PC Paintbrush products no longer support the CGA
 *              color palette.  When a CGA color palette is encountered,
 *              PC Paintbrush maps it to a monochrome (black and white)
 *              palette.
 *
 *              MS-DOS video mode 0x05 (320 x 200 monochrome CGA display,
 *              color burst off) will only display a monochrome image on
 *              a composite video monitor (typically a television set with
 *              an RF adapter).  All other monitors will display a color
 *              palette in this mode (which is different for CGA and EGA
 *              or VGA display adapters).
 *
 *              The "background" color is actually the foreground color
 *              (i.e. - the color of activated pixels) for MS-DOS video
 *              mode 0x06.  The background color is black for CGA display
 *              adapters.
 *
 *************************************************************************
 */

static void pcx_cga_palette
(
  PCX_PAL *palettep,
  int vmode
)
{
  short pal_num;        /* Palette number                               */
  BOOL sel_flag;        /* Palette selector bit flag                    */
  BOOL int_flag;        /* Intensity bit flag                           */
  union REGS regs;      /* 80x86 register values                        */

  /* Set the background color                                           */

  regs.h.ah = 0x0b;     /* Select "Set Color Palette" BIOS routine      */
  regs.h.bh = 0;

  regs.h.bl = (unsigned char) PCX_CGA_BKGND(palettep);

  int86(0x10, &regs, &regs);    /* Call BIOS video service              */

  if (vmode != 0x06)    /* Select the CGA color palette                 */
  {
    /* Check the palette selector bit flag                              */

    sel_flag = PCX_CGA_SELECT(palettep) ? TRUE : FALSE;

    /* Check the intensity bit flag                                     */

    int_flag = PCX_CGA_INTENSITY(palettep) ? TRUE : FALSE;

    /* Determine the CGA palette number                                 */

    if (int_flag == TRUE)       /* Intensity = bright                   */
    {
      if (sel_flag == TRUE)
        pal_num = 3;    /* Light cyan - light magenta - white           */
      else
        pal_num = 1;    /* Cyan - magenta - light grey                  */
    }
    else                        /* Intensity = dim                      */
    {
      if (sel_flag == TRUE)
        pal_num = 2;    /* Light green - light red - yellow             */
      else
        pal_num = 0;    /* Green - red - brown                          */
    }

    /* Select the foreground color palette                              */

    (void) _selectpalette(pal_num);
  }
}

/*
 *************************************************************************
 *
 *  PCX_PUT_HERC - Display Hercules PCX Line
 *
 *  Purpose:    To copy a decoded PCX image scan line to a Hercules
 *              monochrome graphics display adapter buffer.
 *
 *  Setup:      static void pcx_put_herc
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char _far *linep,
 *                int line_num
 *              )
 *
 *  Where:      wbp is a PCX image file workblock pointer.
 *              linep is a PCX scan line buffer pointer.
 *              line_num is the scan line number.
 *
 *************************************************************************
 */

void pcx_put_herc
(
  PCX_WORKBLK *wbp,
  unsigned char _far *linep,
  int line_num
)
{
  unsigned char _far *displayp;         /* Display buffer pointer       */

  /* Mask off unseen pixels                                             */

  linep[wbp->num_bytes - 1] &= wbp->mask;

  /* Calculate Hercules display buffer pointer                          */

  displayp = (unsigned char _far *) (0xb0000000L + wbp->page_offset) +
      ((line_num >> 2) * 90) + 0x2000 * (line_num & 3);

  /* Copy data from the scan line buffer to the Hercules display buffer */

  (void) _fmemcpy(displayp, linep, wbp->num_bytes);
}

/*
 *************************************************************************
 *
 *  PCX_PUT_CGA - Display CGA PCX Line
 *
 *  Purpose:    To copy a decoded PCX image scan line to a CGA display
 *              adapter buffer.
 *
 *  Setup:      static void pcx_put_cga
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char _far *linep,
 *                int line_num
 *              )
 *
 *  Where:      wbp is a PCX image file workblock pointer.
 *              linep is a PCX scan line buffer pointer.
 *              line_num is the scan line number.
 *
 *************************************************************************
 */

static void pcx_put_cga
(
  PCX_WORKBLK *wbp,
  unsigned char _far *linep,
  int line_num
)
{
  unsigned char _far *displayp;         /* Display buffer pointer       */

  /* Mask off unseen pixels                                             */

  linep[wbp->num_bytes - 1] &= wbp->mask;

  /* Calculate CGA display buffer pointer                               */

  displayp = (unsigned char _far *) 0xb8000000L + ((line_num >> 1) * 80)
      + 0x2000 * (line_num & 1);

  /* Copy data from the scan line buffer to the CGA display buffer      */

  (void) _fmemcpy(displayp, linep, wbp->num_bytes);
}

/*
 *************************************************************************
 *
 *  PCX_PUT_EGA - Display EGA/VGA PCX Line
 *
 *  Purpose:    To copy a decoded PCX image scan line to an EGA/VGA
 *              display adapter buffer.
 *
 *  Setup:      static void pcx_put_ega
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char _far *linep,
 *                int line_num
 *              )
 *
 *  Where:      wbp is a PCX image file workblock pointer.
 *              linep is a PCX scan line buffer pointer.
 *              line_num is the scan line number.
 *
 *************************************************************************
 */

static void pcx_put_ega
(
  PCX_WORKBLK *wbp,
  unsigned char _far *linep,
  int line_num
)
{
  int plane_num;                /* EGA/VGA color plane number           */
  int plane_mask;               /* EGA/VGA color plane mask             */
  unsigned char _far *displayp; /* Display buffer pointer               */

  /* Calculate buffer pointer                                           */

  displayp = (unsigned char _far *) (0xa0000000L + wbp->page_offset) +
      line_num * 80;

  outpw(0x03ce, 0x0005);        /* Select EGA/VGA write mode 0          */

  /* Copy PCX scan line data to each color plane                        */

  plane_mask = 0x0100;          /* Start with color plane 0             */

  for (plane_num = 0; plane_num < (int) wbp->header.nplanes; plane_num++)
  {
    /* Mask off unseen pixels                                           */

    linep[wbp->num_bytes - 1] &= wbp->mask;

    outpw(0x03c4, plane_mask + 2);      /* Select current color plane   */

    /* Copy data from the scan line buffer to the EGA/VGA display       */

    (void) _fmemcpy(displayp, linep, wbp->num_bytes);

    linep += wbp->header.bppscan;       /* Increment plane offset       */

    plane_mask <<= 1;   /* Sequence plane mask                          */
  }

  outpw(0x03c4, 0x0f02);        /* Select all color planes              */
}

/*
 *************************************************************************
 *
 *  PCX_PUT_VGA - Display VGA PCX Line
 *
 *  Purpose:    To copy a decoded PCX image scan line to a VGA display
 *              adapter buffer.
 *
 *  Setup:      static void pcx_put_vga
 *              (
 *                PCX_WORKBLK *wbp,
 *                unsigned char _far *linep,
 *                int line_num
 *              )
 *
 *  Where:      wbp is a PCX image file workblock pointer.
 *              linep is a PCX scan line buffer pointer.
 *              line_num is the scan line number.
 *
 *************************************************************************
 */

static void pcx_put_vga
(
  PCX_WORKBLK *wbp,
  unsigned char _far *linep,
  int line_num
)
{
  unsigned char _far *displayp;         /* Display buffer pointer       */

  /* Calculate buffer pointer                                           */

  displayp = (unsigned char _far *) 0xa0000000L + line_num * 320;

  /* Copy data from the scan line buffer to the VGA display buffer      */

  (void) _fmemcpy(displayp, linep, wbp->num_bytes);
}

