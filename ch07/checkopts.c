
#include <netinet/tcp.h>
#include <netinet/ip.h>
#ifdef HAVE_NETINET_SCTP_H
#include <netinet/sctp.h>
#endif
#include <netinet6/in6.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "../lib/unp.h"

union val
{
    int i_val;
    long l_val;
    struct linger linger_val;
    struct timeval timeval_val;
} val;

static char *sock_str_flag(union val *, int);
static char *sock_str_int(union val *, int);
static char *sock_str_linger(union val *, int);
static char *sock_str_timeval(union val *, int);

struct sock_opts
{
    const char *opt_str;
    int opt_level;
    int opt_name;
    char *(*opt_val_str)(union val *, int);
} sock_opts[] = {
    // 禁止活着开启发送广播消息的能力
    {"SO_BROADCAST", SOL_SOCKET, SO_BROADCAST, sock_str_flag},
    // 开启debug模式，可使用trpt程序进行检查
    {"SO_DEBUG", SOL_SOCKET, SO_DEBUG, sock_str_flag},
    // 绕过底层协议的路由机制
    {"SO_DONTROUTE", SOL_SOCKET, SO_DONTROUTE, sock_str_flag},
    {"SO_ERROR", SOL_SOCKET, SO_ERROR, sock_str_int},
    // 设置存活选项，每两小时发送存活探测分节，可关闭半开连接
    {"SO_KEEPALIVE", SOL_SOCKET, SO_KEEPALIVE, sock_str_flag},
    {"SO_LINGER", SOL_SOCKET, SO_LINGER, sock_str_linger},
    {"SO_OOBINLINE", SOL_SOCKET, SO_OOBINLINE, sock_str_flag},
    {"SO_RCVBUF", SOL_SOCKET, SO_RCVBUF, sock_str_int},
    {"SO_SNDBUF", SOL_SOCKET, SO_SNDBUF, sock_str_int},
    {"SO_RCVLOWAT", SOL_SOCKET, SO_RCVLOWAT, sock_str_int},
    {"SO_SNDLOWAT", SOL_SOCKET, SO_SNDLOWAT, sock_str_int},
    {"SO_RCVTIMEO", SOL_SOCKET, SO_RCVTIMEO, sock_str_timeval},
    {"SO_SNDTIMEO", SOL_SOCKET, SO_SNDTIMEO, sock_str_timeval},
    {"SO_REUSEADDR", SOL_SOCKET, SO_REUSEADDR, sock_str_flag},
    {"SO_REUSEPORT", SOL_SOCKET, SO_REUSEPORT, sock_str_flag},
    {"SO_TYPE", SOL_SOCKET, SO_TYPE, sock_str_int},
    {"SO_USELOOPBACK", SOL_SOCKET, SO_USELOOPBACK, sock_str_flag},
    {"IP_TOS", IPPROTO_IP, IP_TOS, sock_str_int},
    {"IP_TTL", IPPROTO_IP, IP_TTL, sock_str_int},
    {"IPV6_DONTFRAG", IPPROTO_IPV6, IPV6_DONTFRAG, sock_str_flag},
    {"IPV6_UNICAST_HOPS", IPPROTO_IPV6, IPV6_UNICAST_HOPS, sock_str_int},
    {"IPV6_V6ONLY", IPPROTO_IPV6, IPV6_V6ONLY, sock_str_flag},
    //  tcp 最大分节
    {"TCP_MAXSEG", IPPROTO_TCP, TCP_MAXSEG, sock_str_int},
    //  开启本选项将禁止Nagle算法
    {"TCP_NODELAY", IPPROTO_TCP, TCP_NODELAY, sock_str_flag},
#ifdef HAVE_NETINET_SCTP_H
    {"SCTP_AUTOCLOSE", IPPROTO_SCTP, SCTP_AUTOCLOSE, sock_str_int},
    {"SCTP_MAXBURST", IPPROTO_SCTP, SCTP_MAXBURST, sock_str_int},
    {"SCTP_MAXSEG", IPPROTO_SCTP, SCTP_MAXSEG, sock_str_int},
    {"SCTP_NODEALY", IPPROTO_SCTP, SCTP_NODEALY, sock_str_flag},
#endif

};

int main(int argc, char const *argv[])
{
    int fd;
    socklen_t len;
    struct sock_opts *ptr;

    for (ptr = sock_opts; ptr->opt_str != NULL; ptr++)
    {
        printf("%s: ", ptr->opt_str);
        if (ptr->opt_val_str == NULL)
        {
            printf("(undefined)\n");
        }
        else
        {
            switch (ptr->opt_level)
            {
            case SOL_SOCKET:
            case IPPROTO_IP:
            case IPPROTO_TCP:
                fd = socket(AF_INET, SOCK_STREAM, 0);
                break;
#ifdef IPV6
            case IPPROTO_IPV6:
                fd = socket(AF_INET6, SOCK_STREAM, 0);
#endif
#ifdef IPPROTO_SCTP
            case IPPROTO_SCTP:
                fd = socket(AF_INET, SOCK_SEQPACKET, 0);
#endif

            default:
                err_quit("can't create fd for level %d\n", ptr->opt_level);
            }
            len = sizeof(val);
            if (getsockopt(fd, ptr->opt_level, ptr->opt_name, &val, &len) == -1)
            {
                err_ret("");
            }
            else
            {
                printf("default = %s\n", (*ptr->opt_val_str)(&val, len));
            }
            close(fd);
        }
    }

    return 0;
}

static char strres[128];
static char *sock_str_flag(union val *ptr, int len)
{
    if (len != sizeof(int))
        snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
    else
        snprintf(strres, sizeof(strres), "%s", (ptr->i_val == 0) ? "off" : "on");
    return (strres);
}
static char *sock_str_int(union val *ptr, int len)
{
    if (len != sizeof(int))
        snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
    else
        snprintf(strres, sizeof(strres), "%d", ptr->i_val);
    return (strres);
}
static char *sock_str_linger(union val *ptr, int len)
{
    if (len != sizeof(struct linger))
        snprintf(strres, sizeof(strres), "size (%d) not sizeof(linger)", len);
    else
        snprintf(strres, sizeof(strres), "onoff: %d, linger: %d", ptr->linger_val.l_onoff, ptr->linger_val.l_linger);
    return (strres);
}

static char *sock_str_timeval(union val *ptr, int len)
{
    if (len != sizeof(struct timeval))
        snprintf(strres, sizeof(strres), "size (%d) not sizeof(linger)", len);
    else
        snprintf(strres, sizeof(strres), "tv_sec: %d, tv_usec: %d", ptr->timeval_val.tv_sec, ptr->timeval_val.tv_usec);
    return (strres);
}