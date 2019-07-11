# 디바이스 드라이버 3일차
## 디바이스 드라이버는 입출력 데이터가 많다.
* copy to user, copy form user 를 사용하는 이유
  * define으로 정의된 매크로 함수 
* 간단한 문자라면 넘어오는 buf로 받아낼 수 있다.

> volatile 두가지  
> 1. 최적화 함부로 하지마라
> 2. 캐시에 넣지마라


## IO를 사용해서 LED 관련 CMD 디바이스를 만들어라 
### test_mydrv.c
```c
/* test_mydrv.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "ioctl_mydrv.h"

int main()
{
  int fd,i,size;
  ioctl_buf *buf_in,*buf_out;
  
  size = sizeof(ioctl_buf);
  buf_in = (ioctl_buf *)malloc(size);
  buf_out = (ioctl_buf *)malloc(size);
  
  fd = open("/dev/mydrv",O_RDWR);
 
  ioctl(fd,IOCTL_MYDRV_TEST);
  
  ioctl(fd, IOCTL_MYDRV_READ, buf_in );
  printf("buf_in = %s\n",buf_in->data);
  
  for(i = 0;i < size;i++)
     buf_out->data[i] = 'a' + i;
  ioctl(fd, IOCTL_MYDRV_WRITE, buf_out );
  
  for(i = 0;i < size;i++)
     buf_out->data[i] = 'z' - i;
  ioctl(fd, IOCTL_MYDRV_WRITE_READ, buf_out );
  printf("buf_in = %s\n",buf_out->data);
    
  free(buf_in);
  free(buf_out);
  close(fd);
  return (0);
}
```

### ioctl_mydrv.h
```c
#ifndef _IOCTL_MYDRV_H_

#define _IOCTL_MYDRV_H_



#define IOCTL_MAGIC    254

#define LED_ON 134


typedef struct

{

	unsigned char data[26];	

	

} __attribute__ ((packed)) ioctl_buf;
/* 
__attribute__ ((packed)) ioctl_buf; 
패딩을 하지 마라 
*/

/*

명령어를 만들어놓는 매크로 



*/

#define IOCTL_MYDRV_TEST                _IO(  IOCTL_MAGIC, 0 )

/*

_IOW(  매직넘버, 구분번호, 변수형 )

- 매직넘버는 아무거나 넣어도 된다.

- 구분번호 : 임의로 넣는 것 

*/

/*

#define IOCTL_MYDRV_TEST1                _IO(  IOCTL_MAGIC, 0 )

#define IOCTL_MYDRV_TEST4                _IO(  IOCTL_MAGIC, 1 )

#define IOCTL_MYDRV_TEST8                _IO(  IOCTL_MAGIC, 2 )

#define IOCTL_MYDRV_TEST12                _IO(  IOCTL_MAGIC, 3 )

*/

#define IOCTL_LED_1_ON_TEST1                _IOR( IOCTL_MAGIC, 0)
#define IOCTL_LED_2_ON_TEST2                _IOR( IOCTL_MAGIC, 1)
#define IOCTL_LED_3_ON_TEST3                _IOR( IOCTL_MAGIC, 2)
#define IOCTL_LED_4_ON_TEST4                _IOR( IOCTL_MAGIC, 3)




#define IOCTL_MYDRV_READ                _IOR( IOCTL_MAGIC, 1 , ioctl_buf )

#define IOCTL_MYDRV_WRITE               _IOW( IOCTL_MAGIC, 2 , ioctl_buf )

#define IOCTL_MYDRV_WRITE_READ     _IOWR( IOCTL_MAGIC, 3 , ioctl_buf )



#define IOCTL_MAXNR                   4

  

#endif // _IOCTL_MYDRV_H_


```


