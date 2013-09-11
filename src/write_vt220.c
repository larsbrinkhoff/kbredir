#include "../config.h"

#include <stdio.h>
#include <string.h>

#include "protocol.h"

int shifted, controlled, metad;

static void
write_sequence (struct key *key)
{
  char *str;

#define ESC "\033"

  switch (key->code)
    {
#if 0
    case KEY_F1:	str = ESC"OP"; break;
    case KEY_F2:	str = ESC"OQ"; break;
    case KEY_F3:	str = ESC"OR"; break;
    case KEY_F4:	str = ESC"OS"; break;
#endif

    case KEY_Up:	str = ESC"[A"; break;
    case KEY_Down:	str = ESC"[B"; break;
    case KEY_Right:	str = ESC"[C"; break;
    case KEY_Left:	str = ESC"[D"; break;
    case KEY_Begin:	str = ESC"[E"; break;

    case KEY_Home:	str = ESC"[1~"; break;
    case KEY_Insert:	str = ESC"[2~"; break;
    case KEY_Delete:	str = ESC"[3~"; break;
    case KEY_End:	str = ESC"[4~"; break;
    case KEY_Next:	str = ESC"[5~"; break;
    case KEY_Prior:	str = ESC"[6~"; break;
#if 0
    case KEY_F1:	str = ESC"[11~"; break;
    case KEY_F2:	str = ESC"[12~"; break;
    case KEY_F3:	str = ESC"[13~"; break;
    case KEY_F4:	str = ESC"[14~"; break;
    case KEY_F5:	str = ESC"[15~"; break;
#endif
    case KEY_F6:	str = ESC"[17~"; break;
    case KEY_F7:	str = ESC"[18~"; break;
    case KEY_F8:	str = ESC"[19~"; break;
    case KEY_F9:	str = ESC"[20~"; break;
    case KEY_F10:	str = ESC"[21~"; break;
    case KEY_F11:	str = ESC"[23~"; break;
    case KEY_F12:	str = ESC"[24~"; break;
    case KEY_F13:	str = ESC"[25~"; break;
    case KEY_F14:	str = ESC"[26~"; break;
    case KEY_F15:	str = ESC"[28~"; break;
    case KEY_F16:	str = ESC"[29~"; break;
    case KEY_F17:	str = ESC"[31~"; break;
    case KEY_F18:	str = ESC"[32~"; break;
    case KEY_F19:	str = ESC"[33~"; break;
    case KEY_F20:	str = ESC"[34~"; break;

    case KEY_F1:	str = ESC"[[A"; break;
    case KEY_F2:	str = ESC"[[B"; break;
    case KEY_F3:	str = ESC"[[C"; break;
    case KEY_F4:	str = ESC"[[D"; break;
    case KEY_F5:	str = ESC"[[E"; break;

    default:
      abort();
    }

#undef ESC

  fputs (str, stdout);
}

static void
modify (struct key *key)
{
  switch (key->code)
    {
    case KEY_Shift:
    case KEY_Shift_L:
    case KEY_Shift_R:
      shifted = (key->state == STATE_DOWN);
      break;
    case KEY_Control:
    case KEY_Control_L:
    case KEY_Control_R:
      controlled = (key->state == STATE_DOWN);
      break;
    case KEY_Meta:
    case KEY_Meta_L:
    case KEY_Meta_R:
      metad = (key->state == STATE_DOWN);
      break;
    }

  if (shifted)
    {
      static char *x = "abcdefghijklmnopqrstuvwxyz`1234567890-=\[];',./";
      static char *y = "ABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_+|{}:\"<>?";
      char *s;

      s = strchr (x, key->code);
      if (s)
	key->code = y[s - x];
    }

  if (controlled)
    {
      if (key->code >= '@' && key->code <= '_')
	key->code -= '@';
      else if (key->code >= 'a' && key->code <= 'z')
	key->code -= '`';
    }

  if (metad)
    {
      if (1)
	putchar (KEY_Escape);
      else
	key->code |= 0x80;
    }
}

int
main (int argc, char **argv)
{
  struct key key;

  if (argc != 1)
    {
      fprintf (stderr, "%s takes no options\n", argv[0]);
      exit (1);
    }

  init_code_to_string ();
  shifted = controlled = metad = 0;

  while (!feof (stdin))
    {
      if (read_key (&key) == EOF)
	break;

      modify (&key);

      if (key.state == STATE_UP)
	continue;

      if (key.code < 256)
	putchar (key.code);
      else
	write_sequence (&key);
      fflush (stdout);
    }

  return 0;
}
