from ctetris import *
from random import *
from enum import Enum
from collections import OrderedDict

import os
import sys
import tty
import termios
import signal

def clearScreen(numlines=100):
	if os.name == 'posix':
		os.system('clear')
	elif os.name in ['nt', 'dos', 'ce']:
		os.system('CLS')
	else:
		print('\n' * numlines)
	return

def printScreen(board):
    TextColor = [
            ["white", "\033[37m"],
            ["red", "\033[31m"],
            ["green", "\033[32m"],
            ["yellow", "\033[33m"],
            ["blue", "\033[34m"],
            ["purple", "\033[35m"],
            ["cyan", "\033[36m"],
            ["pink", "\033[95m"]]
             
    clearScreen()
    # board는 CTetris 객체, oScreen은 Matrix 객체 get_array()는 2차원 행렬 리턴
    array = board.oCScreen.get_array()

    # get_dy()는 세로 길이 리턴, iScreenDW는 테트리스 너비
    for y in range(board.oCScreen.get_dy() - Tetris.iScreenDw):
        line = ''
        for x in range(Tetris.iScreenDw, board.oCScreen.get_dx() - Tetris.iScreenDw):
            if array[y][x] == 0:
                line += TextColor[0][1] + '□'
            elif array[y][x] >= 1:
                line += TextColor[array[y][x]][1] + '■'
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
    registerAlarm(timeout_handler, 1)   # 1초뒤 인터럽트 걸리게 설정
    try:
        key = readKey() # 일단 키를 읽고 잘 리턴되면 키값 리턴
        unregisterAlarm()
        return key
    except RuntimeError as e:   # 1초뒤에 이런 안되면 여기로 빠짐
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
    printScreen(board)
          
    if state != TetrisState.NewBlock:
        return state

    key = getKey(False)
    state = board.accept(key)
    printScreen(board)

    if state != TetrisState.Finished:
        return state

    return state

def getKey(is_keystroke_needed):
    global is_log_mode
    global is_replay_mode

    if is_replay_mode:
        key = get_key_from_log()
    elif is_keystroke_needed:
        key = readKeyWithTimeOut()
        if not key:
            key = 's'
    else:
        idxBlockType = randint(0,nBlocks-1)
        key = '0' + str(idxBlockType)

    if is_log_mode:
        log_key(key)

    return key

def get_key_from_log():
    global keys
    global key_idx

    key = keys[key_idx]
    key_idx+=1

    return key

def log_start():
    fp = open('keylog.py', 'w')
    fp.write('keys = [\n')
    fp.close()
    return

def log_end():
    fp  = open('keylog.py','a')
    fp.write(']\n')
    fp.close
    return

def log_key(key):
    fp = open('keylog.py', 'a')
    fp.write('\'%c\',\n'%key[-1])
    fp.close()
    return

if __name__ == "__main__":
    global is_log_mode
    global is_replay_mode
    global keys
    global key_idx

    is_log_mode = False
    if len(sys.argv) == 2 and sys.argv[1] == 'log':
        is_log_mode == True
        log_start()

    is_replay_mode = False
    if len(sys.argv) == 2 and sys.argv[1] == 'replay':
        is_replay_mode = True
        key_idx = 0
        from keylog import *
        print(keys)
        input('Press any key to continue:')


    setOfBlockArrays = initSetOfBlockArrays()

    CTetris.init(setOfBlockArrays)
    board = CTetris(20, 15)
    key = getKey(False)
    state = board.accept(key)
    printScreen(board)

    while True:
        key = getKey(True)
        
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
    if is_log_mode:
        log_end()

### end of main.py