#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "x.h"
#include "protocol.h"

static struct { int ascii; KeySym keysym; } ascii_to_keysym[] =
{
  /* Control characters */
  { '\b', XK_BackSpace },	{ '\t', XK_Tab },	{ '\r', XK_Return },
  { KEY_Escape, XK_Escape },	{ KEY_Delete, XK_Delete },

  /* Various symbols */
  { ' ', XK_space },		{ '!', XK_exclam },	{ '"', XK_quotedbl },
  { '#', XK_numbersign },	{ '$', XK_dollar },	{ '%', XK_percent },
  { '&', XK_ampersand },	{ '\'',XK_apostrophe },	{ '(', XK_parenleft },
  { ')', XK_parenright },	{ '*', XK_asterisk },	{ '+', XK_plus },
  { ',', XK_comma },		{ '-', XK_minus },	{ '.', XK_period },
  { '/', XK_slash },		{ ':', XK_colon },	{ ';', XK_semicolon },
  { '<', XK_less },		{ '=', XK_equal },	{ '>', XK_greater },
  { '?', XK_question },		{ '@', XK_at },		{ '[', XK_bracketleft},
  { '\\', XK_backslash },	{ ']', XK_bracketright},{ '^', XK_asciicircum},
  { '_', XK_underscore },	{ '`', XK_grave },	{ '{', XK_braceleft },
  { '|', XK_bar },		{ '}', XK_braceright },	{ '~', XK_asciitilde },

  /* Digits */
  { '0', XK_0 }, { '1', XK_1 }, { '2', XK_2 }, { '3', XK_3 }, { '4', XK_4 },
  { '5', XK_5 }, { '6', XK_6 }, { '7', XK_7 }, { '8', XK_8 }, { '9', XK_9 },

  /* Letters */
  { 'a', XK_a }, { 'b', XK_b }, { 'c', XK_c }, { 'd', XK_d }, { 'e', XK_e },
  { 'f', XK_f }, { 'g', XK_g }, { 'h', XK_h }, { 'i', XK_i }, { 'j', XK_j },
  { 'k', XK_k }, { 'l', XK_l }, { 'm', XK_m }, { 'n', XK_n }, { 'o', XK_o },
  { 'p', XK_p }, { 'q', XK_q }, { 'r', XK_r }, { 's', XK_s }, { 't', XK_t },
  { 'u', XK_u }, { 'v', XK_v }, { 'w', XK_w }, { 'x', XK_x }, { 'y', XK_y },
  { 'z', XK_z },

  /* Cursor movement */
  { KEY_Up, XK_Up },		{ KEY_Down, XK_Down },	{ KEY_Right, XK_Right},
  { KEY_Left, XK_Left },	{ KEY_Home, XK_Home },  { KEY_End, XK_End },
  { KEY_Next, XK_Next },	{ KEY_Prior, XK_Prior },{ KEY_Begin, XK_Begin},

  /* Numerical function keys */
  { KEY_F1, XK_F1 },{ KEY_F2, XK_F2 }, { KEY_F3, XK_F3 }, { KEY_F4, XK_F4 },
  { KEY_F5, XK_F5 },{ KEY_F6, XK_F6 }, { KEY_F7, XK_F7 }, { KEY_F8, XK_F8 },
  { KEY_F9, XK_F9 },{ KEY_F10, XK_F10},{ KEY_F11, XK_F11},{ KEY_F12, XK_F12},

  /* Other function keys */
  { KEY_Insert, XK_Insert }, { KEY_Delete, XK_Delete },{ KEY_Pause, XK_Pause },

  /* Modifier keys */
  { KEY_Shift, XK_Shift_L },	   { KEY_Shift_L, XK_Shift_L },
  { KEY_Shift_R, XK_Shift_R },	   { KEY_Control, XK_Control_L },
  { KEY_Control_L, XK_Control_L }, { KEY_Control_R, XK_Control_R },
  { KEY_Meta, XK_Meta_L },	   { KEY_Meta_L, XK_Control_L },
  { KEY_Meta_R, XK_Control_R },
};

static unsigned int num_syms = sizeof ascii_to_keysym /
                               sizeof ascii_to_keysym[0];

struct key2 tab[KEY_MAX];

void
make_tab (Display *display)
{
  KeyCode keycode;
  unsigned int i;

  for (i = 0; i < sizeof tab / sizeof tab[0]; i++)
    tab[i].keycode = 0;

  for (i = 0; i < num_syms; i++)
    {
      keycode = XKeysymToKeycode (display, ascii_to_keysym[i].keysym);
      if (keycode == 0)
	continue;
      
      if (ascii_to_keysym[i].keysym ==
	  XKeycodeToKeysym (display, keycode, 0))
	{
	  tab[ascii_to_keysym[i].ascii].keycode = keycode;
	  tab[ascii_to_keysym[i].ascii].modifiers = 0;
	}
      else if (ascii_to_keysym[i].keysym ==
	       XKeycodeToKeysym (display, keycode, 1))
	{
	  tab[ascii_to_keysym[i].ascii].keycode = keycode;
	  tab[ascii_to_keysym[i].ascii].modifiers = ShiftMask;
	}
      else
	abort ();
    }

  for (i = 'A'; i <= 'Z'; i++)
    {
      if (tab[i].keycode == 0)
	{
	  tab[i].modifiers = tab[i + ('a' - 'A')].modifiers | ShiftMask;
	  tab[i].keycode = tab[i + ('a' - 'A')].keycode;
	}
    }

  for (i = 0; i < 32; i++)
    {
      if (tab[i].keycode == 0)
	{
	  int j = i + '@';

	  if (j >= 'A' && j <= 'Z')
	    j += 'a' - 'A';

	  tab[i].modifiers = tab[j].modifiers | ControlMask;
	  tab[i].keycode = tab[j].keycode;
	}
    }

#if 0
  for (i = 0; i < KEY_MAX; i++)
    {
      if (i == 128)
	i = KEY_Up;

      if (tab[i].keycode == 0)
	fprintf (stderr, "ascii %d not defined\r\n", i);
      else
	{
	  fprintf (stderr, "ascii %d = ", i);
	  if (tab[i].modifiers & ShiftMask)
	    fprintf (stderr, "Shift-");
	  if (tab[i].modifiers & LockMask)
	    fprintf (stderr, "Lock-");
	  if (tab[i].modifiers & ControlMask)
	    fprintf (stderr, "Control-");
	  if (tab[i].modifiers & Mod1Mask)
	    fprintf (stderr, "Meta-");
	  fprintf (stderr, "%s\r\n",
		   XKeysymToString (XKeycodeToKeysym (display,
						      tab[i].keycode,
						      0)));
	}
    }
#endif
}
