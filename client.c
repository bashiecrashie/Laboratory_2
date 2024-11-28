#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>

void print_help();
int create_client(char *ip, int port);
int conn_recv(int socket);

#define red "\033[1;31m"
#define blue "\033[1;34m"
#define green "\033[1;32m"
#define ret "\033[00m"

void print_help()
{
	char *help = "./lab_2_client <server ip> <server port>\n"
		"example: ./lab_2_client 192.168.2.1 1234\n";
	puts(help);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("%s[-]%sInvalid number of arguments\n", red, green);
		print_help();
		exit(1);
	}

	int client_sock = create_client(argv[1], atoi(argv[2]));

	return 0;
}

int create_client(char *ip, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	char buff[100];

	printf("%s[*]%sCreating socket...\n", blue, green);
	if(sock < 0)
	{
		perror("[create_cliet: socket] reason");
		exit(1);
	}
	printf("%s[+] Socket created !\n", green);

	struct sockaddr_in client;

	client.sin_family = AF_INET;
	client.sin_port = htons(port);
	
	int pton_status = inet_pton(AF_INET, ip, &client.sin_addr);

	if(pton_status < 0)
	{
		perror("[create_client: inet_pton ] reason");
		exit(1);
	}

	printf("%s[*]%sConnecting...\n", blue, green);
	
	int conn_status = connect(sock, (struct sockaddr *)&client,  sizeof(client));

	if(conn_status < 0)
	{
		perror("[create_client: connect] reason");
		exit(1);
	}

	printf("%s[+] Connected !\n", green);
	
		int recv_status = recv(sock, buff, sizeof(buff), 0);
		
		printf("Received:%s\n", buff);
		if(recv_status < 0)
		{
			perror("[recv] reason");
			exit(1);
		}	

	close(sock);
}
