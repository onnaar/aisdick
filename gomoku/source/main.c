#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gomoku.h"
#include "dialogue.h"
#include "input.h"

void MenuPrint(const bool has_played);
GomokuStatus PlayMatch(GomokuGame *const game);

int main() {
    GomokuGame game = {};
    GomokuAction cur = NULL;
    GomokuAction func_array[MENU_COUNT] = {[MENU_EXIT] = ProgramEnd, [MENU_START] = DoInit, [MENU_NEW_SIZES] = DoInit};
    int option = 0;
    InputStatus inp_stat = INPUT_OK;
    bool game_initialized = false;
    GomokuStatus init_stat = GOMOKU_OK;
    while (cur != ProgramEnd) {
        MenuPrint(game_initialized);
        printf("select an option:\n");
        inp_stat = GetInt(&option, 0, game_initialized ? MENU_NEW_SIZES : MENU_START);
        if (inp_stat != INPUT_OK) {
            printf("\nINPUT END\n\n");
            break;
        }
        if (option == MENU_START || option == MENU_NEW_SIZES) {
            if (game_initialized) {
                GomokuFree(&game);
                if (option == MENU_NEW_SIZES) {
                    game.board_size = 0;
                    game.win_length = 0;
                }
            }
            init_stat = DoInit(&game);
            if (init_stat == GOMOKU_ERROR) {
                if (feof(stdin)) {
                    printf("\nINPUT END\n\n");
                    break;
                }
                printf("\nINCORRECT\n\n");
            }
            if (init_stat == GOMOKU_OK) {
                game_initialized = true;
                if (PlayMatch(&game) == GOMOKU_ERROR) {
                    printf("\nINPUT END\n\n");
                    break;
                }
            }
            printf("\n");
            continue;
        }
        cur = func_array[option];
        printf("\n");
    }
    if (game_initialized) {
        GomokuFree(&game);
    }
    return 0;
}

GomokuStatus PlayMatch(GomokuGame *const game) {
    if (!game) {
        return GOMOKU_ERROR;
    }
    GomokuStatus stat = GOMOKU_CONTINUE;
    char *conditions[] = { [GOMOKU_OK] = "OK", [GOMOKU_ERROR] = "INCORRECT", [GOMOKU_INVALID_MOVE] = "INVALID MOVE", [GOMOKU_CELL_OCCUPIED] = "CELL OCCUPIED", [GOMOKU_CONTINUE] = "NEXT MOVE", [GOMOKU_WIN] = "GAME OVER", [GOMOKU_DRAW] = "DRAW" };
    DoShowBoard(game);
    while (stat == GOMOKU_CONTINUE) {
        stat = DoMakeTurn(game);
        if (stat == GOMOKU_ERROR) {
            return GOMOKU_ERROR;
        }
        printf("\n%s\n\n", conditions[stat]);
        if (stat == GOMOKU_INVALID_MOVE || stat == GOMOKU_CELL_OCCUPIED) {
            stat = GOMOKU_CONTINUE;
        }
    }
    return stat;
}

void MenuPrint(const bool has_played) {
    printf("%d: program end\n", MENU_EXIT);
    if (!has_played) {
        printf("%d: start game\n", MENU_START);
    }
    if (has_played) {
        printf("%d: play again (same sizes)\n", MENU_START);
        printf("%d: play again (new sizes)\n", MENU_NEW_SIZES);
    }
}
