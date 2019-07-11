#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>

void mySigHdlr(int signo)
{
  if (signo == SIGUSR1) {
    printf("received SIGUSR1\n");
  }
}

int main(void)
{
  pid_t pid;
  int fd;
  int ret;

  pid=getpid();
  fd=open("/dev/signal",O_RDWR);

  ret = write(fd, &pid, 4);
  printf("fd=%d\n",ret);
  
  if (signal(SIGUSR1, mySigHdlr) == SIG_ERR) {
    printf("\ncan't catch SIGINT\n");
  }
  while(1) {
    sleep(1);
  }
  return 0;
}
