#include "Tetris.h"

int Tetris::nBlockTypes;
int Tetris::nBlockDegrees;
Matrix** Tetris::setOfBlockObjects;

Tetris::Tetris()
{
	iScreenDy = 0;
	iScreenDx = 0;
	idxBlockDegree = 0;
	arrayScreen = createArrayScreen();
	iScreen = Matrix(arrayScreen);
	oScreen = Matrix(iScreen);
	justStarted = true;
}

Tetris::Tetris(int dy, int dx)
{
	iScreenDy = dy;
	iScreenDx = dx;
	iScreenDw = 0;
	int arrayBlk_maxSize = 0;
	for (int i = 0; i < nBlockTypes; i++)
	{
		if (arrayBlk_maxSize <= Tetris::setOfBlockObjects[i][0].get_dx())
		{
			arrayBlk_maxSize = Tetris::setOfBlockObjects[i][0].get_dx();
		}
	}
	iScreenDw = arrayBlk_maxSize;
	idxBlockDegree = 0;
	arrayScreen = createArrayScreen();
	iScreen = Matrix(arrayScreen);
	oScreen = Matrix(iScreen);
	justStarted = true;
}

Tetris::~Tetris()
{
	for (int i = 0; i < Tetris::nBlockTypes; i++)
	{
		delete[] setOfBlockObjects[i];
	}
	delete[] setOfBlockObjects;
}

void Tetris::init(int** setOfBlockArrays, int MAX_BLK_TYPES, int MAX_BLK_DEGREES)
{
	Tetris::nBlockTypes = MAX_BLK_TYPES;
	Tetris::nBlockDegrees = MAX_BLK_DEGREES;

	Tetris::setOfBlockObjects = new Matrix * [Tetris::nBlockTypes];
	for (int i = 0; i < Tetris::nBlockTypes; i++)
	{
		Tetris::setOfBlockObjects[i] = new Matrix[Tetris::nBlockDegrees];
	}

	for (int i = 0; i < Tetris::nBlockTypes; i++)
	{
		for (int j = 0; j < Tetris::nBlockDegrees; j++)
		{
			int dx = 0;
			while (setOfBlockArrays[Tetris::nBlockDegrees * i + j][dx] != -1)
			{
				dx++;
			}
			dx = (int)sqrt(dx);
			Tetris::setOfBlockObjects[i][j] = Matrix(setOfBlockArrays[Tetris::nBlockDegrees * i + j], dx, dx);
		}
	}
}

Matrix Tetris::createArrayScreen()
{
	int** array;
	arrayScreenDx = iScreenDw * 2 + iScreenDx;
	arrayScreenDy = iScreenDy + iScreenDw;
	arrayScreen = Matrix(arrayScreenDy, arrayScreenDx);
	array = arrayScreen.get_array();

	for (int y = 0; y < iScreenDy; y++)
	{
		for (int x = 0; x < iScreenDw; x++)
		{
			array[y][x] = 1;
		}
		for (int x = 0; x < iScreenDx; x++)
		{
			array[y][iScreenDw + x] = 0;
		}
		for (int x = 0; x < iScreenDw; x++)
		{
			array[y][iScreenDw + iScreenDx + x] = 1;
		}
	}

	for (int y = 0; y < iScreenDw; y++)
	{
		for (int x = 0; x < arrayScreenDx; x++)
		{
			array[iScreenDy + y][x] = 1;
		}
	}

	return arrayScreen;
}
TetrisState Tetris::accept(char key)
{
	state = Running;

	if (key >= '0' && key <= '6')
	{
		if (justStarted == false)
		{
			deleteFullLines();
		}
		iScreen = Matrix(oScreen);
		idxBlockType = atoi(&key);
		idxBlockDegree = 0;
		currBlk = &Tetris::setOfBlockObjects[idxBlockType][idxBlockDegree];
		top = 0;
		left = iScreenDw + (int)(iScreenDx / 2) - (int)(currBlk->get_dx() / 2);
		tempBlk = iScreen.clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
		tempBlk = tempBlk->add(currBlk);
		justStarted = false;

		if (tempBlk->anyGreaterThan(1))
		{
			state = Finished;
		}
		oScreen = Matrix(iScreen);
		oScreen.paste(tempBlk, top, left);

		return state;
	}
	else if (key == 'q')
	{
	}
	else if (key == 'a')
	{ // move left
		left -= 1;
	}
	else if (key == 'd')
	{ // move right
		left += 1;
	}
	else if (key == 's')
	{ // move down
		top += 1;
	}
	else if (key == 'w')
	{ // rotate the block clockwise
		idxBlockDegree = (idxBlockDegree + 1) % Tetris::nBlockDegrees;
		currBlk = &Tetris::setOfBlockObjects[idxBlockType][idxBlockDegree];
	}
	else if (key == ' ')
	{ // drop the block
		while (!(tempBlk->anyGreaterThan(1)))
		{
			top += 1;
			tempBlk = iScreen.clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
			tempBlk = tempBlk->add(currBlk);
		}
	}
	else
	{
		printf("Wrong key!!!\n");
	}

	tempBlk = iScreen.clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
	tempBlk = tempBlk->add(currBlk);

	if (tempBlk->anyGreaterThan(1))
	{ // 벽 충돌시 undo 수행
		if (key == 'a')
		{ // undo: move right
			left += 1;
		}
		else if (key == 'd')
		{ // undo: move left
			left -= 1;
		}
		else if (key == 's')
		{ // undo: move up
			top -= 1;
			state = NewBlock;
		}
		else if (key == 'w')
		{ // undo: rotate the block counter-clockwise
			idxBlockDegree = (idxBlockDegree + 3) % Tetris::nBlockDegrees;
			currBlk = &Tetris::setOfBlockObjects[idxBlockType][idxBlockDegree];
		}
		else if (key == ' ')
		{ // undo: move up
			top -= 1;
			state = NewBlock;
		}

		tempBlk = iScreen.clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
		tempBlk = tempBlk->add(currBlk);
	}

	oScreen = Matrix(iScreen);
	oScreen.paste(tempBlk, top, left);

	return state;
}

void Tetris::deleteFullLines(void)
{
	top = iScreenDy - 1;
	left = iScreenDw;
	Matrix blackBlk = Matrix(1, iScreenDx);
	tempBlk = oScreen.clip(top, left, top + 1, left + iScreenDx); // 밑에서부터 끌어와서 저장

	while (top > 0)
	{ // 값 비교
		if ((tempBlk->binary())->sum() == iScreenDx)
		{
			tempBlk = oScreen.clip(0, left, top, left + iScreenDx);
			oScreen.paste(tempBlk, 1, left);
			oScreen.paste(&blackBlk, 0, left);
		}
		else
		{
			top -= 1;
		}
		tempBlk = oScreen.clip(top, left, top + 1, left + iScreenDx); // 한줄도 통과면 나머지 줄도 검사
	}
}