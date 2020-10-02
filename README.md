# unix network programing
《unix network programings》知识总结及实验。

不考虑SCTP相关😫

为熟悉函数、类型，示例直接使用系统调用、库函数，而不使用`unp.h`中封装后的系统调用、库函数。

使用`unp.h`中提供的常量及`err_*`等辅助函数。


# contents

## ch01 简介
## ch02 传输层：TCP、UDP和SCTP
### 连接
### TCP终止
### time_wait
### 状态图

## ch03 套接字编程简介
### socket编程地址参数及辅助函数

## ch04 基本套接字编程
### socket编程相关函数

## ch05 tcp回射客户/服务器程序
### 多进程阻塞I/O服务

## ch06 I/O复用:select和poll函数
#### 5种IO模型
#### select函数，单进程I/O复用服务及客户端
#### poll函数，单进程I/O复用服务

## ch07 套接字选项
### getsockopt
### setsockopt
### fcntl



