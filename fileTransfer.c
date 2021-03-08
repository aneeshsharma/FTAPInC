#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#define PORT 5000
#define BUFFER_SIZE 1024
#define CHUNK_SIZE 512
#define TIME_RANGE 2

void receiveFile(int sock_fd, char *filename)
{
    char buffer[BUFFER_SIZE];
    int r = recv(sock_fd, buffer, 16, 0);
    int size;
    sscanf(buffer, "%d", &size);
    printf("Size : %d\n", size);
    int total = 0;
    FILE *file = fopen(filename, "wb");

    struct timeval prev_time, curr_time, start_time;

    gettimeofday(&prev_time, NULL);
    gettimeofday(&start_time, NULL);
    float speed = 0;
    int sample = 0;
    while (total < size)
    {
        int chunk_size = size - total < CHUNK_SIZE ? size - total : CHUNK_SIZE;
        r = recv(sock_fd, buffer, chunk_size, 0);
        total += r;
        fwrite(buffer, sizeof(char), r, file);
        gettimeofday(&curr_time, NULL);

        int seconds = curr_time.tv_sec - prev_time.tv_sec;
        int usec = curr_time.tv_usec - prev_time.tv_usec;

        float time = seconds + ((float)usec) / 1000000;
        float c_time = (curr_time.tv_sec - start_time.tv_sec) + (float)(curr_time.tv_usec - start_time.tv_usec) / 1000000;

        sample += r;

        if (time >= 0.1)
        {
            speed = sample / (time * 1000);
            prev_time.tv_sec = curr_time.tv_sec;
            prev_time.tv_usec = curr_time.tv_usec;
            sample = 0;
        }

        printf("Packet: %4d bytes | Total: %9d KB | Speed: %6.2f KB/s\r", r, total / 1000, speed);
        fflush(stdout);
    }
    fclose(file);
    printf("\n");
    fflush(stdout);
}

void sendFile(int sock_fd, char *filename)
{
    char buffer[1024];
    printf("Staring file transfer....\n");
    FILE *file = fopen(filename, "rb");
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("Size : %d Bytes\n", size);

    char chunk[CHUNK_SIZE];
    sprintf(buffer, "%016d", size);
    send(sock_fd, buffer, 16, 0);
    int total = 0;
    while (total < size)
    {
        int chunk_size = (size - total < CHUNK_SIZE) ? size - total : CHUNK_SIZE;
        int r = fread(chunk, sizeof(char), chunk_size, file);

        int sent = send(sock_fd, chunk, chunk_size, 0);
        total += sent;
        printf("Sent: %d B\tTotal : %d KB\r", sent, total / 1000);
        fflush(stdout);
        fseek(file, sent - r, SEEK_CUR);
    }
    fclose(file);
    printf("\n");
    fflush(stdout);
}
