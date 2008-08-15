/*
 *************************************************************************
 *
 *  PCX_COMM.C - PCX_LIB Library Common Functions
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
 *          int86x      - execute 80x86 interrupt routine (far data)
 *          segread     - get current 80x86 segment register values
 *
 *************************************************************************
 */

/*      INCLUDE FILES                                                   */

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include "pcx_int.h"

/*      FORWARD REFERENCES                                              */

/*      GLOBALS                                                         */

/*      PUBLIC FUNCTIONS                                                */

/*
 *************************************************************************
 *
 *  PCX_OPEN - Open PCX Workblock
 *
 *  Purpose:    To allocate and initialize a PCX image file workblock.
 *
 *  Setup:      PCX_WORKBLK *pcx_open
 *              (
 *                char *fname,
 *                BOOL wrt_flag
 *              )
 *
 *  Where:      fname is a PCX image file name.
 *              wrt_flag is a Boolean flag which if TRUE indicates that
 *                the PCX image file is to be opened for writing;
 *                otherwise it is opened for reading.
 *
 *  Return:     A pointer to a PCX image file workblock if successful;
 *              otherwise NULL.
 *
 *************************************************************************
 */

PCX_WORKBLK *pcx_open
(
  char *fname,
  BOOL wrt_flag
)
{
  PCX_WORKBLK *wbp;     /* PCX image file workblock pointer             */

  /* Allocate a workblock                                               */

  if ((wbp = (PCX_WORKBLK *) malloc(sizeof(PCX_WORKBLK))) == NULL)
    return (NULL);

  /* Open the PCX image file in binary mode                             */

  if (wrt_flag == FALSE)
    wbp->fp = fopen(fname, "rb");       /* Open for reading             */
  else
    wbp->fp = fopen(fname, "wb");       /* Open for writing             */

  if (wbp->fp == NULL)  /* Check for successful file opening            */
  {
    free(wbp);          /* Free the workblock memory                    */
    return (NULL);
  }

  return (wbp);         /* Return the workblock pointer                 */
}

/*
 *************************************************************************
 *
 *  PCX_CLOSE - Close PCX Workblock
 *
 *  Purpose:    To close a PCX image file and release its workblock
 *              memory.
 *
 *  Setup:      BOOL pcx_close
 *              (
 *                PCX_WORKBLK *wbp
 *              )
 *
 *  Where:      wbp is a PCX image file workblock pointer.
 *
 *  Return:     TRUE if successful; otherwise FALSE.
 *
 *************************************************************************
 */

BOOL pcx_close
(
  PCX_WORKBLK *wbp
)
{
  free(wbp->palettep);  /* Free the extended palette (if it exists)     */

  free(wbp);            /* Free the PCX image file workblock            */

  if (fclose(wbp->fp) == EOF)   /* Close the PCX image file             */
    return (FALSE);

  return (TRUE);
}

/*
 *************************************************************************
 *
 *  PCX_ISVGA - Check For VGA Display Adapter
 *
 *  Purpose:    To determine whether a display adapter supports VGA BIOS
 *              service routines.
 *
 *  Setup:      BOOL pcx_isvga(void)
 *
 *  Return:     TRUE if display adapter is VGA-compatible; otherwise
 *              FALSE.
 *
 *************************************************************************
 */

BOOL pcx_isvga(void)
{
  unsigned char *vinfop;        /* VGA information buffer pointer       */
  union REGS regs;              /* 80x86 register values                */
  struct SREGS sregs;           /* 80x86 segment register values        */

  /* Allocate a VGA functionality/state information buffer              */

  if ((vinfop = (unsigned char *) malloc(sizeof(unsigned char) * 64)) ==
      NULL)
    return (FALSE);

  /* Attempt to read the VGA information                                */

  regs.h.ah = 0x1b;     /* Select "Return VGA Info" BIOS routine        */
  regs.x.bx = 0;        /* Implementation type                          */

  /* Get the VGA information buffer offset value                        */

  regs.x.di = (unsigned int) vinfop;

  segread(&sregs);      /* Get the current DS segment register value    */

  sregs.es = sregs.ds;

  int86x(0x10, &regs, &regs, &sregs);   /* Call BIOS video service      */

  free(vinfop);         /* Free the VGA information buffer              */

  /* The value 0x1b is returned in register AL only if a VGA display    */
  /* adapter is present                                                 */

  if (regs.h.al == 0x1b)
    return (TRUE);
  else
    return (FALSE);
}

