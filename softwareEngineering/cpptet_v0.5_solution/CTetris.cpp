#include "CTetris.h"

Matrix **CTetris::setOfCBlockObjects = nullptr;

CTetris::CTetris(int dy, int dx) : Tetris::Tetris(dy, dx)
{
    iCScreen = new Matrix(oScreen);
    iCScreen->mulc(nBlock.type + 1);
    oCScreen = new Matrix(iCScreen);
}

void CTetris::init(int *setOfBlockArrays[], int blkTypes, int blkDegrees)
{
    Tetris::init(setOfBlockArrays, blkTypes, blkDegrees);
    setOfCBlockObjects = new Matrix *[nBlock.type];
    for (int i = 0; i < nBlock.type; ++i)
    {
        setOfCBlockObjects[i] = new Matrix[nBlock.degree];
    }

    for (int i = 0; i < nBlock.type; ++i)
    {
        for (int j = 0; j < nBlock.degree; ++j)
        {
            setOfCBlockObjects[i][j] = Matrix(setOfBlockObjects[i][j]);
            setOfCBlockObjects[i][j].mulc(i + 1);
        }
    }
}

TetrisState CTetris::accept(char key)
{
    if (key >= '0' && key <= ('0' + nBlock.type - 1))
    {
        if (justStarted == false)
        {
            deleteFullLines();
        }
        iCScreen->paste(oCScreen, 0, 0);
    }
    TetrisState state = Tetris::accept(key);

    currCBlk = setOfCBlockObjects[currBlkState.shape.type][currBlkState.shape.degree];
    int currBlkStateBottom = currBlkState.top + currCBlk.get_dy();
    int currBlkStateRight = currBlkState.left + currCBlk.get_dx();
    Matrix tempCBlk = iCScreen->clip(currBlkState.top, currBlkState.left, currBlkStateBottom, currBlkStateRight);
    tempCBlk = tempCBlk.add(&currCBlk);

    oCScreen->paste(iCScreen, 0, 0);
    oCScreen->paste(&tempCBlk, currBlkState.top, currBlkState.left);
    return state;
}

void CTetris::deleteFullLines()
{
    Tetris::deleteFullLines();
    iCScreen->paste(oCScreen, 0, 0);

    for (const auto &line : fullLine)
    {
        Matrix tempCBlk = iCScreen->clip(0, iScreenDw, line, iScreenDw + iScreenD.x);
        iCScreen->paste(&tempCBlk, 1, iScreenDw);
    }
    oCScreen->paste(iCScreen, 0, 0);
}

CTetris::~CTetris()
{
    for (int i = 0; i < nBlock.type; ++i)
    {
        delete[] setOfCBlockObjects[i];
    }
    delete[] setOfCBlockObjects;
    delete oCScreen;
    delete iCScreen;
}