#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MAXLINE 8192 /* Max text line length */

int open_clientfd(char* host, int port)
{
    int clientfd;
    struct sockaddr_in sa;
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }
    bzero((char *)&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, host, &(sa.sin_addr.s_addr));
    sa.sin_port = htons(port);
    if (connect(clientfd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        return -1;
    }
    return clientfd;
}

int comparator (const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "usage: %s <NETID> <port> <host>\n", argv[0]);
        return 1;
    }

    int clientfd;
    char *netid = argv[1];
    int port = atoi(argv[2]);
    char *host = argv[3];

    if ((clientfd = open_clientfd(host, port)) < 0) {
        return 2;
    }

    char sendbuf[MAXLINE];
    char recvbuf[MAXLINE];
    char status[100];
    char op[10];
    int nums[5];

    int len = sprintf(sendbuf, "cs230 HELLO %s\n", netid);
    ssize_t ns = send(clientfd, sendbuf, strlen(sendbuf), 0);

    int go = 0;
    int max = 2022;
    while (go++ <= max)
    {   

        ssize_t nr = recv(clientfd, recvbuf, MAXLINE, 0);

        char* token = strtok(recvbuf, " ");
        for (int i = 0; token != NULL; ++i) {
            if (i == 1) {
                strcpy(status, token);
            } else if (i == 2) {
                strcpy(op, token);
            } else {
                nums[i - 3] = atoi(token);
            }
            token = strtok(NULL, " ");
        }

        int result = 0;

        qsort(nums, 5, sizeof(int), comparator);
        if (strcmp(op, "min") == 0) {
            result = nums[0];
        } else if (strcmp(op, "max") == 0) {
            result = nums[4];
        } else if (strcmp(op, "median") == 0) {
            result = nums[2];
        } else {
            printf("flag: %s\n", status);
            break;
        }

        int len = sprintf(sendbuf, "cs230 %d\n", result);
        ssize_t ns = send(clientfd, sendbuf, len, 0);
        
    }
    close(clientfd);
    return 0;
}

// FLAG: d301e4a0b90b0fe9db6d59a7bcf90cf3705160af8b1c6ff3ca8937ec1af1c560
