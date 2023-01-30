from matrix import *
import random


def draw_matrix(m):
    array = m.get_array()
    for y in range(m.get_dy()):
        for x in range(m.get_dx()):
            if array[y][x] == 0:
                print("□", end='')
            elif array[y][x] == 1:
                print("■", end='')
            else:
                print("XX", end='')
        print()


###
# initialize variables
###
arrayBlk = [[[0, 0, 1, 0],  # I
             [0, 0, 1, 0],
             [0, 0, 1, 0],
             [0, 0, 1, 0]],

            [[0, 1, 0],  # J
             [0, 1, 0],
             [1, 1, 0]],

            [[0, 1, 0],  # L
             [0, 1, 0],
             [0, 1, 1]],

            [[1, 1],  # O
             [1, 1]],

            [[0, 1, 1], # S
             [1, 1, 0],
             [0, 0, 0]],

            [[1, 1, 1],  # T
             [0, 1, 0],
             [0, 0, 0]],

            [[1, 1, 0],  # Z
             [0, 1, 1],
             [0, 0, 0]],
            ]


# integer variables: must always be integer!
iScreenDy = 15
iScreenDx = 10
iScreenDw = 4
top = 0
left = iScreenDw + iScreenDx//2 - 2

newBlockNeeded = False

arrayScreen = [
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]]

###
# additional functions
###


def rotate_90(src, opt):  # opt == 0 : rotate clockwise, opt == 1 : rotate counter clockwise
    N = len(src._array)
    ret = [[0] * N for _ in range(N)]

    if(opt == 0):
        for row in range(N):
            for column in range(N):
                ret[column][N - 1 - row] = src._array[row][column]

    elif(opt == 1):
        for row in range(N):
            for column in range(N):
                ret[N - 1 - column][row] = src._array[row][column]

    return Matrix(ret)


###
# prepare the initial screen output
###
iScreen = Matrix(arrayScreen)
oScreen = Matrix(iScreen)
currBlk = Matrix(arrayBlk[random.randrange(0, 7)])
tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
tempBlk = tempBlk + currBlk
oScreen.paste(tempBlk, top, left)
draw_matrix(oScreen)
print()

###
# execute the loop
###

keepDown = 0

while True:
    if(not keepDown):
        key = input(
            'Enter a key from [ q (quit), a (left), d (right), s (down), w (rotate), \' \' (drop) ] : ')
    if key == 'q':
        print('Game terminated...')
        break
    elif key == 'a':  # move left
        left -= 1
    elif key == 'd':  # move right
        left += 1
    elif key == 's':  # move down
        top += 1
    elif key == 'w':  # rotate the block clockwise
        currBlk = rotate_90(currBlk, 0)
    elif (key == ' ') or (keepDown == 1):  # drop the block
        if(keepDown == 0):
            keepDown = 1
            top += 1
        else:
            top += 1
    else:
        print('Wrong key!!!')
        continue

    tempBlk = iScreen.clip(
        top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
    tempBlk = tempBlk + currBlk

    if tempBlk.anyGreaterThan(1):
        if key == 'a':  # undo: move right
            left += 1
        elif key == 'd':  # undo: move left
            left -= 1
        elif key == 's':  # undo: move up
            top -= 1
            newBlockNeeded = True
        elif key == 'w':  # undo: rotate the block counter-clockwise
            currBlk = rotate_90(currBlk, 1)
        elif key == ' ':  # undo: move up
            top -= 1
            newBlockNeeded = True
            keepDown = 0

        tempBlk = iScreen.clip(
            top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
        tempBlk = tempBlk + currBlk

    oScreen = Matrix(iScreen)
    oScreen.paste(tempBlk, top, left)
    draw_matrix(oScreen)
    print()

    if newBlockNeeded:
        iScreen = Matrix(oScreen)
        top = 0
        left = iScreenDw + iScreenDx//2 - 2
        newBlockNeeded = False
        currBlk = Matrix(arrayBlk[random.randrange(0, 7)])
        tempBlk = iScreen.clip(
            top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
        tempBlk = tempBlk + currBlk

        if tempBlk.anyGreaterThan(1):
            print('Game Over!!!')
            break

        oScreen = Matrix(iScreen)
        oScreen.paste(tempBlk, top, left)
        draw_matrix(oScreen)
        print()

###
# end of the loop
###
