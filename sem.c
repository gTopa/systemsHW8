#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union senum{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

int main(int argc, char *argv[]){
  int key=ftok("./sem.c",1);
  int semid;
  if (strcmp(argv[1],"-c")==0){
    semid=semget(key,1,IPC_CREAT|0644|IPC_EXCL);
    union senum su;
    su.val=1;
    semctl(semid,0,SETVAL,su);
    printf("%d\n",semid);
  }else if(strcmp(argv[1],"-v")==0){
    semid=semget(key,1,0);
    int val=semctl(semid,0,GETVAL);
    printf("value: %d\n", val);
  }else if(strcmp(argv[1],"-r")==0){
    semid=semget(key,1,0);
    semctl(semid,0,IPC_RMID);
  }
}
