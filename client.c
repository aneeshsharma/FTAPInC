#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PORT 5000
#define BUFFER_SIZE 1024
#define CHUNK_SIZE 512

int main()
{
    int sock_fd = 0, recv_len;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};

    char input[100];

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

    while (1)
    {
        scanf("%s%*c", input);
        send(sock_fd, input, BUFFER_SIZE, 0);

        if (strcmp(input, "Bye") == 0)
            break;

        if (strcmp(input, "GivemeyourVideo") == 0)
        {
            int r = recv(sock_fd, buffer, 16, 0);
            int size;
            sscanf(buffer, "%d", &size);
            printf("Size : %d\n", size);
            int total = 0;
            FILE *file = fopen("file2.txt", "wb");
            while (total < size)
            {
                int chunk_size = size - total < CHUNK_SIZE ? size - total : CHUNK_SIZE;
                r = recv(sock_fd, buffer, chunk_size, 0);
                total += r;
                fwrite(buffer, sizeof(char), r, file);
                printf("Received : %d bytes\tTotal : %d\r", r, total);
                fflush(stdout);
            }
            fclose(file);
            printf("\n");
            continue;
        }

        int len = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        buffer[len] = 0;
        printf("%s\n", buffer);
    }

    close(sock_fd);
}