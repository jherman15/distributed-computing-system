#include        <sys/types.h>   /* basic system data types */
#include        <sys/socket.h>  /* basic socket definitions */
#include        <sys/time.h>    /* timeval{} for select() */
#include        <time.h>                /* timespec{} for pselect() */
#include        <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include        <arpa/inet.h>   /* inet(3) functions */
#include        <errno.h>
#include        <fcntl.h>               /* for nonblocking */
#include        <netdb.h>
#include        <signal.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <unistd.h>
#include        <sys/wait.h>
#include        <ctype.h>

void
sig_chld(int signo)                                                             //jh: SIGCHILD
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
     int sockfd, newsockfd, portno;
     int temp = 0;
     socklen_t clilen;
     char buffer[1024];
     char buffer1[1024];
     struct sockaddr_in serv_addr, cli_addr;
     void                            sig_chld(int);

     int n,ans;
     socklen_t addr_size;
     pid_t childpid;


#define SIGCHLD_                                                                //jh: SIGCHILD
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
      //signal(SIGCHLD, sig_chld);
      signal(SIGCHLD, SIG_IGN);



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


        while(1){
                newsockfd = accept(sockfd, (struct sockaddr*)&serv_addr, &addr_size);
                if(newsockfd < 0){
                        exit(1);
                }
                printf("Connection accepted from %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

int num1,num2;


                if((childpid = fork()) == 0){
                        close(sockfd);

                        while(1){
 
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

                                if(strcmp(buffer1, ":exit") == 0){
                                        printf("Disconnected from %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
                                        break;
                                }else{
                                        read(newsockfd, &ans, sizeof(int));
                                        if(ans == 20){
                                                for(int i=0; i<10; i++){
                                                        printf("Waiting...\n");
                                                        sleep(1);
                                                        temp++;
                                                        read(newsockfd, &ans, sizeof(int));
                                                        if(ans != 20) break;
                                                }
                                                if(temp == 10){
                                                        printf("Client not responding\n");
                                                        close(newsockfd);
                                                }
	                                        else{
        	                                        printf("The result is: %d\n", ans);
                	                        }//else
                                        }//if(ans==20)
					else{
                                                 printf("The result is: %d\n", ans);
                                        }//else

                                }//outer else

                        }// if(strcmp...)
                }// inner while
        }//outer while

     close(newsockfd);
     close(sockfd);
     return 0;
}//main
