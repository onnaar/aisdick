#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <ncurses.h>
#include "gomoku.h"
#include "dialogue.h"
#include "map.h"

GomokuStatus PlayMatch(GomokuGame *const game);
int RunMenuLoop();

int main(const int argc, const char *const argv[]) {
    GomokuGame game = {};
    int size = 0;
    int win = 0;
    int selected_option = 0;
    bool game_initialized = false;
    bool in_menu = true;
    if (argc < 3) {
        return 0;
    }
    size = atoi(argv[1]);
    win = atoi(argv[2]);
    if (size < 3 || size > 19 || win < 3 || win > size) {
        printf("error:>\n");
        return 0;
    }
    game.board_size = (size_t)size;
    game.win_length = (size_t)win;
    if (GomokuInit(&game, game.board_size, game.win_length) != GOMOKU_OK) {
        printf("error:>\n");
        return 0;
    }
    game_initialized = true;
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    while (in_menu) {
        selected_option = RunMenuLoop();
        if (selected_option == MENU_EXIT) {
            in_menu = false;
            continue;
        }
        if (selected_option == MENU_START) {
            if (game_initialized) {
                GomokuFree(&game);
                GomokuInit(&game, game.board_size, game.win_length);
            }
            if (PlayMatch(&game) == GOMOKU_ERROR) {
                in_menu = false;
            }
        }
    }
    if (game_initialized) {
        GomokuFree(&game);
    }
    endwin();
    return 0;
}

int RunMenuLoop() {
    MenuItem menu[] = {{"play", MENU_START}, {"exit", MENU_EXIT}};
    int highlight = 0;
    int ch = 0;
    int i = 0;
    int num_options = 2;
    while (true) {
        clear();
        mvprintw(0, 0, "😷😃😍😤😮🙉 MENU 😷😃😍😤😮🙉");
        for (i = 0; i < num_options; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(i + 1, 2, "%s", menu[i].label);
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }
        refresh();
        ch = getch();
        switch (ch) {
            case KEY_UP:
                highlight = (highlight > 0) ? highlight - 1 : num_options - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight < num_options - 1) ? highlight + 1 : 0;
                break;
            case '\n':
                return menu[highlight].action_type;
        }
    }
}

GomokuStatus PlayMatch(GomokuGame *const game) {
    if (!game) {
        return GOMOKU_ERROR;
    }
    GomokuStatus stat = GOMOKU_CONTINUE;
    char *conditions[] = {"OK", "INCORRECT", "INVALID MOVE", "CELL OCCUPIED", "NEXT MOVE", "GAME OVER", "DRAW"};
    char *players[] = {"CROSS WON", "NOUGHT WON"};
    clear();
    DoShowBoard(game);
    while (stat == GOMOKU_CONTINUE) {
        stat = DoMakeTurn(game);
        if (stat == GOMOKU_ERROR) {
            return GOMOKU_ERROR;
        }
        if (stat != GOMOKU_CONTINUE) {
            mvprintw((int)game->board_size * 2 + 5, 0, "status: %s", (stat != GOMOKU_WIN) ? conditions[stat] : (game->current_player == CELL_CROSS) ? players[0] : players[1]);
            refresh();
        }
        if (stat == GOMOKU_INVALID_MOVE || stat == GOMOKU_CELL_OCCUPIED) {
            stat = GOMOKU_CONTINUE;
        }
    }
    mvprintw(game->board_size * 2 + 7, 0, "press any key to return to menu🙏");
    refresh();
    getch();
    return stat;
}
