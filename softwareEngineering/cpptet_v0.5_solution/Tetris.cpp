#include <iostream>
#include <cstring>
#include <cmath>
#include "Tetris.h"

Matrix **Tetris::setOfBlockObjects = nullptr;
int Tetris::iScreenDw = 0;
BlockShape Tetris::nBlock = {0, 0};

Tetris::Tetris(int dy, int dx)
{
    iScreenD.y = dy;
    iScreenD.x = dx;
    int aryScreenDy = iScreenD.y + iScreenDw;
    int aryScreenDx = iScreenD.x + iScreenDw * 2;
    iScreen = new Matrix(arrayScreen(), aryScreenDy, aryScreenDx);
    oScreen = new Matrix(iScreen);
    justStarted = true;
}

int blkArrayLength(int *setOfBlockArrays[], int num)
{
    int size = 0;
    while (setOfBlockArrays[num][size] != -1)
    {
        size += 1;
    }
    return sqrt(size);
}

void Tetris::init(int *setOfBlockArrays[], int blkTypes, int blkDegrees)
{
    nBlock.type = blkTypes;
    nBlock.degree = blkDegrees;
    setOfBlockObjects = new Matrix *[nBlock.type];
    for (int i = 0; i < nBlock.type; ++i)
    {
        setOfBlockObjects[i] = new Matrix[nBlock.degree];
    }

    int maxLength = 0;
    for (int i = 0; i < nBlock.type; ++i)
    {
        for (int j = 0; j < nBlock.degree; ++j)
        {
            int len = blkArrayLength(setOfBlockArrays, i * nBlock.degree + j);
            setOfBlockObjects[i][j] = Matrix(setOfBlockArrays[i * nBlock.degree + j], len, len);
            // 위에 new 안붙여도, 붙여도 정상 작동 함 , new 붙여야하나?
            if (maxLength < len)
            {
                maxLength = len;
            }
        }
    }
    iScreenDw = maxLength;
}

TetrisState Tetris::accept(char key)
{
    state = Running;
    Matrix tempBlk;
    int currBlkStateBottom, currBlkStateRight;

    if (key >= '0' && key <= ('0' + nBlock.type - 1))
    {
        if (justStarted == false)
        {
            deleteFullLines();
        }
        iScreen->paste(oScreen, 0, 0);
        currBlkState.shape.type = key - '0';
        currBlkState.shape.degree = 0;
        currBlk = setOfBlockObjects[currBlkState.shape.type][currBlkState.shape.degree];
        currBlkState.top = 0;
        currBlkState.left = iScreenDw + iScreenD.x / 2 - currBlk.get_dx() / 2;
        currBlkStateBottom = currBlkState.top + currBlk.get_dy();
        currBlkStateRight = currBlkState.left + currBlk.get_dx();
        tempBlk = iScreen->clip(currBlkState.top, currBlkState.left, currBlkStateBottom, currBlkStateRight);
        tempBlk = tempBlk.add(&currBlk);
        justStarted = false;
        cout << endl;

        if (tempBlk.anyGreaterThan(1))
        {
            state = Finished;
        }
        oScreen->paste(iScreen, 0, 0);
        oScreen->paste(&tempBlk, currBlkState.top, currBlkState.left);
        return state;
    }
    else if (key == 'q')
    {
    }
    else if (key == 'a')
    {
        currBlkState.left -= 1;
    }
    else if (key == 'd')
    {
        currBlkState.left += 1;
    }
    else if (key == 's')
    {
        currBlkState.top += 1;
    }
    else if (key == 'w')
    {
        currBlkState.shape.degree += 1;
        currBlkState.shape.degree %= nBlock.degree;
        currBlk = setOfBlockObjects[currBlkState.shape.type][currBlkState.shape.degree];
    }
    else if (key == ' ')
    {
        while (!tempBlk.anyGreaterThan(1))
        {
            currBlkState.top += 1;
            currBlkStateBottom = currBlkState.top + currBlk.get_dy();
            currBlkStateRight = currBlkState.left + currBlk.get_dx();
            tempBlk = iScreen->clip(currBlkState.top, currBlkState.left, currBlkStateBottom, currBlkStateRight);
            tempBlk = tempBlk.add(&currBlk);
        }
    }

    currBlkStateBottom = currBlkState.top + currBlk.get_dy();
    currBlkStateRight = currBlkState.left + currBlk.get_dx();
    tempBlk = iScreen->clip(currBlkState.top, currBlkState.left, currBlkStateBottom, currBlkStateRight);
    tempBlk = tempBlk.add(&currBlk);

    if (tempBlk.anyGreaterThan(1))
    { // undo
        if (key == 'a')
        {
            currBlkState.left += 1;
        }
        else if (key == 'd')
        {
            currBlkState.left -= 1;
        }
        else if (key == 's')
        {
            currBlkState.top -= 1;
            state = NewBlock;
        }
        else if (key == 'w')
        {
            currBlkState.shape.degree += (nBlock.degree - 1);
            currBlkState.shape.degree %= nBlock.degree;
            currBlk = setOfBlockObjects[currBlkState.shape.type][currBlkState.shape.degree];
        }
        else if (key == ' ')
        {
            currBlkState.top -= 1;
            state = NewBlock;
        }
        currBlkStateBottom = currBlkState.top + currBlk.get_dy();
        currBlkStateRight = currBlkState.left + currBlk.get_dx();
        tempBlk = iScreen->clip(currBlkState.top, currBlkState.left, currBlkStateBottom, currBlkStateRight);
        tempBlk = tempBlk.add(&currBlk);
    }
    oScreen->paste(iScreen, 0, 0);
    oScreen->paste(&tempBlk, currBlkState.top, currBlkState.left);
    return state;
}

