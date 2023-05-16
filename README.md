# single-threaded-asynchronous-socket-server-client-in-C
It is a small project which has a socket server and a socket client implemented in C. More than one clients can be connected to the server and communicate with themselves (in the console, no GUI). Both, client and server are single threaded and uses asynchronous I/O to simultaneously read from or write into other sockets.

- ## How to spin up the SERVER  
> **```server.c```** is the main server application. It usees only one thread and asynchronous I/O to read from and write into sockets simultaneously. The Makefile associated with the project has a target name server to compile and run the whole thing. Or I am providing the instructions if you want to DIY.   
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
> **```app.c```** is the main client application. It usees only one thread and asynchronous I/O to read from and write into sockets simultaneously. The Makefile associated with the project has a target name client to compile and run the whole thing. Or I am providing the instructions if you want to DIY.   
> Run the following commands TO COMPILE it into ONE EXECUTABLE (FOR LINUX) from the ROOT DIRECTORY of the project -   
> ```gcc -c -Wall ./lib/tcp_client.c```   
> ```gcc -c -Wall ./lib/logger.c```   
> ```gcc -c -Wall ./error_handling.c```   
> ```gcc -c -Wall ./message_parser.c```   
> ```gcc -c -Wall app.c```   
> ```gcc -o c.out app.o tcp_client.o logger.o error_handling.o message_parser.o```   
>   
> Now run that c.out   

- ## Some Images
> 1. Running the server. The port where the server should run and the clients should try to connect and also what is the limit of maximum clients that can be connected is configurable in **```/lib/defaults.h```**.   
> ![running the server](/images/running_server.png "server running")   
> 1. Creating first client.
> ![creating first client](/images/make_1st_client.png "creating first client")   
> 1. Providing name of first client.
> ![name of first client](/images/giving_name_1st_user.png "name of first client")
> 1. Creating second client.
> ![creating second client](/images/make_2nd_client.png "creating second client")   
> 1. Giving a name to the second client and also trying out the **```whoami```** command. Now here a client named 'Ram' has already joined so, trying to give the name 'Ram' will fail and the application will try to provide a suggestion that may work or actually should work in most of the cases.
> ![name of second client](/images/giving_same_name_2nd_client.png "name of second client")   
> 1. Providing a unique name to the second client.
> ![joining second client](/images/2nd_client_joined.png "joining second client")   
> 1. Creating third and last client (but not giving a name to the client to test something).
> ![creating third and last client](/images/make_3rd_client.png "creating third client")   
> 1. Running **```whoarethey```** command from the other two connected users before the third client has given his name. The socket of the third client is connected to the server but in that case also as it has not given a name, it should not be displayed to other connected clients.
> ![whoarethey command 1](/images/whoarethey1.png "whoarethey command 1")
> 1. Running **```whoarethey```** command from the third client after joining the network with a name.
> ![whoarethey command 2](/images/whoarethey2.png "whoarethey command 2")
> 1. Trying to send a message without mentioning sender (from the third client).
> ![wrong message](/images/wrong_message.png "sending wrong message")
> 1. Running **```help```** command to see all the available commands (there are actually three at this moment).
> ![help command](/images/help1.png "running help command")
> 1. Sending a broadcast message (from the third client).
> ![broadcast message](/images/client_broadcast.png "broadcast message")
> 1. Sending specific messages (from second client to first and third client).
> ![specific messages](/images/specific_message.png "specific message")
> 1. **Here is an interesting scenario**. It shows what actually happens when a client receives a message at the time it is typing one. The third client John is being interrupted by the second client Sam. Here, the received message is displayed straight away. But does that mean the message that was being typed is discarded?
> ![interrupting in between typing message](/images/interrupting.png "interrupting in between typing message")
> 1. The answer to the previous question is NO. The third client John who was interrupted can continue typing the rest of the message because the previously typed message is saved in the standard input STDIN. The reason of the second client's message being displayed straight away while typing a current message is **Asynchronous I/O**. I think this is normal and this is actually what happens in the chat apps we use in our regular lives. The messsages are received continuously while we type our message. The difference is that here we do not have a fancy text box.
> ![after interruption](/images/after_interrupting.png "after interruption")
> 1. Disconnecting first client Ram.
> ![disconnecting](/images/disconnect1.png "disconnecting")
> 1. Disconnecting everyone else.
> ![disconnecting](/images/disconnect2.png "disconnecting")
