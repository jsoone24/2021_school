from game import *
from matrix import *

##################################################
### ColorDecorator for Tetris Class
##################################################

class ColorDecorator(Game):
	def initCBlocks(self, setOfBlockObjects):
		game = self.game
		self.setOfCBlockObjects = [[0]* game.nBlockDegrees for _ in range(game.nBlockTypes)]

		for i in range(game.nBlockTypes):
			for j in range(game.nBlockDegrees):
				obj = Matrix(game.setOfBlockObjects[i][j])
				obj.mulc(i+1)
				self.setOfCBlockObjects[i][j] = obj
		return
		
	def __init__(self, game):
		self.game = game
		self.initCBlocks(game.setOfBlockObjects)
		arrayScreen = game.createArrayScreen()
		self.iCScreen = Matrix(arrayScreen)
		self.oCScreen = Matrix(self.iCScreen)
		return
	
	def accept(self, key):
		if key >= '0' and key <= '6':
			if self.game.justStarted == False:
				self.deleteFullLines()
			self.iCScreen = Matrix(self.oCScreen)

		state = self.game.accept(key)
		
		currCBlk = self.setOfCBlockObjects[self.game.idxBlockType][self.game.idxBlockDegree]
		tempBlk = self.iCScreen.clip(self.game.top, self.game.left, 
									self.game.top + currCBlk.get_dy(), 
									self.game.left + currCBlk.get_dx())
		tempBlk = tempBlk + currCBlk

		self.oCScreen = Matrix(self.iCScreen)
		self.oCScreen.paste(tempBlk, self.game.top, self.game.left)
		return state
	
	def getScreen(self):
		return self.oCScreen

	def deleteFullLines(self):
		nDeleted = 0
		nScanned = self.game.currBlk.get_dy()

		if self.game.top + self.game.currBlk.get_dy() - 1 >= self.game.iScreenDy:
			nScanned -= (self.game.top + self.game.currBlk.get_dy() - self.game.iScreenDy)

		zero = Matrix([[ 0 for x in range(0, (self.game.iScreenDx - 2*self.game.iScreenDw))]])
		for y in range(nScanned - 1, -1, -1):
			cy = self.game.top + y + nDeleted
			line = self.game.oScreen.clip(cy, 0, cy+1, self.game.oScreen.get_dx())
			if line.sum() == self.game.oScreen.get_dx():
				### Tetris screen
				temp = self.game.oScreen.clip(0, 0, cy, self.game.oScreen.get_dx())
				self.game.oScreen.paste(temp, 1, 0)
				self.game.oScreen.paste(zero, 0, self.game.iScreenDw)

				### CTetris screen
				temp = self.oCScreen.clip(0, 0, cy, self.oCScreen.get_dx())
				self.oCScreen.paste(temp, 1, 0)
				self.oCScreen.paste(zero, 0, self.game.iScreenDw)

				nDeleted += 1
		return

