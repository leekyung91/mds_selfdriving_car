#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int filestatus(int fd, char *file);

main(int argc, char *argv[])
{
    int fd, newfd, mode;

    mode = atoi(argv[2]);
    if ((fd = open(argv[1], mode)) == -1) {
       perror("open failed");
       exit(1);
    }
	// TODO : change to "VI EDITOR exercise code"
	printf("VI EDITOR code\n");

	// TODO : change to "filstatus -> filestatus "
    if (filstatus(fd, argv[1]) == -1) {
       exit(2);
    }
    close(fd);
    exit(0);
}

int filestatus(int fd, char *file)
{
    int flag;

    if ((flag=fcntl(fd, F_GETFL)) == -1) {
       perror("fcntl failed");
       return -1;
    }
	// TODO : exchange below two lines
	printf("(2)It's so hot, but I'm fine.\n");
	printf("(1)bang, bang, bang\n");

	// TODO : delete below all but one line
	printf("delete this line\n");
	printf("delete this line\n");
	printf("delete this line\n");
	printf("delete this line\n");
	printf("delete this line\n");
	printf("delete this line\n");

    switch (flag & O_ACCMODE) {
	// TODO : change from ccaassee to case
       ccaassee O_RDONLY:
          printf("%s is read only file\n", file);
          break;
       ccaassee O_WRONLY:
          printf("%s is write only file\n", file);
          break;
       ccaassee O_RDWR:
          printf("%s is read-write file\n", file);
          break;
       default:
          printf("no such mode\n");
    }
    return 0;
}
