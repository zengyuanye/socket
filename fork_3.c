#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
//fd[0]是读取,fd[1]是写入
int main(){
    //1.父进程创建管道,得到两个文件描述符指向管道的两端
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1) {
        perror("pipe error\n");
        return 1;
    }
    //2.父进程fork出子进程,子进程也有两个文件描述符指向同一管道;
    pid_t id = fork();//创建进程
    //3.父进程关闭fd[0],子进程关闭fd[1],即父进程关闭管道读端,子进程关闭管道写端(因为管道只支持单向通信).父进程可以往管道里写,子进程可以
            // 从管道里读，管道是用环型队列实现的，数据从写端流入从读端流程，这样就实现了进程间的通信
    if(id==0){
        //子进程写入
        int i =0;
        close(fd[0]);
        char *child = "I am child!";
        while (i<5){
            write(fd[1],child,strlen(child)+1);
            sleep(2);
            i++;
        }
    }else if(id>0){
        //父进程读取
        close(fd[1]);
        char msg[100];
        int j = 0;
        while (j<5){
            memset(msg,'\0', sizeof(msg));
            printf("read is blocking start\n");
            ssize_t s = read(fd[0],msg, sizeof(msg));
            printf("read is blocking end \n");
            if(s>0){
                msg[s-1] = '\0';
                printf("%s\n",msg);
                j++;
            }
        }
    }else{
        perror("fork error\n");
        return 2;
    }
    return 0;
}