#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#include "fileTransfer.c"

#define PORT 5000
#define BUFFER_SIZE 1024
#define CHUNK_SIZE 512
#define TIME_RANGE 2
#define REQ_SIZE 50

void listdir(int sockfd)
{
    char buffer[BUFFER_SIZE];
    int l = recv(sockfd, buffer, BUFFER_SIZE, 0);
    printf("\n-------------\n");
    printf("Files on Server - \n%s\n", buffer);
}

int main()
{
    int sock_fd = 0, recv_len;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    char request[REQ_SIZE];
    char name[REQ_SIZE], password[REQ_SIZE];

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error creating socket!!\n");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        printf("Invalid address\\Address not supported\n");
        return -1;
    }

    if (connect(sock_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("Connection failed!\n");
        return -1;
    }

    strcpy(request, "START");
    send(sock_fd, request, REQ_SIZE, 0);

    printf("Username : ");
    scanf("%s", name);
    printf("Password : ");
    scanf("%s", password);

    send(sock_fd, name, REQ_SIZE, 0);
    send(sock_fd, password, REQ_SIZE, 0);

    recv_len = recv(sock_fd, request, REQ_SIZE, 0);
    if (strcmp(request, "DONE") != 0)
    {
        printf("Incorrect password\n");
        close(sock_fd);
        return -1;
    }

    while (1)
    {
        scanf("%s%*c", request);
        send(sock_fd, request, REQ_SIZE, 0);

        if (strcmp(request, "QUIT") == 0)
            break;

        if (strcmp(request, "GetFile") == 0)
        {
            scanf("%s", request);
            send(sock_fd, request, REQ_SIZE, 0);
            receiveFile(sock_fd, request);
        }

        if (strcmp(request, "ListDir") == 0)
        {
            listdir(sock_fd);
        }

        if (strcmp(request, "StoreFile") == 0)
        {
            scanf("%s", request);
            send(sock_fd, request, REQ_SIZE, 0);
            sendFile(sock_fd, request);
        }
        if (strcmp(request, "CreateFile") == 0)
        {
            scanf("%s", request);
            send(sock_fd, request, REQ_SIZE, 0);
        }
    }

    close(sock_fd);
}