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

int main(){
  int semid;
  int shmid;
  semid=semget(ftok(".",1), 1,0);
  
}
