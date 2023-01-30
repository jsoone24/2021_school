#pragma once

#include "Matrix.h"
#include <cmath>

enum TetrisState
{
    Running,
    NewBlock,
    Finished
};

class Tetris
{
public:
    // Static Type Variables
    static int nBlockTypes;
    static int nBlockDegrees;
    static Matrix **setOfBlockObjects;

    // Integer Type Variables
    int iScreenDw = 0;
    int iScreenDx = 0;
    int iScreenDy = 0;

    int arrayScreenDx = 0;
    int arrayScreenDy = 0;

    int idxBlockType = 0;
    int idxBlockDegree = 0;

    int top = 0;
    int left = 0;

    // Matrix Type Variables
    Matrix iScreen;
    Matrix oScreen;
    Matrix arrayScreen;
    Matrix *currBlk;
    Matrix *tempBlk;

    // Other Type Variables
    TetrisState state;
    bool justStarted = 0;

    // Constructor and Destructor
    Tetris();
    Tetris(int dy, int dx);
    ~Tetris();
    
    // Methods
    static void init(int **setOfBlockArrays, int MAX_BLK_TYPES, int MAX_BLK_DEGREES);
    Matrix createArrayScreen();
    TetrisState accept(char key);
    void deleteFullLines();
};