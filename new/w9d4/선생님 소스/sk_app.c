/***************************************
 * Filename: sk_app.c
 * Title: Skeleton Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

void usersignal(int sig);
int count=0;

int main(void)
{
    int retn;
    int fd;
    pid_t id;

    // char buf[100] = "write...\n";
    char buf[100] = {0};
(void)signal(SIGUSR1, usersignal);

id = getpid();
  printf("\n\n\n\n\n\n\n\n");
  printf("\n [APP] = %d", id);
 printf("\n\n\n\n\n\n\n\n\n");
    
    fd = open("/dev/sk", O_RDWR);
    printf("fd = %d\n", fd);
    
    if (fd<0) {
        perror("/dev/SK error");
        exit(-1);
    }
    else
        printf("SK has been detected...\n");
    
    retn = write(fd, &id, 4);
    
    retn = read(fd, buf, 20); // fd�� ����Ű�� ���Ͽ� buf���� 20byte ����
    printf("\ndata : %s\n", buf);

    pause();

    close(fd);
    
    return 0;
}


void usersignal(int sig)
{
   printf("\n sig...%d",count++);
}
