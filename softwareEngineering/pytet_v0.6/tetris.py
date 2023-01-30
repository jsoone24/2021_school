from matrix import *
from enum import Enum
from abc import *

class ActionHandler(metaclass = ABCMeta):
	@abstractmethod
	def run(self, t, key):            # run(Tetris t, char key)
		pass
### end of class ActionHandler(): 

class TetrisState(Enum):
	Running = 0
	NewBlock = 1
	Finished = 2
### end of class TetrisState(): 

class Tetris():
	nBlockTypes = 0
	nBlockDegrees = 0
	setOfBlockObjects = 0
	iScreenDw = 0   # larget enough to cover the largest block
	opDic = dict()

	@classmethod
	def setOperation(cls, key, currState, op1, op1State, op2, op2State):
		op = [currState, op1, op1State, op2, op2State]
		Tetris.opDic[key] = op
		return

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
				Tetris.setOfBlockObjects[i][j] = Matrix(setOfBlockArrays[i][j])
		return

	def createArrayScreen(self):
		self.arrayScreenDx = Tetris.iScreenDw * 2 + self.iScreenDx
		self.arrayScreenDy = self.iScreenDy + Tetris.iScreenDw
		self.arrayScreen = [[0] * self.arrayScreenDx for _ in range(self.arrayScreenDy)]
		for y in range(self.iScreenDy):
			for x in range(Tetris.iScreenDw):
				self.arrayScreen[y][x] = 1
			for x in range(self.iScreenDx):
				self.arrayScreen[y][Tetris.iScreenDw + x] = 0
			for x in range(Tetris.iScreenDw):
				self.arrayScreen[y][Tetris.iScreenDw + self.iScreenDx + x] = 1

		for y in range(Tetris.iScreenDw):
			for x in range(self.arrayScreenDx):
				self.arrayScreen[self.iScreenDy + y][x] = 1

		return self.arrayScreen

	def __init__(self, iScreenDy, iScreenDx):
		self.iScreenDy = iScreenDy
		self.iScreenDx = iScreenDx
		arrayScreen = self.createArrayScreen()
		self.iScreen = Matrix(arrayScreen)
		self.oScreen = Matrix(self.iScreen)
		self.justStarted = True
		self.idxBlockType = -1
		self.idxBlockDegree = -1
		self.top = -1
		self.left = -1
		return

	def anyConflictWhileUpdate(self):
		doesConflict = False

		tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
		tempBlk = tempBlk + self.currBlk
		if tempBlk.anyGreaterThan(1):
			doesConflict = True

		self.oScreen = Matrix(self.iScreen)
		self.oScreen.paste(tempBlk, self.top, self.left)

		return doesConflict

	def accept(self, key):
		state = TetrisState.Running
		if key in Tetris.opDic:
			op = Tetris.opDic[key]
			state = op[2] ### post state after do Handler
			if op[1].run(self, key): ### do Handler
				op[3].run(self, key) ### undo Handler
				state = op[4] ### post state after undo Handler
		else:
			print('Wrong key(=%s)!!!' % key)

		return state

### end of class Tetris():
