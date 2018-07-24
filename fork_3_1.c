#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main(){
    int fd[2];
    int ret = pipe(fd);
    if(ret==-1){
        perror("pipe error\n");
        return 1;
    }
    pid_t id = fork();
    if(id==0){
        int i=0;
        close(fd[0]);
        char* child = "I am child!";
        while (i<10){
            write(fd[1],child, strlen(child) + 1);
            sleep(2);
            i++;
        }
    } else if(id>0){
        close(fd[1]);
        char msg[100];
        int status = 0;
        int j = 0 ;
        while (j<5){
            memset(msg,'\0', sizeof(msg));
            ssize_t  s = read(fd[0],msg, sizeof(msg));
            if(s>0){
                msg[s-1]= '\0';
            }
            printf("%s %d\n",msg,j);
            j++;
        }

        //写方法还在继续，而读方以及关闭它的读端
        close(fd[0]);
        // waitpid(pid_t pid,status,options);
        //1. pid>0只等待进程ID等于pid的子进程,不管是否已经有其他子进程结束退出,只要指定的子进程还没有结束,waitpid()就会一直等下去
        //2. pid=-1:等待任何一个子进程退出,此时和wait()作用一样
        //3. pid=0:等待其组ID等于调用进程的组ID的任意子进程
        //4. pid<-1:等待其组ID等于pid的绝对值的任意子进程
        // options WNOHANG 直接返回,0:阻塞父进程,等待子进程退出

        pid_t ret = waitpid(id,&status,0);
        printf("exit single(%d),exit(%d)\n",status&0xff,(status>>8)&0xff);
        //低八位存放该子进程退出时是否收到信号
        //此低八位子进程正常退出时,退出码是多少

    }else{
        perror("fork error\n");
        return 2;
    }
    return 0;

}
//使用kill -l 查看13号信号，可以知道13号信号代表SIGPIPE。
//总结：
//如果一个管道的写端一直在写，而读端的引⽤计数是否⼤于0决定管道是否会堵塞，引用计数大于0，只写不读再次调用write会导致管道堵塞；
//如果一个管道的读端一直在读，而写端的引⽤计数是否⼤于0决定管道是否会堵塞，引用计数大于0，只读不写再次调用read会导致管道堵塞；
//而当他们的引用计数等于0时，只写不读会导致写端的进程收到一个SIGPIPE信号，导致进程终止，只写不读会导致read返回0,就像读到⽂件末尾⼀样。