#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MY_PORT 8989  // Port number
#define MAXBUF 256  // Maximum length of the message
#define EXIT_SERVER_LEN 11  // Length of the "exit server" message
#define DATE_LEN 4  // Length of the "date" message
char exitservercommand[] = "exit server"; // Message to exit the server
char displaydatecommand[] = "DATE"; // Message to get the date 

int main(int argc, char *argv[])
{
  WSADATA wsa;
  SOCKET sockfd, clientfd;
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
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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
  if (bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0)
  {
    perror("socket--bind");
    exit(errno);
  }

  puts("Bind done");

  //make it a listening socket
  if (listen(sockfd, 20) != 0)
  {
    perror("socket--listen");
    exit(errno);
  }

  puts("Waiting for incoming connections...");


  int running = 1;
  while (running)
  {
    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    //accept a connection
    clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
    if (clientfd < 0)
    {
      perror("accept");
      continue;
    }

    // Display the IP address and port number of the client
    printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Port number:%d \n",MY_PORT);
    while (1)
    {
      int n = recv(clientfd, buffer, MAXBUF, 0);
      if (n < 0)
      {
        perror("recv");
        continue;
      }
          // Display the length of the message received
      printf("Message received: %d characters\n", n-1);

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
  printf("The server has been terminated\n");
  // Close the client socket
  closesocket(clientfd);

  // Break out of the inner while loop to accept a new connection
  break;
}

// Check if the message is "date"
int is_displaydatecommand = 1;  // Flag to indicate if the message is "date"
for (i = 0; i < DATE_LEN; i++) {
  if (buffer[i] != displaydatecommand[i]) {
    is_displaydatecommand = 0;
    break;
  }
}

if (is_displaydatecommand) {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char date_time_str[100];  // Buffer to store the date and time string
  strftime(date_time_str, sizeof(date_time_str), "%a %b %d %H:%M:%S %Y \r\n ", tm);   // Format the date and time string 
  send(clientfd, date_time_str, strlen(date_time_str), 0);
  continue;
}

// Reverse the input message
for (i = 0; i < n/2; i++)
{
  char temp = buffer[i];
  buffer[i] = buffer[n-i-1];
  buffer[n-i-1] = temp;
}

for (i = 0; i < n; i++)
{
    if (buffer[i] >= 'A' && buffer[i] <= 'Z')
    {
        buffer[i] = buffer[i] + 32;
    }
}

send(clientfd, buffer, n, 0);
    }
  }


  closesocket(sockfd);
  WSACleanup();

  return 0;
}
