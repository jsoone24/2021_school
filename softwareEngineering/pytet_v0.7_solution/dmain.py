from tetris import *
from decorator import *
from random import *

import os
import sys
import tty
import termios
import signal

class TextColor():
    red    = "\033[31m"
    green  = "\033[32m"
    yellow = "\033[33m"
    blue   = "\033[34m"
    purple = "\033[35m"
    cyan   = "\033[36m"
    white  = "\033[37m"
    pink   = "\033[95m"
### end of class TextColor():

def clearScreen(numlines=100):
	if os.name == 'posix':
		os.system('clear')
	elif os.name in ['nt', 'dos', 'ce']:
		os.system('CLS')
	else:
		print('\n' * numlines)
	return

def _printScreen(screen):
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

def printScreen(screen):
	clearScreen()
	array = screen.get_array()

	for y in range(screen.get_dy()-Tetris.iScreenDw):
		line = ''
		for x in range(Tetris.iScreenDw, screen.get_dx()-Tetris.iScreenDw):
			if array[y][x] == 0:
				line += TextColor().white+'□'
			elif array[y][x] == 1:
				line += TextColor().red+'■'
			elif array[y][x] == 2:
				line += TextColor().green+'■'
			elif array[y][x] == 3:
				line += TextColor().yellow+'■'
			elif array[y][x] == 4:
				line += TextColor().blue+'■'
			elif array[y][x] == 5:
				line += TextColor().purple +'■'
			elif array[y][x] == 6:
				line += TextColor().cyan+'■'
			elif array[y][x] == 7:
				line += TextColor().pink+'■'
			else:
				line += 'XX'
		print(line)

	print()
	return

def unregisterAlarm():
	signal.alarm(0)
	return

def registerAlarm(handler, seconds):
	unregisterAlarm()
	signal.signal(signal.SIGALRM, handler)
	signal.alarm(seconds)
	return

def timeout_handler(signum, frame): 
	#print("timeout!")
	raise RuntimeError ### we have to raise error to wake up any blocking function
	return

def getChar():
	fd = sys.stdin.fileno()
	old_settings = termios.tcgetattr(fd)
	try:
		tty.setraw(sys.stdin.fileno())
		ch = sys.stdin.read(1)
		unregisterAlarm()
	finally:
		termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
	return ch
 
def readKey():
	c1 = getChar()
	if ord(c1) != 0x1b: ### ESC character
		return c1
	c2 = getChar()
	if ord(c2) != 0x5b: ### '[' character
		return c1
	c3 = getChar()
	return chr(0x10 + ord(c3) - 65)

def readKeyWithTimeOut():
	registerAlarm(timeout_handler, 1)
	try:
		key = readKey()
		unregisterAlarm()
		return key
	except RuntimeError as e:
		pass # print('readkey() interrupted!')

	return
 
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
	screen = board.getScreen()
	printScreen(screen)
          
	if state != TetrisState.NewBlock:
		return state

	idxBlockType = randint(0, nBlocks-1)
	key = str(idxBlockType)
	state = board.accept(key)
	screen = board.getScreen()
	printScreen(screen)

	if state != TetrisState.Finished:
		return state

	return state

if __name__ == "__main__":

	setOfBlockArrays = initSetOfBlockArrays()

	Tetris.init(setOfBlockArrays)
	board = ColorDecorator(Tetris(20, 15)) ### decorator pattern applied!!

	idxBlockType = randint(0, nBlocks-1)
	key = '0' + str(idxBlockType)
	state = board.accept(key)
	screen = board.getScreen()
	printScreen(screen)

	while True:
		key = readKeyWithTimeOut()
		if not key:
			key = 's'
		#print(repr(key))
        
		if key == 'q':
			state = TetrisState.Finished
			print('Game aborted...')
			break

		state = processKey(board, key)
		if state == TetrisState.Finished:
			print('Game Over!!!')
			break
    
	unregisterAlarm()
	print('Program terminated...')

### end of main.py

