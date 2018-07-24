#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
//管道容量大小:读端不读且不关闭fd[0]即可
int main(){
    int fd[2];
    int ret = pipe(fd);

    if(ret==-1){
        perror("pipe error\n");
        return  1;
    }

    pid_t id = fork();

    if(id==0){
        close(fd[0]);
        int i = 1;
        char* child = "I am a child";
        printf("child node start\n");
        while (i++){
            write(fd[1],child,strlen(child)+1);
            printf("pipe capacity:%lu\n",i*(strlen(child)+1));
        }
        close(fd[1]);
        printf("child node end\n");

    } else if (id>0){
        printf("parent node start\n");
        close(fd[1]);
        waitpid(id,NULL,0);
        printf("parent node end\n");

    } else{
        perror("fork error\n");
        return 2;
    }
    return 0;
}
/*
 *
 * out put=>pipe capacity:65481
            pipe capacity:65494
            pipe capacity:65507
            pipe capacity:65520
            pipe capacity:65533
            pipe capacity:65546ls

所以说管道的最大容量是65546(每个人机器略有不同),osx
 tsy 65533
 *
 * */
