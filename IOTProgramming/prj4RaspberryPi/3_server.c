#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char * message);

int main(int argc, char **argv)
{
    const int TRUE = 1;
	// output pin
	const int d0 = 12;
	const int d1 = 18;
	const int d2 = 23;
	const int d3 = 25;
	// gpio count variable
	signed char num = 0;
	char temp = 0;
    // socket variable
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char buf[10];

	// gpio check
	if(gpioInitialise() <0){
		printf("[server] init fail\n");
		return 0;
	}
    // argc check
    if(argc!=2){
		printf("[server] Usage : %s <port>\n", argv[0]);
        gpioTerminate();
		exit(1);
	}

    // gpio setup
	gpioSetMode(d0, PI_OUTPUT);
	gpioSetMode(d1, PI_OUTPUT);
	gpioSetMode(d2, PI_OUTPUT);
	gpioSetMode(d3, PI_OUTPUT);
	gpioWrite(d0, 0);
	gpioWrite(d1, 0);
	gpioWrite(d2, 0);
	gpioWrite(d3, 0);

    // socket setup and listen
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("[server] socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, TRUE, sizeof(TRUE));
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
		error_handling("[server] bind() error"); 
	if(listen(serv_sock, 5)==-1)
		error_handling("[server] listen() error");

	// loop start
	while(1)
	{
        clnt_addr_size=sizeof(clnt_addr);  
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock==-1)
            error_handling("[server] accept() error");

		// read command from client
        read(clnt_sock, buf, sizeof(buf));
		printf("[client] command : %s, %c, %d\n", buf, buf[0], atoi(&buf[2]));
		if (buf[0] == 'u')
			num += atoi(&buf[2]);
		else if(buf[0] == 'd')
			num -= atoi(&buf[2]);
		else continue;

		if(num < 0)
			temp = 0;
		else
			temp = num % 16;
		// write led status
		printf("[server] num : %d, temp : %d, d0 : %d, d1 : %d, d2 : %d, d3 : %d\n", num, temp, temp & 1, (temp >> 1) & 1, (temp >> 2) & 1, (temp >> 3) & 1);

		gpioWrite(d0, temp & 0x01);
		gpioWrite(d1, (temp >> 1) & 0x01);
		gpioWrite(d2, (temp >> 2) & 0x01);
		gpioWrite(d3, (temp >> 3) & 0x01);

        close(clnt_sock);
	}
	gpioTerminate();
	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
