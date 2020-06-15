#include "unp.h"
#include <time.h>

int main(int argc, char const *argv[])
{
    int sockfd, connfd;
    char buff[MAXLINE];
    time_t ticks;

    struct sockaddr_in sa;
    bzero(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(13);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_quit("socket error");

    if (bind(sockfd, (SA *)&sa, sizeof(sa)) < 0)
        err_quit("bind error");

    if (listen(sockfd, LISTENQ) < 0)
        err_quit("listen error");

    for (;;)
    {
        if ((connfd = accept(sockfd, (SA *)NULL, NULL)) < 0)
            err_quit("accept error");

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        if (write(connfd, buff, strlen(buff)) < 0)
            err_quit("write error");

        if (close(connfd) < 0)
            err_quit("close connfd error");
    }
    return 0;
}
