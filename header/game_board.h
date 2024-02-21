#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "console_output.h"
#include "info_board.h"
#include "tetromino.h"

class GameBoard {
private:
    // From ConsoleOutput
    WORD* pColor; 
    // From ConsoleOutput
    wchar_t* pBuffer; 
    // From ConsoleOutput
    int* pMatrix; 

    // Information area on playing board
    InfoBoard info_board;
    // Current piece on playing board
    Tetromino active_piece;
    int next_type;

    // List of deleted lines when full-brick rows appear
    vector<int> deleted_lines;
    bool pressed_keys[4];
    bool go_down;
    int frame_num;
    bool is_over;

    int random()
	{
		random_device rd;
		mt19937 rng(rd());
		uniform_int_distribution<int> uni(0, 6);
		auto num = uni(rng);
		return num;
	}

    void set_background() {
        for (int i = 0; i < screenWidth; i++)
        {
            for (int j = 0; j < screenHeight; j++)
            {
                pBuffer[j * screenWidth + i] = L' ';
                pColor[j * screenWidth + i] = 7 * 16;
            }
        }
    }

    void set_bucket() {
        for (int i = 0; i < boardWidth; i++)
        {
            for (int j = 0; j < boardHeight; j++)
            {
                if (j == boardHeight - 1)
                {
                    if (i == 0)
                    {
                        pMatrix[j * boardWidth + i] = 4;
                    }
                    else if (i == boardWidth - 1)
                    {
                        pMatrix[j * boardWidth + i] = 5;
                    }
                    else
                    {
                        pMatrix[j * boardWidth + i] = 7;
                    }
                }
                else
                {
                    if (i == 0 || i == boardWidth - 1)
                    {
                        pMatrix[j * boardWidth + i] = 6;
                    }
                    else
                    {
                        pMatrix[j * boardWidth + i] = 0;
                    }
                }
                if(i > 0 && i < boardWidth - 1 && j < boardHeight - 1) {
                    pColor[j * screenWidth + i] = 8 * 16;
                }
            }
        }
    }

    bool is_valid(int tetromino_rotation, int x_offset, int y_offset) {
        int tetromino_type = active_piece.get_type();
        int x_coor = active_piece.get_curr_x() + x_offset;
        int y_coor = active_piece.get_curr_y() + y_offset;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (x_coor + i >= 0 && x_coor + i < boardWidth)
				{
					if (y_coor + j >= 0 && y_coor + j < boardHeight)
					{
						if (tetrominoList[tetromino_type][tetromino_rotation][j * 8 + i] != L'.' 
                            && pMatrix[(y_coor + j) * boardWidth + (x_coor + i)] != 0)
						{
							return 0;
						}
					}
				}
                else
                {
                    if (tetrominoList[tetromino_type][tetromino_rotation][j * 8 + i] != L'.')
                    {
                        return 0;
                    }
                }
			}
		}
		return 1;
	}

