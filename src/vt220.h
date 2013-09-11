#ifndef VT220_H
#define VT220_H

struct prefix
{
  int character;
#define ENDCHAR -1	/* ends the table */
#define ANYCHAR -2	/* matches any character */

  int mapping;
#define NOKEY -1	/* this sequence doesn't map to any key */
#define METAIZE -2	/* this sequence maps to this key with meta pressed */

  struct prefix *next;
};

extern struct prefix *prefix_start;

#endif /* VT220_H */
