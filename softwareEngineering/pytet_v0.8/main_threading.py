from tetris import *
from random import *

import os
import sys
import tty
import termios
import signal

import threading
import time

##############################################################
### Data model related code
##############################################################
def rotate(m_array):
    size = len(m_array)
    r_array = [[0] * size for _ in range(size)]

    for y in range(size):
        for x in range(size):
            r_array[x][size-1-y] = m_array[y][x]

    return r_array

def initSetOfBlockArrays():
    global nBlocks

    arrayBlks = [ [ [ 0, 0, 1, 0 ],     # I shape
                    [ 0, 0, 1, 0 ],     
                    [ 0, 0, 1, 0 ],     
                    [ 0, 0, 1, 0 ] ],   
                  [ [1, 0, 0],          # J shape
                    [1, 1, 1],          
                    [0, 0, 0] ],
                  [ [0, 0, 1],          # L shape
                    [1, 1, 1],          
                    [0, 0, 0] ],        
                  [ [1, 1],             # O shape
                    [1, 1] ],           
                  [ [0, 1, 1],          # S shape
                    [1, 1, 0],          
                    [0, 0, 0] ],
                  [ [0, 1, 0],          # T shape    
                    [1, 1, 1],          
                    [0, 0, 0] ],
                  [ [1, 1, 0],          # Z shape
                    [0, 1, 1],          
                    [0, 0, 0] ]         
                ]

    nBlocks = len(arrayBlks)
    setOfBlockArrays = [[0] * 4 for _ in range(nBlocks)]

    for idxBlockType in range(nBlocks):
        temp_array = arrayBlks[idxBlockType]
        setOfBlockArrays[idxBlockType][0] = temp_array
        for idxBlockDegree in range(1,4):
            temp_array = rotate(temp_array)
            setOfBlockArrays[idxBlockType][idxBlockDegree] = temp_array

    return setOfBlockArrays
    
def processKey(board, key):
	global nBlocks 

	state = board.accept(key)
	printScreen(board.getScreen())
          
	if state != TetrisState.NewBlock:
		return state

	idxBlockType = randint(0, nBlocks-1)
	key = str(idxBlockType)
	state = board.accept(key)
	printScreen(board.getScreen())

	if state != TetrisState.Finished:
		return state

	return state

##############################################################
### UI code
##############################################################

def clearScreen(numlines=100):
	if os.name == 'posix':
		os.system('clear')
	elif os.name in ['nt', 'dos', 'ce']:
		os.system('CLS')
	else:
		print('\n' * numlines)
	return

def printScreen(screen):
	clearScreen()
	array = screen.get_array()

	for y in range(screen.get_dy()-Tetris.iScreenDw):
		line = ''
		for x in range(Tetris.iScreenDw, screen.get_dx()-Tetris.iScreenDw):
			if array[y][x] == 0:
				line += '□'
			elif array[y][x] == 1:
				line += '■'
			else:
				line += 'XX'
		print(line)

	print()
	return

def arrayToString(array):
	line = ''
	for x in array:
		if x == 0:
			line += '□'
		elif x == 1:
			line += '■'
		else:
			line += 'XX'

	return line

def printDualScreen(screen1, screen2):
	clearScreen()
	array1 = screen1.get_array()
	array2 = screen2.get_array()

	for y in range(screen1.get_dy()-Tetris.iScreenDw):
		line = ''
		line += arrayToString(array1[y][Tetris.iScreenDw:-Tetris.iScreenDw])
		line += '       '
		line += arrayToString(array2[y][Tetris.iScreenDw:-Tetris.iScreenDw])
		print(line)

	print()
	return

def getChar():
	ch = sys.stdin.read(1)
	return ch


##############################################################
### Threading code (Observer pattern)
##############################################################

from abc import *

class Publisher(metaclass = ABCMeta):
	@abstractmethod
	def addObserver(self, observer):
		pass

	@abstractmethod
	def notifyObservers(self, key):
		pass

isGameDone = False

class KeyProducer(threading.Thread, Publisher):
	def __init__(self, *args, **kwargs):
		super(KeyProducer, self).__init__(*args, **kwargs)
		self.observers = list()
		return

	def addObserver(self, observer):
		self.observers.append(observer)
		return
	
	def notifyObservers(self, key):
		for observer in self.observers:
			observer.update(key)
		return

	def run(self):
		global isGameDone

		while not isGameDone:
			try:
				key = getChar()
			except:
				isGameDone = True
				print('getChar() wakes up!!')
				break

			self.notifyObservers(key)

			if key == 'q':
				isGameDone = True
				break
		return

class TimeOutProducer(threading.Thread, Publisher):
	def __init__(self, *args, **kwargs):
		super(TimeOutProducer, self).__init__(*args, **kwargs)
		self.observers = list()
		return

	def addObserver(self, observer):
		self.observers.append(observer)
		return
	
	def notifyObservers(self, key):
		for observer in self.observers:
			observer.update(key)
		return

	def run(self):
		while not isGameDone:
			time.sleep(1)
			self.notifyObservers('s')
		return

class Observer(metaclass = ABCMeta):
	@abstractmethod
	def update(self, key):
		pass

class Consumer(threading.Thread, Observer):
	def __init__(self, *args, **kwargs):
		super(Consumer, self).__init__(*args, **kwargs)
		self.queue = list()
		self.cv = threading.Condition()
		return

	def update(self, key):
		self.cv.acquire()
		self.queue.append(key)
		self.cv.notify()
		self.cv.release()
		return
	
	def read(self):
		self.cv.acquire()
		while len(self.queue) < 1:
			self.cv.wait()
		key = self.queue.pop(0)
		self.cv.release()
		return key
	
	def addKeypad(self, keypad):
		self.keypad = keypad
		return

	def run(self):
		global isGameDone

		setOfBlockArrays = initSetOfBlockArrays()

		Tetris.init(setOfBlockArrays)
		board = Tetris(20, 15)

		idxBlockType = randint(0, nBlocks-1)
		key = str(idxBlockType)
		state = board.accept(key)
		printScreen(board.getScreen())

		while not isGameDone:
			key = self.read()

			if key == 'q':
				state = TetrisState.Finished
				print('Game aborted...')
				break

			if key not in self.keypad:
				continue

			key = self.keypad[key]

			state = processKey(board, key)
			if state == TetrisState.Finished:
				isGameDone = True
				print('Game Over!!!')
				os.kill(os.getpid(), signal.SIGINT)
				break
		return

def signal_handler(num, stack):
	print('signal_handler called!!')
	raise RuntimeError

if __name__ == "__main__":
	global fd
	global old_settings

	signal.signal(signal.SIGINT, signal_handler)

	keypad1 = { 'a': 'a', 'd': 'd', 's': 's', 'w': 'w', ' ': ' ' }
	th_con1 = Consumer()
	th_con1.addKeypad(keypad1)

	th_pro1 = KeyProducer()
	th_pro1.addObserver(th_con1)

	th_pro2 = TimeOutProducer()
	th_pro2.addObserver(th_con1)

	threads = list()
	threads.append(th_con1)
	threads.append(th_pro1)
	threads.append(th_pro2)

	fd = sys.stdin.fileno()
	old_settings = termios.tcgetattr(fd)
	tty.setcbreak(sys.stdin.fileno())

	for th in threads:
		th.start()
	
	for th in threads:
		try:
			th.join()
		except:
			print('th.join() wakes up!!')
	
	termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
	print('Program terminated...')

### end of main.py

