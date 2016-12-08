#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

union senum{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

void view(){
    int fd = open("story.txt",O_RDONLY);
    char story[1000];
    int len=read(fd,story,sizeof(story)-1);
    story[len]=0;
    close(fd);
    printf("%s", story);
}

int main(int argc, char *argv[]){
  int key=ftok("./sem.c",1);
  int semid;
  int shmid;
  if (strcmp(argv[1],"-c")==0){
    semid=semget(key,1,IPC_CREAT|0644|IPC_EXCL);

    union senum su;
    su.val=1;
    semctl(semid,0,SETVAL,su);
    
    shmid=shmget(ftok(".",12), sizeof(int), IPC_CREAT|0644|IPC_EXCL);

    int fd=open("./story.txt",O_CREAT|O_EXCL,0644);
    close(fd);
    
  }else if(strcmp(argv[1],"-v")==0){
    view();
  }else if(strcmp(argv[1],"-r")==0){
    semid=semget(key,1,0);
    semctl(semid,0,IPC_RMID);
    shmid=shmget(ftok(".",12),sizeof(int),0);
    shmctl(shmid,IPC_RMID);
    view();
  }
}
