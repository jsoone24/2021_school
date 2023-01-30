from tetris import *
from random import *

import os
import sys
import tty
import termios

import threading
import time
import curses

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
    
def processKey(window, board, key):
	global nBlocks 

	state = board.accept(key)
	printWindow(window, board.getScreen())
          
	if state != TetrisState.NewBlock:
		return state

	idxBlockType = randint(0, nBlocks-1)
	key = str(idxBlockType)
	state = board.accept(key)
	printWindow(window, board.getScreen())

	if state != TetrisState.Finished:
		return state

	return state

##############################################################
### UI code
##############################################################
def printMsg(msg):
	window = win0

	window.clear()
	window.addstr(0, 0, msg)

	lock.acquire()
	window.refresh()
	lock.release()
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

def printWindow(window, screen):
	array = screen.get_array()
	window.clear()

	for y in range(screen.get_dy()-Tetris.iScreenDw):
		line = arrayToString(array[y][Tetris.iScreenDw:-Tetris.iScreenDw])
		window.addstr(y, 0, line)

	lock.acquire()
	window.refresh()
	lock.release()
	return


def getChar():
	ch = sys.stdin.read(1)
	return ch


##############################################################
### Threading code (Observer pattern)
##############################################################
from abc import *

isGameDone = False

class Publisher(metaclass = ABCMeta):
	@abstractmethod
	def addObserver(self, observer):
		pass

	@abstractmethod
	def notifyObservers(self, key):
		pass

class KeyController(threading.Thread, Publisher):
	def __init__(self, *args, **kwargs):
		super(KeyController, self).__init__(*args[1:], **kwargs)
		self.name = args[0]
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
				printMsg('getChar() wakes up!!')
				break

			self.notifyObservers(key)

		printMsg('%s terminated... Press any key to continue' % self.name)
		time.sleep(1)
		self.notifyObservers('')
		return

class TimeController(threading.Thread, Publisher):
	def __init__(self, *args, **kwargs):
		super(TimeController, self).__init__(*args[1:], **kwargs)
		self.name = args[0]
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
			self.notifyObservers('y')

		printMsg('%s terminated... Press any key to continue' % self.name)
		time.sleep(1)
		self.notifyObservers('')
		return

class Observer(metaclass = ABCMeta):
	@abstractmethod
	def update(self, key):
		pass

class ModelView(threading.Thread, Observer):
	def __init__(self, *args, **kwargs):
		super(ModelView, self).__init__(*args[1:], **kwargs)
		self.name = args[0]
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

	def addWindow(self, window):
		self.window = window
		return

	def run(self):
		global isGameDone

		setOfBlockArrays = initSetOfBlockArrays()

		Tetris.init(setOfBlockArrays)
		board = Tetris(20, 15)

		idxBlockType = randint(0, nBlocks-1)
		key = str(idxBlockType)
		state = board.accept(key)
		printWindow(self.window, board.getScreen())

		while not isGameDone:
			key = self.read()
			if key == '':
				break
			if key not in self.keypad:
				continue

			key = self.keypad[key]
			if key == 'q':
				state = TetrisState.Finished
			else:
				state = processKey(self.window, board, key)

			if state == TetrisState.Finished:
				isGameDone = True
				printMsg('%s IS DEAD!!!' % self.name)
				time.sleep(2)
				break

		printMsg('%s terminated... Press any key to continue' % self.name)
		time.sleep(1)
		return

##############################################################
### Main code
##############################################################

def main(args):
	global lock
	global win0

	lock = threading.Lock()

	screen = curses.initscr()
	screen.clear()

	curses.echo()
	curses.start_color()
	curses.use_default_colors()

	win1 = curses.newwin(20, 30, 0, 0)
	win2 = curses.newwin(20, 30, 0, 40)
	win0 = curses.newwin(3, 70, 21, 0)

	keypad1 = { 'q': 'q', 'w': 'w', 'a': 'a', 's': 'y', 'd': 'd', ' ': ' ', 'y': 'y' }
	th_model1 = ModelView('model1')
	th_model1.addKeypad(keypad1)
	th_model1.addWindow(win1)

	keypad2 = { 'u': 'q', 'i': 'w', 'j': 'a', 'k': 'y', 'l': 'd', '\r': ' ', 'y': 'y' }
	th_model2 = ModelView('model2')
	th_model2.addKeypad(keypad2)
	th_model2.addWindow(win2)

	th_cont1 = KeyController('kcont')
	th_cont1.addObserver(th_model1)
	th_cont1.addObserver(th_model2)

	th_cont2 = TimeController('tcont')
	th_cont2.addObserver(th_model1)
	th_cont2.addObserver(th_model2)

	threads = list()
	threads.append(th_model1)
	threads.append(th_model2)
	threads.append(th_cont1)
	threads.append(th_cont2)

	exited = list()

	fd = sys.stdin.fileno()
	old_settings = termios.tcgetattr(fd)
	tty.setcbreak(sys.stdin.fileno())

	for th in threads:
		th.start()
	
	for th in threads:
		th.join()
		exited.append(th.name)
	
	string = ''
	for name in exited:
		string += ':%s' % name
	string += ' terminated!!!'

	printMsg(string)
	time.sleep(2)
	printMsg('Program terminated...')
	time.sleep(1)

curses.wrapper(main)

### end of main.py
