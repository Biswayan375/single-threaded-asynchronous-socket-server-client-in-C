/**
 * This is the main server application. It usees only one thread and asynchronous I/O to read from and
 * write into sockets simultaneously. The Makefile associated with the project has a target names server
 * to compile and run the whole thing. Or I am providing the instructions if you want to DIY.
 * 
 *							H O W   T O   S P I N   U P   T H E   S E R V E R
 * Run the following commands TO COMPILE it into ONE EXECUTABLE (FOR LINUX) from the ROOT DIRECTORY of the project -
 * gcc -c -Wall ./lib/server_utils.c
 * gcc -c -Wall ./lib/logger.c
 * gcc -c -Wall ./error_handling.c
 * gcc -c -Wall ./message_parser.c
 * gcc -c -Wall server.c
 * gcc -o c.out server.o server_utils.o logger.o error_handling.o message_parser.o
 *
 * Now run that s.out
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>			    // functions and macros for asynchronous I/O

#include "lib/message_parser.h"		// utility function for parsing a message into sender and body portion - parse_message
#include "lib/error_handling.h"		// error handling utility function & macro - err_and_die & CHECK
#include "lib/logger.h"				// utility function and macros for logging in the console in different colors - console_log, DANGER, SUCCESS etc.
#include "lib/defaults.h"			// default configuration macros - SERVER_PORT, MAXLINE, MAX_CLIENTS, HOST
#include "lib/server_utils.h"       // utility methods to be used by the server - new_user, find_user_from_set, find_name_from_set, remove_user_from_set, broadcast


int main() {
    struct sockaddr_in servaddr;
    int listenfd, connfd, optval = 1, n, k;
    char *recvline, *receiver, *body;
    char message[200];

    recvline    = (char*) malloc(sizeof(char)*MAXLINE);
    receiver    = (char*) malloc(sizeof(char)*MAXLINE);
    body        = (char*) malloc(sizeof(char)*MAXLINE);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, getprotobyname("IP")->p_proto)) < 0)
        err_and_die("SERVER", "cannot create socket");

    servaddr.sin_family         = AF_INET;
    servaddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    servaddr.sin_port           = htons(SERVER_PORT);

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        err_and_die("SERVER", "cannot bind server to port %d", SERVER_PORT);

    if (listen(listenfd, MAX_CLIENTS) < 0)
        err_and_die("SERVER", "cannot listen on port %d", SERVER_PORT);

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        err_and_die("SERVER", "error setting socket options");
    }

    connected_user_set_t user_set = { NULL, 0 };
    fd_set current_sockets, ready_sockets;
    FD_ZERO(&current_sockets);
    FD_SET(listenfd, &current_sockets);


    // the server loop
    console_log("SERVER", SUCCESS, "server is listening on port %d\n", SERVER_PORT);
    while (1) {
        ready_sockets = current_sockets;
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
            err_and_die("SERVER", "select error\n");

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &ready_sockets)) {
                if (i == listenfd) {
                    // this is a new connection to accept
                    connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
                    if (connfd < 0) {
                        console_log("SERVER", DANGER, "cannot accept client request\n");
                    } else {
                        // after accepting the request we need to add the connection to the set
                        int status = insert_into_user_set(&user_set, new_user(connfd, NULL));

                        if (status > 0) {
                            sprintf(message, "SERVER:hello, please enter your name > ");
                            write(connfd, message, strlen(message));
                            FD_SET(connfd, &current_sockets);
                        } else {
                            memset(message, 0, 199);
                            sprintf(message, "ROOM FULL");
                            write(connfd, message, strlen(message));
                            close(connfd);
                        }
                    }
                } else {
                    connected_user_t *user = find_user_from_set(&user_set, i);

                    if (user != NULL) {
                        memset(recvline, 0, MAXLINE-1);
                        memset(message, 0, 199);

                        n = recv(i, recvline, MAXLINE-1, 0);

                        if (n > 0) {
                            int receiverfd = i;
                            if (user->name == NULL) {
                                // when the user has no name
                                if (strcmp(recvline, "disconnect") == 0) {                      // disconnect user
                                    // even if the user name no name, sending the message disconnect will disconnect the user from the network anyway
                                    console_log("SERVER", DANGER, "client[%d] has disconnected\nTotal %d clients\n", i, user_set.user_count-1);
                                    sprintf(message, "see ya!");
                                    write(i, message, strlen(message));
                                    remove_user_from_set(&user_set, i);
                                    FD_CLR(i, &current_sockets);
                                    receiverfd = -1;
                                } else {
                                    // otherwise, the sent message is set as the name of the user
                                    int flag = 0;
                                    for (int j = 0; j < user_set.user_count; j++) {
                                        if (user_set.users[j]->name != NULL)
                                            if (strcmp(user_set.users[j]->name, recvline) == 0) {
                                                flag = 1;
                                                break;
                                            }
                                    }
                                    if (flag == 0) {
                                        user->name = (char *)malloc(n * sizeof(char));
                                        strncpy(user->name, recvline, n);
                                        sprintf(message, "SERVER:welcome to the network %s\nwhoami - shows your name\nwhoarethey - shows all the connected users\ndisconnect - disconnect from the network\nTo send message to a specific client type - \n<receiver>:<message>\nTo broadcast a message to everyone on the network type -\n All:<message>\n", user->name);
                                        write(user->connfd, message, strlen(message));

                                        // broadcasting to other connected users that a new user has joined the party
                                        sprintf(message, "SERVER(broadcast):%s has joined the network\ntype `whoarethey` to see all connected users\n", user->name);
                                        broadcast(&user_set, user->connfd, message);

                                        // don't need to send message to anyone else
                                        receiverfd = -1;
                                    } else {
                                        sprintf(message, "SERVER:sorry but you cannot use the name %s as it is already taken by someone currently in the network\nyou can use - %s%d though\nplease enter your name >", recvline, recvline, user_set.user_count+1);
                                    }
                                }
                            } else {
                                if (strcmp(recvline, "help") == 0) {                                // show help
                                    sprintf(message, "SERVER:\nwhoami - shows your name\nwhoarethey - shows all the connected users\ndisconnect - disconnect from the network\n");
                                } else if (strcmp(recvline, "whoami") == 0) {                       // show user name
                                    sprintf(message, "SERVER:%s\n", user->name);
                                } else if (strcmp(recvline, "whoarethey") == 0) {                   // show all connected clients
                                    sprintf(message, "SERVER:\n");
                                    for (int j = 0, k = 1; j < user_set.user_count; j++) {
                                        if (user_set.users[j]->name != NULL) {
                                            sprintf(message, "%s%d - %s", message, k++, user_set.users[j]->name);
                                            if (user->connfd == user_set.users[j]->connfd)
                                                sprintf(message, "%s (You)", message);
                                            sprintf(message, "%s\n", message);
                                        }
                                    }
                                } else if (strcmp(recvline, "disconnect") == 0) {                   // disconnect user
                                    connected_user_t *u = find_user_from_set(&user_set, i);
                                    console_log("SERVER", DANGER, "client[%d] has disconnected\nTotal %d clients\n", i, user_set.user_count-1);
                                    sprintf(message, "SERVER(broadcast):Bye from %s\n%s has left the network\ntype `whoarethey` to see all connected users\n", u->name, u->name);
                                    broadcast(&user_set, u->connfd, message);

                                    sprintf(message, "see ya!");
                                    write(i, message, strlen(message));
                                    remove_user_from_set(&user_set, i);
                                    FD_CLR(i, &current_sockets);
                                    receiverfd = -1;
                                } else {
                                    // the message is either for a specfic user or a broadcast message for all
                                    memset(receiver, 0, MAXLINE-1);
                                    memset(body, 0, MAXLINE-1);
                                    int status = parse_message(recvline, &receiver, &body);
                                    if (status < 0) {
                                        // cannot resolve receiver name from message
                                        sprintf(message, "SERVER:receiver name not resolved from message\nmessage format - <receiver_name>:<message>\ntype `whoarethey` to list all connected users\n");
                                    } else {
                                        // receiver name resolved from message
                                        if (strcmp(receiver, "All") == 0) {
                                            sprintf(message, "%s(broadcast):%s\n", user->name, body);
                                            broadcast(&user_set, user->connfd, message);
                                            receiverfd = -1;
                                        } else {
                                            connected_user_t *receiver_user = find_name_from_set(&user_set, receiver);
                                            if (receiver_user != NULL) {
                                                receiverfd = receiver_user->connfd;
                                                sprintf(message, "%s:%s\n", user->name, body);
                                            } else {
                                                // receiverfd = -1;
                                                sprintf(message, "SERVER:receiver name (%s) not found\n", receiver);
                                            }
                                        }
                                    }
                                }
                            }

                            if (receiverfd > 0) {
                                // console_log("SERVER", SUCCESS, "sending message to %d\n", receiverfd);
                                write(receiverfd, message, strlen(message));
                                if (receiverfd != i) {
                                    sprintf(message, "SERVER:message received\n");
                                    write(i, message, strlen(message));
                                }
                            }
                        } // if more than 0 bytes are read from socket
                    } // if user is not null
                }
            }
        } // for

    } // while
}