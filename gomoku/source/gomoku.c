#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "gomoku.h"

static size_t CountInDirection(const GomokuGame *const game, Point point, const int dx, const int dy, const CellType player) {
    size_t count = 0;
    CellType type = CELL_EMPTY;
    while ((dx >= 0 || point.x > 0) && (dy >= 0 || point.y > 0) && (dx <= 0 || point.x + 1 < game->board_size) && (dy <= 0 || point.y + 1 < game->board_size)) {
        point.x += dx;
        point.y += dy;
        if (MapGet(game->moves, point, &type) != MAP_OK || type != player) {
            break;
        }
        count++;
    }
    return count;
}

GomokuStatus GomokuInit(GomokuGame *const game, const size_t board_size, const size_t win_length) {
    if (!game || board_size == 0 || win_length == 0 || win_length > board_size) {
        return GOMOKU_ERROR;
    }
    game->moves = MapCreate();
    if (!game->moves) {
        return GOMOKU_ERROR;
    }
    game->board_size = board_size;
    game->win_length = win_length;
    game->current_player = CELL_CROSS;
    game->is_finished = false;
    game->moves_count = 0;
    return GOMOKU_OK;
}

bool GomokuCheckWin(const GomokuGame *const game, const Point last_move) {
    CellType player = CELL_EMPTY;
    if (!game || MapGet(game->moves, last_move, &player) != MAP_OK) {
        return false;
    }
    size_t i = 0;
    int directions[DIR_COUNT][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (i = 0; i < DIR_COUNT; i++) {
        size_t count = 1;
        count += CountInDirection(game, last_move, directions[i][0], directions[i][1], player);
        count += CountInDirection(game, last_move, -directions[i][0], -directions[i][1], player);
        if (count >= game->win_length) {
            return true;
        }
    }
    return false;
}

GomokuStatus GomokuProcessTurn(GomokuGame *const game, const Point point) {
    if (!game) {
        return GOMOKU_INVALID_MOVE;
    }
    CellType dummy = CELL_EMPTY;
    MapStatus status = MAP_NOT_VALID;
    if (point.x >= game->board_size || point.y >= game->board_size) {
        return GOMOKU_INVALID_MOVE;
    }
    if (MapGet(game->moves, point, &dummy) == MAP_OK) {
        return GOMOKU_CELL_OCCUPIED;
    }
    status = MapInsert(game->moves, point, game->current_player);
    if (status != MAP_OK) {
        return GOMOKU_INVALID_MOVE;
    }
    game->moves_count++;
    if (GomokuCheckWin(game, point)) {
        game->is_finished = true;
        return GOMOKU_WIN;
    }
    if (game->moves_count >= game->board_size * game->board_size) {
        game->is_finished = true;
        return GOMOKU_DRAW;
    }
    game->current_player = (game->current_player == CELL_CROSS) ? CELL_NOUGHT : CELL_CROSS;
    return GOMOKU_CONTINUE;
}

void GomokuFree(GomokuGame *const game) {
    if (!game) {
        return;
    }
    if (game->moves) {
        MapFree(game->moves);
        game->moves = NULL;
    }
}
