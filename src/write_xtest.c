#include "../config.h"

#if defined (HAVE_X11_XLIB_H) && defined (HAVE_X11_KEYSYM_H) && \
    defined (HAVE_X11_EXTENSIONS_XTEST_H)

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include "x.h"
#include "protocol.h"

static void
press_key (Display *display, KeyCode keycode)
{
  XTestFakeKeyEvent (display, keycode, True, 0);
}

static void
release_key (Display *display, KeyCode keycode)
{
  XTestFakeKeyEvent (display, keycode, False, 0);
}

static void
send_key (struct destination *dest, struct key *key)
{
  int shift, control, meta;
  Window focus, rw, cw;
  unsigned int mask;
  KeyCode keycode;
  int revert, i;

  XQueryPointer (dest->display, dest->root, &rw, &cw,
		 &i, &i, &i, &i, &mask);

  if (dest->window != InputFocus)
    {
      XGetInputFocus (dest->display, &focus, &revert);
      XSetInputFocus (dest->display, dest->window, RevertToNone, CurrentTime);
    }

  shift = (tab[key->code].modifiers & ShiftMask) && !(mask & ShiftMask);
  control = (tab[key->code].modifiers & ControlMask) && !(mask & ControlMask);
  meta = (tab[key->code].modifiers & Mod1Mask) && !(mask & Mod1Mask);

  if (shift)
    press_key (dest->display, dest->shift);
  if (control)
    press_key (dest->display, dest->control);
  if (meta)
    press_key (dest->display, dest->meta);

  keycode = tab[key->code].keycode;
  switch (key->state)
    {
    case STATE_DOWN:
      press_key (dest->display, keycode);
      break;
    case STATE_UP:
      release_key (dest->display, keycode);
      break;
    }

  if (shift)
    release_key (dest->display, dest->shift);
  if (control)
    release_key (dest->display, dest->control);
  if (meta)
    release_key (dest->display, dest->meta);

  if (dest->window != InputFocus)
    XSetInputFocus (dest->display, focus, revert, CurrentTime);

  XFlush (dest->display);
}

static void
init_xtest (struct destination *dest)
{
  int i;

  if (!XTestQueryExtension (dest->display, &i, &i, &i, &i))
    {
      fprintf (stderr, "XTest extension not supported by server\n");
      XCloseDisplay (dest->display);
      exit (1);
    }                                                                   

  XTestGrabControl (dest->display, True);
}

int
main (int argc, char **argv)
{
  struct destination dest;
  struct key key;

  init_destination (&dest, &argc, argv);
  init_xtest (&dest);
  init_code_to_string ();

  for (;;)
    {
      if (read_key (&key) == EOF)
	break;

      send_key (&dest, &key);
    }

  return 0;
}

#else/*!(HAVE_X11_XLIB_H && HAVE_X11_KEYSYM_H && HAVE_X11_EXTENSIONS_XTEST_H)*/

#error Must have X11 and XTest headers.

#endif

