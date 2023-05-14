#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "message_parser.h"
#include "defaults.h"

/**
 * Parses the message and separates into receiver and body if possible i.e.:
 * if the message was in the format -
 * <receiver>:<body>
 * @param message the message to be parsed
 * @param receiver address of the string where the receiver portion from the message is to be stored if parsing is successful
 * @param body address of the string where the body portion from the message is to be stored if parsing is successful
 * @return 1 if parsing is successful, 0 otherwise
*/
int parse_message(char *message, char **receiver, char **body) {
	// printf("called");
	if (message != NULL) {
		int got_receiver = 0;

		for (int i = 0; message[i] != 0 || message[i] != '\0';) {
			if (got_receiver == 0) {
				while (message[i] != ':' && message[i] != 0 && message[i] != '\0')
					i++;
				if (got_receiver == 0 && message[i] == ':') {
					got_receiver = 1;
					for (int j = 0, k = 0; k < i;) {
						// printf("%c", message[k]);
						(*receiver)[j++] = message[k++];
					}
				} else if (got_receiver == 0 && (message[i] == 0 && message[i] != '\0')) {
					break;
				}
			} else {
				i = i+1;
				for (int j = 0; message[i] != 0 && message[i] != '\0';)
					(*body)[j++] = message[i++];
			}
		}

		if (got_receiver == 0)
			return -1;
		return 0;
	}

	return -1;
} 