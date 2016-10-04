/*
 *************************************************************************
 *
 *  PCX_EXAM.C - PCX_LIB File Header Examination Utility
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

/*
 *************************************************************************
 *
 *  PORTABILITY NOTES
 *
 *  1.  When porting this program to other processors, remember that words
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
 ************************************************************************* 
 */

/*      INCLUDE FILES                                                   */

#include <stdio.h>
#include <stdlib.h>
#include "pcx_int.h"

/*      FORWARD REFERENCES                                              */

/*      GLOBALS                                                         */

/*      PUBLIC FUNCTIONS                                                */

/*
 *************************************************************************
 *
 *  MAIN - Executive Function
 *
 *  Purpose:    To read and display a PCX-format image file header.
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
 *  Result:     The file header information is written to "stdout".
 *
 *  Note:       Usage is:
 *
 *                PCX_EXAM filename
 *
 *              where "filename" is the name of a PCX-format image file.
 *
 *************************************************************************
 */

int main
(
  int argc,
  char **argv
)
{
  char *vers;                   /* Version number string pointer        */
  char *pal_type;               /* Palette type string pointer          */
  int range;                    /* Palette range                        */
  int indicator;                /* Extended palette indicator byte      */
  BOOL status = TRUE;           /* Return status                        */
  BOOL ep_flag = FALSE;         /* Extended palette flag                */
  FILE *fp;                     /* File pointer                         */
  PCX_HDR *hdrp;                /* File header structure pointer        */
  PCX_PAL *ext_palettep;        /* Extended palette pointer             */

  /* Display program title                                              */

  puts("\nPCX_EXAM - PCX Image File Examination Utility\n");

  /* Check for filename                                                 */

  if (argc < 2)
  {
    /* Display usage information                                        */

    fputs("  Synopsis:  This public domain utility displays inf", stderr);
    fputs("ormation contained in the\n             header of a ", stderr);
    fputs("Paintbrush (R) PCX-format image file.\n\n  Usage:   ", stderr);
    fputs("  PCX_EXAM filename\n\n  Example:   PCX_EXAM picture", stderr);
    fputs(".pcx\n", stderr);

    return (2);
  }

  /* Allocate a PCX file header buffer                                  */

  if ((hdrp = (PCX_HDR *) malloc(sizeof(PCX_HDR))) == (PCX_HDR *) NULL)
  {
    fputs("ERROR: out of memory\n", stderr);
    return (2);
  }

  /* Open the PCX image file in binary mode                             */

  if ((fp = fopen(argv[1], "rb")) == (FILE *) NULL)
  {
    fprintf(stderr, "ERROR: cannot open file %s\n", argv[1]);

    free(hdrp);         /* Free the file header memory                  */

    return (2);
  }

  /* Read the file header                                               */

  if (status == TRUE)
    if (fseek(fp, 0L, SEEK_SET) != 0)
    {
      fprintf(stderr, "ERROR: cannot read file %s\n", argv[1]);
      status = FALSE;
    }

  if (status == TRUE)
    if (fread(hdrp, sizeof(PCX_HDR), 1, fp) != 1)
    {
      fprintf(stderr, "ERROR: cannot read file %s\n", argv[1]);
      status = FALSE;
    }

  /* Validate the PCX file format                                       */

  if (status == TRUE)
    if ((hdrp->pcx_id != 0x0a) || (hdrp->encoding != 1))
    {
      fprintf(stderr, "ERROR: file %s not valid PCX format\n", argv[1]);
      status = FALSE;
    }

  /* Determine the version number                                       */

  switch (hdrp->version)
  {
    case 0:

      vers = "PC Paintbrush 2.5";

      break;

    case 2:

      vers = "PC Paintbrush 2.8 (with palette information)";

      break;

    case 3:

      vers = "PC Paintbrush 2.8 (without palette information)";

      break;

    case 4:

      vers = "PC Paintbrush for Windows (not 3.0)";

      break;

    case 5:

      vers = "PC Paintbrush 3.0 and greater";

      break;

    default:

      vers = "Unknown version";

      break;
  }

  /* Display the PCX file header information                            */

  printf("PCX filename: %s\n", argv[1]);
  printf("Version: %s\n", vers);
  printf("Encoding: %s\n", hdrp->encoding == 1 ? "Run length" :
      "Unknown");
  printf("%d bits per pixel\n", hdrp->bppixel);
  printf("Image from (%d, %d) to (%d, %d) pixels.\n", hdrp->xul,
      hdrp->yul, hdrp->xlr, hdrp->ylr);
  printf("Created on a device with %d x %d dpi resolution\n",
      hdrp->horz_res, hdrp->vert_res);
  printf("Number of color planes: %d\n", hdrp->nplanes);
  printf("Bytes per color plane scan line: %d\n", hdrp->bppscan);

  switch (hdrp->palette_type & PCX_PAL_MASK)
  {
    case 1:

      pal_type = "color or B&W";

      break;

    case 2:

      pal_type = "grayscale";

      break;

    default:

      pal_type = "unknown";

      break;
  }

  printf("Palette type: %s\n", pal_type);

  /* Check for extended (256-color) palette                             */

  if (hdrp->version == 5)
  {
    /* Check for valid palette indicator byte                           */

    if (fseek(fp, -769L, SEEK_END) != 0)
    {
      fprintf(stderr, "ERROR: cannot read file %s\n", argv[1]);
      status = FALSE;
    }

    if (status == TRUE)
    {
      if ((indicator = getc(fp)) == PCX_EPAL_FLAG)
      {
        /* Allocate an extended palette buffer                          */

        if ((ext_palettep = (PCX_PAL *) calloc(sizeof(PCX_PAL),
            PCX_EPAL_SIZE)) == (PCX_PAL *) NULL)
        {
          fputs("ERROR: out of memory\n", stderr);
          status = FALSE;
        }

        /* Read the extended palette                                    */

        if (status == TRUE)
        {
          if (fread(ext_palettep, sizeof(PCX_PAL), PCX_EPAL_SIZE, fp) !=
              PCX_EPAL_SIZE)
          {
            free(ext_palettep);         /* Free extended palette buffer */
            status = FALSE;
          }
        }

        ep_flag = TRUE;         /* Indicate extended palette exists     */
      }
    }
  }

  if (status == TRUE)
    if (ep_flag == TRUE)
    {
      /* Display extended (256-color) palette                           */

      puts("Extended 256-color palette:\n");

      puts("COLOR   RED     GREEN   BLUE\n");

      for (range = 0; range < PCX_EPAL_SIZE; range++)
        printf(" %03d      %2x      %2x     %2x\n", range,
            ext_palettep[range].red, ext_palettep[range].green,
            ext_palettep[range].blue);

      putchar('\n');

      free(ext_palettep);       /* Free the extended palette            */
    }
    else
    {
      /* Display file header palette                                    */

      puts("File header color palette:\n");

      printf("RED ...   ");

      for (range = 0; range < PCX_PAL_SIZE; range++)
        printf("%2x  ", hdrp->palette[range].red);

      printf("\nGREEN ... ");

      for (range = 0; range < PCX_PAL_SIZE; range++)
        printf("%2x  ", hdrp->palette[range].green);

      printf("\nBLUE ...  ");

      for (range = 0; range < PCX_PAL_SIZE; range++)
        printf("%2x  ", hdrp->palette[range].blue);

      putchar('\n');
    }

  if (fclose(fp) == EOF)        /* Close the file                       */
  {
    fprintf(stderr, "Error: cannot close file %s\n", argv[1]);
    status = FALSE;
  }

  free (hdrp);          /* Free the file header buffer                  */

  return (0);
}

