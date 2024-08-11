# WebSocket-Project

I made this project during my first year, the project was meant to teach us how to create a server in C programming language using the Winsock API. I used PuTTY as well as XAMPP's Apache for the development. They are not needed for launching the server since I also created an EchoClient.

#TO START THE SERVER:

1. Install the project
2. Open the terminal and cd into the project folder
3. Paste the following commands to create the executable files:

   gcc -o server server4.c -lws2_32  #Change Server number as needed
   gcc -o client EchoClient.c -lws2_32

4. Open "Server.exe" first and when it gives the message: "Waiting for incoming connections...", open "client.c"
5. Now the client should be connected to the server.

#FUNCTIONS:

The server reverses the strings inputted in the client, if the string is lower case it reverses it to lower case, if it contains capital letters it gets switched to lower case.
The server shows the number of characters in the inputted string.

#SPECIAL FUNCTIONS (CASE SENSITIVE):

TIME: Displays time in current timezone
DATE: Displays date and time
exit server: exits from server

TIME "TimeZone": Displays time in selected time zone
--Use one of the follwing supported time zones instead of "TimeZone":

1. PST
2. MST
3. CST
4. EST
5. GMT
6. CET
7. MSK
8. JST
9. AEDT
