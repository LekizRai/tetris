#ifndef TETROMINO_H
#define TETROMINO_H

#include "console_output.h"

class Tetromino {
private:
    WORD* pColor;
    wchar_t* pBuffer;
	int* pMatrix;
	int type;
	int rotation;
	int curr_x;
	int curr_y;

public:
	Tetromino() {
		pColor = ConsoleOutput::pColor;
		pBuffer = ConsoleOutput::pBuffer;
		pMatrix = ConsoleOutput::pMatrix;
		type = 5;
		rotation = 0;
		curr_x = boardWidth / 2 - 4;
		curr_y = -4;
	}

	Tetromino(int tetro_type) {
		pColor = ConsoleOutput::pColor;
		pBuffer = ConsoleOutput::pBuffer;
		pMatrix = ConsoleOutput::pMatrix;
		type = tetro_type;
		rotation = 0;
		curr_x = boardWidth / 2 - 4;
		curr_y = -4;
	}

	wchar_t operator[](int index) {
		return tetrominoList[type][rotation][index];
	}

	int get_type() {
		return type;
	}

	int get_rotation() {
		return rotation;
	}

	int get_curr_x() {
		return curr_x;
	}

	int get_curr_y() {
		return curr_y;
	}

	void move_right() {
		curr_x += 2;
	}

	void move_left() {
		curr_x -= 2;
	}

	void rotate() {
		rotation = (rotation + 1) % 4;
	}

	void move_down(int offset) {
		curr_y += offset;
	}

	void update_buffer() {
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (tetrominoList[type][rotation][j * 8 + i] != L'.' && curr_y + j >= 0)
				{
					pColor[(curr_y + j) * screenWidth + (curr_x + i)] = 8 * 16 + type;
					pBuffer[(curr_y + j) * screenWidth + (curr_x + i)] = L'▓';
				}
			}
		}
	}


};

#endif