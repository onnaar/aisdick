#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <string.h>
#include "dialogue.h"
#include "gomoku.h"

GomokuStatus DoShowBoard(GomokuGame *const game) {
    size_t x = 0;
    size_t y = 0;
    int row = 0;
    CellType type = CELL_EMPTY;
    if (!game) {
        return GOMOKU_ERROR;
    }
    mvprintw(row, 4, " ");
    for (x = 0; x < game->board_size; x++) {
        printw(" %2zu ", x);
    }
    row++;
    mvprintw(row++, 4, "┏");
    for (x = 0; x < game->board_size - 1; x++) {
        printw("━━━┳");
    }
    printw("━━━┓");
    for (y = 0; y < game->board_size; y++) {
        if (y > 0) {
            mvprintw(row++, 4, "┣");
            for (x = 0; x < game->board_size - 1; x++) {
                printw("━━━╋");
            }
            printw("━━━┫");
        }
        mvprintw(row++, 0, "%2zu  ┃", y);
        for (x = 0; x < game->board_size; x++) {
            Point p = {x, y};
            if (MapGet((BoardMap *)game->moves, p, &type) == MAP_OK) {
                if (type == CELL_CROSS) {
                    printw(" X ┃");
                }
                if (type == CELL_NOUGHT) {
                    printw(" O ┃");
                }
            }
            if (MapGet((BoardMap *)game->moves, p, &type) != MAP_OK) {
                printw("   ┃");
            }
        }
    }
    mvprintw(row++, 4, "┗");
    for (x = 0; x < game->board_size - 1; x++) {
        printw("━━━┻");
    }
    printw("━━━┛");
    
    return GOMOKU_CONTINUE;
}

GomokuStatus DoMakeTurn(GomokuGame *const game) {
    static int cursor_x = 0;
    static int cursor_y = 0;
    int ch = 0;
    int screen_x = 0;
    int screen_y = 0;
    bool turn_made = false;
    Point p = {};
    GomokuStatus res = GOMOKU_CONTINUE;
    if (!game) {
        return GOMOKU_ERROR;
    }
    while (!turn_made) {
        DoShowBoard(game);
        screen_x = 6 + cursor_x * 4;
        screen_y = 2 + cursor_y * 2;
        mvprintw((int)game->board_size * 2 + 3, 0, "current player: %s", (game->current_player == CELL_CROSS) ? "X" : "O");
        move(screen_y, screen_x);
        curs_set(1);
        refresh();
        ch = getch();
        switch (ch) {
            case KEY_UP:
                if (cursor_y > 0) {
                    cursor_y--;
                }
                break;
            case KEY_DOWN:
                if ((size_t)cursor_y < game->board_size - 1) {
                    cursor_y++;
                }
                break;
            case KEY_LEFT:
                if (cursor_x > 0) {
                    cursor_x--;
                }
                break;
            case KEY_RIGHT:
                if ((size_t)cursor_x < game->board_size - 1) {
                    cursor_x++;
                }
                break;
            case ' ':
            case '\n':
                curs_set(0);
                p.x = (size_t)cursor_x;
                p.y = (size_t)cursor_y;
                res = GomokuProcessTurn(game, p);
                turn_made = true;
                break;
            case 27:
                curs_set(0);
                return GOMOKU_ERROR;
        }
        clear();
    }
    return res;
}
