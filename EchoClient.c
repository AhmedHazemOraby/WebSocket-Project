#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <ctype.h>

#define SERVER_IP "127.0.0.1" 
#define SERVER_PORT 8989  // port number
#define MAXBUF 256 // max buffer size
#define EXIT_SERVER_LEN 11  // length of "exit server"
#define EXIT_CLIENT_LEN 11  // length of "exit client"
char exitservercommand[] = "exit server"; // message to exit server
char exitclientcommand[] = "exit client"; // message to exit client
int main(int argc, char *argv[])
{
WSADATA wsa;
SOCKET sockfd;
struct sockaddr_in server;
char buffer[MAXBUF];
printf("\nInitialising Winsock...");
if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
{
    printf("Failed. Error Code : %d",WSAGetLastError());
    return 1;
}
printf("Initialised.\n");

//create socket
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
    perror("Socket");
    exit(errno);
}

printf("Socket created.\n");

//initialize address/port structure
memset(&server, 0, sizeof(server));
server.sin_family = AF_INET;
server.sin_port = htons(SERVER_PORT);
server.sin_addr.s_addr = inet_addr(SERVER_IP);

//connect to the server
if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) != 0)
{
    perror("Connect");
    exit(errno);
}

//connected to server
printf("Connected to server.\n");  


while (1)
{
    // read input from the console
    printf("Enter a message: ");
    fgets(buffer, MAXBUF, stdin);

    // Check if the message is "exit server"
        int i;
        int is_exitservercommand = 1;
        for (i = 0; i < EXIT_SERVER_LEN; i++) {
          if (buffer[i] != exitservercommand[i]) {
            is_exitservercommand = 0;
            break;
          }
        }

        if (is_exitservercommand) {

          closesocket(sockfd);

          // Break out of the inner while loop to accept a new connection
          break;
        }

// Check if the message is "exit server"
        int x;
        int is_exitclientcommand = 1;
        for (x = 0; x < EXIT_CLIENT_LEN; x++) {
          if (buffer[x] != exitclientcommand[x]) {
            is_exitclientcommand = 0;
            break;
          }
        }

        if (is_exitclientcommand) {
          // Close the client socket
          closesocket(sockfd);

          // Break out of the inner while loop to accept a new connection
          break;
        }
    // send the message to the server
    send(sockfd, buffer, strlen(buffer), 0);

    // receive the response from the server
    int n = recv(sockfd, buffer, MAXBUF, 0);
    if (n < 0)
    {
        perror("recv");
        continue;
    }

    // Display the response from the server
    printf("Server response: %s\n", buffer);
}


WSACleanup();
return 0;
}
