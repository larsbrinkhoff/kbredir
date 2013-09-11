#include "../config.h"

#ifdef HAVE_X11_XLIB_H

#include <stdio.h>
#include <X11/Xlib.h>

#include "x.h"
#include "protocol.h"

static void
send_keyevent (struct destination *dest, int event_type, KeyCode keycode,
	       unsigned int state)
{
  XKeyPressedEvent event;
  long event_mask;

  event_mask = NoEventMask;
  /*event_mask = KeyPressMask;*/

  event.type = event_type;
  /* No need to set these:
     event.serial = NextRequest (dest->display);
     event.send_event = True; */
  event.window = dest->window;
  event.root = dest->root;
  event.subwindow = None;
  event.time = CurrentTime;
  event.x = 10;
  event.y = 10;
  event.x_root = 100;
  event.y_root = 100;
  event.state = state;
  event.keycode = keycode;
  event.same_screen = True;
  
  if (!XSendEvent (dest->display,
		   dest->window,
		   False,
		   event_mask,
		   (XEvent *)&event))
    fprintf (stderr, "[XSendEvent: error]\n");
}

static void
send_key (struct destination *dest, struct key *key)
{
  unsigned int state;
  KeyCode keycode;
  Window rw, cw;
  int i;

  keycode = tab[key->code].keycode;
  XQueryPointer (dest->display, dest->root, &rw, &cw,
		 &i, &i, &i, &i, &state);

  switch (key->state)
    {
    case STATE_DOWN:
      send_keyevent (dest, KeyPress, keycode, state);
      break;
    case STATE_UP:
      send_keyevent (dest, KeyRelease, keycode, state);
      break;
    default:
      abort ();
    }

  XFlush (dest->display);
}

int
main (int argc, char **argv)
{
  struct destination dest;
  struct key key;

  init_destination (&dest, &argc, argv);
  init_code_to_string ();

  while (!feof (stdin))
    {
      if (read_key (&key) == EOF)
	break;

      send_key (&dest, &key);
    }

  return 0;
}

#else /* !HAVE_X11_XLIB_H */

#error Must have X11 headers.

#endif /* !HAVE_X11_XLIB_H */