int *Tetris::arrayScreen()
{
    int aryScreenDy = iScreenD.y + iScreenDw;
    int aryScreenDx = iScreenD.x + iScreenDw * 2;

    tempScreen = new int[aryScreenDy * aryScreenDx];
    memset(tempScreen, 0, sizeof(int) * aryScreenDy * aryScreenDx);

    for (int y = 0; y < iScreenD.y; ++y)
    {
        for (int x = iScreenDw; x < iScreenD.x + iScreenDw; ++x)
        {
            tempScreen[y * (aryScreenDx) + x] = -1;
        }
    }

    for (int y = 0; y < aryScreenDy; ++y)
    {
        for (int x = 0; x < aryScreenDx; ++x)
        {
            tempScreen[y * (aryScreenDx) + x] += 1;
        }
    }

    return tempScreen;
}

void Tetris::deleteFullLines()
{
    iScreen->paste(oScreen, 0, 0);
    int currBlkBottom = currBlkState.top + currBlk.get_dy();
    currBlkBottom = currBlkBottom < iScreenD.y ? currBlkBottom : iScreenD.y;

    fullLine.clear();
    for (int line = currBlkState.top; line < currBlkBottom; ++line)
    {
        Matrix tempBlk = iScreen->clip(line, iScreenDw, line + 1, iScreenDw + iScreenD.x);
        if (iScreenD.x == tempBlk.sum())
        {
            fullLine.push_back(line);
            tempBlk = iScreen->clip(0, iScreenDw, line, iScreenDw + iScreenD.x);
            iScreen->paste(&tempBlk, 1, iScreenDw);
        }
    }
    oScreen->paste(iScreen, 0, 0);
}

Tetris::~Tetris()
{
    for (int i = 0; i < nBlock.type; ++i)
    {
        delete[] setOfBlockObjects[i];
    }
    delete[] setOfBlockObjects;
    delete[] tempScreen;
    delete oScreen;
    delete iScreen;
}

//test
void Tetris::printSetOfBlock()
{
    for (int i = 0; i < nBlock.type; ++i)
    {
        for (int j = 0; j < nBlock.degree; ++j)
        {
            int **tempArr = setOfBlockObjects[i][j].get_array();
            int dy = setOfBlockObjects[i][j].get_dy();
            int dx = setOfBlockObjects[i][j].get_dx();

            for (int y = 0; y < dy; ++y)
            {
                for (int x = 0; x < dx; ++x)
                {
                    if (tempArr[y][x] == 0)
                        cout << "□";
                    else /* if (array[y][x] == 1)*/
                        cout << "■";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
}
