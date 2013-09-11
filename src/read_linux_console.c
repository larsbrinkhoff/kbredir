#include "../config.h"

#if defined (HAVE_LINUX_KD_H) && defined (HAVE_LINUX_VT_H) && \
    defined (HAVE_LINUX_KEYBOARD_H)

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/keyboard.h>

#include "protocol.h"

static struct termios saved_termios;
static int saved_termios_is_valid = 0;
static long saved_kbmode = -1;

#define SHIFT	1
#define ALTGR	2
#define CONTROL	4
#define ALT	8
static int tab2[NR_KEYS][16];

#define KEY(c,m) ((struct key){ 0, c, m, 0 })

static int shifted, controlled, metad;

static RETSIGTYPE
cleanup (int sig)
{
  if (sig)
    fprintf (stderr, "\nAborted!  Cleaning up.\n");
  if (saved_kbmode != -1)
    ioctl (0, KDSKBMODE, saved_kbmode);
  if (saved_termios_is_valid)
    tcsetattr (0, TCSANOW, &saved_termios);
  if (sig)
    exit (1);
  exit (0);
}

/* Always use cleanup() instead of exit(). */
#define exit use_cleanup_instead

static int
get_char (int timeout)
{
  struct pollfd p[] = { { 0, POLLIN, 0 } };
  unsigned char c;
  ssize_t n;

  if (poll (p, sizeof p / sizeof p[0], timeout) == 0)
    return -1;
  n = read (0, &c, 1);
  if (n == 0)
    return -1;
  if (n == -1)
    return -1;

  return c;
}

int
handle_key (void)
{
  int c, e;
  int down;
  char *state;

  c = get_char (-1);
  if (c == -1)
    return 1;

  if (c & 0x80)
    {
      down = 0;
      state = "up";
    }
  else
    {
      down = 1;
      state = "down";
    }
  c &= 0x7f;

  switch (tab2[c][0])
    {
    case K_HOLE:
      return 1;
    case K_SHIFT:
      fputs ("Shift", stdout);
      shifted = down ? SHIFT : 0;
      break;
    case K_CTRL:
      fputs ("Control", stdout);
      controlled = down ? CONTROL : 0;
      break;
    case K_ALT:
      fputs ("Meta", stdout);
      metad = down ? ALT : 0;
      break;
    case '\b':
      if (controlled && metad)
	cleanup (1);
      fputs ("BackSpace", stdout);
      break;
    case '\t':
      fputs ("Tab", stdout);
      break;
    case '\n':
      fputs ("Linefeed", stdout);
      break;
    case '\r':
      fputs ("Return", stdout);
      break;
    case KEY_Escape:
      fputs ("Escape", stdout);
      break;
    case ' ':
      fputs ("space", stdout);
      break;
    case KEY_Delete:
      if (controlled && metad)
	cleanup (0);
      fputs ("Delete", stdout);
      break;
    case K_ENTER:
      fputs ("Return", stdout);
      break;
    case K_BREAK:
      fputs ("Break", stdout);
      break;
    case K_CAPS:
      fputs ("Caps", stdout);
      break;
    case K_NUM:
      fputs ("Numlock", stdout);
      break;
    case K_HOLD:
      fputs ("Hold", stdout);
      break;
    case K_COMPOSE:
      fputs ("Compose", stdout);
      break;
    case K_BARENUMLOCK:
      fputs ("Numlock", stdout);
      break;
    case  K_F1: case  K_F2: case  K_F3: case  K_F4: case  K_F5:
    case  K_F6: case  K_F7: case  K_F8: case  K_F9: case K_F10:
    case K_F11: case K_F12: case K_F13: case K_F14: case K_F15:
    case K_F16: case K_F17: case K_F18: case K_F19: case K_F20:
      if (metad)
	{
	  if (ioctl (0, VT_ACTIVATE, tab2[c][0] - K_F1 + 1) == -1)
	    fprintf (stderr, "Error switching virtual console: %s\n",
		     strerror (errno));
	  return 1;
	}
      else
	{
	  char str[8];
	  sprintf (str, "F%d", tab2[c][0] - K_F1 + 1);
	  fputs (str, stdout);
	}
      break;
    case K_FIND:
      fputs ("Home", stdout);
      break;
    case K_INSERT:
      fputs ("Insert", stdout);
      break;
    case K_REMOVE:
      fputs ("Remove", stdout);
      break;
    case K_SELECT:
      fputs ("End", stdout);
      break;
    case K_PGUP:
      fputs ("Prior", stdout);
      break;
    case K_PGDN:
      fputs ("Next", stdout);
      break;
    case K_MACRO:
      fputs ("Macro", stdout);
      break;
    case K_HELP:
      fputs ("Help", stdout);
      break;
    case K_DO:
      fputs ("Do", stdout);
      break;
    case K_PAUSE:
      fputs ("Pause", stdout);
      break;
    case K_DOWN:
      fputs ("Down", stdout);
      break;
    case K_LEFT:
      fputs ("Left", stdout);
      break;
    case K_RIGHT:
      fputs ("Right", stdout);
      break;
    case K_UP:
      fputs ("Up", stdout);
      break;
    case K_P0:
      fputs ("Pad0", stdout);
      break;
    case K_P1:
      fputs ("Pad1", stdout);
      break;
    case K_P2:
      fputs ("Pad2", stdout);
      break;
    case K_P3:
      fputs ("Pad3", stdout);
      break;
    case K_P4:
      fputs ("Pad4", stdout);
      break;
    case K_P5:
      fputs ("Pad5", stdout);
      break;
    case K_P6:
      fputs ("Pad6", stdout);
      break;
    case K_P7:
      fputs ("Pad7", stdout);
      break;
    case K_P8:
      fputs ("Pad8", stdout);
      break;
    case K_P9:
      fputs ("Pad9", stdout);
      break;
    case K_PPLUS:
      fputs ("Pad+", stdout);
      break;
    case K_PMINUS:
      fputs ("Pad-", stdout);
      break;
    case K_PSTAR:
      fputs ("Pad*", stdout);
      break;
    case K_PSLASH:
      fputs ("Pad/", stdout);
      break;
    case K_PENTER:
      fputs ("PadEnter", stdout);
      break;
    case K_PCOMMA:
      fputs ("Pad,", stdout);
      break;
    case K_PDOT:
      fputs ("Pad.", stdout);
      break;
    case K_PPLUSMINUS:
      fputs ("Padplusminus", stdout);
      break;
    case K_PPARENL:
      fputs ("Pad(", stdout);
      break;
    case K_PPARENR:
      fputs ("Pad)", stdout);
      break;
    default:
      e = tab2[c][shifted | controlled | metad];
      if ((e & 0xff00) == 0xb00 || (e & 0xff00) == 0x800)
	e &= 0xff;
      if (e < 32)
	e += '@';
      if (e >= 'A' && e <= 'Z')
	e += 'a' - 'A';
      if (e > 32 && e < 127)
	putchar (e);
      else if (e == K_HOLE)
	return 1;
      else
	printf ("0x%x", e);
    }

  putchar (' ');
  fputs (state, stdout);
  putchar ('\n');
  fflush (stdout);

  return 1;
}

