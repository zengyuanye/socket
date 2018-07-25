# 进程是linux操作系统环境的基础
- fork系统调用,以及exec系列调用
- 避免或处理僵尸进程
- 进程间通信简单方式:管道
- 3种System V 进程间通信方式:信号量、消息队列和共享内存
- 进程间传递文件描述符:通过UNIX本地域socket传递特殊的辅助数据

## 管道通信

* 管道的特点
    1.管道只允许具有主从关系的进程间通信,如父子进程间的通信
    2.管道只允许单向通信
    3.管道内部保证同步机制,从而保证访问数据的一致性
    4.面向字节流
    5.管道随进程,进程在管道在,进程消失,管道也消失
    ![image](https://github.com/zengyuanye/socket/blob/master/img/83BF97A4-0D22-433E-849F-AEEAC8E9BCB4.png)
## 信号量

   #### 不传送数据,保护共享资源使用信号量(本质上是一个计数器,原子操作),而不使用全局变量(费原子操作)
   #### 信号量有两种操作等待和发送信号,即P(sv)和V(sv),他们的行为是这样的
     1.P(sv):如果sv的值大于零,就给它减1;如果它的值为零,就挂起该进程的执行
     2.V(sv):如果有其他进程因等待sv而被挂起,就让它恢复运行,如果没有进程因等待sv而挂起,就给它加1
   #### 二元信号量是最简单的一种锁(互斥锁),它只用两种状态:占用与非占用.所以它的引用次数为1
   #### 进程获取共享资源
     1.测试控制该资源的信号量
     2.信号量的值为正,进程获得该资源的使用权,进程将信号量减1，表示它使用一个资源单位
     3.若此时信号量的值为0，则进程如进入挂起状态(进程状态改变),知道信号量的值大于0,若进程被唤醒则返回至第一步。   

