#include <stdio.h>
#include <string.h>

#include "protocol.h"

void
write_key (struct key *key)
{
  fputs (code_to_string[key->code], stdout);

  switch (key->state)
    {
    case STATE_DOWN:
      fputs (" down", stdout);
      break;
    case STATE_UP:
      fputs (" up", stdout);
      break;
    default:
      abort ();
    }

  putchar ('\n');
  fflush (stdout);
}
