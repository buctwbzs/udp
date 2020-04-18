
一. 服务器正常启动(p101)

1.编译服务器及客户端程序

`
gcc ch05/tcpcserv01.c -o ch05/tcpcserv01 -Ilib -L. -lunp
`

`
gcc ch05/tcpcli01.c -o ch05/tcpcli01 -Ilib -L. -lunp
`

2.启动服务器端

`
./ch05/tcpcserv01
`

3.观察服务器套接字状态

`
netstat -a | grep 9877
`

可看到当前服务器套接字处于LISTEN状态

`
tcp        0      0 0.0.0.0:9877            0.0.0.0:*               LISTEN 
`

4.启动客户端

`
./ch05/tcpcli01 127.0.0.1
`

5.观察服务器及客户端状态

`
netstat -a | grep 9877
`

可看到当前服务器及客户端套接字状态


服务端主进程

`
tcp        0      0 0.0.0.0:9877            0.0.0.0:*               LISTEN
`     

服务端子进程

`
tcp        0      0 localhost:9877          localhost:46108         ESTABLISHED
`

客户端

`
tcp        0      0 localhost:46108         localhost:9877          ESTABLISHED
`

二. 客户端正常关闭

*ctrl+d*关闭客户端后执行继续查看

`
netstat -a | grep 9877
`

服务端主进程

`
tcp        0      0 0.0.0.0:9877            0.0.0.0:*               LISTEN
`


客户端

`
tcp        0      0 localhost:46108         localhost:9877       TIME_WAIT   
`

可以看到当前客户端套接字处于*TIME_WAIT*状态，该状态会持续2*MSL的时间。

注：
- TIME_WAIT状态(p37)
- MSL为最长分节生命周期(p37)。