#include <stdio.h>

//fork library
#include <sys/types.h>
#include <unistd.h>

//wait library
#include <sys/types.h>
#include <sys/wait.h>

//execve library
#include <unistd.h>

int main()
{
    char *argv[2];
    argv[0] = "ls";
    argv[1] = 0;

    int wstatus = 0;
    pid_t pid = fork();

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
        execve("/bin/ls", argv, NULL);
    }

    return 0;
}