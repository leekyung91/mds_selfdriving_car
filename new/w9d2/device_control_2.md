# 2일차

## open.c
```c
/***************************************

 * Filename: sk.c

 * Title: Skeleton Device

 * Desc: Implementation of system call

 ***************************************/

#include <linux/module.h>

#include <linux/init.h>

#include <linux/major.h>

#include <linux/fs.h>

#include <linux/cdev.h>



MODULE_LICENSE("GPL");



static int sk_major = 0, sk_minor = 0;

static int result;

static dev_t sk_dev;



static struct cdev sk_cdev;



static int sk_register_cdev(void);



/* TODO: Define Prototype of functions */

static int sk_open(struct inode *inode, struct file *filp);

static int sk_release(struct inode *inode, struct file *filp);



/* TODO: Implementation of functions */
// int (*open)(struct indoe * , struct fild *)과 같은 형태 
static int sk_open(struct inode *inode, struct file *filp) // 유저가 짠 것

{

    printk("Device has been opened...\n");

    

    /* H/W Initalization */

    

    //MOD_INC_USE_COUNT;  /* for kernel 2.4 */

    

    return 0;

}



static int sk_release(struct inode *inode, struct file *filp)

{

    printk("Device has been closed...\n");

    

    return 0;

}


// 제일 중요한 포인트!!!
// sk_fops 부분이 tag명 
struct file_operations sk_fops = { 

    .open       = sk_open,
    // .open은 함수 포인터..? sk_open을 연결
    // int (*open)(struct indoe * , )
    .release    = sk_release,

};



static int __init sk_init(void)

{

    printk("SK Module is up... \n");


    // sk_register_cdev는 유저 함수
	if((result = sk_register_cdev()) < 0)

	{

		return result;

	}



    return 0;

}



static void __exit sk_exit(void)

{

    printk("The module is down...\n");

	cdev_del(&sk_cdev);

	unregister_chrdev_region(sk_dev, 1);

}



static int sk_register_cdev(void)

{

	int error;



	/* allocation device number */

	if(sk_major) {
        // 번호를 임의로 지정해서 할당하고 싶을 때
		sk_dev = MKDEV(sk_major, sk_minor);

		error = register_chrdev_region(sk_dev, 1, "sk");

	} else {

		error = alloc_chrdev_region(&sk_dev, sk_minor, 1, "sk");
        // alloc_chrdev_region은 커널에 있는 함수 
		sk_major = MAJOR(sk_dev);
        // MAJOR 펌웨어서 자동으로 할당해줄 수 있는 번호
	}



	if(error < 0) {

		printk(KERN_WARNING "sk: can't get major %d\n", sk_major);

		return result;

	}

	printk("major number=%d\n", sk_major);



	/* register chrdev */

	cdev_init(&sk_cdev, &sk_fops);
    // cdev_init : 시스템 함수, character device
    // sk_fops : 

	sk_cdev.owner = THIS_MODULE;    // this는 자기 자신, this..현재 시간?

	sk_cdev.ops = &sk_fops; // 함수가 등록된것은 sk_cdev.ops에 연결

	error = cdev_add(&sk_cdev, sk_dev, 1);  // 커널에 등록, device를 한개 등록



	if(error)

		printk(KERN_NOTICE "sk Register Error %d\n", error);



	return 0;

}







module_init(sk_init); //init는 insmod할 때 호출된다. 

module_exit(sk_exit); 

```

## open_app.c
- main이 들어가있어, 응용계층? 
```c
/***************************************

 * Filename: sk_app.c

 * Title: Skeleton Device Application

 * Desc: Implementation of system call

 ***************************************/

#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

#include <fcntl.h>



int main(void)

{

    int fd;

    

    fd = open("/dev/SK", O_RDWR);

    printf("fd = %d\n", fd);

    

    if (fd<0) {

        perror("/dev/SK error");

        exit(-1);

    }

    else

        printf("SK has been detected...\n");

    

    getchar();

    close(fd);

    

    return 0;

}

```

## Makefile
```c
obj-m   := sk.o



#KDIR    := /work/REBIS_BSP/linux-2.6.17.13-rebis

# KDIR	:= /root/work/embedded/linux-3.12.14

KDIR	:= /root/kernel-mds2450-3.0.22



all:

	make -C $(KDIR) SUBDIRS=$(PWD) modules



clean:	

	make -C $(KDIR) SUBDIRS=$(PWD) clean
```
- 모듈을 만든것
  - .ko 파일(kernel object)생성
