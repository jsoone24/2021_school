#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>

void error_handling(char *message);

int main(int argc, char **argv)
{
    const int TRUE = 1;
    // input pin
    const int i0 = 17;
    // gpio variable
    int state = 0;
    int value = 0;
    // socket variable
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    time_t cur_t;
    struct tm *cur_tm;
    char cur_time[50];
    char buf[100];

    // gpio check
    if (gpioInitialise() < 0)
    {
        printf("[server] init fail\n");
        return 0;
    }
    // argc check
    if (argc != 2)
    {
        printf("[server] Usage : %s <port>\n", argv[0]);
        return 0;
    }

    // gpio setup
    gpioSetMode(i0, PI_INPUT);

    // socket setup, listen, accept
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("[server] socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, TRUE, sizeof(TRUE));
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("[server] bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("[server] listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("[server] accept() error");
    printf("[server] connection from : %s\n", inet_ntoa(clnt_addr.sin_addr));

    // loop start
    while (1)
    {
        // data read
        if (gpioRead(i0))
        {
            // make msg
            memset(buf, 0, sizeof(buf));
            cur_t = time(NULL);
            cur_tm = localtime(&cur_t);
            strftime(cur_time, sizeof(cur_time), "(%Y/%m/%d %Hh %M)", cur_tm);
            sprintf(buf, "���۰��� %s", cur_time);
            printf("[server] %s\n", buf);

            // send msg
            if (write(clnt_sock, buf, sizeof(buf)) == -1)
                printf("[server] write error. errno : %d, strerror : %s\n", errno, strerror(errno));
            // wait for 5seconds
            gpioDelay(5000000);
        }
    }
    gpioTerminate();
    close(clnt_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}