### mydrv.c
```c
/*

  mydrv.c - kernel 3.0 skeleton device driver

               ioctl

 */



#include <linux/module.h>

#include <linux/moduleparam.h>

#include <linux/init.h>



#include <linux/kernel.h>   /* printk() */

#include <linux/slab.h>   /* kmalloc() */

#include <linux/fs.h>       /* everything... */

#include <linux/errno.h>    /* error codes */

#include <linux/types.h>    /* size_t */

#include <asm/uaccess.h>

#include <linux/kdev_t.h>

#include <linux/cdev.h>

#include <linux/device.h>

#include "ioctl_mydrv.h"



#define DEVICE_NAME "mydrv"

static int mydrv_major = 240;

module_param(mydrv_major, int, 0);





static int mydrv_open(struct inode *inode, struct file *file)

{

  printk("mydrv opened !!\n");

  return 0;

}



static int mydrv_release(struct inode *inode, struct file *file)

{

  printk("mydrv released !!\n");

  return 0;

}



static ssize_t mydrv_read(struct file *filp, char __user *buf, size_t count,

                loff_t *f_pos)

{

  printk("mydrv_read is invoked\n");

  return 0;



}



static ssize_t mydrv_write(struct file *filp,const char __user *buf, size_t count,

                            loff_t *f_pos)

{

  printk("mydrv_write is invoked\n");

  return 0;

}



static int mydrv_ioctl ( struct file *filp, unsigned int cmd, unsigned long arg)  

{  

  

   ioctl_buf *k_buf;

   int   i,err, size;  

      // 여기부터는 에러 안 넣어도 ok

   if( _IOC_TYPE( cmd ) != IOCTL_MAGIC ) return -EINVAL;  

   if( _IOC_NR( cmd )   >= IOCTL_MAXNR ) return -EINVAL;  



   size = _IOC_SIZE( cmd );   

 

   if( size )  

   {  

       err = -EFAULT;  

       if( _IOC_DIR( cmd ) & _IOC_READ  ) 

		err = !access_ok( VERIFY_WRITE, (void __user *) arg, size );  

       else if( _IOC_DIR( cmd ) & _IOC_WRITE ) 

		err = !access_ok( VERIFY_READ , (void __user *) arg, size );  

       if( err ) return err;          

   }  

            // 이 이후가 메인 

   switch( cmd )  

   {  

      case IOCTL_LED_1_ON_TEST1:
		break;
      case IOCTL_LED_2_ON_TEST2:
		break;
      case IOCTL_LED_3_ON_TEST3:
		break;
      case IOCTL_LED_4_ON_TEST4:
		break;

       case IOCTL_MYDRV_TEST :

            printk("IOCTL_MYDRV_TEST Processed!!\n");

	    break;



       case IOCTL_MYDRV_READ :

            k_buf = kmalloc(size,GFP_KERNEL);

            for(i = 0 ;i < size;i++)

                      k_buf->data[i] = 'A' + i;

            if(copy_to_user ( (void __user *) arg, k_buf, (unsigned long ) size ))

	          return -EFAULT;   

            kfree(k_buf);

            printk("IOCTL_MYDRV_READ Processed!!\n");

	    break;

	    	       

       case IOCTL_MYDRV_WRITE :

            k_buf = kmalloc(size,GFP_KERNEL);

            if(copy_from_user(k_buf,(void __user *) arg,size))

		   return -EFAULT;

            printk("k_buf = %s\n",k_buf->data);

            kfree(k_buf);

            printk("IOCTL_MYDRV_WRITE Processed!!\n");

	    break;

	                          

       case IOCTL_MYDRV_WRITE_READ : 

            k_buf = kmalloc(size,GFP_KERNEL);

            if(copy_from_user(k_buf,(void __user *) arg,size))

                return -EFAULT;

            printk("k_buf = %s\n",k_buf->data);

            

            for(i = 0 ;i < size;i++)

                      k_buf->data[i] = 'Z' - i;

            if(copy_to_user ( (void __user *) arg, k_buf, (unsigned long ) size ))

       	   return -EFAULT;

	      kfree(k_buf);

            printk("IOCTL_MYDRV_WRITE_READ Processed!!\n");

	    break;

       

      default :

            printk("Invalid IOCTL  Processed!!\n");

            break; 

    }  

  

    return 0;  

}  


/* Set up the cdev structure for a device. */

static void mydrv_setup_cdev(struct cdev *dev, int minor,

		struct file_operations *fops)

{

	int err, devno = MKDEV(mydrv_major, minor);

    

	cdev_init(dev, fops);

	dev->owner = THIS_MODULE;

	dev->ops = fops;

	err = cdev_add (dev, devno, 1);

	

	if (err)

		printk (KERN_NOTICE "Error %d adding mydrv%d", err, minor);

}



static struct file_operations mydrv_fops = {

	.owner   = THIS_MODULE,

   	.read	   = mydrv_read,

    	.write   = mydrv_write,

	.unlocked_ioctl   = mydrv_ioctl,

	.open    = mydrv_open,

	.release = mydrv_release,

};



#define MAX_MYDRV_DEV 1



static struct cdev MydrvDevs[MAX_MYDRV_DEV];



static int mydrv_init(void)

{

	int result;

	dev_t dev = MKDEV(mydrv_major, 0);



	/* Figure out our device number. */

	if (mydrv_major)

		result = register_chrdev_region(dev, 1, DEVICE_NAME);

	else {

		result = alloc_chrdev_region(&dev,0, 1, DEVICE_NAME);

		mydrv_major = MAJOR(dev);

	}

	if (result < 0) {

		printk(KERN_WARNING "mydrv: unable to get major %d\n", mydrv_major);

		return result;

	}

	if (mydrv_major == 0)

		mydrv_major = result;



	mydrv_setup_cdev(MydrvDevs,0, &mydrv_fops);

	printk("mydrv_init done\n");	

	return 0;

}



static void mydrv_exit(void)

{

	cdev_del(MydrvDevs);

	unregister_chrdev_region(MKDEV(mydrv_major, 0), 1);

	printk("mydrv_exit done\n");

}



module_init(mydrv_init);

module_exit(mydrv_exit);



MODULE_LICENSE("Dual BSD/GPL");
```

## 

## 선생님 자료
https://docs.huihoo.com/doxygen/linux/kernel/3.7/gpio-nrs_8h.html


https://elixir.bootlin.com/linux/v3.2/source/arch/arm/plat-samsung/include/plat/gpio-cfg.h#L72