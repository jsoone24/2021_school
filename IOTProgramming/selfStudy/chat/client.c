#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	int ss;								 // serv_sock
	struct sockaddr_in sa;				 // serv_addr
	fd_set bs, bst;						 // bit_set, bit_set_temp
	struct timeval t;					 // timeout
	char rbuf[BUFSIZ], wbuf[BUFSIZ], ct; // msg_buffer
	int mbs, r, strl;					 // max_bit_set, result, string_len
	const int true = 1;
	int i;

	ss = socket(PF_INET, SOCK_STREAM, 0);
	memset(&sa, 0, sizeof(sa));
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(true));
	sa.sin_family = PF_INET;
	inet_aton(argv[1], &sa.sin_addr);
	sa.sin_port = htons(atoi(argv[2]));

	connect(ss, (struct sockaddr *)&sa, sizeof(sa));

	mbs = ss;
	FD_ZERO(&bs);
	FD_SET(0, &bs);
	FD_SET(ss, &bs);

	while (1)
	{
		bst = bs;
		t.tv_sec = 5;
		t.tv_usec = 0;

		r = select(mbs + 1, &bst, NULL, NULL, &t);
		if (r == -1)
		{
			perror("select error\n");
			break;
		}
		for (i = 0; i <= mbs; i++)
		{
			if (FD_ISSET(i, &bst))
			{
				if (i == ss)
				{
					memset(&rbuf, 0, sizeof(rbuf));
					printf("1st size : %d\n", read(ss, rbuf, sizeof(rbuf)));
					printf("msg : %s\n" , rbuf);
					printf("2nd size : %d\n", read(ss, rbuf, sizeof(rbuf)));
					//recv(ss, rbuf, sizeof(rbuf), MSG_WAITALL);
					//printf("msg : %s", rbuf);
				}
				else if (i == 0)
				{
					memset(&wbuf, 0, sizeof(wbuf));
					strl = 0;
					while ((wbuf[strl++] = getchar()) != '\n')
						;
					wbuf[500] = EOF;
					send(ss, wbuf, sizeof(wbuf), 0);
					printf("send : %s", wbuf);

					if (strncmp("exit", wbuf, 4) == 0)
					{
						printf("[client] closing...\n");
						shutdown(ss, SHUT_WR);

						memset(&rbuf, 0, sizeof(rbuf));
						recv(ss, rbuf, sizeof(rbuf), MSG_WAITALL);
						printf("[sever] %s", rbuf);

						close(ss);
						return 0;
					}
				}
				else
					printf("nothing\n");
			}
		}
	}
}