from tetris import *
from matrix import *
from enum import Enum


class CTetris(Tetris):
    setOfCBlockObjects = 0

    @classmethod
    def init(cls, setOfBlockArrays):
        Tetris.init(setOfBlockArrays)
        CTetris.setOfCBlockObjects = [[0] * Tetris.nBlockDegrees for _ in range(Tetris.nBlockTypes)]

        for i in range(Tetris.nBlockTypes):
            for j in range(Tetris.nBlockDegrees):
                obj = Matrix(setOfBlockArrays[i][j])
                obj.mulc(i+1)
                CTetris.setOfCBlockObjects[i][j] = obj
        return

    def __init__(self, cy, cx):
        Tetris.__init__(self, cy, cx)
        arrayScreen = self.createArrayScreen()
        self.iCScreen = Matrix(arrayScreen)
        self.oCScreen = Matrix(self.iCScreen)
        return

    def accept(self, key):
        if key >= '0' and key <= '6':
            if self.justStarted == False:
                self.deleteFullLines()
            self.iCScreen = Matrix(self.oCScreen)  # 현재 oScreen 받아옴

        state = Tetris.accept(self, key)

        currCBlk = CTetris.setOfCBlockObjects[self.idxBlockType][self.idxBlockDegree]
        tempBlk = self.iCScreen.clip(self.top, self.left, self.top+currCBlk.get_dy(), self.left+currCBlk.get_dx())
        # 설정된 값으로 블럭 모양 읽어옴

        tempBlk = tempBlk + currCBlk

        self.oCScreen = Matrix(self.iCScreen)
        self.oCScreen.paste(tempBlk, self.top, self.left)  # 아니면 출력

        return state

    def deleteFullLines(self):
        self.top = self.iScreenDy - 1
        self.left = self.iScreenDw
        blackBlk = Matrix([[0 for _ in range(self.iScreenDx)]])
        tempBlk = self.oCScreen.clip(self.top, self.left, self.top + 1, self.left + self.iScreenDx)  # 밑에서부터 끌어와서 저장

        while self.top > 0:  # 값 비교
            if(tempBlk.binary().sum() == self.iScreenDx):
                tempBlk = self.oCScreen.clip(0, self.left, self.top, self.left + self.iScreenDx)
                self.oCScreen.paste(tempBlk, 1, self.left)
                self.oCScreen.paste(blackBlk, 0, self.left)
            else:
                self.top -= 1
            tempBlk = self.oCScreen.clip(self.top, self.left, self.top + 1, self.left + self.iScreenDx)  # 한줄도 통과면 나머지 줄도 검사

        return
