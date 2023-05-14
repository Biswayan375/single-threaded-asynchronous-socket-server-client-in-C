/**
 * This is the main client application. It usees only one thread and asynchronous I/O to read from and
 * write into sockets simultaneously. The Makefile associated with the project has a target names client
 * to compile and run the whole thing. Or I am providing the instructions if you want to DIY.
 * 
 *							H O W   T O   S P I N   U P   T H E   C L I E N T
 * Run the following commands TO COMPILE it into ONE EXECUTABLE (FOR LINUX) from the ROOT DIRECTORY of the project -
 * gcc -c -Wall ./lib/tcp_client.c
 * gcc -c -Wall ./lib/logger.c
 * gcc -c -Wall ./error_handling.c
 * gcc -c -Wall ./message_parser.c
 * gcc -c -Wall app.c
 * gcc -o c.out app.o tcp_client.o logger.o error_handling.o message_parser.o
 *
 * Now run that c.out
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>				// functions and macros for asynchronous I/O


#include "lib/tcp_client.h"			// utility functions for opening and closing a client socket connection - create_connection, close_connection
#include "lib/logger.h"				// utility function and macros for logging in the console in different colors - console_log, DANGER, SUCCESS etc.
#include "lib/error_handling.h"		// error handling utility function & macro - err_and_die & CHECK
#include "lib/defaults.h"			// default configuration macros - SERVER_PORT, MAXLINE, MAX_CLIENTS, HOST
#include "lib/message_parser.h"		// utility function for parsing a message into sender and body portion - parse_message


int main() {
	int fd, n;
	char *sender, *body;
	char *recvline, message[100];

	/**
	 * There are two types of messages that can be received from the socket -
	 * 1. <SENDER>:<MESSAGE_BODY>		- where we can easily separate the sender portion from the message body
	 * 2. <MESSAGE_BODY>				- where there is no sender portion in the messsage, only the message itself
	 * 
	 * The first kind of messages are used by the server or other users of the application to send general messages
	 * and the second kind of messages are used only by the server as control messages such as, disconnection an user.
	 *
	 * There are currently two control messages that can be received from the server - 'ROOM FULL' and 'see ya!'
	 * */
	recvline = (char*) malloc(sizeof(char)*MAXLINE);			// for receiving the entire message from the socket
	sender = (char*) malloc(sizeof(char)*MAXLINE);				// for getting the sender portion of the message if any
	body = (char*) malloc(sizeof(char)*MAXLINE);				// for getting the body portion of the message if any

	fd = create_connection(HOST);								// creating the socket connection to the HOST

    fd_set current_sockets, ready_sockets;						// creating fd_sets for monitoring file descriptors
    FD_ZERO(&current_sockets);									// initializing fd_set
    FD_SET(fd, &current_sockets);								// including the socket file descriptor into the fd_set for monitoring
    FD_SET(STDIN_FILENO, &current_sockets);						// including the standard input into the fd_set for monitoring

    while (1) {
		// the application loop starts and continues until the user disconnects
    	ready_sockets = current_sockets;						// getting the original fd_set current_socket into temporary fd_set ready_sockets
																// this is done because the select function we will use is destructive and changes the fd_set passed to it

    	if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
            err_and_die("Application", "select error\n");		// closing the application if select does not work
		}

        for (int i = 0; i < FD_SETSIZE; i++) {
			// checking for all available file descriptors (however, checking for only two, the STDIN one and the socket one would work too just fine and even faster)
        	if (FD_ISSET(i, &ready_sockets)) {
				// checking if the i-th file descriptor is ready for I/O i.e.: set by the select function in the ready_sockets fd_set
        		if (i == fd) {
					// when the file descriptor that is ready for I/O is the socket one
        			memset(recvline, 0, MAXLINE-1);
        			memset(sender, 0, MAXLINE-1);
        			memset(body, 0, MAXLINE-1);

        			n = read(i, recvline, MAXLINE-1);


					if (n > 0) {
						// when read message from the socket has more than 0 bytes, doing actions based on the message
						if (strcmp(recvline, "ROOM FULL") == 0) {
							// cannot connect this due to maximum users are already connected to the server
							close(fd);
							err_and_die("Aplication", "Sorry, cannot connect you to the network\n");
						} else if (strcmp(recvline, "see ya!") == 0) {
							// disconnecting this client from the server
							close(fd);
							err_and_die("Aplication", "bye, see ya!\n");
						}

						int status = parse_message(recvline, &sender, &body);
						if (status == 0) {
							console_log(sender, WARNING, "%s\n", body);
						} else {
							console_log("Application", INFO, "%s\n", recvline);
						}
					}
        		} else {
					// when the file descriptor that is ready for I/O is not the socket one must have to be the STDIN one
					// so, I can now take input the message and send it
        			gets(message);
        			write(fd, message, strlen(message));
        			console_log("Application", SUCCESS, "message sent\n");
        		}
        	}
        } // for
    } // while
}