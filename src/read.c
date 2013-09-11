#include <stdio.h>
#include <string.h>

#include "protocol.h"

#define LINE_MAX 40

static void
chomp (char *s)
{
  int n = strlen (s);

  if (n > 0 && s[n - 1] == '\n')
    {
      s[n - 1] = 0;
      if (n > 1 && s[n - 2] == '\r')
	s[n - 2] = 0;
    }
}

int
read_key (struct key *key)
{
  char line[LINE_MAX];
  char *p, *s;

  do
    {
      if (fgets (line, sizeof line, stdin) == NULL)
	return EOF;
      chomp (line);
    }
  while (line[0] == 0);

  s = line;
  key->code = -1;

  p = strchr (s, ' ');
  if (p == NULL)
    abort();
  *p++ = 0;
  if (strcmp (p, "up") == 0)
    key->state = STATE_UP;
  else if (strcmp (p, "down") == 0)
    key->state = STATE_DOWN;
  else
    abort ();

  if (strlen (s) == 1)
    {
      key->code = *s;
      return 1;
    }
  else
    {
      int i;

      for (i = 0; i < KEY_MAX; i++)
	{
	  if (strcmp (s, code_to_string[i]) == 0)
	    {
	      key->code = i;
	      return 1;
	    }
	}

      abort ();
    }

  return 0;
}
