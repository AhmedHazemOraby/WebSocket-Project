        #include <stdio.h>
        #include <winsock2.h>
        #include <string.h>
        #include <ctype.h>
        #include <time.h>

        #define MY_PORT 8989  // Port number
        #define MAXBUF 256  // Maximum length of the message
        #define EXIT_SERVER_LEN 11  // Length of "exit server"
        #define DATE_LEN 4  // Length of "date"
        #define TIME_LEN 4  // Length of "time"



        char exitservercommand[] = "exit server"; // Command to exit the server
        char displaydatecommand[] = "DATE"; // Command to get the date
        char displaytimecommand[] = "TIME"; // Command to get the time



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
        for (i = 0; i < EXIT_SERVER_LEN; i++)   
        {
          if (buffer[i] != exitservercommand[i]) {
            is_exitservercommand = 0;
            break;
          }
        }

        if (is_exitservercommand) {
          // Close the client socket
          closesocket(clientfd);

          // Break out of the inner while loop to accept a new connection
          break;
        }

        // Check if the message is "date"
        int is_displaydatecommand = 1;
        for (i = 0; i < DATE_LEN; i++) 
        {
          if (buffer[i] != displaydatecommand[i])  // If the message is not "date", break out of the loop, and send the message to the client
           {
            is_displaydatecommand = 0;
            break;
          }
        }

        if (is_displaydatecommand) // If the message is "date", send the date to the client
         {
          time_t t = time(NULL);
          struct tm *tm = localtime(&t);
          char date_time_str[100];
          strftime(date_time_str, sizeof(date_time_str), "%a %b %d %H:%M:%S %Y \r\n ", tm);
          send(clientfd, date_time_str, strlen(date_time_str), 0);
          continue;
        }



    else {
        if (buffer[0] == 'T' && buffer[1] == 'I' && buffer[2] == 'M' && buffer[3] == 'E' && buffer[4] == ' ') {
            char tz[4]; // 3 character Time zones
            char tzaedt[5]; // Time zone AEDT
            tz[0] = buffer[5];  
            tz[1] = buffer[6];  
            tz[2] = buffer[7];  //Get time zone 
            tz[3] = '\0';
            tzset();  // Set time zone
            tzaedt[0] = buffer[5];  
            tzaedt[1] = buffer[6];
            tzaedt[2] = buffer[7];
            tzaedt[3] = buffer[8];  // Get time zone AEDT
            tzaedt[4] = '\0';
            tzset(); // Set time zone AEDT
            time_t current_time = time(NULL); // Get current time
            struct tm *tm = localtime(&current_time); // Convert to local time
            char time_string[100]; // String to hold time
            tm->tm_hour = tm->tm_hour - 8; // Convert to AEDT

            // Check time zones
            if (tz[0] == 'P' && tz[1] == 'S' && tz[2] == 'T') {
                tm->tm_hour = tm->tm_hour - 8;  // Convert to PST
            } else if (tz[0] == 'M' && tz[1] == 'S' && tz[2] == 'T') {
                tm->tm_hour = tm->tm_hour - 7;  // Convert to MST
            } else if (tz[0] == 'C' && tz[1] == 'S' && tz[2] == 'T') {
                tm->tm_hour = tm->tm_hour - 6;  // Convert to CST
            } else if (tz[0] == 'E' && tz[1] == 'S' && tz[2] == 'T') {
                tm->tm_hour = tm->tm_hour - 5;  // Convert to EST
            } else if (tz[0] == 'G' && tz[1] == 'M' && tz[2] == 'T') {
                tm->tm_hour = tm->tm_hour;  // Convert to GMT
            } else if (tz[0] == 'C' && tz[1] == 'E' && tz[2] == 'T') {
                tm->tm_hour = tm->tm_hour + 1;  // Convert to CET
            } else if (tz[0] == 'M' && tz[1] == 'S' && tz[2] == 'K') {
                tm->tm_hour = tm->tm_hour + 3;  // Convert to MSK
            } else if (tzaedt[0] == 'A' && tzaedt[1] == 'E' && tzaedt[2] == 'D' && tzaedt[3] == 'T') {
                tm->tm_hour = tm->tm_hour + 11; // Convert to AEDT
            } else if (tz[0] == 'J' && tz[1] == 'S' && tz[2] == 'T') {
                tm->tm_hour = tm->tm_hour + 9;  // Convert to JST
            } else {
                for (int i = 0; i < 5; i++) {
                    time_string[i] = "ERROR"[i];  // Error message
                }
                send(clientfd, time_string, 256, 0);  // Send error message
                continue;
            }

            // Format time string
                    int hour = tm->tm_hour;
            int min = tm->tm_min;
            int sec = tm->tm_sec;
            time_string[0] = (hour / 10) + '0'; 
            time_string[1] = (hour % 10) + '0';  
            time_string[2] = ':';
            time_string[3] = (min / 10) + '0';
            time_string[4] = (min % 10) + '0';
            time_string[5] = ':';
            time_string[6] = (sec / 10) + '0';
            time_string[7] = (sec % 10) + '0';  
            time_string[8] = '\n';  
            time_string[9] = '\0';

            // Send time string to client
            send(clientfd, time_string, 256, 0);
            continue;
        } else if (buffer[0] == 'T' && buffer[1] == 'I' && buffer[2] == 'M' && buffer[3] == 'E') {
            tzset();  
            time_t current_time = time(NULL); 
            struct tm *tm = localtime(&current_time);
            char time_string[100];  

            // Format time string
            int hour = tm->tm_hour;
            int min = tm->tm_min;
            int sec = tm->tm_sec;
            time_string[0] = (hour / 10) + '0';
            time_string[1] = (hour % 10) + '0';
            time_string[2] = ':';
            time_string[3] = (min / 10) + '0';
            time_string[4] = (min % 10) + '0';
            time_string[5] = ':';
            time_string[6] = (sec / 10) + '0';
            time_string[7] = (sec % 10) + '0';
            time_string[8] = '\n';
            time_string[9] = '\0';

            // Send time string to client
            send(clientfd, time_string, 256, 0);
            continue;
        }
    }



        // Reverse the input message
        for (i = 0; i < n/2; i++)
        { 
          char temp = buffer[i];
          buffer[i] = buffer[n-i-1];
          buffer[n-i-1] = temp;
        }

        // Convert all alphabets to lowercase
       
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
