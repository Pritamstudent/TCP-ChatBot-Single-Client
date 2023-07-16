#include<stdio.h>
#include<string.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
void error(const char *msg){
    perror(msg);//present in stderr
    exit(1);
}
int main(int argc, char *argv[]){
    int sockfd, portNo, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[255];

    if(argc<3){
        fprintf(stderr,"usage %s hostname port \n", argv[0]);
        exit(0);
    }
    portNo = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd<0){
        error("error opening socket\n");
    }
   // printf("yes");
    server = gethostbyname(argv[1]);
    if(server==NULL){
        fprintf(stderr,"error no such host");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portNo);
    if(connect(sockfd,(struct sockaddr *)&serv_addr , sizeof(serv_addr))<0){
        error("connection failed\n");
    }

    while(1){
        bzero(buffer,255);
        printf("Enter the message to send to server\n");
        fgets(buffer,255,stdin);
      //  printf("%s",buffer);
        n = send(sockfd,buffer,strlen(buffer),0);
        if(n<0){
            error("error on writing\n");
        }
        bzero(buffer,255);
        n = read(sockfd, buffer, 255);
        if(n<0){
            error("Error in reading");
        }
        printf("Server: %s",buffer);
        int i = strncmp("Bye",buffer,3);
        if(i==0){
            break;
        }
    }
    close(sockfd);
    return 0;
}
