#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logger.h"

/**
 * To log the formatted message in the console in a fancy colorful way.
 * For colors, some macros are available - RED, DANGER, BLUE, INFO, GREEN,
 * SUCCESS, ORANGE, WARNING & RESET. RED & DANGER are same, BLUE & INFO are
 * same, GREEN & SUCCESS are same and ORANCE & WARNING are same. The fancy
 * way of printing is -
 * [<source>]:<message>
*/
void console_log(char *source, char *color_code, const char *format, ...) {
	va_list	args;

	printf("%s[%s]: ", color_code, source);
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	printf("%s", RESET_COLOR);
}