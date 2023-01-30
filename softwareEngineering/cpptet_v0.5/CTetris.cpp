#include "CTetris.h"

Matrix **CTetris::setOfCBlockObjects;

CTetris::CTetris(void) : Tetris()
{
    arrayScreen = createArrayScreen();
    iCScreen = Matrix(arrayScreen);
    oCScreen = Matrix(iCScreen);
}

CTetris::CTetris(int dy, int dx) : Tetris(dy, dx)
{
        arrayScreen = createArrayScreen();
        iCScreen = Matrix(arrayScreen);
        oCScreen = Matrix(iCScreen);
}

CTetris::~CTetris(void)
{
    for (int i = 0; i < Tetris::nBlockTypes; i++)
    {
        delete[] setOfCBlockObjects[i];
    }
    delete[] setOfCBlockObjects;
}

void CTetris::init(int **setOfBlockArrays, int MAX_BLK_TYPES, int MAX_BLK_DEGREES)
{
        Tetris::init(setOfBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);

        CTetris::setOfCBlockObjects = new Matrix *[Tetris::nBlockTypes];
        for (int i = 0; i < Tetris::nBlockTypes; i++)
        {
                CTetris::setOfCBlockObjects[i] = new Matrix[Tetris::nBlockDegrees];
        }

        for (int i = 0; i < Tetris::nBlockTypes; i++)
        {
                for (int j = 0; j < Tetris::nBlockDegrees; j++)
                {
                        CTetris::setOfCBlockObjects[i][j] = Matrix(Tetris::setOfBlockObjects[i][j]);
                        CTetris::setOfCBlockObjects[i][j].mulc(i + 2);
                }
        }
}

TetrisState CTetris::accept(int key)
{
        if (key >= '0' && key <= '6')
        {
                if (justStarted == false)
                {
                        deleteFullLines();
                }
                iCScreen = Matrix(oCScreen); // 현재 oScreen 받아옴
        }

        state = Tetris::accept(key);

        currCBlk = &CTetris::setOfCBlockObjects[idxBlockType][idxBlockDegree];
        Matrix * tempCBlk = iCScreen.clip(top, left, top + currCBlk->get_dy(), left + currCBlk->get_dx());
        // 설정된 값으로 블럭 모양 읽어옴

        tempCBlk = tempCBlk->add(currCBlk);

        oCScreen = Matrix(iCScreen);
        oCScreen.paste(tempCBlk, top, left); // 아니면 출력

        return state;
}

void CTetris::deleteFullLines(void)
{
    top = iScreenDy - 1;
    left = iScreenDw;
    Matrix blackBlk = Matrix(1, iScreenDx);
    Matrix *tempCBlk = oCScreen.clip(top, left, top + 1, left + iScreenDx); // 밑에서부터 끌어와서 저장

    while (top > 0)
    { // 값 비교
        if ((tempCBlk->binary())->sum() == iScreenDx)
        {
            tempCBlk = oCScreen.clip(0, left, top, left + iScreenDx);
            oCScreen.paste(tempCBlk, 1, left);
            oCScreen.paste(&blackBlk, 0, left);
        }
        else
        {
            top -= 1;
        }
        tempCBlk = oCScreen.clip(top, left, top + 1, left + iScreenDx); // 한줄도 통과면 나머지 줄도 검사
    }
}