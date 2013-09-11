#include "../config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/poll.h>

#include "vt220.h"
#include "protocol.h"

struct termios saved_termios;

#define KEY(c,m) ((struct key){ c, m, 0 })

#define TIMEOUT -2
#define ERROR -3

static int
get_char (int timeout)
{
  struct pollfd p[] = { { 0, POLLIN, 0 } };
  unsigned char c;
  ssize_t n;

  n = poll (p, sizeof p / sizeof p[0], timeout);
  if (n == 0)
    return TIMEOUT;
  else if (n == -1)
    return ERROR;

  n = read (0, &c, 1);
  if (n == 0)
    return EOF;
  else if (n == -1)
    return ERROR;

  return c;
}

static void
press_key (int code)
{
  struct key key = { code, STATE_DOWN };
  write_key (&key);
}

static void
release_key (int code)
{
  struct key key = { code, STATE_UP };
  write_key (&key);
}

static void
send_key (int chr)
{
  if (chr < 32)
    {
      switch (chr)
	{
	case '\b':
	case '\t':
	case '\n':
	case '\r':
	case KEY_Escape:
	  press_key (chr);
	  release_key (chr);
	  break;
	default:
	  chr += '@';
	  if (chr >= 'A' && chr <= 'Z')
	    chr += 'a' - 'A';
	  press_key (KEY_Control);
	  press_key (chr);
	  release_key (chr);
	  release_key (KEY_Control);
	}
    }
  else if (chr >= 'A' && chr <= 'Z')
    {
      chr += 'a' - 'A';
      press_key (KEY_Shift);
      press_key (chr);
      release_key (chr);
      release_key (KEY_Shift);
    }
  else
    {
      press_key (chr);
      release_key (chr);
    }
}

static int
lookup (int chr, struct prefix *prefix)
{
  int c2;

  for (; prefix->character != ENDCHAR; prefix++)
    {
      if (prefix->character != ANYCHAR &&
	  prefix->character != chr)
	continue;

      if (!prefix->next || ((c2 = get_char (1)) == -1))
	{
	  switch (prefix->mapping)
	    {
	    case METAIZE:
	      press_key (KEY_Meta);
	      send_key (chr);
	      release_key (KEY_Meta);
	      break;
	    case NOKEY:
	      /* FIXME */
	      abort ();
	      return 0;
	    default:
	      send_key (prefix->mapping);
	      break;
	    }
	  return 1;
	}

      return lookup (c2, prefix->next);
    }

  send_key (chr);
  return 1;
}

int
handle_key (void)
{
  int c;

  c = get_char (-1);
  if (c == TIMEOUT)
    return 1;
  else if (c == EOF || c == ERROR)
    return 0;
  else if (c == 'q')
    return 0;

  lookup (c, prefix_start);
  return 1;
}

int
main (int argc, char **argv)
{
  if (argc != 1)
    {
      fprintf (stderr, "%s takes no options\n", argv[0]);
      exit (1);
    }

  if (isatty (0))
    {
      struct termios t;

      if (tcgetattr (0, &t) == -1)
	abort ();
      saved_termios = t;
      t.c_lflag = 0;
      t.c_iflag = 0;
      if (tcsetattr (0, TCSANOW, &t) == -1)
	abort ();
      fprintf (stderr, "Type q (TODO: replace with a sane key (sequence)) "
	       "to quit.\n");
    }

  init_code_to_string ();

  while (handle_key ())
    ;

  tcsetattr (0, TCSANOW, &saved_termios);
  return 0;
}
