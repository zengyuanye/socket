#进程是linux操作系统环境的基础
- fork系统调用,以及exec系列调用
- 避免或处理僵尸进程
- 进程间通信简单方式:管道
- 3种System V 进程间通信方式:信号量、消息队列和共享内存
- 进程间传递文件描述符:通过UNIX本地域socket传递特殊的辅助数据


*管道通信
    <img src="https://img-blog.csdn.net/20170509162502566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2t5cm9iZW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast"/>