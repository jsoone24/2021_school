#include <iostream>
#include <cstdlib>
#include <ctime>
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
 
#include "CTetris.h"

using namespace std;


/**************************************************************/
/**************** Linux System Functions **********************/
/**************************************************************/

char saved_key = 0;
int tty_cbreak(int fd);	/* put terminal into cbreak mode */
int tty_reset(int fd);	/* restore terminal's mode */
  
/* Read 1 character - echo defines echo mode */
char getch() {
  char ch;
  int n;
  while (1) {
    tty_cbreak(0);
    n = read(0, &ch, 1);
    tty_reset(0);
    if (n > 0)
      break;
    else if (n < 0) {
      if (errno == EINTR) {
	if (saved_key != 0) {
	  ch = saved_key;
	  saved_key = 0;
	  break;
	}
      }
    }
  }
  return ch;
}

void sigint_handler(int signo) {
  // cout << "SIGINT received!" << endl;
  // do nothing;
}

void sigalrm_handler(int signo) {
  alarm(1);
  saved_key = 's';
}

void unregisterAlarm() {
	alarm(0);
}

void registerAlarm() {
  struct sigaction act, oact;
  act.sa_handler = sigalrm_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGALRM, &act, &oact) < 0) {
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

int T0D0[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1}; // I shape
int T0D1[] = {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1};
int T0D2[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1};
int T0D3[] = {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1};

int T1D0[] = {1, 0, 0, 1, 1, 1, 0, 0, 0, -1}; // J shape
int T1D1[] = {0, 1, 1, 0, 1, 0, 0, 1, 0, -1};
int T1D2[] = {0, 0, 0, 1, 1, 1, 0, 0, 1, -1};
int T1D3[] = {0, 1, 0, 0, 1, 0, 1, 1, 0, -1};

int T2D0[] = {0, 0, 1, 1, 1, 1, 0, 0, 0, -1}; // L shape
int T2D1[] = {0, 1, 0, 0, 1, 0, 0, 1, 1, -1};
int T2D2[] = {0, 0, 0, 1, 1, 1, 1, 0, 0, -1};
int T2D3[] = {1, 1, 0, 0, 1, 0, 0, 1, 0, -1};

int T3D0[] = {1, 1, 1, 1, -1}; // O shape
int T3D1[] = {1, 1, 1, 1, -1};
int T3D2[] = {1, 1, 1, 1, -1};
int T3D3[] = {1, 1, 1, 1, -1};

int T4D0[] = {0, 1, 0, 0, 1, 1, 0, 0, 1, -1}; // S shape
int T4D1[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, -1};
int T4D2[] = {0, 1, 0, 0, 1, 1, 0, 0, 1, -1};
int T4D3[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, -1};

int T5D0[] = {0, 1, 0, 1, 1, 1, 0, 0, 0, -1}; // T shape
int T5D1[] = {0, 1, 0, 0, 1, 1, 0, 1, 0, -1};
int T5D2[] = {0, 0, 0, 1, 1, 1, 0, 1, 0, -1};
int T5D3[] = {0, 1, 0, 1, 1, 0, 0, 1, 0, -1};

int T6D0[] = {0, 1, 0, 1, 1, 0, 1, 0, 0, -1}; //Z shape
int T6D1[] = {1, 1, 0, 0, 1, 1, 0, 0, 0, -1};
int T6D2[] = {0, 1, 0, 1, 1, 0, 1, 0, 0, -1};
int T6D3[] = {1, 1, 0, 0, 1, 1, 0, 0, 0, -1};

int *setOfCBlockArrays[] = {
    T0D0, T0D1, T0D2, T0D3, // I
    T1D0, T1D1, T1D2, T1D3, // J
    T2D0, T2D1, T2D2, T2D3, // L
    T3D0, T3D1, T3D2, T3D3, // O
    T4D0, T4D1, T4D2, T4D3, // S
    T5D0, T5D1, T5D2, T5D3, // T
    T6D0, T6D1, T6D2, T6D3, // Z
};

#if 1
void drawScreen(CTetris *board)
{
    int dy = board->oScreen.get_dy();
    int dx = board->oScreen.get_dx();
    int dw = board->iScreenDw;
    int** array = board->oCScreen.get_array();
    system("clear");

    for (int y = 0; y < dy - dw + 1; y++) {
        for (int x = dw - 1; x < dx - dw + 1; x++) {
            if (array[y][x] == 0)
                cout << color_black << "□ " << color_normal;
            else if (array[y][x] == 2)
                cout << color_red << "■ " << color_normal;
            else if (array[y][x] == 3)
                cout << color_green << "■ " << color_normal;
            else if (array[y][x] == 4)
                cout << color_yellow << "■ " << color_normal;
            else if (array[y][x] == 5)
                cout << color_blue << "■ " << color_normal;
            else if (array[y][x] == 6)
                cout << color_purple << "■ " << color_normal;
            else if (array[y][x] == 7)
                cout << color_cyan << "■ " << color_normal;
            else if (array[y][x] == 8)
                cout << color_pink << "■ " << color_normal;
            else // array[y][x] == 1 // wall
                cout << b_color_black << "■ " << color_normal;
        }
        cout << endl;
    }
}
#endif

/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/

int main(int argc, char *argv[]) {
  int dy, dx;
  char key = 0;

  if (argc != 3) {
    cout << "usage: " << argv[0] << " dy dx" << endl;
    exit(1);
  }
  if ((dy = atoi(argv[1])) <= 0 || (dx = atoi(argv[2])) <= 0) {
    cout << "dy and dx should be greater than 0" << endl;
    exit(1);
  }

#if 1
  CTetris::init(setOfCBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
  CTetris *board = new CTetris(dy, dx);
  TetrisState state;

  srand((unsigned int)time(NULL));
  key = (char)('0' + rand() % MAX_BLK_TYPES);
#endif

  registerAlarm();
  while (key != 'q') {
#if 1
    state = board->accept(key);
    if (state == NewBlock) {
      key = (char)('0' + rand() % MAX_BLK_TYPES);
      state = board->accept(key);
      if (state == Finished) {
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

