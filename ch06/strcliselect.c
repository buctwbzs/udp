#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>

#include "../lib/unp.h"

void str_cli(FILE *fp, int sockfd)
{
    int maxfdp1, stdinof = 0, n, nready;
    fd_set read_set;
    char buf[MAXLINE];

    FD_ZERO(&read_set);

    for (;;)
    {
        fputs("loop...", stdout);
        if (stdinof == 0)
            FD_SET(fileno(fp), &read_set);

        FD_SET(sockfd, &read_set);
        maxfdp1 = max(sockfd, fileno(fp)) + 1;

        if ((nready = select(maxfdp1, &read_set, NULL, NULL, NULL)) < 0)
            err_sys("select failed");

        if (FD_ISSET(sockfd, &read_set))
        {
            if ((n = read(sockfd, buf, sizeof buf)) == 0)
                if (stdinof == 1)
                    return;
                else
                    err_quit("read from sockfd failed");

            if (write(fileno(stdout), buf, n) == 0)
                err_sys("read from sockfd failed");
        }

        if (FD_ISSET(fileno(fp), &read_set))
        {
            if ((n = read(fileno(fp), buf, MAXLINE)) == 0)
            {
                stdinof = 1;
                if (shutdown(sockfd, SHUT_WR) < 0)
                    err_sys("shutdown socket failed");
                FD_CLR(fileno(fp), &read_set);
                continue;
            }
            if (write(sockfd, buf, n) < 0)
                err_sys("write to socket failed");
        }
    }
}
