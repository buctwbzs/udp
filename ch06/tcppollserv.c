#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <poll.h>

#include "../lib/unp.h"

int main(int argc, char const *argv[])
{
    struct sockaddr_in servaddr, cliaddr;
    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    struct pollfd clients[OPEN_MAX];

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

    clients[0].fd = listenfd;
    clients[0].events = POLLRDNORM;
    for (int i = 1; i < OPEN_MAX; i++)
        clients[i].fd = -1;

    maxi = 0;

    for (;;)
    {
        if ((nready = poll(clients, maxi + 1, INFTIM)) < 0)
            err_sys("poll failed");
        printf("clients len: %d\n", sizeof(clients) / sizeof(*clients));
        if (clients[0].revents & POLLRDNORM)
        {
            // 接收新的连接
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
                err_sys("accept failed");

            for (i = 1; i < OPEN_MAX; i++)
            {
                if (clients[i].fd < 0)
                {
                    clients[i].fd = connfd;
                    break;
                }
            }

            printf("client %d come, fd: %d \n", i, clients[i].fd);

            if (i == OPEN_MAX)
                err_quit("too many clients");

            clients[i].events = POLLRDNORM;

            if (i > maxi)
                maxi = i;
            // 除了listenfd之外，如无就绪fd，则进入下个轮询
            if (--nready == 0)
                continue;
        }

        for (i = 1; i <= maxi; i++)
        {
            if ((sockfd = clients[i].fd) < 0)
                continue;

            printf("i: %d, sockfd: %d\n", i, sockfd);
            if (clients[i].revents & (POLLRDNORM | POLL_ERR))
            {
                if ((n = read(sockfd, buf, MAXLINE)) == 0)
                {
                    fputs("conn closed by client\n", stdout);
                    if (close(sockfd) < 0)
                        err_sys("close sockfd failed");
                    clients[i].fd = -1;
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
