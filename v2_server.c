#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[1024];
     char buffer1[1024];
     struct sockaddr_in serv_addr, cli_addr;

     int n,ans;
     socklen_t addr_size;
     pid_t childpid;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);                  //0 - TCP
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);                                    // argv[1] - contain the port number
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);                                          // The number gives the maximum limit of client at same time
     //newsockfd = accept(sockfd,
       //          (struct sockaddr *) &cli_addr,
         //        &clilen);
     //if (newsockfd < 0)
       //   error("ERROR on accept");


 //int num1, num2, num3, num4, ans, ans2;


        while(1){
                newsockfd = accept(sockfd, (struct sockaddr*)&serv_addr, &addr_size);
                if(newsockfd < 0){
                        exit(1);
                }
                printf("Connection accepted from %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
//@@@@@@@@@@@@@@@@@ CALCULATOR PART @@@@@@@@@@@@@@@@@@@@

//     int num1, num2, num3, num4, ans, ans2;

  //   printf("Enter number 1: ");                                        //first number (client2)
    // scanf("%d", &num1);
     //n = write(newsockfd, &num1, sizeof(int));
    // if (n < 0) error("ERROR writing to socket");

   //  printf("Enter number 2: ");                                        //second number (client2)
   //  scanf("%d", &num2);
   //  n = write(newsockfd, &num2, sizeof(int));
   //  if (n < 0) error("ERROR writing to socket");

  //   read(newsockfd, &ans, sizeof(int));
  //   printf("Ans is:%d\n", ans);

int num1,num2;


                if((childpid = fork()) == 0){
                        close(sockfd);

                        while(1){
                                //recv(newsockfd, buffer, 1024, 0);
                                //printf("Server: \t");

                                printf("Enter number 1: ");
                                scanf("%s", &buffer);
                                send(newsockfd, &buffer, strlen(buffer), 0);
                                n = write(newsockfd, &buffer, sizeof(num1));
                                if (n < 0) error("ERROR writing to socket");

                                printf("Enter number 2: ");                                        //second number (client2)
                                scanf("%s", &buffer1);
                                send(newsockfd, &buffer1, strlen(buffer1), 0);
                                n = write(newsockfd, &buffer1, sizeof(buffer1));
                                if (n < 0) error("ERROR writing to socket");

///                             send(newsockfd, buffer, strlen(buffer), 0);
                                if(strcmp(buffer1, ":exit") == 0){
                                        printf("Disconnected from %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
                                        break;
                                }else{
                                        read(newsockfd, &ans, sizeof(int));
                                        if(ans == 20){
                                                for(int i=0; i<5; i++){
                                                        printf("Waiting...\n");
                                                        sleep(1);
                                                        if(ans != 20) break;
                                                }
                                                close(newsockfd);
                                        }else{
                                                printf("The result is: %d\n", ans);
                                        }

                                }



                        }
                }
        }

     close(newsockfd);
     close(sockfd);
     return 0;
}

