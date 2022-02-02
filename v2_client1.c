//v2_client1.c

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
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];
    if (argc < 3)
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

int suma;
int num1, num2;
char buffer1[1024];

	while(1){
	//	printf("Client: \t");
	//	scanf("%s", &buffer[0]);
	//	send(sockfd, buffer, strlen(buffer), 0);

                if(strcmp(buffer, ":exit") == 0){
			close(sockfd);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

                //for(int i=0; i<=1; i++){
                        n = recv(sockfd, buffer, 1024, 0);                         //Read Server String (num3)
                        if (n < 0)
                             error("ERROR reading from socket");

                        num1 = int(buffer) - 48;
 			num2 = int(buffer1) - 48;
                        suma = num1+num2;			//Send ans2 to server
		        write(sockfd,&suma, sizeof(int));     

close(sockfd);
return 0;
}
