#ifndef DIALOG_H
#define DIALOG_H

#include "gomoku.h"

typedef enum {
    MENU_EXIT = 0,
    MENU_START,
    MENU_NEW_SIZES,
    MENU_COUNT
} MenuOption;

typedef GomokuStatus (*GomokuAction)(GomokuGame *const game);

GomokuStatus DoInit(GomokuGame *const game);
GomokuStatus DoShowBoard(GomokuGame *const game);
GomokuStatus DoMakeTurn(GomokuGame *const game);
GomokuStatus ProgramEnd(GomokuGame *const game);

#endif
