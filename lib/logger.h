#ifndef __logger_h
#define __logger_h

	#define ORANGE "\033[93m"
	#define WARNING "\033[93m"

	#define GREEN "\033[92m"
	#define SUCCESS "\033[92m"

	#define BLUE "\033[94m"
	#define INFO "\033[94m"

	#define RED "\033[91m"
	#define DANGER "\033[91m"

	#define RESET_COLOR "\033[0m"

	void console_log(char *source, char *color_code, const char *format, ...);

#endif