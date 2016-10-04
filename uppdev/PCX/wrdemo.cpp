#include "pcx.h"
/*
 *************************************************************************
 *
 *  WR_DEMO.C - PCX_LIB PCX Image File Write Demonstration Program
 *
 *  Version:    1.00B
 *
 *  History:    91/02/14 - Created
 *              91/04/01 - Release 1.00A
 *              91/04/06 - Release 1.00B
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

/*      INCLUDE FILES                                                   */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graph.h>
#include "pcx_ext.h"

/*      FORWARD REFERENCES                                              */

/*      GLOBALS                                                         */

static char *use_msg[] =        /* Program usage message                */
{
  "  Synopsis:  This public domain program displays a Paintbrush (R) PCX",
  "-format\n             image file, then captures the image directly fr",
  "om the display\n             and writes it to the file PCX_DEMO.PCX.",
  "\n\n  Usage:     WR_DEMO filename video_mode\n\n             where \"",
  "filename\" is the name of the PCX-format image file to be\n          ",
  "   written and \"video_mode\" is an MS-DOS video mode.  Valid values",
  "\n             are:\n\n                4   - 320 x 200 4-color CGA\n ",
  "               5   - 320 x 200 4-color CGA (color burst off)\n       ",
  "         6   - 640 x 200 2-color CGA\n               13   - 320 x 200",
  " 16-color EGA/VGA\n               14   - 640 x 200 16-color EGA/VGA\n",
  "               15   - 640 x 350 2-color EGA/VGA\n               16   ",
  "- 640 x 350 16-color EGA/VGA\n               17   - 640 x 480 2-color",
  " VGA\n               18   - 640 x 480 16-color VGA\n               19",
  "   - 320 x 200 256-color VGA\n",
  (unsigned char *) NULL
};

/*      PUBLIC FUNCTIONS                                                */

/*
 *************************************************************************
 *
 *  MAIN - Executive Function
 *
 *  Purpose:    To write a PCX-format image file.
 *
 *  Setup:      int main
 *              (
 *                int argc,
 *                char **argv
 *              )
 *
 *  Where:      argc is the number of command-line arguments.
 *              argv is a pointer to an array of command-line argument
 *                strings.
 *
 *  Return:     0 if successful; otherwise 2.
 *
 *  Note:       Usage is:
 *
 *                WR_DEMO filename video_mode
 *
 *              where:
 *
 *                filename is the name of a PCX-format image file.
 *                video_mode is the MS-DOS video mode.  Valid values are:
 *
 *                    4 -        320 x 200 4-color CGA
 *                    5 -        320 x 200 4-color CGA (color burst off)
 *                    6 -        640 x 200 2-color CGA
 *                   13 -        320 x 200 16-color EGA/VGA
 *                   14 -        640 x 200 16-color EGA/VGA
 *                   15 -        640 x 350 2-color EGA/VGA
 *                   16 -        640 x 350 16-color EGA/VGA
 *                   17 -        640 x 480 2-color VGA
 *                   18 -        640 x 480 16-color VGA
 *                   19 -        320 x 200 256-color VGA
 *
 *************************************************************************
 */

int main
(
  int argc,
  char **argv
)
{
  int i;                /* Scratch counter                              */
  int vmode;            /* Video mode                                   */
  BOOL status = FALSE;  /* Return status                                */
  PCX_VSB *vsbp;        /* Video services data save buffer ptr          */

  vsbp = (PCX_VSB *) NULL;      /* Initialize video services buffer ptr */

  /* Display program title                                              */

  puts("\nWR_DEMO - PCX Image File Write Demonstration Program\n");

  if (argc == 3)        /* Check for filename and video mode parameters */
  {
    vmode = atoi(argv[2]);      /* Get the video mode                   */

    /* Validate the video mode (must be valid MS-DOS graphics mode)     */

    if ((vmode >= 4 && vmode <= 6) || (vmode >= 13 && vmode <= 19))
      status = TRUE;
  }

  if (status == TRUE)
  {
    if (_setvideomode(vmode) == 0)      /* Set the video mode           */
    {
      /* Report error                                                   */

      fprintf(stderr,
          "ERROR: could not set display adapter to mode %d.\n", vmode);

      return (2);
    }

    if (vmode >= 13 && vmode <= 18)     /* EGA-compatible video mode ?  */
    {
      if (pcx_isvga() == TRUE)  /* Is a VGA display present ?           */
      {
        /* An EGA display adapter is present - instantiate a Dynamic    */
        /* Save Area buffer to capture the color palette settings each  */
        /* time the palette is updated                                  */

        status = pcx_init_dsa(&vsbp);
      }
    }

    if (status == TRUE)
    {
      /* Read and display the file (assume video page zero)             */

      if ((status = pcx_read(argv[1], vmode, 0)) == TRUE)
      {
        /* Capture the entire screen as a PCX-format file               */

        status = pcx_write("PCX_DEMO.PCX", vmode, 0, 640, 480);

        (void) _setvideomode(_DEFAULTMODE);     /* Reset the video mode */

        if (status == FALSE)
        {
          /* Report error                                               */

          fprintf(stderr,
              "\nWR_DEMO - PCX Image File Write Demonstration");
          fprintf(stderr,
              " Program\n\nERROR: Could not write file %s.\n", argv[1]);
        }
      }
      else
      {
        (void) _setvideomode(_DEFAULTMODE);     /* Reset the video mode */

        /* Report error                                                 */

        fprintf(stderr, "\nWR_DEMO - PCX Image File Write Demonstration");
        fprintf(stderr, " Program\n\nERROR: Could not read file %s.\n",
            argv[1]);
      }

      /* Free the Dynamic Save Area (if necessary)                      */

      if (vsbp != (PCX_VSB *) NULL)
        pcx_free_dsa(vsbp);
    }
    else
    {
      (void) _setvideomode(_DEFAULTMODE);       /* Reset the video mode */

      fputs("ERROR: out of memory.\n", stderr);         /* Report error */
    }
  }
  else          /* Display usage information                            */
  {
    while (use_msg[i] != (unsigned char *) NULL)
      fputs(use_msg[i++], stderr);
  }

  if (status == TRUE)
    return (0);
  else
    return (2);
}

