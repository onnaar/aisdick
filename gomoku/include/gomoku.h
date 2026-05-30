#ifndef GOMOKU_H
#define GOMOKU_H

#include <stdbool.h>
#include "map.h"

typedef enum {
    GOMOKU_OK = 0,
    GOMOKU_ERROR,
    GOMOKU_INVALID_MOVE,
    GOMOKU_CELL_OCCUPIED,
    GOMOKU_CONTINUE,
    GOMOKU_WIN,
    GOMOKU_DRAW
} GomokuStatus;

typedef enum {
    HORIZONTAL = 0,
    VERTICAL,
    MAIN_DIAGONAL,
    ANTI_DIAGONAL,
    DIR_COUNT
} DirectionType;

typedef struct {
    BoardMap *moves;
    size_t board_size;
    size_t win_length;
    size_t moves_count;
    CellType current_player;
    bool is_finished;
} GomokuGame;

GomokuStatus GomokuInit(GomokuGame *const game, const size_t board_size, const size_t win_length);

bool GomokuCheckWin(const GomokuGame *const game, const Point last_move);
GomokuStatus GomokuProcessTurn(GomokuGame *const game, const Point point);

void GomokuFree(GomokuGame *const game);

#endif
