#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>


#define PORT 3264
#define BACKLOG 10
#define BUF_SIZE 2048
#define PATH_LENGTH 1024
#define FILE_LENGTH 256
#define DIRECTORY "/home/kali/Desktop/L3 Rouen/Réseaux/tp8/files"

void handle_sigint();
int is_safe_filename(const char *filename) ;

int main(int argc, char* argv[]){

    signal(SIGINT, handle_sigint);

    if (argc !=2){
        fprintf(stderr,"[*] Usage: %s <directory>",argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock_fd;
    if ((sock_fd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("[-] socket_creation_error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family=AF_INET; 
    server_addr.sin_port=htons(PORT);
    server_addr.sin_addr.s_addr=INADDR_ANY;

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("[-] bind_failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, BACKLOG)==-1){
        perror("[-] listen_failed");
        exit(EXIT_FAILURE);
    }
    
    printf("[+] Server listening on port %d\n",PORT);

    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);    

    while (1){
        if ((client_fd = accept(sock_fd,(struct sockaddr*)&client_addr,&client_len))== -1){
            perror("[-] accept_failed");
            exit(EXIT_FAILURE);
        }

        printf("[+] Connection accepted from %s/%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        char window_size_str[BUF_SIZE];
        char filename[FILE_LENGTH];
        char error_message[BUF_SIZE];

        recv(client_fd,window_size_str,BUF_SIZE,0);
        recv(client_fd,filename,sizeof(filename),0);

        int window_size = atoi(window_size_str);
        printf("[*] Window size: %d\n", window_size);

        if (is_safe_filename(filename)!=0) {
            fprintf(stderr, "[!] Nom de fichier non sécurisé\n");
            snprintf(error_message, BUF_SIZE, "[!] Nom de fichier non sécurisé : %s\n", filename);
            send(client_fd, error_message, strlen(error_message), 0);
            exit(EXIT_FAILURE);
        }
        printf("[*] Filename: %s\n", filename);

        char filepath[PATH_LENGTH];
        size_t filepath_len = (size_t)snprintf(filepath, sizeof(filepath), "%s/%s", DIRECTORY, filename);
        if (filepath_len >= BUFSIZ) {
            fprintf(stderr, "[-] File path is too long\n");
            exit(EXIT_FAILURE);
        }

        FILE *f = fopen(filepath, "rb");
        if (f == NULL) {
            
            if (errno == ENOENT) {
                snprintf(error_message, BUF_SIZE, "[-] fopen_failed: File '%s' does not exist\n", filename);
            } else if (errno == EACCES) {
                snprintf(error_message, BUF_SIZE, "[-] fopen_failed: Permission denied for file '%s'\n", filename);
            } else {
                snprintf(error_message, BUF_SIZE, "[-] fopen_failed: Unable to open file '%s'. Error code: %d\n", filename, errno);
            }
            send(client_fd, error_message, strlen(error_message), 0);
            perror("[-] fopen_failed");
            
            close(client_fd);
            continue;        
        }
        char buffer[BUF_SIZE];
        size_t bytes_read;

        while ((bytes_read=fread(buffer,1,sizeof(buffer),f)) >0){
            send(client_fd,buffer,bytes_read,0);
        }
        fclose(f);
        close(client_fd);
    }
    close(sock_fd);
    return EXIT_SUCCESS;
}

int is_safe_filename(const char *filename) {
    size_t len = strlen(filename);
    for (size_t i = 0; i < len-1; ++i) {
        // Vérifier si le caractère est un caractère de contrôle ou un caractère de saut de ligne
        if (iscntrl(filename[i])) {
            return -1; 
        }
    }
    if (strstr(filename, "..") != NULL) {
        return -1;
    }
    return 0;
}


void handle_sigint() {
    printf("\n[*] Received SIGINT. Closing server...\n");
    exit(EXIT_SUCCESS);
}