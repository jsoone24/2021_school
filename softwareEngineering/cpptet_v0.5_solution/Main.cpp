#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <stdio.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "Tetris.h"
#include "CTetris.h"

using namespace std;

/**************************************************************/
/**************** Linux System Functions **********************/
/**************************************************************/

char saved_key = 0;
int tty_cbreak(int fd); /* put terminal into cbreak mode */
int tty_reset(int fd);  /* restore terminal's mode */

/* Read 1 character - echo defines echo mode */
char getch()
{
    char ch;
    int n;
    while (1)
    {
        tty_cbreak(0);
        n = read(0, &ch, 1);
        tty_reset(0);
        if (n > 0)
            break;
        else if (n < 0)
        {
            if (errno == EINTR)
            {
                if (saved_key != 0)
                {
                    ch = saved_key;
                    saved_key = 0;
                    break;
                }
            }
        }
    }
    return ch;
}

void sigint_handler(int signo)
{
    // cout << "SIGINT received!" << endl;
    // do nothing;
}

void sigalrm_handler(int signo)
{
    alarm(1);
    saved_key = 's';
}

void unregisterAlarm()
{
    alarm(0);
}

void registerAlarm()
{
    struct sigaction act, oact;
    act.sa_handler = sigalrm_handler;
    sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
    act.sa_flags = SA_INTERRUPT;
#else
    act.sa_flags = 0;
#endif
    if (sigaction(SIGALRM, &act, &oact) < 0)
    {
        cerr << "sigaction error" << endl;
        exit(1);
    }
    alarm(1);
}

/**************************************************************/
/**************** Tetris Blocks Definitions *******************/
/**************************************************************/
#define MAX_BLK_TYPES 7
#define MAX_BLK_DEGREES 4

int T0D0[] = {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, -1}; //I
int T0D1[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, -1};
int T0D2[] = {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1};
int T0D3[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1};

int T1D0[] = {1, 0, 0, 1, 1, 1, 0, 0, 0, -1}; //J
int T1D1[] = {0, 1, 1, 0, 1, 0, 0, 1, 0, -1};
int T1D2[] = {0, 0, 0, 1, 1, 1, 0, 0, 1, -1};
int T1D3[] = {0, 1, 0, 0, 1, 0, 1, 1, 0, -1};

int T2D0[] = {0, 0, 1, 1, 1, 1, 0, 0, 0, -1}; //L
int T2D1[] = {0, 1, 0, 0, 1, 0, 0, 1, 1, -1};
int T2D2[] = {0, 0, 0, 1, 1, 1, 1, 0, 0, -1};
int T2D3[] = {1, 1, 0, 0, 1, 0, 0, 1, 0, -1};

int T3D0[] = {1, 1, 1, 1, -1}; //O
int T3D1[] = {1, 1, 1, 1, -1};
int T3D2[] = {1, 1, 1, 1, -1};
int T3D3[] = {1, 1, 1, 1, -1};

int T4D0[] = {0, 1, 1, 1, 1, 0, 0, 0, 0, -1}; //S
int T4D1[] = {0, 1, 0, 0, 1, 1, 0, 0, 1, -1};
int T4D2[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, -1};
int T4D3[] = {1, 0, 0, 1, 1, 0, 0, 1, 0, -1};

int T5D0[] = {0, 1, 0, 1, 1, 1, 0, 0, 0, -1}; //T
int T5D1[] = {0, 1, 0, 0, 1, 1, 0, 1, 0, -1};
int T5D2[] = {0, 0, 0, 1, 1, 1, 0, 1, 0, -1};
int T5D3[] = {0, 1, 0, 1, 1, 0, 0, 1, 0, -1};

int T6D0[] = {1, 1, 0, 0, 1, 1, 0, 0, 0, -1}; //Z
int T6D1[] = {0, 0, 1, 0, 1, 1, 0, 1, 0, -1};
int T6D2[] = {0, 0, 0, 1, 1, 0, 0, 1, 1, -1};
int T6D3[] = {0, 1, 0, 1, 1, 0, 1, 0, 0, -1};

int *setOfCBlockArrays[] = {
  T0D0, T0D1, T0D2, T0D3,
  T1D0, T1D1, T1D2, T1D3,
  T2D0, T2D1, T2D2, T2D3,
  T3D0, T3D1, T3D2, T3D3,
  T4D0, T4D1, T4D2, T4D3,
  T5D0, T5D1, T5D2, T5D3,
  T6D0, T6D1, T6D2, T6D3,
};

string color_red = "\033[31m";
string color_green = "\033[32m";
string color_yellow = "\033[33m";
string color_blue = "\033[34m";
string color_purple = "\033[35m";
string color_cyan = "\033[36m";
string color_normal = "\033[37m";
string color_pink = "\033[95m";
string color_black = "\033[30m";

#if 1
void drawScreen(CTetris *board)
{
    int dy = board->oCScreen->get_dy();
    int dx = board->oCScreen->get_dx();
    int dw = board->iScreenDw;
    int **array = board->oCScreen->get_array();
    system("clear");

    string screen = "";
    for (int y = 0; y < dy - dw + 1; y++)
    {
        string line = "";
        for (int x = dw - 1; x < dx - dw + 1; x++)
        {
            if (array[y][x] == 0)
                line += color_normal + "□" + color_normal;
            else if (array[y][x] == 1) // I
                line += color_red + "■" + color_normal;
            else if (array[y][x] == 2) // J
                line += color_green + "■" + color_normal;
            else if (array[y][x] == 3) // L
                line += color_yellow + "■" + color_normal;
            else if (array[y][x] == 4) // O
                line += color_blue + "■" + color_normal;
            else if (array[y][x] == 5) // S
                line += color_purple + "■" + color_normal;
            else if (array[y][x] == 6) // T
                line += color_cyan + "■" + color_normal;
            else if (array[y][x] == 7) // Z
                line += color_pink + "■" + color_normal;
            else // if (array[y][x] == 8) // wall
                line += color_normal + "■" + color_normal;
        }
        screen += line + "\n";
    }
    cout << screen;
}
#endif

/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/

int main(int argc, char *argv[])
{
    int dy, dx;
    char key = 0;

    if (argc != 3)
    {
        cout << "usage: " << argv[0] << " dy dx" << endl;
        exit(1);
    }
    if ((dy = atoi(argv[1])) <= 0 || (dx = atoi(argv[2])) <= 0)
    {
        cout << "dy and dx should be greater than 0" << endl;
        exit(1);
    }

#if 1
    CTetris::init(setOfCBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
    CTetris *board = new CTetris(dy, dx);
    //Tetris::init(setOfCBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
    //Tetris *board = new Tetris(dy, dx);

    TetrisState state;

    srand((unsigned int)time(NULL));
    key = (char)('0' + rand() % MAX_BLK_TYPES);
#endif

    registerAlarm();
    while (key != 'q')
    {
#if 1
        state = board->accept(key);
        if (state == NewBlock)
        {
            key = (char)('0' + rand() % MAX_BLK_TYPES);
            state = board->accept(key);
            if (state == Finished)
            {
                drawScreen(board);
                cout << endl;
                break;
            }
        }
        drawScreen(board);
        cout << endl;
#endif
        key = getch();
        cout << key << endl;
    }
#if 1
    delete board;
#endif

    cout << "Program terminated!" << endl;
    return 0;
}
