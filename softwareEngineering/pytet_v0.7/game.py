from abc import *

class Game(metaclass = ABCMeta):
	@abstractmethod
	def accept(self, key):
		pass

	def getScreen(self):
		pass

### end of class MatrixGame(): 

