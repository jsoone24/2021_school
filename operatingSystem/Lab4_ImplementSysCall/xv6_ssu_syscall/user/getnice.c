#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv)
{
    int result = 0;
    int pid = 0;
    
  if (argc < 2)
  {
    printf(2, "usage: getnice pid ...\n");
    exit();
  }
pid = atoi(argv[1]);
  result = getnice(atoi(argv[1])); 

  if(result < 0)
  {
      printf(2, "non-existing process\n");
  }
  else
  {
      printf(2, "pid : %d nice : %d\n", pid, result);
  }
  exit();
}
