#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<string.h>
#include<ctype.h>

#define MY_PORT 8989 // port number
#define MAXBUF 256 // max buffer size

int main(int argc , char *argv[])
{
WSADATA wsa;
SOCKET sockfd , clientfd;
struct sockaddr_in self;
char buffer[MAXBUF];
printf("\nInitialising Winsock...");
if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
{
    printf("Failed. Error Code : %d",WSAGetLastError());
    return 1;
}

printf("Initialised.\n");

//create socket
if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
{
	perror("Socket");
	exit(errno);
}

    printf("Socket created.\n");

//initialize address/port structure

self.sin_family = AF_INET;
self.sin_port = htons(MY_PORT);
self.sin_addr.s_addr = INADDR_ANY;

//assign a port number to the socket
if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
{
	perror("socket--bind");
	exit(errno);
}

    puts("Bind done");

//make it a listening socket
if ( listen(sockfd, 20) != 0 )
{
	perror("socket--listen");
	exit(errno);
}

    puts("Waiting for incoming connections...");


while (1)
{	struct sockaddr_in client_addr;
	int addrlen=sizeof(client_addr);

	//accept a connection
	clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

	int n = recv(clientfd, buffer, MAXBUF, 0);

// reverse the input message
int i;
for (i = 0; i < n/2; i++)
{
    char temp = buffer[i];
    buffer[i] = buffer[n-i-1];
    buffer[n-i-1] = temp;
}

// convert all alphabets to lowercase
for (i = 0; i < n; i++)
{
    if (buffer[i] >= 'A' && buffer[i] <= 'Z')
    {
        buffer[i] = buffer[i] + 32;
    }
}

send(clientfd, buffer, n, 0);

	//closes connection
	close(clientfd);
}

//close socket
close(sockfd);
WSACleanup();
	return 0;
}