public:
    GameBoard() {
        pColor = ConsoleOutput::pColor;
        pBuffer = ConsoleOutput::pBuffer;
        pMatrix = ConsoleOutput::pMatrix;
        active_piece = Tetromino(random());
        next_type = random();
        go_down = false;
        frame_num = 0;
        is_over = false;
    }

    void init() {
        ConsoleOutput::init();
        set_background();
        set_bucket();
        info_board.init();
        ConsoleOutput::display();
    }

    void input() {
        pressed_keys[0] = (GetAsyncKeyState('R')) != 0;
        pressed_keys[1] = (GetAsyncKeyState('D')) != 0;
        pressed_keys[2] = (GetAsyncKeyState(VK_LEFT)) != 0;
        pressed_keys[3] = (GetAsyncKeyState(VK_RIGHT)) != 0;
    }

    bool execute() {
        frame_num += 1;
        if(frame_num == 5) {
            go_down = true;
        }
        else {
            go_down = false;
        }

        if (pressed_keys[2] == 1)
        {
            if (is_valid(active_piece.get_rotation(), -2, 0) == 1)
            {
                active_piece.move_left();
            }
        }

        if (pressed_keys[3] == 1)
        {
            if (is_valid(active_piece.get_rotation(), 2, 0) == 1)
            {
                active_piece.move_right();
            }
        }

        if (pressed_keys[0] == 1 
            && is_valid((active_piece.get_rotation() + 1) % 4, 0, 0) == 1)
        {
            active_piece.rotate();
        }

        if (pressed_keys[1] == 1)
		{
            int i{};
            while (is_valid(active_piece.get_rotation(), 0, 0 + i) == 1)
            {
                i++;
            }
            active_piece.move_down(i - 1);
        }

        if(go_down) {
            frame_num = 0;

            if (is_valid(active_piece.get_rotation(), 0, 1))
            {
                active_piece.move_down(1);
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if ((active_piece.get_curr_y() + j >= 0) && active_piece[j * 8 + i] != L'.')
                        {
                            pMatrix[(active_piece.get_curr_y() + j) * boardWidth + (active_piece.get_curr_x() + i)] = 1;
                            pColor[(active_piece.get_curr_y() + j) * screenWidth + (active_piece.get_curr_x() + i)] = 8 * 16 + active_piece.get_type();
                        }
                    }
                }

                if ((active_piece.get_type() != 3 && active_piece.get_curr_y() < 0)
                    || (active_piece.get_type() == 3 && active_piece.get_curr_y() < -1))     
                {
                    is_over = true;
                    return true;
                }
                else
                {
                    for (int j = 0; j < 4; j++)
                    {
                        if (active_piece.get_curr_y() + j < boardHeight - 1
                            && active_piece.get_curr_y() + j >= 0)
                        {
                            bool bLine = 1;
                            for (int i = 1; i < boardWidth - 1; i++)
                            {
                                if (pMatrix[(active_piece.get_curr_y() + j) * boardWidth + i] == 0)
                                {
                                    bLine = 0;
                                    break;
                                }
                            }

                            if (bLine == 1)
                            {
                                for (int i = 1; i < boardWidth - 1; i++)
                                {
                                    pMatrix[(active_piece.get_curr_y() + j) * boardWidth + i] = 3;
                                }
                                deleted_lines.push_back(active_piece.get_curr_y() + j);
                            }
                        }
                    }

                    info_board.add_score(25);
                    if (!deleted_lines.empty())
                    {
                        info_board.add_score((1 << deleted_lines.size()) * 100);
                    }

                    active_piece = Tetromino(next_type);
                    next_type = random();
                }
            }
        }
        return false;
    }

    void update() {
        for (int i = 0; i < boardWidth; i++)
        {
            for (int j = 0; j < boardHeight; j++)
            {
                pBuffer[j * screenWidth + i] = detail[pMatrix[j * boardWidth + i]];
            }
        }

        if(!is_over) active_piece.update_buffer();
        info_board.update_buffer(next_type);
    }

    void display() {
        if (!deleted_lines.empty())
        {
            ConsoleOutput::display();
            Sleep(400);

            for (int l = 0; l < deleted_lines.size(); l++)
            {
                for (int i = 1; i < boardWidth - 1; i++)
                {
                    for (int j = deleted_lines.at(l); j > 0; j--)
                    {
                        if (j % 2 == 0)
                        {
                            if (i % 4 == 1 || i % 4 == 2)
                            {
                                pColor[j * screenWidth + i] = pColor[(j - 1) * screenWidth + i];
                            }
                            else
                            {
                                pColor[j * screenWidth + i] = pColor[(j - 1) * screenWidth + i];
                            }
                        }
                        else
                        {
                            if (i % 4 == 1 || i % 4 == 2)
                            {
                                pColor[j * screenWidth + i] = pColor[(j - 1) * screenWidth + i];
                            }
                            else
                            {
                                pColor[j * screenWidth + i] = pColor[(j - 1) * screenWidth + i];
                            }
                        }
                        pMatrix[j * boardWidth + i] = pMatrix[(j - 1) * boardWidth + i];
                    }
                }
            }
            deleted_lines.clear();
        }
        ConsoleOutput::display();
    }

    void game_over() {
        bool add = true;
        int x_coor = 1;
        int y_coor = 0;
        while(true) {
            Sleep(800);
            set_bucket();
            update();
            wstring content[12] = {L".████.████.████.███.",
                                   L".█....█..█.█▐▌█.█....",
                                   L".█.██.████.█▐▌█.███.",
                                   L".█..█.█..█.█..█.█....",
                                   L".████.█..█.█..█.███.",
                                   L"....................",
                                   L"....................",
                                   L".████.█..█.███.████.",
                                   L".█..█.█..█.█...█..█.",
                                   L".█..█.▐▌▐▌.███.████.",
                                   L".█..█.▐▌▐▌.█...█▐█..",
                                   L".████..▐▌..███.█..█."};
            for (int i = 0; i < 12; i++)
            {
                for (int j = 0; j < content[i].length(); j++)
                {
                    if(content[i][j] != L'.') {
                        pBuffer[(y_coor + i) * screenWidth + (j + 1)] = content[i][j];
                        pColor[(y_coor + i) * screenWidth + (j + 1)] = 8 * 16 + 7;
                    }
                }
            }
            ConsoleOutput::display();
            if(add) {
                y_coor += 1;
                if(y_coor >= 8) {
                    add = false;
                }
            }
            else {
                y_coor -= 1;
                if(y_coor <= 0) {
                    add = true;
                }
            }
        }
    }

    ~GameBoard() {};

    friend class ConsoleOutput;
};

#endif