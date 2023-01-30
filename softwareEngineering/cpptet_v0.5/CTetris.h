#pragma once

#include "Tetris.h"

#define color_black   "\033[37m"
#define color_red     "\033[31m"
#define color_green   "\033[32m"
#define color_yellow  "\033[33m"
#define color_blue    "\033[34m"
#define color_purple  "\033[35m"
#define color_cyan    "\033[36m"
#define color_pink    "\033[95m"
#define color_normal  "\033[37m"
#define b_color_black "\033[37m"

class CTetris : public Tetris
{
public:
        // Matrix Type Variables
        Matrix iCScreen;
        Matrix oCScreen;
        Matrix arrayScreen;
        Matrix *currCBlk;
        static Matrix **setOfCBlockObjects;

        // Constructor and Destructor
        CTetris();
        CTetris(int dy, int dx);
        ~CTetris();

        // Methods
        static void init(int **setOfBlockArrays, int MAX_BLK_TYPES, int MAX_BLK_DEGREES);
        TetrisState accept(int key);
        void deleteFullLines(void);
};