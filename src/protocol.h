#ifndef PROTOCOL_H
#define PROTOCOL_H

struct key
{
  int code;
  int state;
};

#define STATE_DOWN 1
#define STATE_UP 2

enum
{
  /* Symolic names for ASCII characters. */
  KEY_Escape = 27,
  KEY_Delete = 127,
  /* Function keys, numbered above the range of 8-bit characters. */
  KEY_Up = 256, KEY_Down, KEY_Right, KEY_Left, KEY_Home, KEY_End, KEY_Next,
  KEY_Prior, KEY_Insert, KEY_Pause, KEY_Begin, KEY_Hold, KEY_Print,
  KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9,
  KEY_F10, KEY_F11, KEY_F12, KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17,
  KEY_F18, KEY_F19, KEY_F20,
  /* Modifier keys */
  KEY_Shift, KEY_Shift_L, KEY_Shift_R,
  KEY_Control, KEY_Control_L, KEY_Control_R,
  KEY_Meta, KEY_Meta_L, KEY_Meta_R,
  KEY_Alt, KEY_Alt_L, KEY_Alt_R,
  /* End */
  KEY_MAX
};


extern char *code_to_string[];
extern int read_key (struct key *);
extern void write_key (struct key *);
extern void init_code_to_string (void);

#endif /* PROCOTOL_H */
