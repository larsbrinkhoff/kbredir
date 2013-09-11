#include <stdio.h>
#include <stdlib.h>

#include "protocol.h"

char *code_to_string[KEY_MAX];

void
init_code_to_string (void)
{
  int i, j;

  for (i = 0; i < KEY_MAX; i++)
    code_to_string[i] = "";

  for (i = 0; i < ' '; i++)
    {
      switch (i)
	{
	case '\b':
	  code_to_string[i] = "BackSpace";
	  break;
	case '\t':
	  code_to_string[i] = "Tab";
	  break;
	case '\n':
	  code_to_string[i] = "Linefeed";
	  break;
	case '\r':
	  code_to_string[i] = "Return";
	  break;
	case KEY_Escape:
	  code_to_string[i] = "Escape";
	  break;
	default:
	  j = i + '@';
	  if (j >= 'A' && j <= 'Z')
	    j += 'a' - 'A';
	  code_to_string[i] = malloc (10);
	  strcpy (code_to_string[i], "Control-");
	  code_to_string[i][8] = j;
	  code_to_string[i][9] = 0;
	  break;
	}
    }

  code_to_string[' '] = "space";

  for (i = ' ' + 1; i < 127; i++)
    {
      code_to_string[i] = malloc (2);
      code_to_string[i][0] = i;
      code_to_string[i][1] = 0;
    }

  for (i = 128; i < 255; i++)
    {
      code_to_string[i] = malloc (2);
      code_to_string[i][0] = i;
      code_to_string[i][1] = 0;
    }

  code_to_string[KEY_Delete] = "Delete";

  code_to_string[KEY_Up] = "Up";
  code_to_string[KEY_Down] = "Down";
  code_to_string[KEY_Right] = "Right";
  code_to_string[KEY_Left] = "Left";
  code_to_string[KEY_Home] = "Home";
  code_to_string[KEY_End] = "End";
  code_to_string[KEY_Next] = "Next";
  code_to_string[KEY_Prior] = "Prior";
  code_to_string[KEY_Insert] = "Insert";
  code_to_string[KEY_Pause] = "Pause";
  code_to_string[KEY_Delete] = "Delete";
  code_to_string[KEY_Begin] = "Begin";
  code_to_string[KEY_Hold] = "Hold";
  code_to_string[KEY_Shift] = "Shift";
  code_to_string[KEY_Shift_L] = "Shift_L";
  code_to_string[KEY_Shift_R] = "Shift_R";
  code_to_string[KEY_Control] = "Control";
  code_to_string[KEY_Control_L] = "Control_L";
  code_to_string[KEY_Control_R] = "Control_R";
  code_to_string[KEY_Meta] = "Meta";
  code_to_string[KEY_Meta_L] = "Meta_L";
  code_to_string[KEY_Meta_R] = "Meta_R";
  code_to_string[KEY_Alt] = "Alt";
  code_to_string[KEY_Alt_L] = "Alt_L";
  code_to_string[KEY_Alt_R] = "Alt_R";

  for (i = 0; i < 20; i++)
    {
      code_to_string[KEY_F1 + i] = malloc (4);
      sprintf (code_to_string[KEY_F1 + i], "F%d", i + 1);
    }
}
