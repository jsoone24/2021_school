#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv) {
    int pid;
    int mypid;

    // Change the priority of init process.
    setnice(1, 10);

    // Change the priority of current processes.
    setnice(getpid(), 2);

    // Create a child process
    pid = fork();

    if (pid == 0) {
        printf(1, "#### State 2 ####\n");
    } else {
        printf(1, "#### State 1 ####\n");

        // Change the priority of parent process.
        setnice(pid, 10);
        wait(); //Yield CPU

        printf(1, "#### State 3 ####\n");
    }

    mypid = getpid();
    printf(1, "PID %d is finished\n", mypid);

    exit();
}

