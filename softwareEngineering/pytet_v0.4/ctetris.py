from tetris import *
from matrix import *
from enum import Enum


class CTetris(Tetris):

    @classmethod
    def init(cls, setOfBlockArrays):
        Tetris.nBlockTypes = len(setOfBlockArrays)
        Tetris.nBlockDegrees = len(setOfBlockArrays[0])
        Tetris.setOfBlockObjects = [[0] * Tetris.nBlockDegrees for _ in range(Tetris.nBlockTypes)]
        arrayBlk_maxSize = 0
        for i in range(Tetris.nBlockTypes):
            if arrayBlk_maxSize <= len(setOfBlockArrays[i][0]):
                arrayBlk_maxSize = len(setOfBlockArrays[i][0])
        Tetris.iScreenDw = arrayBlk_maxSize     # larget enough to cover the largest block

        for i in range(Tetris.nBlockTypes):
            for j in range(Tetris.nBlockDegrees):
                temp_array = [[i + 1 if y > 0 else 0 for y in x] for x in setOfBlockArrays[i][j]]
                Tetris.setOfBlockObjects[i][j] = Matrix(temp_array)
        return

    def accept(self, key):
        self.state = TetrisState.Running

        if key >= '0' and key <= '6':
            if self.justStarted == False:
                self.deleteFullLines()
            self.iScreen = Matrix(self.oScreen)  # 현재 oScreen 받아옴
            self.idxBlockType = int(key)  # 들어온 블럭 모양 저장
            self.idxBlockDegree = 0  # 블럭 각도 설정
            # 설정된 값으로 블럭 모양 읽어옴
            self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
            self.top = 0
            self.left = Tetris.iScreenDw + self.iScreenDx//2 - self.currBlk.get_dx()//2  # 블럭 어디에 놓을지 위치 선정
            self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())

            # binarytempBlk는 겹치는지 감지하기 위해 0과 1로만 계산한 블럭.
            self.binarytempBlk = self.tempBlk.binary() + self.currBlk.binary()
            # temp block 은 현재 화면에서 잘라온 블럭. currblock은 현재 선택된 블럭
            self.tempBlk = self.tempBlk + self.currBlk
            self.justStarted = False
            print()

            if self.binarytempBlk.anyGreaterThan(1):  # 넘었는지 체크
                self.state = TetrisState.Finished
                self.tempBlk = Matrix([[y % self.nBlockTypes for y in x]for x in self.tempBlk.get_array()])

            self.oScreen = Matrix(self.iScreen)
            self.oScreen.paste(self.tempBlk, self.top, self.left)  # 아니면 출력
            return self.state
        elif key == 'q':
            pass
        elif key == 'a':  # move left
            self.left -= 1
        elif key == 'd':  # move right
            self.left += 1
        elif key == 's':  # move down
            self.top += 1
        elif key == 'w':  # rotate the block clockwise
            self.idxBlockDegree = (self.idxBlockDegree + 1) % Tetris.nBlockDegrees
            self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
        elif key == ' ':  # drop the block
            while not self.binarytempBlk.anyGreaterThan(1):
                self.top += 1
                self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())

                self.binarytempBlk = self.tempBlk.binary() + self.currBlk.binary()
                self.tempBlk = self.tempBlk + self.currBlk
        else:
            print('Wrong key!!!')

        self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())

        self.binarytempBlk = self.tempBlk.binary() + self.currBlk.binary()
        self.tempBlk = self.tempBlk + self.currBlk

        if self.binarytempBlk.anyGreaterThan(1):  # 벽 충돌시 undo 수행
            if key == 'a':  # undo: move right
                self.left += 1
            elif key == 'd':  # undo: move left
                self.left -= 1
            elif key == 's':  # undo: move up
                self.top -= 1
                self.state = TetrisState.NewBlock
            elif key == 'w':  # undo: rotate the block counter-clockwise
                self.idxBlockDegree = (self.idxBlockDegree - 1) % Tetris.nBlockDegrees
                self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
            elif key == ' ':  # undo: move up
                self.top -= 1
                self.state = TetrisState.NewBlock

            self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())

            self.binarytempBlk = self.tempBlk.binary() + self.currBlk.binary()
            self.tempBlk = self.tempBlk + self.currBlk

        self.oScreen = Matrix(self.iScreen)
        self.oScreen.paste(self.tempBlk, self.top, self.left)

        return self.state

    def deleteFullLines(self):
        self.top = self.iScreenDy - 1
        self.left = self.iScreenDw
        blackBlk = Matrix([[0 for _ in range(self.iScreenDx)]])
        self.tempBlk = self.oScreen.clip(self.top, self.left, self.top + 1, self.left + self.iScreenDx)  # 밑에서부터 끌어와서 저장

        while self.top > 0:  # 값 비교
            if(self.tempBlk.binary().sum() == self.iScreenDx):
                self.tempBlk = self.oScreen.clip(0, self.left, self.top, self.left + self.iScreenDx)
                self.oScreen.paste(self.tempBlk, 1, self.left)
                self.oScreen.paste(blackBlk, 0, self.left)
            else:
                self.top -= 1
            self.tempBlk = self.oScreen.clip(self.top, self.left, self.top + 1, self.left + self.iScreenDx)  # 한줄도 통과면 나머지 줄도 검사
        
        return
