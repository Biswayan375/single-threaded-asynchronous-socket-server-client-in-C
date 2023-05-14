#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server_utils.h"
#include "defaults.h"
#include "logger.h"

/**
 * Creates a new user i.e.: struct connected_user and returns it
 * @param fd file descriptor of the user
 * @param name name of the user
 * @return the new user i.e.: struct connected_user
*/
connected_user_t *new_user (int fd, char *name) {
	if (fd > 0) {
		connected_user_t *user = (connected_user_t *)malloc(sizeof(connected_user_t));
		user->connfd = fd;

		if (name != NULL) {
			user->name = (char *)malloc(sizeof(char) * strlen(name));
			strncpy(user->name, name, strlen(name));
		} else {
			user->name = NULL;
		}

		return user;
	}

	return NULL;
}

/**
 * Inserts an user i.e.: struct connected_user into the user set i.e.: connected_user_set.
 * @param user_set the set of all connected users i.e.: struct connected_user_set
 * @param user the user i.e.: struct connected_user that is to be inserted
 * @return 1 is successful, 0 otherwise
*/
int insert_into_user_set(connected_user_set_t *user_set, connected_user_t *user) {
	if (user_set != NULL && user != NULL) {
		// connected_user_set_t set = *user_set;
		if ((*user_set).users == NULL)
			(*user_set).users = (connected_user_t **)malloc(MAX_CLIENTS*(sizeof(connected_user_t *)));
		if ((*user_set).user_count < MAX_CLIENTS) {
			(*user_set).users[(*user_set).user_count++] = user;
			console_log("SERVER", SUCCESS, "client[%d] has connected to the network\nTotal %d clients\n", user->connfd, (*user_set).user_count);
			return 1;
		} else {
			console_log("SERVER", DANGER, "cannot connect client\n");
		}
	}

	return -1;
}

/**
 * Finds the user i.e.: struct connected_user from the user set i.e.: struct connected_user_set by its
 * file descriptor. It uses linear search but I guess binary search can be employed with some slight
 * extra overhead for better performance. But that remains for future.
 * @param user_set the set of all connected users i.e.: struct connected_user_set
 * @param fd file descriptor of the user to be found
 * @return the user i.e.: struct connected_user if found, NULL otherwise
*/
connected_user_t *find_user_from_set(connected_user_set_t *user_set, int fd) {
	if (user_set != NULL && fd > 0) {
		connected_user_set_t set = *user_set;
		for (int i = 0; i < set.user_count; i++) {
			if (set.users[i]->connfd == fd)
				return set.users[i];
		}
	}

	return NULL;
}

/**
 * Finds the user i.e.: struct connected_user from the user set i.e.: struct connected_user_set
 * by its name. It uses linear search but I guess binary search can be employed with some slight
 * extra overhead for better performance. But that remains for future.
 * @param user_set the set of all connected users i.e.: struct connected_user_set
 * @param name name of the user to be found
 * @return the user i.e.: struct connected_user if found, NULL otherwise
*/
connected_user_t *find_name_from_set(connected_user_set_t *user_set, char *name) {
	if (user_set != NULL && name != NULL) {
		for (int i = 0; i < (*user_set).user_count; i++) {
			if (strcmp((*user_set).users[i]->name, name) == 0) {
				return (*user_set).users[i];
			}
		}
	}

	return NULL;
}

/**
 * Utility function used in the function remove_user_from_set
*/
int get_index_of_user(connected_user_set_t *user_set, int fd) {
	for (int i = 0; i < (*user_set).user_count; i++) {
		if ((*user_set).users[i]->connfd == fd)
			return i;
	}

	return -1;
}

/**
 * Remove a user i.e.: struct connected_user from the set of users i.e.: struct connected_user_set.
 * @param user_set the set of all connected users i.e.: struct connected_user_set
 * @param fd the file descriptor of the user that is to be removed
 * @return the removed user i.e.: struct connected_user
*/
connected_user_t *remove_user_from_set(connected_user_set_t *user_set, int fd) {
	if (fd > 0 && user_set != NULL) {
		int in = get_index_of_user(user_set, fd);
		connected_user_t *removed = (*user_set).users[in];
		for (int i = in; i < (*user_set).user_count-1; i++)
			(*user_set).users[i] = (*user_set).users[i+1];
		
		(*user_set).user_count--;
		return removed;
	}

	return NULL;
}

/**
 * Broadcast a message to all users in the user set except the user specified by the
 * except_fd. In case of no exceptions, pass -1 to except_fd. Generally, broadcast messages
 * are not to be sent to the receiver itself.
 * @param user_set the set of all connected users i.e.: struct connected_user_set
 * @param except_fd file descriptor of the exception user
 * @param message the message to broadcast (sprintf can be used to format the message)
*/
void broadcast(connected_user_set_t *user_set, int except_fd, char *message) {
	for (int i = 0; i < user_set->user_count; i++) {
		if (except_fd != user_set->users[i]->connfd && user_set->users[i]->name != NULL) {
			write(user_set->users[i]->connfd, message, strlen(message));
		}
	}
}