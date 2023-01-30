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
	int ss, cs;							 // serv_sock, client_sock, client_addr_size
	struct sockaddr_in sa, ca;			 // serv_addr, client_addr
	fd_set bs, bst;						 // bit_set, bit_set_temp
	struct timeval t;					 // timeout
	char rbuf[BUFSIZ], wbuf[BUFSIZ], ct; // msg_buffer, char
	int mbs, r, strl;					 // max_bit_set, result, string_len
	unsigned int true = 1, cas, cli;

	ss = socket(PF_INET, SOCK_STREAM, 0);
	memset(&sa, 0, sizeof(sa));
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(true));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(atoi(argv[1]));
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ss, (struct sockaddr *)&sa, sizeof(sa)) == -1)
	{
		printf("connection fail\n");
		return 0;
	}
	if (listen(ss, 5) == -1)
	{
		printf("listen error\n");
		return 0;
	}

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
		for (int i = 0; i <= mbs; i++)
		{
			if (FD_ISSET(i, &bst))
			{
				if (i == ss)
				{
					cas = sizeof(ca);
					cs = accept(ss, (struct sockaddr *)&ca, &cas);
					if (!cs)
					{
						printf("accept error\n");
						return 0;
					}
					printf("connected client : %d\n", cs);
					FD_SET(cs, &bs);
					if (mbs < cs)
						mbs = cs;
				}
				else if (i == 0)
				{
					memset(&wbuf, 0, sizeof(wbuf));
					strl = 0;
					while ((wbuf[strl++] = getchar()) != '\n')
						;
					cli = atoi(&wbuf[0]);
					send(cli, wbuf, sizeof(wbuf), 0);
					printf("to %d, msg : %s", cli, wbuf);
				}
				else
				{
					memset(&rbuf, 0, sizeof(rbuf));
					strl = recv(i, rbuf, sizeof(rbuf), MSG_WAITALL);
					printf("from %d, size : %d,  msg : %s", i, strl, rbuf);

					if (strncmp("exit", rbuf, 4) == 0)
					{
						printf("[server] end with %d\n", i);
						memcpy(wbuf, "bye.\n", sizeof("bye.\n"));
						send(i, wbuf, sizeof(wbuf), 0);
						close(i);
						FD_CLR(i, &bs);
					}
				}
			}
		}
	}
}