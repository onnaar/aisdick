#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "gomoku.h"
#include "dialogue.h"
#include "input.h"

//😷😷 

GomokuStatus DoInit(GomokuGame *const game) {
    if (!game) {
        return GOMOKU_ERROR;
    }
    size_t board_size = 0;
    size_t win_length = 0;
    InputStatus stat = INPUT_OK;
    if (game->board_size > 0) {
        return GomokuInit(game, game->board_size, game->win_length);
    }
    printf("enter board size:\n");
    stat = GetSizeT(&board_size);
    if (stat != INPUT_OK) {
        return GOMOKU_ERROR;
    }
    printf("enter win length:\n");
    stat = GetSizeT(&win_length);
    if (stat != INPUT_OK) {
        return GOMOKU_ERROR;
    }
    return GomokuInit(game, board_size, win_length);
}

GomokuStatus DoShowBoard(GomokuGame *const game) {
    if (!game) {
        return GOMOKU_ERROR;
    }
    size_t x = 0;
    size_t y = 0;
    CellType type = CELL_EMPTY;
    printf("\033[H\033[J    ");
    for (x = 0; x < game->board_size; x++) {
        printf(" %2zu ", x);
    }
    printf("\n");
    printf("    ┏");
    for (x = 0; x < game->board_size - 1; x++) {
        printf("━━━┳");
    }
    printf("━━━┓\n");
    for (y = 0; y < game->board_size; y++) {
        if (y > 0) {
            printf("    ┣");
            for (x = 0; x < game->board_size - 1; x++) {
                printf("━━━╋");
            }
            printf("━━━┫\n");
        }
        printf("%2zu  ┃", y);
        for (x = 0; x < game->board_size; x++) {
            Point p = {x, y};
            if (MapGet((BoardMap *)game->moves, p, &type) == MAP_OK) {
                if (type == CELL_CROSS) {
                    printf(" X ┃");
                }
                if (type == CELL_NOUGHT) {
                    printf(" O ┃");
                }
            }
            if (MapGet((BoardMap *)game->moves, p, &type) != MAP_OK) {
                printf("   ┃");
            }
        }
        printf("\n");
    }
    printf("    ┗");
    for (x = 0; x < game->board_size - 1; x++) {
        printf("━━━┻");
    }
    printf("━━━┛\n");
    return GOMOKU_CONTINUE;
}

GomokuStatus DoMakeTurn(GomokuGame *const game) {
    if (!game) {
        return GOMOKU_ERROR;
    }
    Point p = {0, 0};
    InputStatus stat = INPUT_OK;
    GomokuStatus turn_stat = GOMOKU_CONTINUE;
    printf("current player: %s\n", (game->current_player == CELL_CROSS) ? "X" : "O");
    printf("enter X coordinate:\n");
    stat = GetSizeT(&p.x);
    if (stat != INPUT_OK) {
        return GOMOKU_ERROR;
    }
    printf("enter Y coordinate:\n");
    stat = GetSizeT(&p.y);
    if (stat != INPUT_OK) {
        return GOMOKU_ERROR;
    }
    turn_stat = GomokuProcessTurn(game, p);
    if (turn_stat == GOMOKU_CONTINUE || turn_stat == GOMOKU_WIN || turn_stat == GOMOKU_DRAW) {
        DoShowBoard(game);
    }
    return turn_stat;
}

GomokuStatus ProgramEnd(GomokuGame *const game) {
    (void)game;
    return GOMOKU_OK;
}
