#ifndef INFO_BOARD_H
#define INFO_BOARD_H

#include "console_output.h"

class InfoBoard {
private:
    WORD* pColor;
    wchar_t* pBuffer;
    int* pMatrix;
    int score;
    int score_x;
    int score_level;

    void set_information_area(wstring caption, int width, int height, int x_coor, int y_coor) 
    {
        for (int i = x_coor; i < width + x_coor; i++)
        {
            for (int j = y_coor; j < height + y_coor; j++)
            {
                if (i == x_coor)
                {
                    if (j == y_coor)
                    {
                        pBuffer[j * screenWidth + i] = L'╔';
                    }
                    else if (j == height + y_coor - 1)
                    {
                        pBuffer[j * screenWidth + i] = L'╚';
                    }
                    else
                    {
                        pBuffer[j * screenWidth + i] = L'║';
                    }
                }
                else if (i == width + x_coor - 1)
                {
                    if (j == y_coor)
                    {
                        pBuffer[j * screenWidth + i] = L'╗';
                    }
                    else if (j == height + y_coor - 1)
                    {
                        pBuffer[j * screenWidth + i] = L'╝';
                    }
                    else
                    {
                        pBuffer[j * screenWidth + i] = L'║';
                    }
                }
                else
                {
                    if (j == y_coor || j == height + y_coor - 1)
                    {
                        pBuffer[j * screenWidth + i] = L'═';
                    }
                    else
                    {
                        pBuffer[j * screenWidth + i] = L' ';
                    }

                }
            }
        }

        int CapIndex = y_coor * screenWidth + (x_coor + 1);
        for (int i = 0; i < caption.length(); i++, CapIndex++)
        {
            pBuffer[CapIndex] = caption.at(i);
        }
    }

    void update_next_block(int next_type, int x_coor, int y_coor)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 8; i++)
            {
                if (tetrominoList[next_type][0][j * 8 + i] != L'.')
                {
                    pBuffer[(y_coor + j) * screenWidth + (x_coor + i)] = L'█';
                }
                else
                {
                    pBuffer[(y_coor + j) * screenWidth + (x_coor + i)] = L' ';
                }
                pColor[(y_coor + j) * screenWidth + (x_coor + i)] = 7 * 16 + next_type;
            }
        }
    }

    void update_text(wstring content, int x_coor, int y_coor)
    {
        for (int i = 0; i < content.length(); i++, x_coor++)
        {
            pBuffer[y_coor * screenWidth + x_coor] = content.at(i);
        }
    }

    void set_instruction() {
        int x_coor = 24;
        int y_coor = 2;
        wstring instructions[6] = {L"⯇: Move left",
                                   L"⯈: Move right",
                                   L"D: Drop",
                                   L"R: Rotate",
                                   L"",
                                   L"ENJOY THE GAME!"};

        for (int i = 0; i < 6; i++) {
            if (i == 5) x_coor -= 1;
            for (int j = 0; j < instructions[i].length(); j++) {
                pBuffer[(y_coor + i) * screenWidth + (x_coor + j)] = instructions[i][j];
                if (j == 0 && i != 5) {
                    pColor[(y_coor + i) * screenWidth + (x_coor + j)] = 16 * 7 + 4;
                }
            }
        }
    }
    
public:
    InfoBoard() {
        pColor = ConsoleOutput::pColor;
        pBuffer = ConsoleOutput::pBuffer;
        pMatrix = ConsoleOutput::pMatrix;
        score = 0;
        score_x = 37;
        score_level = 10;
    }

    void init() {
        set_information_area(L"[ TETRIS GAME ]", 17, 10, 22, 0);
        set_instruction();
        set_information_area(L"[ SCORE ]", 17, 3, 22, 12);
        set_information_area(L"[ NEXT BLOCK ]", 17, 6, 22, 15);
    }

    void update_buffer(int next_type) {
        update_next_block(next_type, 26, 16);

        if (score >= score_level)
        {
            score_x--;
            score_level *= 10;
        }
        update_text(to_wstring(score), score_x, 13);
    }

    void add_score(int score_offset) {
        score += score_offset;
    }
};

#endif