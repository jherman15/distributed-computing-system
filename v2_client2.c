/* Distributed computing system - client 2
Bulanda Karol, Herman Jaroslaw, Rosol Jan*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n, num1, num2, tw=20, suma;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024], buffer1[1024];

    if (argc < 3)
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);					// TCP connection (IPv4)

    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");


        while(1){

                n = recv(sockfd, &buffer, 1024, 0);                         //receiving the first digit from the server
                if (n < 0)
                        error("ERROR reading from socket");
                recv(sockfd, &buffer, 1024, 0);
                printf("First digit: %s\n", buffer);

                n = recv(sockfd, &buffer1, 1024, 0);                         //receiving the second digit from server
                if (n < 0)
                        error("ERROR reading from socket");
                recv(sockfd, &buffer1, 1024, 0);
                printf("Second digit: %s\n", buffer1);

                if(strcmp(buffer, ":exit") == 0){
                        close(sockfd);
                        printf("[-]Disconnected from server.\n");
                        exit(1);
                }

                num1 = *buffer - '0';
                num2 = *buffer1 - '0';
                suma = num1+num2;                       //computing the result

                write(sockfd,&tw, sizeof(tw));		//sending tw=20 to the server, which is interpreted as error in computing

<<<<<<< HEAD
		sleep(5);
=======
                sleep(7);                              //simulating a broken connection
>>>>>>> 3d5bdf704c3d2211f28397bcd155c2dd573488f0

                write(sockfd,&suma, sizeof(suma));     //if the connection doesn't break, a correct result is sent back to server
         }

close(sockfd);
return 0;
}

