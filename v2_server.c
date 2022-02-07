/* Distributed computing system - server
Bulanda Karol, Herman Jaroslaw, Rosol Jan*/

#include        <sys/types.h>
#include        <sys/socket.h>
#include        <sys/time.h>
#include        <time.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>
#include        <errno.h>
#include        <fcntl.h>
#include        <netdb.h>
#include        <signal.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <unistd.h>
#include        <sys/wait.h>
#include        <ctype.h>

void
sig_chld(int signo)                                                             //SIGCHILD
{
        pid_t   pid;
        int             stat;

        while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
                printf("child %d terminated\n", pid);
        return;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, num1, num2;
     int temp = 0;
     socklen_t clilen;
     char buffer[1024];
     struct sockaddr_in serv_addr, cli_addr;
     void                     sig_chld(int);
     int n,ans;
     socklen_t addr_size;
     pid_t childpid;


#define SIGCHLD_                                                                //SIGCHILD
#ifdef SIGCHLD
    struct sigaction new_action, old_action;

  /* Set up the structure to specify the new action. */
    new_action.sa_handler = sig_chld;
    new_action.sa_handler = SIG_IGN;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;

    if( sigaction (SIGCHLD, &new_action, &old_action) < 0 ){
          fprintf(stderr,"sigaction error : %s\n", strerror(errno));
          return 1;
    }

#endif
      signal(SIGCHLD, SIG_IGN);



     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);                  //0 - TCP
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);                                    // argv[1] - port number
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);


        while(1){
                newsockfd = accept(sockfd, (struct sockaddr*)&serv_addr, &addr_size);
                if(newsockfd < 0){
                       exit(1);
                }
                printf("\nConnection accepted from %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));


                if((childpid = fork()) == 0){								//forking of processes (concurrent server)
                        close(sockfd);

                        while(1){

                                printf("Enter the first digit: ");                                       // first digit inserted by user
                                scanf("%s", &buffer);
                                send(newsockfd, &buffer, strlen(buffer), 0);
                                n = write(newsockfd, &buffer, sizeof(num1));
                                if (n < 0) error("ERROR writing to socket");

                                printf("Enter the second digit: ");                                        //second digit inserted by user
                                scanf("%s", &buffer);
                                send(newsockfd, &buffer, strlen(buffer), 0);
                                n = write(newsockfd, &buffer, sizeof(buffer));
                                if (n < 0) error("ERROR writing to socket");

                                if(strcmp(buffer, ":exit") == 0){
                                        printf("Disconnected from %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
                                        break;
                                }else{
                                        recv(newsockfd, &ans, sizeof(int),0);
                                        if(ans == 20){
                                                for(int i=0; i<10; i++){
                                                        printf("Waiting...\n");                  //the highest possible computed value is 18, the answer becomes 20 when connection breaks
                                                        sleep(1);
                                                        temp++;
                                                        recv(newsockfd, &ans, sizeof(int),0);
                                                        if(ans != 20){ 
								break;
							}
                                                }
                                                if(temp == 10){
                                                        printf("Client not responding. Closing the socket.\n");       //implementation of the exception: client disconnected from server
                                                        close(newsockfd);
                                                }
                                                else{
                                                        printf("The result is: %d\n", ans);
                                                }
                                        }
                                        else{
                                                 printf("The result is: %d\n", ans);
                                        }//else

                                }//outer else

                        }//inne while
                }//if((childpid==fork)
        }//outer while

     close(newsockfd);
     close(sockfd);
     return 0;
}

