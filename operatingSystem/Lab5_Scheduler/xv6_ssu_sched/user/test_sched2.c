#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv)
{
        int pid;
        int mypid;

        // Change the priority of init process.
        setnice(1, 10);

        // Change the priority of current processes.
        // setnice(getpid(), 2);
        setnice(getpid(), 9);

        // Crete a child process
        pid = fork();

        if (pid == 0)
        {
                // Child
                printf(1, "#### State 2 ####\n");
        }
        else
        {
                // Parent
                printf(1, "#### State 1 ####\n");

                // Change the priority of parent process
                setnice(pid, 10);
                wait();
                printf(1, "#### State 3 ####\n");
        }

        mypid = getpid();
        printf(1, "PID %d is finished\n", mypid);

        exit();
}
