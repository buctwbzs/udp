#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "../lib/unp.h"

int main(int argc, char const *argv[])
{
    struct sockaddr_in servaddr, cliaddr;
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, clients[FD_SETSIZE];
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    fd_set readset, allset;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket failed");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ((bind(listenfd, (struct sockaddr *)&servaddr, sizeof servaddr)) < 0)
        err_sys("bind failed");

    if (listen(listenfd, LISTENQ) < 0)
        err_sys("listen failed");

    maxfd = listenfd;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
        clients[i] = -1;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for (;;)
    {
        fputs("loop...\n", stdout);
        readset = allset;
        if ((nready = select(maxfd + 1, &readset, NULL, NULL, NULL)) < 0)
            err_sys("select failed");

        if (FD_ISSET(listenfd, &readset))
        {
            // 接收下新的连接
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
            {
                err_sys("accept failed");
            }
            else
            {
                for (i = 0; i < FD_SETSIZE; i++)
                {
                    if (clients[i] < 0)
                    {
                        clients[i] = connfd;
                        break;
                    }
                }
            }
            printf("client %d come\n", i);
            if (i == FD_SETSIZE)
                err_quit("too many clients");

            FD_SET(connfd, &allset);

            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)
                maxi = i;
            // 除了listenfd之外，如无就绪fd，则进入下个轮询
            if (--nready == 0)
                continue;
        }

        for (i = 0; i <= maxi; i++)
        {
            if ((sockfd = clients[i]) < 0)
                continue;

            printf("i: %d,sockfd: %d\n", i, sockfd);
            if (FD_ISSET(sockfd, &readset))
            {
                if ((n = read(sockfd, buf, MAXLINE)) == 0)
                {
                    fputs("conn closed by client\n", stdout);
                    if (close(sockfd) < 0)
                        err_sys("close sockfd failed");

                    FD_CLR(sockfd, &allset);
                    clients[i] = -1;
                }
                else
                {
                    if (write(fileno(stdout), buf, n) <= 0)
                        fputs("write to stdout failed\n", stdout);
                    if (write(sockfd, buf, n) <= 0)
                        fputs("write to client failed\n", stdout);
                }
                // 无就绪fd
                if (--nready <= 0)
                    break;
            }
        }
    }
    return 0;
}
