


## 协议无关性
`daytimetcpcli`,`daytimetcpsrv`程序若在IPv6下运行，需将serveaddr的类型从sockaddr_in改为sockaddr_in6

## 相关函数
    - bzero(vs memset)
    - htons
    - htonl
    - inet_pton


## unix 命令

- netstat -ni 提供网络接口信息
    
    - -n 以数值形式展示接口地址
- netstat -nr 展示路由表


- ifconfig etho 查看网络接口详细信息

- ping -b broadcast_addr 找出本地网络中的所有主机 