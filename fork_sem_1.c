#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <assert.h>
#define PATHNAME "."
#define PROJ_ID 0
//union semun{
//    int val;
//    struct semid_ds* buf;//semget创建成功的时候会创建(权限,信号量数目,操作记录)
//    unsigned short int* array;
//    struct seminfo* __buf;//用于IPC_INFO命令
//};
//struct sembuf {
//    unsigned short	sem_num;	/* [XSI] semaphore # */
//    short		sem_op;		/* [XSI] semaphore operation */
//    short		sem_flg;	/* [XSI] operation flags */
//};
static int commSemid(int nums,int flags){
    key_t _key = ftok(PATHNAME,PROJ_ID);
    if(_key>0){
        return semget(_key,nums,flags);

    } else{
        perror("comsemid");
        return  -1;
    }
}
int createSemid(int nums){
    return commSemid(nums,IPC_CREAT|0666);
}

int getSemid(int nums){
    return commSemid(nums,IPC_CREAT);
}

int initSem(int semid,int _val){
    union semun _semun;
    _semun.val = _val;
    if(semctl(semid,0,SETVAL,_semun)<0){
        perror("initSem");
        return -1;
    }
    return 0;
}
static int pv(int sem_id,  short op){
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = op;
    sem_b.sem_flg = SEM_UNDO;
    return semop(sem_id,&sem_b,1);
}


int P(int semid, short _op){
    if(pv(semid,_op)<0){
        return  -1;
    }
    return 0;
}
int V(int semid, short _op){
    if(pv(semid,_op)<0){
        return  -1;
    }
    return 0;
}

int main(int argc,char*argv[]){
    int semid = createSemid(1);
    printf("%d\n",semid);
    //初始化信号量
    int init = initSem(semid,1);
    assert(init!=-1);
    pid_t id = fork();
    if(id==0){
        semid = getSemid(0);
        int i =100;
        while (i){
            P(semid,-1);
            printf("A");
            fflush(stdout);
            usleep(10000);
            printf("A");
            fflush(stdout);
            usleep(20000);
            V(semid,1);
            i--;
            usleep(20000);

        }
    }else{
        int j =100;
        while (j){
            P(semid,-1);
            usleep(30000);
            printf("B");
            fflush(stdout);
            usleep(8000);
            printf("B");
            fflush(stdout);
            usleep(20000);
            V(semid,1);
            j--;
            usleep(20000);
        }
        if(waitpid(id,NULL,0)<0){
            perror("waitpid failed!\n");
            return  -1;
        }

    }

    semctl(semid,0,IPC_RMID);
    return 0;

}