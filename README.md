# single-threaded-asynchronous-socket-server-client-in-C
It is a small project which has a socket server and a socket client implemented in C. More than one clients can be connected to the server and communicate with themselves (in the console, no GUI). Both the client and server is single threaded and uses asynchronous I/O to simultaneously read from or write into other sockets.

- ## How to spin up the SERVER  
> **```server.c```** is the main server application. It usees only one thread and asynchronous I/O to read from and write into sockets simultaneously. The Makefile associated with the project has a target names server to compile and run the whole thing. Or I am providing the instructions if you want to DIY.   
> Run the following commands **TO COMPILE** it into **ONE EXECUTABLE (FOR LINUX ONLY)** from the **ROOT DIRECTORY** of the project -   
> ```gcc -c -Wall ./lib/server_utils.c```   
> ```gcc -c -Wall ./lib/logger.c```   
> ```gcc -c -Wall ./error_handling.c```   
> ```gcc -c -Wall ./message_parser.c```   
> ```gcc -c -Wall server.c```   
> ```gcc -o s.out server.o server_utils.o logger.o error_handling.o message_parser.o```   
>
> Now run that s.out   

- ## How to spin up the CLIENTS
> **```app.c```** is the main client application. It usees only one thread and asynchronous I/O to read from and write into sockets simultaneously. The Makefile associated with the project has a target names client to compile and run the whole thing. Or I am providing the instructions if you want to DIY.   
> Run the following commands TO COMPILE it into ONE EXECUTABLE (FOR LINUX) from the ROOT DIRECTORY of the project -   
> ```gcc -c -Wall ./lib/tcp_client.c```   
> ```gcc -c -Wall ./lib/logger.c```   
> ```gcc -c -Wall ./error_handling.c```   
> ```gcc -c -Wall ./message_parser.c```   
> ```gcc -c -Wall app.c```   
> ```gcc -o c.out app.o tcp_client.o logger.o error_handling.o message_parser.o```   
>   
> Now run that c.out   
