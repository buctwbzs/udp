#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../lib/unp.h"

int main(int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    void sig_chld(int);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit(1);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof servaddr) < 0)
        exit(1);

    if (listen(listenfd, LISTENQ) < 0)
        exit(1);

    Signal(SIGCHLD, sig_chld);

    for (;;)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
        {
            if (errno == EINTR)
                continue;
            else
                exit(1);
        }

        if ((childpid = fork()) == 0)
        {
            if (close(listenfd) < 0)
                exit(1);

            str_echo(connfd);
            exit(0);
        }
        if (close(connfd) < 0)
            exit(1);
    }
}