#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "gomoku.h"

typedef struct {
    const char *label;
    int action_type;
} MenuItem;

typedef enum {
    MENU_EXIT = 0,
    MENU_START = 1,
    MENU_NEW_SIZES = 2
} MenuOption;

GomokuStatus DoInit(GomokuGame *const game);
GomokuStatus DoShowBoard(GomokuGame *const game);
GomokuStatus DoMakeTurn(GomokuGame *const game);

#endif
