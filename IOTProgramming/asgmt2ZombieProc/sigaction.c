#include <stdio.h>
#include <signal.h>
void timeout(int sig)
{
    printf("Time out\n");
    alarm(2);
}

int main()
{
    int i;
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);
    alarm(2);
    for (i = 0; i < 3; i++)
    {
        printf("wait...\n");
        sleep(100);
    }
    return 0;
}
