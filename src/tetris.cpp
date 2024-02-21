#include "../header/config.h"
#include "../header/game_board.h"
#include "../header/console_output.h"

int main() {
    configure();
    GameBoard board = GameBoard();
    board.init();

    bool stop = false;
    while(!stop) {
        Sleep(100);
        board.input();
        stop = board.execute();
        board.update();
        board.display();
    }

    board.game_over();
    return 0;
}