#include <stdio.h>

//fork library
#include <sys/types.h>
#include <unistd.h>

//wait library
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    int wstatus = 0;

    if (pid > 0)
    {
        printf("parent: child = %d\n", pid);
        pid = wait(&wstatus);
        if(wstatus >= 0)
            printf("child %d is done\n", pid);
        else
            printf("child %d returned error\n", pid);
    }
    else if (pid == 0)
    {
        printf("child: existing\n");
    }
    else
    {
        printf("fork error\n");
    }
    return 0;
}