# unix network programing
《unix network programings》书中示例及基于示例的实验。
示例实验步骤位于各章节*lab*。lab中涉及的的概念及关注的点会标注其在书中的页码。例如第五章的[lab](https://github.com/buctwbzs/unp/blob/master/ch05/lab.md)。
# examples
编译chapter05服务器程序

`
gcc ch05/tcpcserv01.c -o tcpcserv01 -Ilib -L. -lunp
`

编译chapter05客户端程序

`
gcc ch05/tcpcli01.c -o tcpcli01 -Ilib -L. -lunp
`

进行实验...
# contents:
- ch05：tcp回射客户/服务器程序
    - tcp回射客户/服务器程序
    - 信号处理
- ch06
    - IO复用:select和poll函数
- ch07
    - 套接字选项
- ch08
    - 基本UDP套接字编程
- ch11
    - 名字与地址转换

