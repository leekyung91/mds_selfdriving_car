#include <semaphore.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>

const char *countFile = "count.db";
int main(int argc, char **argv)
{
    int count = 100;
    int fd;
    sem_t *mysem;
    sem_unlink("mysem");
    if((mysem = sem_open("mysem", O_CREAT, 0777, 1)) == NULL)
    {
        perror("Sem Open Error");
        return 1;
    }
    if( (fd = open(countFile,O_CREAT|O_RDWR, S_IRUSR|S_IWUSR )) == -1)
    {
        perror("Open Error\n");
        return 1;
    }
    while(1)
    {
    sem_wait(mysem);
//        lseek(fd, 0, SEEK_SET);
//        read(fd, (void *)&count, sizeof(count));
//        printf("Read Data %d\n",count);

        printf("producer: %d\n",count++);
        lseek(fd, 0, SEEK_SET);
        write(fd, (void *)&count, sizeof(count));

    sem_post(mysem);
        sleep(1); // stop consumer from getting semaphore
    }
    close(fd);
}

