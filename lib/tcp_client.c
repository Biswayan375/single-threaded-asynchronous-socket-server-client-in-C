#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "tcp_client.h"
#include "logger.h"
#include "error_handling.h"
#include "defaults.h"

/**
 * Creates a socket connection to the host specified by the addr.
 * @param addr IP address (string) of the host
 * @return file descriptor of the socket connection
*/
int create_connection(char *addr) {
	int 				fd, status;
	struct sockaddr_in 	*serv_addr;

	fd = socket(AF_INET, SOCK_STREAM, getprotobyname("IP")->p_proto);
	CHECK(fd);			// see tcp_client.h

	serv_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	serv_addr->sin_family 	= AF_INET;
	serv_addr->sin_port 	= htons(SERVER_PORT);

	status = inet_pton(AF_INET, addr, &serv_addr->sin_addr);
	CHECK(status);

	status = connect(fd, (struct sockaddr *)serv_addr, sizeof(struct sockaddr));
	CHECK(status);

	return fd;
}

/**
 * Closes the socket connection specified by the file descriptor
*/
void close_connection(int fd) { close(fd); }