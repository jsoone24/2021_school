#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define BUF_SIZE 1024
#define SMALL_BUF 2048

void read_childproc(int sig);
void request_handler(void *arg);
void send_data(FILE *fp, char *ct, char *file_name);
char *content_type(char *file);
void send_error(FILE *fp);
void error_handling(char *message);
const int TRUE = 1;

int main(int argc, char *argv[])
{
	// set serv_socket, client_socket and other variables
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_size;
	pid_t pid;
	struct sigaction act;
	int state;

	// check input
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// enroll signal
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	// create server socket and bind
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &TRUE, sizeof(int));
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 20) == -1)
		error_handling("listen() error");

	while (1)
	{
		// accept client
		clnt_adr_size = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
		if (clnt_sock < 0)
		{
			switch (errno)
			{
			case EINTR:
				continue;
			default:
				perror("accept");
			}
		}

		// create process
		pid = fork();
		if (pid == -1)
		{
			printf("fork error");
			close(clnt_sock);
			continue;
		}
		if (pid == 0)
		{
			close(serv_sock);
			request_handler(&clnt_sock);
			close(clnt_sock);
			puts("[server] client disconnected...");
			return 0;
		}
		else
		{
			printf("[server] Connection Request : %s:%d, forked pid : %d\n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port), pid);
			close(clnt_sock);
		}
	}
	close(serv_sock);

	return 0;
}

void request_handler(void *arg)
{
	// initialize variables
	int clnt_sock = *((int *)arg);
	char req_line[SMALL_BUF];
	FILE *clnt_read;
	FILE *clnt_write;

	// to store http request messages
	char method[10];
	char ct[15];
	char file_name[30];

	// open write stream to client
	clnt_write = fdopen(dup(clnt_sock), "w");

	// open read stream and read request message from client
	clnt_read = fdopen(clnt_sock, "r");
	fgets(req_line, SMALL_BUF, clnt_read);
	strcpy(method, strtok(req_line, " /"));
	strcpy(file_name, strtok(NULL, " /"));
	strcpy(ct, content_type(file_name));
	printf("[client] method : %s, file_name : %s, content_type : %s\n", method, file_name, ct);

	// check if method is GET
	if (strcmp(method, "GET") != 0)
	{
		send_error(clnt_write);
		fclose(clnt_read);
		return;
	}

	// close read stream from client and send requested data;
	fclose(clnt_read);
	send_data(clnt_write, ct, file_name);
}

void send_data(FILE *fp, char *ct, char *file_name)
{
	char protocol[] = "HTTP/1.0 200 OK\r\n";
	char server[] = "Server: Linux Web Server \r\n";
	char cnt_len[SMALL_BUF];
	char cnt_type[SMALL_BUF];
	char buf[BUF_SIZE];
	int file_size = 0;
	char ch;
	FILE *send_file;

	// get file size
	send_file = fopen(file_name, "r");
	if (send_file == NULL)
	{
		send_error(fp);
		return;
	}
	fseek(send_file, 0, SEEK_END);
	file_size = ftell(send_file);
	fseek(send_file, 0, SEEK_SET);

	// store file size, content-type
	sprintf(cnt_len, "Content-Length: %d\r\n", file_size);
	sprintf(cnt_type, "Content-Type: %s\r\n\r\n", ct);

	/* send head */
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	printf("[server] %s, %s, file_name : %s, %s, %s\n", protocol, server, file_name, cnt_len, cnt_type);

	/* send file */
	while (fread(&ch, 1, 1, send_file) > 0)
	{
		fwrite(&ch, 1, 1, fp);
	}
	fclose(send_file);
	fclose(fp);
}

char *content_type(char *file)
{
	// parse content_type from message
	char extension[SMALL_BUF];
	char file_name[SMALL_BUF];
	char *ptr;
	strcpy(file_name, file);
	strtok(file_name, ".");
	ptr = strtok(NULL, ".");
	if (ptr == NULL)
		return "text/plain";
	strcpy(extension, ptr);

	// return proper content_type
	if (!strcmp(extension, "html"))
		return "text/html";
	else if (!strcmp(extension, "jpg"))
		return "image/jpg";
	else if (!strcmp(extension, "png"))
		return "image/png";
	else
		return "text/plain";
}

void send_error(FILE *fp)
{
	// error send code
	char protocol[] = "HTTP/1.0 404 Not Found\r\n";
	char server[] = "Server: Linux Web Server \r\n";
	char cnt_len[SMALL_BUF];
	char cnt_type[] = "Content-Type: text/html\r\n\r\n";
	int file_size = 0;
	char ch;
	FILE *send_file;

	// get, store file size
	send_file = fopen("404.html", "r");
	if (send_file == NULL)
	{
		send_error(fp);
		return;
	}
	fseek(send_file, 0, SEEK_END);
	file_size = ftell(send_file);
	fseek(send_file, 0, SEEK_SET);
	sprintf(cnt_len, "Content-Length: %d\r\n", file_size);

	// send head
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	printf("[server] %s, %s, file_name : %s, %s, cnt_type : %s\n", protocol, server, "404.html", cnt_len, "text/html");

	// send file
	while (fread(&ch, 1, 1, send_file) > 0)
	{
		fwrite(&ch, 1, 1, fp);
	}
	fclose(send_file);
	fflush(fp);
	fclose(fp);
}

void read_childproc(int sig)
{
	// find all zombie process and kill
	pid_t pid;
	int status;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
		printf("[server] removed proc id: %d \n", pid);
}

void error_handling(char *message)
{
	// print error message
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
