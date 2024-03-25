#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>


#define PORT 3264
#define SERVER_IP "10.0.2.15"


int main(int argc, char* argv[]){
    if (argc !=3){
        fprintf(stderr,"[*] Usage: %s <window_size> <filename>",argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int sock_fd;
    if ((sock_fd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("[-] socket_creation_error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr_server;
    addr_server.sin_family=AF_INET;
    addr_server.sin_port=htons(PORT);
    inet_pton(AF_INET,SERVER_IP, &addr_server.sin_addr);

    if (connect(sock_fd, (struct sockaddr*)&addr_server, sizeof(addr_server))==-1){
        perror("[-] Connection error");
        exit(EXIT_FAILURE);
    }
    printf("[+] Connected to server....\n");

    int window_size = atoi(argv[1]);
    char* filename =argv[2];

    send(sock_fd,argv[1],strlen(argv[1])+1,0);
    send(sock_fd, filename, strlen(filename) + 1, 0);
    printf("[*] filename to send:%s\n",filename);



    char buf[window_size];
    buf[window_size-1]=0;

    ssize_t bytes;

    while ((bytes=recv(sock_fd,buf,sizeof(buf)-1,0)) > 0){
        printf("[+] Contenu du fichier recu:\n");
        fwrite(buf, 1, (size_t)bytes, stdout);
    }

    close(sock_fd);

    return EXIT_SUCCESS;
}