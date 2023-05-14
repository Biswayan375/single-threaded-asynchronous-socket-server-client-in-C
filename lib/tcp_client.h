#ifndef __TCP_CLIENT_H
#define __TCP_CLIENT_H

	int create_connection(char *addr);		// creates a socket to the given host address (IPv4) and returns its handle

	void close_connection(int fd);			// closes an open connection

#endif