- 모듈 실행
  - 타겟보드에서 insmod, rmmod 사용
  - mknod를 시켜줘야 모듈을 장치로 변환시켜준다. 
  - mknod /dev/SK c 251 0

## arm 파일 컴파일
- arm-none-linux-gnueabi-gcc -o sk_app sk_app.c
- 파일 실행
  - 타겟보드에서 ./sk_app

## write, read 추가
```c
/***************************************
 * Filename: sk.c
 * Title: Skeleton Device
 * Desc: Implementation of system call
 ***************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

static int sk_major = 0, sk_minor = 0;
static int result;
static dev_t sk_dev;

static struct cdev sk_cdev;

static int sk_register_cdev(void);

/* TODO: Define Prototype of functions */
static int sk_open(struct inode *inode, struct file *filp);
static int sk_release(struct inode *inode, struct file *filp);
static int sk_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static int sk_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);

/* TODO: Implementation of functions */
static int sk_open(struct inode *inode, struct file *filp)
{
    printk("Device has been opened...\n");
    
    /* H/W Initalization */
    
    return 0;
}

static int sk_release(struct inode *inode, struct file *filp)
{
    printk("Device has been closed...\n");
    
    return 0;
}

static int sk_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	char data[11];

	copy_from_user(data, buf, count);
	printk("data >>>>> = %s\n", data);

	return count;
}

static int sk_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	char data[20] = "this is read func...";

	copy_to_user(buf, data, count); // kernel에서 user로 copy

	return 0;
}

struct file_operations sk_fops = { 
    .open       = sk_open,
    .release    = sk_release,
	.write		= sk_write,
	.read		= sk_read,  
};

static int __init sk_init(void)
{
    printk("SK Module is up... \n");

	if((result = sk_register_cdev()) < 0)
	{
		return result;
	}

    return 0;
}

static void __exit sk_exit(void)
{
    printk("The module is down...\n");
	cdev_del(&sk_cdev);
	unregister_chrdev_region(sk_dev, 1);
}

static int sk_register_cdev(void)
{
	int error;

	/* allocation device number */
	if(sk_major) {
		sk_dev = MKDEV(sk_major, sk_minor);
		error = register_chrdev_region(sk_dev, 1, "sk");
	} else {
		error = alloc_chrdev_region(&sk_dev, sk_minor, 1, "sk");
		sk_major = MAJOR(sk_dev);
	}

	if(error < 0) {
		printk(KERN_WARNING "sk: can't get major %d\n", sk_major);
		return result;
	}
	printk("major number=%d\n", sk_major);

	/* register chrdev */
	cdev_init(&sk_cdev, &sk_fops);
	sk_cdev.owner = THIS_MODULE;
	sk_cdev.ops = &sk_fops;
	error = cdev_add(&sk_cdev, sk_dev, 1);

	if(error)
		printk(KERN_NOTICE "sk Register Error %d\n", error);

	return 0;
}



module_init(sk_init); 
module_exit(sk_exit); 

```

## 문제
```c
: copy_to_user()/copy_from_user() 사용
 응용프로그램이 보내준
아래 구조체 형식의 데이터를 드라이버가 받아서 출력시키고 
드라이버는 같은 구조체 형식으로 또 다른 데이터를 응용프로그램에게
보내주고 응용프로그램이 출력시키는 코드를 구현하세요

/*  구조체 포맷  */
typedef struct
{
   int age;  //나이 :35
   char name[30];// 이름 : HONG KILDONG
   char address[20]; // 주소 : SUWON CITY
   int  phone_number; // 전화번호 : 1234
   char depart[20]; // 부서 : mds
} __attribute__ ((packed)) mydrv_data;
//  linux version : __attribute__ ((packed)) mydrv_data;
//   sprintf(k_buf->name,"HONG KILDONG");

```

## 과제
```c
#define rGPLCON1 *(volatile unsigned long *)(kva + 4)
#define rGPLDAT *(volatile unsigned long *)(kva + 8)
#define rGPMCON *(volatile unsigned long *)(kva + 0x10)
#define rGPMDAT *(volatile unsigned long *)(kva + 0x14)

static void *kva;

kva = ioremap(0x7F008810,28) ;
    // ex > ioremap(0x56000060,28) ; 이 주소를 기준으로 28바이트를 사용하겠다.\
    // 주소를 다이렉트로 사용할수없다. 
    // write에 넣으면 깔끔할 듯
	printk("kva = 0x%x\n",(int)kva);

```