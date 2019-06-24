#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

struct userInfo
{
	char name[28];
	int age;
	int sex;
	char hobby[28];
};

int main()
{
	int fd;
	struct userInfo myAddrBook;
	int dataSize;
	fd = open("hello.txt", O_RDONLY);
	if(fd<0)
	{
		perror("file open error");
		return 1;
	}
	
	dataSize = sizeof(myAddrBook);
	printf("User Info=================================\n\n");
	while(read(fd, (void*)&myAddrBook, dataSize) == dataSize)
	{
		printf("name : %s\n", myAddrBook.name);
		printf("name : %d\n", myAddrBook.age);
		printf("name : %d\n", myAddrBook.sex);
		printf("name : %s\n", myAddrBook.hobby);
		printf("======================================\n");
	}
	
	close(fd);
	return 0;
}
