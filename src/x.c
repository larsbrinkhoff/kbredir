#include "../config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "x.h"

static int
error_handler (Display *display, XErrorEvent *event)
{
  char error[100];
  XGetErrorText (display, event->error_code, error, sizeof error);
  fprintf (stderr, "[serial %ld request %d/%d error: %s]\n",
	   event->serial, event->request_code, event->minor_code, error);
  return 0;
}

static int
io_error_handler (Display *display)
{
  fprintf (stderr, "[an i/o error occurred on display %s]\n",
	   DisplayString (display));
  return 0;
}

void
init_destination (struct destination *dest, int *argc, char **argv)
{
  char *display = NULL;
  Window window = InputFocus;
  int i;

  for (i = 1; i < *argc; i++)
    {
      int skip = 0;

      if (strcmp (argv[i], "--display") == 0)
	{
	  if (i + 1 >= *argc)
	    {
	      fprintf (stderr, "%s: --display takes an argument\n", argv[0]);
	      exit (1);
	    }

	  display = strdup (argv[i + 1]);
	  if (display == NULL)
	    {
	      fprintf (stderr, "%s: out of memory\n", argv[0]);
	      exit (1);
	    }
	  skip = 2;
	}
      else if (strcmp (argv[i], "--window") == 0)
	{
	  char *end;

	  if (i + 1 >= *argc)
	    {
	      fprintf (stderr, "%s: --window takes an argument\n", argv[0]);
	      exit (1);
	    }

	  if (strcmp (argv[i + 1], "InputFocus") == 0)
	    window = InputFocus;
	  else
	    {
	      window = strtol (argv[i + 1], &end, 0);
	      if (end == argv[i + 1] || *end != 0)
		{
		  fprintf (stderr,
			   "%s: --window argument must be numeric "
			   "or \"InputFocus\"\n",
                           argv[0]);
		  exit (1);
		}
	    }
	  skip = 2;
	}

      memmove (&argv[i],
	       &argv[i + skip],
	       sizeof (char *) * (*argc - i - skip));
      (*argc) -= skip;
    }

  dest->display = XOpenDisplay (display);
  if (dest->display == NULL)
    {
      if (display)
	fprintf (stderr, "%s: Couldn't connect to display %s\n",
		 argv[0], display);
      else
	fprintf (stderr, "%s: Couldn't connect to display\n", argv[0]);
      exit (1);
    }

  dest->root = DefaultRootWindow (dest->display);
  dest->shift = XKeysymToKeycode (dest->display, XK_Shift_L);
  dest->control = XKeysymToKeycode (dest->display, XK_Control_L);
  dest->meta = XKeysymToKeycode (dest->display, XK_Meta_L);
  dest->window = window;

  XSetErrorHandler (error_handler);
  XSetIOErrorHandler (io_error_handler);

  make_tab (dest->display);

  XSync (dest->display, True);
}
