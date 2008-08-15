#include <Core/Core.h>

#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture

#include <X11/Xlib.h>

#undef Picture
#undef Time
#undef Font
#undef Display

CONSOLE_APP_MAIN
{
  XDisplay *display;
  Window window;
  char *locale, *modifiers;
  XIC xic;
  XIM xim;
  XIMStyles *xim_styles;
  XIMStyle xim_style;
  char *imvalret;
  char *buffer;
  int bsize;
  int i;

//  locale = setlocale (LC_ALL, "");
  locale = setlocale (LC_ALL, "en_US.utf8");
  if (locale == NULL)
    {
      fprintf (stderr, "setlocale failed\n");
      exit (EXIT_FAILURE);
    }

  if (! XSupportsLocale ())
    {
      fprintf (stderr, "X does not support locale \"%s\"\n", locale);
      exit (EXIT_FAILURE);
    }

  display = XOpenDisplay (NULL);
  if (display == NULL)
    {
      fprintf (stderr, "XOpenDisplay failed\n");
      exit (EXIT_FAILURE);
    }

  /* we're testing the default input method */
  modifiers = XSetLocaleModifiers ("@im=none");
  if (modifiers == NULL)
    {
      fprintf (stderr, "XSetLocaleModifiers failed\n");
      exit (EXIT_FAILURE);
    }

  xim = XOpenIM (display, NULL, NULL, NULL);
  if (xim == NULL)
    {
      fprintf (stderr, "XOpenIM failed\n");
      exit (EXIT_FAILURE);
    }
/*
  imvalret = XGetIMValues (xim, XNQueryInputStyle, &xim_styles, NULL);
  if (imvalret != NULL || xim_styles == NULL)
    {
      fprintf (stderr, "input method doesn't support any styles\n");
      exit (EXIT_FAILURE);
    }

  xim_style = 0;
  for (i = 0;  i < xim_styles->count_styles;  i++)
    {
      if (xim_styles->supported_styles[i] == (XIMPreeditNothing | XIMStatusNothing))
        {
          xim_style = xim_styles->supported_styles[i];
          break;
        }
    }

  if (xim_style == 0)
    {
      fprintf (stderr, "input method doesn't support the style we support\n");
      exit (EXIT_FAILURE);
    }

  XFree (xim_styles);
*/
  window = XCreateSimpleWindow (display, DefaultRootWindow (display),
                                0, 0, 200, 50, 0, 0, 0);
  XSelectInput (display, window, KeyPressMask);
  XMapWindow (display, window);

  xic = XCreateIC (xim,
                   XNInputStyle, (XIMPreeditNothing | XIMStatusNothing)/*xim_style*/,
                   XNClientWindow, window,
                   XNFocusWindow, window,
                   NULL);
  if (xic == NULL)
    {
      fprintf (stderr, "XCreateIC failed\n");
      exit (EXIT_FAILURE);
    }

  bsize = 8;
  buffer = (char *)malloc (bsize);

  for (;;)
    {
      KeySym keysym;
      Status status;
      XEvent event;
      int n;

      XNextEvent (display, &event);

      switch (event.type)
        {
          case KeyPress:

            /* have to do this to handle dead keys and composes */
            if (XFilterEvent (&event, window))
              break;

          try_again:

            /* we want the string to be in the locale encoding */
            n = XmbLookupString (xic, &event.xkey, buffer, bsize - 1, &keysym, &status);
            buffer[n] = '\0';

            switch (status)
              {
                case XBufferOverflow:
                  bsize = n + 1;
                  buffer = (char *) realloc (buffer, bsize);
                  goto try_again;

                case XLookupNone:
                  printf ("key press: XLookupNone\n");
                  break;

                case XLookupChars:
                  LOG("key press: XLookupChars: " << buffer);
                  break;

                case XLookupKeySym:
                  LOG ("key press: XLookupKeySym: " << XKeysymToString (keysym));
                  break;

                case XLookupBoth:
                  LOG ("key press: XLookupBoth: " << XKeysymToString (keysym) << buffer);
                  break;

                default:
                  LOG ("key press: unexpected status\n");
                  break;
              }

            break;
        }
    }

}
