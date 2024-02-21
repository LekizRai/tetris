#ifndef CONSOLEOUTPUT_H
#define CONSOLEOUTPUT_H

#include "common.h"

class ConsoleOutput {
private:
    static HANDLE hConsole;
    static WORD* pColor;
    static wchar_t* pBuffer;
    static int* pMatrix;

    static void init() {
        SetConsoleActiveScreenBuffer(hConsole);
    }

    static void display() {
        DWORD dwBytesWritten = 0;
        for (int i = 0; i < screenWidth; i++)
        {
            for (int j = 0; j < screenHeight; j++)
            {
                COORD char_coor;
                char_coor.X = i;
                char_coor.Y = j;
                WriteConsoleOutputCharacter(hConsole, &pBuffer[j * screenWidth + i], 1, char_coor, &dwBytesWritten);
                WriteConsoleOutputAttribute(hConsole, &pColor[j * screenWidth + i], 1, char_coor, &dwBytesWritten);
            }
        }
    }

    ~ConsoleOutput() {
        delete[] pColor;
        delete[] pBuffer;
    }

    friend class GameBoard;
    friend class Tetromino;
    friend class InfoBoard;
};

HANDLE ConsoleOutput::hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 
                                                    0, NULL, CONSOLE_TEXTMODE_BUFFER, 
                                                    NULL);
WORD* ConsoleOutput::pColor = new WORD[screenWidth * screenHeight];
wchar_t* ConsoleOutput::pBuffer = new wchar_t[screenWidth * screenHeight];
int* ConsoleOutput::pMatrix = new int[boardWidth * boardHeight];

#endif 