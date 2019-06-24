#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#define SHMSZ 27

char SEM_NAME[]= "vik";
int main()
{
	char ch;
	int shmid;
	key_t key;
	char *shm,*s;
	sem_t *mutex;	//name the shared memory segment
	
	key = 1000;

	//세마포어생성초기화
	mutex = sem_open(SEM_NAME,0,0644,0);
	if(mutex == SEM_FAILED){
		perror("reader:unable to execute semaphore");
		sem_close(mutex);
		exit(-1);
	}
	//이key를사용하여공유메모리생성
	shmid = shmget(key,SHMSZ,0666);
	if(shmid<0){
		perror("reader:failure in shmget");
		exit(-1);
	}
	//이조각을가상메모리에덧붙인다.
	shm = shmat(shmid,NULL,0);
	//start reading
	s = shm;
	for(s=shm;*s!=0;s++){
		sem_wait(mutex);
		putchar(*s);
		sem_post(mutex);
	}
	//다른세마포어로대체가능
	*shm = '*';
	sem_close(mutex);
	shmctl(shmid, IPC_RMID, 0);
	exit(0);
}
