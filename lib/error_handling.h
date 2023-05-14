#ifndef __ERROR_HANDLING_H
#define __ERROR_HANDLING_H
	#define CHECK(X) { \
						if (X == -1) { \
							printf("\nError: %s(%d)\nMessage: %s\n", __FILE__, __LINE__, strerror(errno)); \
							exit(-1); \
						} \
					}
					
	void err_and_die(char *source, const char *format, ...);
#endif