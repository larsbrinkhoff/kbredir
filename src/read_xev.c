#include "../config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include "protocol.h"

static int shifted, controlled, metad;

static const char *keypress_event = "KeyPress event";
static const char *keyrelease_event = "KeyRelease event";
static int keypress_len;
static int keyrelease_len;

static void
get_key2 (struct key *key)
{
  int keypress, keyrelease;
  char line[100];
  char *keysym;
  char *end;

  do
    {
      if (fgets (line, sizeof line, stdin) == NULL)
	exit (0);

      keypress = (strncmp (line, keypress_event, keypress_len) == 0);
      keyrelease = (strncmp (line, keyrelease_event, keyrelease_len) == 0);
    }
  while (!keypress && !keyrelease);

  if (keypress)
    key->state = STATE_DOWN;
  else
    key->state = STATE_UP;

  do
    {
      fgets (line, sizeof line, stdin);
      keysym = strstr (line, "keysym");
    }
  while (keysym == NULL);

  keysym = strchr (keysym, ',');
  if (keysym == NULL)
    abort ();
  keysym++;

  if (*keysym == ' ')
    keysym++;

  end = strchr (keysym, ')');
  if (end == NULL)
    abort ();

  *end = 0;

  if (strlen (keysym) == 1)
    {
      key->code = *keysym;
      if (key->code >= 'A' && key->code <= 'Z')
	key->code += 'a' - 'A';
    }
  else
    {
      if (strcmp (keysym, "Escape") == 0)
	key->code = KEY_Escape;
      else if (strcmp (keysym, "exclam") == 0)
	key->code = '!';
      else if (strcmp (keysym, "at") == 0)
	key->code = '@';
      else if (strcmp (keysym, "numbersign") == 0)
	key->code = '#';
      else if (strcmp (keysym, "dollar") == 0)
	key->code = '$';
      else if (strcmp (keysym, "percent") == 0)
	key->code = '%';
      else if (strcmp (keysym, "asciicircum") == 0)
	key->code = '^';
      else if (strcmp (keysym, "ampersand") == 0)
	key->code = '&';
      else if (strcmp (keysym, "asterisk") == 0)
	key->code = '*';
      else if (strcmp (keysym, "parenleft") == 0)
	key->code = '(';
      else if (strcmp (keysym, "parenright") == 0)
	key->code = ')';
      else if (strcmp (keysym, "minus") == 0)
	key->code = '-';
      else if (strcmp (keysym, "underscore") == 0)
	key->code = '_';
      else if (strcmp (keysym, "equal") == 0)
	key->code = '=';
      else if (strcmp (keysym, "plus") == 0)
	key->code = '+';
      else if (strcmp (keysym, "backslash") == 0)
	key->code = '\\';
      else if (strcmp (keysym, "bar") == 0)
	key->code = '|';
      else if (strcmp (keysym, "grave") == 0)
	key->code = '`';
      else if (strcmp (keysym, "asciitilde") == 0)
	key->code = '~';
      else if (strcmp (keysym, "Tab") == 0)
	key->code = '\t';
      else if (strcmp (keysym, "bracketleft") == 0)
	key->code = '[';
      else if (strcmp (keysym, "braceleft") == 0)
	key->code = '{';
      else if (strcmp (keysym, "bracketright") == 0)
	key->code = ']';
      else if (strcmp (keysym, "braceright") == 0)
	key->code = '}';
      else if (strcmp (keysym, "Delete") == 0)
	key->code = KEY_Delete;
      else if (strcmp (keysym, "BackSpace") == 0)
	key->code = '\b';
      else if (strcmp (keysym, "Control_L") == 0)
	key->code = KEY_Control_L;
      else if (strcmp (keysym, "Control_R") == 0)
	key->code = KEY_Control_R;
      else if (strcmp (keysym, "semicolon") == 0)
	key->code = ';';
      else if (strcmp (keysym, "colon") == 0)
	key->code = ':';
      else if (strcmp (keysym, "apostrophe") == 0)
	key->code = '\'';
      else if (strcmp (keysym, "quotedbl") == 0)
	key->code = '"';
      else if (strcmp (keysym, "Return") == 0)
	key->code = '\r';
      else if (strcmp (keysym, "Linefeed") == 0)
	key->code = '\n';
      else if (strcmp (keysym, "Shift_L") == 0)
	key->code = KEY_Shift_L;
      else if (strcmp (keysym, "Shift_R") == 0)
	key->code = KEY_Shift_R;
      else if (strcmp (keysym, "comma") == 0)
	key->code = ',';
      else if (strcmp (keysym, "less") == 0)
	key->code = '<';
      else if (strcmp (keysym, "period") == 0)
	key->code = '.';
      else if (strcmp (keysym, "greater") == 0)
	key->code = '>';
      else if (strcmp (keysym, "slash") == 0)
	key->code = '/';
      else if (strcmp (keysym, "question") == 0)
	key->code = '?';
      else if (strcmp (keysym, "Alt_L") == 0 ||
	       strcmp (keysym, "Meta_L") == 0)
	key->code = KEY_Meta_L;
      else if (strcmp (keysym, "Alt_R") == 0 ||
	       strcmp (keysym, "Meta_R") == 0)
	key->code = KEY_Meta_R;
      else if (strcmp (keysym, "space") == 0)
	key->code = ' ';
      else if (strcmp (keysym, "F1") == 0)
	key->code = KEY_F1;
      else if (strcmp (keysym, "F2") == 0)
	key->code = KEY_F2;
      else if (strcmp (keysym, "F3") == 0)
	key->code = KEY_F3;
      else if (strcmp (keysym, "F4") == 0)
	key->code = KEY_F4;
      else if (strcmp (keysym, "F5") == 0)
	key->code = KEY_F5;
      else if (strcmp (keysym, "F6") == 0)
	key->code = KEY_F6;
      else if (strcmp (keysym, "F7") == 0)
	key->code = KEY_F7;
      else if (strcmp (keysym, "F8") == 0)
	key->code = KEY_F8;
      else if (strcmp (keysym, "F9") == 0)
	key->code = KEY_F9;
      else if (strcmp (keysym, "F10") == 0)
	key->code = KEY_F10;
      else if (strcmp (keysym, "F11") == 0)
	key->code = KEY_F11;
      else if (strcmp (keysym, "F12") == 0)
	key->code = KEY_F12;
      else if (strcmp (keysym, "F13") == 0)
	key->code = KEY_F13;
      else if (strcmp (keysym, "F14") == 0)
	key->code = KEY_F14;
      else if (strcmp (keysym, "F15") == 0)
	key->code = KEY_F15;
      else if (strcmp (keysym, "F16") == 0)
	key->code = KEY_F16;
      else if (strcmp (keysym, "F17") == 0)
	key->code = KEY_F17;
      else if (strcmp (keysym, "F18") == 0)
	key->code = KEY_F18;
      else if (strcmp (keysym, "F19") == 0)
	key->code = KEY_F19;
      else if (strcmp (keysym, "F20") == 0)
	key->code = KEY_F20;
      else if (strcmp (keysym, "Up") == 0)
	key->code = KEY_Up;
      else if (strcmp (keysym, "Down") == 0)
	key->code = KEY_Down;
      else if (strcmp (keysym, "Left") == 0)
	key->code = KEY_Left;
      else if (strcmp (keysym, "Right") == 0)
	key->code = KEY_Right;
      else if (strcmp (keysym, "Insert") == 0)
	key->code = KEY_Insert;
      else if (strcmp (keysym, "Home") == 0)
	key->code = KEY_Home;
      else if (strcmp (keysym, "End") == 0)
	key->code = KEY_End;
      else if (strcmp (keysym, "Prior") == 0)
	key->code = KEY_Prior;
      else if (strcmp (keysym, "Next") == 0)
	key->code = KEY_Next;
      else if (strcmp (keysym, "Pause") == 0)
	key->code = KEY_Pause;
      else if (strcmp (keysym, "Print") == 0)
	key->code = KEY_Print;
      else if (strcmp (keysym, "Scroll_Lock") == 0)
	key->code = KEY_Hold;
      else
	{
	  fprintf (stderr, "keysym = %s\n", keysym);
	  abort ();
	}
    }
}

int
main (int argc, char **argv)
{
  if (argc != 1)
    {
      fprintf (stderr, "%s takes no options\n", argv[0]);
      exit (1);
    }

  shifted = controlled = metad = 0;
  keypress_len = strlen (keypress_event);
  keyrelease_len = strlen (keyrelease_event);
  init_code_to_string ();

  for (;;)
    {
      struct key key;

      get_key2 (&key);
      write_key (&key);
    }

  return 0;
}
