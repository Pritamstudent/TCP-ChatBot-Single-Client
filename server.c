#include<stdio.h>
#include<string.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
int count = 0;
void error(const char *msg){
    perror(msg);//present in stderr
    exit(1);
}
int main(int argc, char *argv[]){
    if(argc!=2)//checks if no port number is given
    {
        fprintf(stderr,"Port Number is not provided\n");
        exit(1);
    }
    int sockfd, newsockfd, portNo, n;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //0 is used for TCP connection
    if(sockfd<0){
        error("Error in opening the socket\n");
        exit(1);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portNo = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNo);
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("Binding error\n");
    }
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd<0){
        error("Error on the Accept\n");
    }
    while(1){
        bzero(buffer,250);
        n = read(newsockfd, buffer, 255);
        if(n<0){
            error("Error on reading\n");
        }
        
      //  buffer[bytes_read]='\0';
        printf("Client: %s\n",buffer);
        bzero(buffer,255);
       fgets(buffer,255,stdin);
     //   n = write(newsockfd, buffer, strlen(buffer));
        n = send(newsockfd, buffer, strlen(buffer),0);
        if(n<0){
            error("Error on writing\n");
        }
        int i = strncmp("Bye",buffer,3);
        if(i==0)
        break;
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
