#ifndef __SERVER_UTILS_H
#define __SERVER_UTILS_H
	
	/**
	 * for the purpose of storing information about connected users
	 * with their name and connfd i.e.: the socket file descriptor
	*/
	typedef struct connected_user {
		char *name; 		// the name of the user
		int connfd;			// the connection id of the user
	} connected_user_t;

	/**
	 * for the purpose of keeping an array of all connected users i.e.:
	 * an array of struct connected_user and the number of connected
	 * users
	*/
	typedef struct connected_user_set {
		connected_user_t **users;		// array of struct connected_user
		int user_count;					// number of conected users i.e.: no. of elements in the array
	} connected_user_set_t;


	int insert_into_user_set(connected_user_set_t *user_set, connected_user_t *user);

	connected_user_t *new_user(int fd, char *name);
	connected_user_t *find_user_from_set(connected_user_set_t *user_set, int fd);
	connected_user_t *find_name_from_set(connected_user_set_t *user_set, char *name);
	connected_user_t *remove_user_from_set(connected_user_set_t *user_set, int fd);

	void broadcast(connected_user_set_t *user_set, int except_fd, char *message);
#endif