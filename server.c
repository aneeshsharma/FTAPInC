#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>

#include "fileTransfer.c"

#define PORT 5000

#define BUFFER_SIZE 1024
#define MAX_CONN 100
#define CHUNK_SIZE 512
#define REQ_SIZE 50

void listdir(int sockfd)
{
    char buffer[BUFFER_SIZE];
    struct dirent *files;
    DIR *dir = opendir("files");
    if (dir == NULL)
    {
        printf("not exist\n");
        sprintf(buffer, "NO FILES");
        send(sockfd, buffer, BUFFER_SIZE, 0);
    }
    int count = 0;
    while ((files = readdir(dir)) != NULL)
    {
        count += sprintf(buffer + count, "%s\n", files->d_name);
    }
    closedir(dir);

    send(sockfd, buffer, BUFFER_SIZE, 0);
}

int main()
{
    int server_fd, conn_socket, recv_len;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    char request[REQ_SIZE];

    char name[REQ_SIZE], password[REQ_SIZE];

    char fname[REQ_SIZE], fpasswd[REQ_SIZE];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nError creating socket\n");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("couldn't bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((conn_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        printf("Error accepting connection!!!\n");
        return -1;
    }

    while (1)
    {
        int len = recv(conn_socket, request, REQ_SIZE, 0);
        if (strcmp(request, "START") == 0)
        {
            printf("Start!\n");
            break;
        }
    }

    recv_len = recv(conn_socket, name, REQ_SIZE, 0);

    recv_len = recv(conn_socket, password, REQ_SIZE, 0);

    FILE *passwd = fopen("passwd_list.txt", "r");

    while (fscanf(passwd, "%[^,]%*c%[^\n]%*c", fname, fpasswd) != EOF)
    {

        memset(buffer, 0, BUFFER_SIZE);
        if (strcmp(fname, name) == 0)
        {
            if (strcmp(fpasswd, password) == 0)
            {
                printf("User authenticated!\n");
                break;
            }
            else
            {
                printf("Incorrect password\n");
                sprintf(request, "WRONG");
                send(conn_socket, request, REQ_SIZE, 0);
                close(conn_socket);
                return -1;
            }
        }
    }
    sprintf(request, "DONE");
    send(conn_socket, request, REQ_SIZE, 0);

    while (1)
    {
        int len = recv(conn_socket, request, REQ_SIZE, 0);
        printf("Message received: %s\n", request);
        if (strcmp(request, "QUIT") == 0)
        {
            break;
        }

        if (strcmp(request, "GetFile") == 0)
        {
            char src[100];
            recv_len = recv(conn_socket, request, REQ_SIZE, 0);
            strcpy(src, "files/");
            strcat(src, request);
            printf("Getting %s\n", src);
            sendFile(conn_socket, src);
        }

        if (strcmp(request, "ListDir") == 0)
        {
            listdir(conn_socket);
        }

        if (strcmp(request, "StoreFile") == 0)
        {
            recv_len = recv(conn_socket, request, REQ_SIZE, 0);
            char dest[100];
            strcpy(dest, "files/");
            strcat(dest, request);
            receiveFile(conn_socket, dest);
        }

        if (strcmp(request, "CreateFile") == 0)
        {
            recv_len = recv(conn_socket, request, REQ_SIZE, 0);
            char dest[100];
            strcpy(dest, "files/");
            strcat(dest, request);
            FILE *temp = fopen(dest, "wb");
            fflush(temp);
            fclose(temp);
        }
    }
    close(conn_socket);
    close(server_fd);
    return 0;
}