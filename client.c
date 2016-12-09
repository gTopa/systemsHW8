#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

union senum{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

void view(){
  int shmid;
  shmid=shmget(ftok(".",12),sizeof(int),0);
  
  int ptr;
  shmat(shmid,&ptr,SHM_RDONLY);
  printf("pointer: %d\n", ptr);

  int fd = open("story.txt",O_RDONLY);
  char story[1000];
  lseek(fd,-1*ptr,SEEK_END);
  int len=read(fd,story,sizeof(story)-1);
  story[len]=0;
  
  shmdt(&ptr);
  
  close(fd);
  printf("%s", story);
}

void add(){
  char buff[1000];
  fgets(buff,sizeof(buff),stdin);

  int fd=open("story.txt",O_WRONLY|O_APPEND,0);

  write(fd,buff,sizeof(buff));
  close(fd);

  int shmid;
  shmid=shmget(ftok(".",12),sizeof(int),0);
  
  int ptr;
  shmat(shmid,&ptr,SHM_RDONLY);
  printf("len of buff: %d\n", strlen(buff));
  ptr=strlen(buff);

  shmdt(&ptr);
}

int main(){
  printf("Waiting...\n");
  int semid;
  semid=semget(ftok(".",1),1,0);
  
  struct sembuf sb;
  sb.sem_flg=SEM_UNDO;
  sb.sem_num=0;
  sb.sem_op=-1;

  semop(semid,&sb,1);

  printf("Here is the last line of the story:\n");
  view();

  printf("\n\nAdd to story (at most 999 characters):\n");
  add();

  sb.sem_op=1;
  semop(semid,&sb,1);
}
