#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv)
{
    int result =0;
    int pid = 0;
    int nice = 0;
   if (argc < 3)
  {
    printf(2, "usage: setnice pid nice ...\n");
    exit();
  }
   pid = atoi(argv[1]);
   nice = atoi(argv[2]);
  result = setnice(pid, nice);

  if(result < 0)
  {
      printf(2, "non-existing process\n");
  }
  else
  {
      printf(2, "set pid:%d nice to %d\n", pid, nice);
  }
  exit();
}
