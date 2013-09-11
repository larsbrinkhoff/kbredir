#include "../config.h"
#include "vt220.h"
#include "protocol.h"

/* Use this to define a character in the middle of a sequence. */
#define MIDDLESEQ(ch, next) { ch, NOKEY, next }

/* Use this to define a character that ends a sequence. */
#define ENDSEQ(ch, key) { ch, key, 0 }

/* Use this to end the table. */
#define ENDTAB { ENDCHAR, NOKEY, 0 }

/* Use this to define a table with a single sequence-ending character.*/
#define SINGLE(name, ch, key) \
  static struct prefix name[] = { ENDSEQ (ch, key), ENDTAB }

SINGLE (esc_bracket_1_1, '~', KEY_F1);
SINGLE (esc_bracket_1_2, '~', KEY_F2);
SINGLE (esc_bracket_1_3, '~', KEY_F3);
SINGLE (esc_bracket_1_4, '~', KEY_F4);
SINGLE (esc_bracket_1_5, '~', KEY_F5);
SINGLE (esc_bracket_1_7, '~', KEY_F6);
SINGLE (esc_bracket_1_8, '~', KEY_F7);
SINGLE (esc_bracket_1_9, '~', KEY_F8);
static struct prefix esc_bracket_1[] =
  {
    ENDSEQ ('~', KEY_Home),
    MIDDLESEQ ('1', esc_bracket_1_1),
    MIDDLESEQ ('2', esc_bracket_1_2),
    MIDDLESEQ ('3', esc_bracket_1_3),
    MIDDLESEQ ('4', esc_bracket_1_4),
    MIDDLESEQ ('5', esc_bracket_1_5),
    MIDDLESEQ ('7', esc_bracket_1_7),
    MIDDLESEQ ('8', esc_bracket_1_8),
    MIDDLESEQ ('9', esc_bracket_1_9),
    ENDTAB
  };

SINGLE (esc_bracket_2_0, '~', KEY_F9);
SINGLE (esc_bracket_2_1, '~', KEY_F10);
SINGLE (esc_bracket_2_3, '~', KEY_F11);
SINGLE (esc_bracket_2_4, '~', KEY_F12);
SINGLE (esc_bracket_2_5, '~', KEY_F13);
SINGLE (esc_bracket_2_6, '~', KEY_F14);
SINGLE (esc_bracket_2_8, '~', KEY_F15);
SINGLE (esc_bracket_2_9, '~', KEY_F16);
static struct prefix esc_bracket_2[] =
  {
    ENDSEQ ('~', KEY_Insert),
    MIDDLESEQ ('0', esc_bracket_2_0),
    MIDDLESEQ ('1', esc_bracket_2_1),
    MIDDLESEQ ('3', esc_bracket_2_3),
    MIDDLESEQ ('4', esc_bracket_2_4),
    MIDDLESEQ ('5', esc_bracket_2_5),
    MIDDLESEQ ('6', esc_bracket_2_6),
    MIDDLESEQ ('8', esc_bracket_2_8),
    MIDDLESEQ ('9', esc_bracket_2_9),
    ENDTAB
  };

SINGLE (esc_bracket_3_1, '~', KEY_F17);
SINGLE (esc_bracket_3_2, '~', KEY_F18);
SINGLE (esc_bracket_3_3, '~', KEY_F19);
SINGLE (esc_bracket_3_4, '~', KEY_F20);
static struct prefix esc_bracket_3[] =
  {
    ENDSEQ ('~', KEY_Delete),
    MIDDLESEQ ('1', esc_bracket_3_1),
    MIDDLESEQ ('2', esc_bracket_3_2),
    MIDDLESEQ ('3', esc_bracket_3_3),
    MIDDLESEQ ('4', esc_bracket_3_4),
    ENDTAB
  };

static struct prefix esc_bracket_bracket[] =
  {
    ENDSEQ ('A', KEY_F1),
    ENDSEQ ('B', KEY_F2),
    ENDSEQ ('C', KEY_F3),
    ENDSEQ ('D', KEY_F4),
    ENDSEQ ('E', KEY_F5),
    ENDTAB
  };

SINGLE (esc_bracket_4, '~', KEY_End);
SINGLE (esc_bracket_5, '~', KEY_Next);
SINGLE (esc_bracket_6, '~', KEY_Prior);
static struct prefix esc_bracket[] =
  {
    ENDSEQ ('A', KEY_Up),
    ENDSEQ ('B', KEY_Down),
    ENDSEQ ('C', KEY_Right),
    ENDSEQ ('D', KEY_Left),
    ENDSEQ ('E', KEY_Begin),
    ENDSEQ ('G', '5'),
    ENDSEQ ('P', KEY_Pause),
    MIDDLESEQ ('[', esc_bracket_bracket),
    MIDDLESEQ ('1', esc_bracket_1),
    MIDDLESEQ ('2', esc_bracket_2),
    MIDDLESEQ ('3', esc_bracket_3),
    MIDDLESEQ ('4', esc_bracket_4),
    MIDDLESEQ ('5', esc_bracket_5),
    MIDDLESEQ ('6', esc_bracket_6),
    ENDTAB
  };

static struct prefix esc_o[] =
  {
    ENDSEQ (' ', ' '), /* Keypad */
    ENDSEQ ('I', '\t'), /* Keypad */
    ENDSEQ ('M', '\r'), /* Keypad */
    ENDSEQ ('X', '='), /* Keypad */
    ENDSEQ ('j', '*'), /* Keypad */
    ENDSEQ ('k', '+'), /* Keypad */
    ENDSEQ ('l', ','), /* Keypad */
    ENDSEQ ('m', '-'), /* Keypad */
    ENDSEQ ('o', '/'), /* Keypad */
    ENDSEQ ('p', '0'), /* Keypad */
    ENDSEQ ('q', '1'), /* Keypad */
    ENDSEQ ('r', '2'), /* Keypad */
    ENDSEQ ('s', '3'), /* Keypad */
    ENDSEQ ('t', '4'), /* Keypad */
    ENDSEQ ('u', '5'), /* Keypad */
    ENDSEQ ('v', '6'), /* Keypad */
    ENDSEQ ('w', '7'), /* Keypad */
    ENDSEQ ('x', '8'), /* Keypad */
    ENDSEQ ('y', '9'), /* Keypad */
    ENDSEQ ('P', KEY_F1),
    ENDSEQ ('Q', KEY_F2),
    ENDSEQ ('R', KEY_F3),
    ENDSEQ ('S', KEY_F4),
    ENDTAB
  };

static struct prefix esc[] =
  {
    { 'O', METAIZE, esc_o },
    { '[', METAIZE, esc_bracket },
    { ANYCHAR, METAIZE, 0 },
    ENDTAB
  };

static struct prefix start[] =
  {
    { KEY_Escape, KEY_Escape, esc }, 
    ENDTAB
  };

struct prefix *prefix_start = start;
