struct sigaction act;
act.sa_handler = zombie_handler;
sigemptyset(&act.sa_mask);
act.sa_flags = 0;
sigaction(SIGCHLD, &act, 0);

void zombie_handler()
{
    int status;
    int spid;
    spid = wait(&status);
    printf("PID: \t%d\n", spid);
    printf("Exit value: \t%d\n", WEXITSTATUS(status));
    printf("Exit state: \t%d\n", WIFEXITED(status));
}
