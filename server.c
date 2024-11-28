#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<netdb.h>
#include<string.h>
#include<fcntl.h>

void print_logo();
void print_help();
int server(int port);
int read_blacklist(char *ip, int port);

#define red "\033[1;31m"
#define blue "\033[1;34m"
#define green "\033[1;32m"
#define ret "\033[00m"

struct sockaddr_in srv, client_info;

void print_logo()
{
    char *logo = ""                                           
"┌───────────────────────────────────────────────┐\n"
"│  ██████  █████  ██████ ██    ██  █████  ██████│\n"
"│ ██░░░░  ██░░░██░░██░░█░██   ░██ ██░░░██░░██░░█│\n"
"│░░█████ ░███████ ░██ ░ ░░██ ░██ ░███████ ░██ ░ │\n"
"│ ░░░░░██░██░░░░  ░██    ░░████  ░██░░░░  ░██   │\n"
"│ ██████ ░░██████░███     ░░██   ░░██████░███   │\n"
"│░░░░░░   ░░░░░░ ░░░       ░░     ░░░░░░ ░░░    │\n"
"└───────────────────────────────────────────────┘\n";

			puts(logo);

}

void print_help()
{
	char *help = "\t./lab_2_server <port>\n"
		"\texample: ./lab_2_server 1234\n";
	puts(help);
}

int main(int argc, char *argv[])
{

	if(argc != 2)
	{
		printf("%s[-]%sInvalid number of arguments !\n", red, green);
		print_help();
		exit(1);
	}

	print_logo();
	
	server(atoi(argv[1]));

	return 0;
}

int read_blacklist(char *ip, int port)
{
	char *list_path = "blacklist";
	char word[20], ch;
	FILE *f = fopen(list_path, "r");
	int status = 99;
	while((fscanf(f, "%20s", word) == 1))
	{
		if((strcmp(word, ip)) == 0)
		{
			status = 1;
			break;
		}
		else
			status = 0;
	}

	return status;

}

int server(int port)
{
	int s = 0;
	
	printf("%s[*]%sCreaaing socket...\n", blue, green);
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0)
	{
		perror("[socket] reason");
		exit(1);
	}
	printf("%s[+] Socket created !\n", green);
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port);
	srv.sin_addr.s_addr = htonl(INADDR_ANY);
	
	printf("%s[*]%sBinding...\n", blue, green);

	int bind_status = bind(s, (struct sockaddr *)&srv, sizeof(srv));
	if(bind_status < 0)
	{
		perror("[bind] reason");
		exit(1);
	}
	printf("%s[+] Binded !\n%s[*]%sListening...\n", green, blue, green);
	
	char ip[255];
	char *hello_msg = "Hello";
	struct sockaddr_in client_info;
	int client_info_len = sizeof(client_info);
	
	while(1)
	{
		int listen_status = listen(s, 1);
		
		if(listen_status < 0)
		{
			perror("[listen] reason");
			exit(1);
		}
		int accept_sock = accept(s, (struct sockaddr *)&client_info, &client_info_len);
		if(accept_sock < 0)
		{
			perror("[accept] reason");
			exit(1);
		}

		inet_ntop(AF_INET, &(client_info.sin_addr), ip, sizeof(ip));

		if((read_blacklist(ip, htons(client_info.sin_port))) == 1)
		{
			printf("%s[-]%sОтказано в доступе для %s:%d\n", red, green, ip, htons(client_info.sin_port));
			close(accept_sock);
			
		}
		else
		{
			int send_status = send(accept_sock, hello_msg, sizeof(hello_msg), 0);
			
			if((send(accept_sock, hello_msg, 20, 0)) < 0)
			{
				perror("[send] reason");
				break;
			}
		}
	}
	close(s);
	
}
