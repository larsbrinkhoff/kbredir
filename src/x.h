#ifndef _X_H
#define _X_H

#include <X11/Xlib.h>

struct key2
{
  KeyCode keycode;
  unsigned int modifiers;
};

struct destination
{
  Display *display;
  Window root;
  Window window;
  KeyCode shift, control, meta;
};

extern struct key2 tab[];
extern void make_tab (Display *);
extern void init_destination (struct destination *dest,
			      int *argc, char **argv);

#endif /* _X_H */