int
main (int argc, char **argv)
{
  struct termios t;

  if (argc != 1)
    {
      fprintf (stderr, "%s takes no options\n", argv[0]);
      cleanup (1);
    }

  if (!isatty (0))
    {
      fprintf (stderr, "Input is not a tty.\n");
      cleanup (1);
    }
  if (tcgetattr (0, &saved_termios) == -1)
    {
      fprintf (stderr, "Could not get tty attributes.\n");
      cleanup (1);
    }
  saved_termios_is_valid = 1;
  t = saved_termios;
  t.c_lflag = 0;
  t.c_iflag = 0;
  if (tcsetattr (0, TCSANOW, &t) == -1)
    {
      fprintf (stderr, "Could not set tty attributes.\n");
      cleanup (1);
    }

  if (ioctl (0, KDGKBMODE, &saved_kbmode) == -1 ||
      ioctl (0, KDSKBMODE, K_MEDIUMRAW) == -1)
    {
      fprintf (stderr, "Input is not a console.\n");
      cleanup (1);
    }

  signal (SIGINT, cleanup);
  signal (SIGPIPE, cleanup);
  signal (SIGTERM, cleanup);
  signal (SIGABRT, cleanup);
  signal (SIGSEGV, cleanup);

  {
    struct kbentry kbe;
    int i, j;

    for (i = 0; i < NR_KEYS; i++)
      {
	for (j = 0; j < 16; j++)
	  {
	    tab2[i][j] = -1;

	    kbe.kb_index = i;
	    kbe.kb_table = j;
	    if (ioctl (0, KDGKBENT, &kbe) == -1)
	      fprintf (stderr, "%d %d: error\n", i, j);
	    else if (kbe.kb_value != K_HOLE)
	      {
#if 0
		if (j & 1)
		  fprintf (stderr, "shift ");
		else
		  fprintf (stderr, "      ");
		if (j & 2)
		  fprintf (stderr, "altgr ");
		else
		  fprintf (stderr, "      ");
		if (j & 4)
		  fprintf (stderr, "control ");
		else
		  fprintf (stderr, "        ");
		if (j & 8)
		  fprintf (stderr, "alt ");
		else
		  fprintf (stderr, "    ");
		fprintf (stderr, "keycode %3d = %x\n",
			 i, kbe.kb_value);
#endif
	      }

	    tab2[i][j] = kbe.kb_value;
	  }
      }
  }

  shifted = controlled = metad = 0;

  init_code_to_string ();

  fprintf (stderr,
	   "Type Alt-Fn to change virtual console, or\n"
	   "Control-Alt-Backspace to quit.\n");

  while (handle_key ())
    ;

  cleanup (0);
  return 0;
}

#else /* !(HAVE_LINUX_KD_H && HAVE_LINUX_VT_H && HAVE_LINUX_KEYBOARD_H) */

#error Must have Linux header files.

#endif /* !(HAVE_LINUX_KD_H && HAVE_LINUX_VT_H && HAVE_LINUX_KEYBOARD_H) */
