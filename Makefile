CC=gcc
CFLAGS=-c -Wall
TESTFILE=./test.c



server: server.o logger.o error.o server_utils.o message_parser.o
	$(CC) server.o logger.o error_handling.o server_utils.o message_parser.o -o s.out
	./s.out

client: app.o tcp_client.o logger.o error.o message_parser.o
	$(CC) app.o tcp_client.o logger.o error_handling.o message_parser.o -o c.out
	./c.out

app.o: app.c
	$(CC) $(CFLAGS) app.c

server.o: server.c
	$(CC) $(CFLAGS) server.c

tcp_client.o: ./lib/tcp_client.c
	$(CC) $(CFLAGS) ./lib/tcp_client.c

message_parser.o: ./lib/message_parser.c
	$(CC) $(CFLAGS) ./lib/message_parser.c

server_utils.o: ./lib/server_utils.c
	$(CC) $(CFLAGS) ./lib/server_utils.c

logger.o: ./lib/logger.c
	$(CC) $(CFLAGS) ./lib/logger.c

error.o: ./lib/error_handling.c
	$(CC) $(CFLAGS) ./lib/error_handling.c

clean:
	clear
	rm *.o *.out


test: message_parser.o
	$(CC) $(CFLAGS) $(TESTFILE)
	$(CC) test.o message_parser.o