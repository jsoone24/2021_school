#pragma once
#include <vector>
#include "Matrix.h"

enum TetrisState
{
    Running,
    NewBlock,
    Finished
};

struct Idx
{
    int x;
    int y;
    int z;
};

struct BlockShape
{
    int type;
    int degree;
};

struct BlockState
{
    int top;
    int left;
    BlockShape shape;
};

class Tetris
{
public:
    Tetris(int dy, int dx);
    static void init(int *setOfBlockArrays[], int blkTypes, int blkDegrees);
    TetrisState accept(char key);
    virtual ~Tetris();
    Matrix *oScreen;
    static int iScreenDw;

protected:
    int *arrayScreen();
    void deleteFullLines();
    vector<int> fullLine;
    Idx iScreenD;
    static Matrix **setOfBlockObjects;
    static BlockShape nBlock;
    BlockState currBlkState;
    bool justStarted;

private:
    void printSetOfBlock(); //testcode
    int *tempScreen;
    Matrix *iScreen;
    TetrisState state;
    Matrix currBlk;
};
