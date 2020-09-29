#include <netinet/in.h> // for struct sockaddr_in
#include <sys/socket.h> // socket func
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lib/unp.h"

int main(int argc, char **argv)
{
    int sockfd[5];
    struct sockaddr_in servaddr;

    if (argc != 2)
        exit(1);

    for (int i = 0; i < 5; i++)
    {
        if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            exit(1);

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

        if (connect(sockfd[i], (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
            exit(1);

        fputs("connect success", stdin);
    }

    str_cli(stdin, sockfd[0]);

    exit(0);
}