#pragma once
#include "Tetris.h"

class CTetris : public Tetris
{
public:
    CTetris(int dy, int dx);
    static void init(int *setOfBlockArrays[], int blkTypes, int blkDegrees);
    TetrisState accept(char key);
    ~CTetris();

    Matrix *oCScreen;

private:
    void deleteFullLines();
    Matrix *iCScreen;
    static Matrix **setOfCBlockObjects;
    Matrix currCBlk;
};
