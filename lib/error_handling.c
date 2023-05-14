#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include "error_handling.h"
#include "logger.h"

/**
 * Shows the error message and the extra message passed to it as formatted string
 * in the same fancy way as console_log in the terminal in red and terminates the
 * program. It shows actual error messages based on errno.
*/
void err_and_die(char *source, const char *format, ...) {
	int 	errno_save;
	va_list	args;

	// first saving the errno
	errno_save = errno;

	// printing the message
	printf("%s[%s]: ", RED, source);
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	// logging the error message if errno was set
	if (errno_save != 0)
		printf("\nERROR: %s", strerror(errno_save));
	printf("%s\n", RESET_COLOR);
	// terminating the execution
	exit(1);
}