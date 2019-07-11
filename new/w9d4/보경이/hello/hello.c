#include <linux/module.h>	/* Needed by all modules */
#include <linux/moduleparam.h>
#include <linux/kernel.h>	/* Needed for KERN_INFO	*/
#include <linux/init.h>		/* Needed for the macros */
#include <linux/fs.h>       /* everything... */
#include <linux/types.h>    /* size_t */
#include <asm/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>

#define	DRIVER_AUTHOR	"DongKyu, Kim <dongkyu@mdstec.com>"
#define	DRIVER_DESC		"A sample driver"
#define     DRV_NAME                "signal"

#define rGPGCON *(volatile unsigned long *)(kva)
#define rGPFCON *(volatile unsigned long *)(kva1)


static void *kva,*kva1;
static int mydrv_major =240;
module_param(mydrv_major,int,0);
struct timer_list timer;

char *data;
pid_t apid;

int my_kill_proc(pid_t pid, int sig) {
    int error = -ESRCH;           /* default return value */
    struct task_struct* p;
    struct task_struct* t = NULL; 
    struct pid* pspid;
    rcu_read_lock();
    p = &init_task;               /* start at init */
    do {
        if (p->pid == pid) {      /* does the pid (not tgid) match? */
            t = p;    
            break;
        }
        p = next_task(p);         /* "this isn't the task you're looking for" */
    } while (p != &init_task);    /* stop when we get back to init */
    if (t != NULL) {
        pspid = t->pids[PIDTYPE_PID].pid;
        if (pspid != NULL) error = kill_pid(pspid,sig,1);
    }
    rcu_read_unlock();
    return error;
}
void my_timer(unsigned long data)
{
	int i;
	my_kill_proc(apid, 10);
	timer.expires = jiffies + 2*HZ;
	add_timer(&timer);
}
static int mydrv_release(struct inode *inode, struct file *file)
{
  printk("mydrv released !!\n");
  return 0;
}

static ssize_t mydrv_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
  copy_to_user(buf, data, count);
  data="";
  return 0;

}
static ssize_t mydrv_write(struct file *filp,const char __user *buf, size_t count,
                            loff_t *f_pos)
{
  printk("mydrv_write is invoked\n");
  get_user(apid, (int*)buf);
  printk("pid=%d",apid);
  return 0;
}
static irqreturn_t keyinterrupt_func1(int irq, void *dev_id, struct pt_regs *resgs)
{
     data="4 pressed....";
      //printk("Key 14 pressed....\n");
      return IRQ_HANDLED;
}

static int mydrv_open(struct inode *inode, struct file *file)
{	int ret;
	data=kmalloc(20,GFP_KERNEL);

// set Interrupt mode
        s3c_gpio_cfgpin(S3C2410_GPF(2), S3C_GPIO_SFN(2));
        s3c_gpio_cfgpin(S3C2410_GPF(3), S3C_GPIO_SFN(2));
	s3c_gpio_cfgpin(S3C2410_GPF(4), S3C_GPIO_SFN(2));
        s3c_gpio_cfgpin(S3C2410_GPF(5), S3C_GPIO_SFN(2));
	s3c_gpio_cfgpin(S3C2410_GPF(6), S3C_GPIO_SFN(2));
	
	kva = ioremap(0x56000060,28) ;
        kva1 = ioremap(0x56000050,28) ;
	
	rGPGCON &= ~(0x3);
	//rGPGCON |= (0x1);
	//rGPFCON &= ~(0x3<<14);
	//rGPFCON |=(0x1<<14);

        if( request_irq(IRQ_EINT2, (void *)keyinterrupt_func1,
                IRQF_DISABLED|IRQF_TRIGGER_FALLING, DRV_NAME, NULL) )
        {
                printk("failed to request external interrupt1.\n");
                ret = -ENOENT;
                return ret;
        }
	printk("mydrv opened !!\n");
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
	.open    = mydrv_open,
	.release = mydrv_release,
};

#define MAX_MYDRV_DEV 1

static struct cdev MydrvDevs[MAX_MYDRV_DEV];

static int __init init_hello_4(void)
{	

	int result;
	dev_t dev=MKDEV(mydrv_major,0);
       
/* Figure out our device number. */
	if (mydrv_major)
		result = register_chrdev_region(dev, 1, DRV_NAME);
	else {
		result = alloc_chrdev_region(&dev,0, 1, DRV_NAME);
		mydrv_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "mydrv: unable to get major %d\n", mydrv_major);
		return result;
	}
	if (mydrv_major == 0)
		mydrv_major = result;

	
	
	init_timer(&timer);
	//timer.expires = get_jiffies_64() + 3*HZ;
	timer.expires = jiffies + 3*HZ;
	timer.function = my_timer;
	timer.data = 5;
	add_timer(&timer);

	mydrv_setup_cdev(MydrvDevs,0, &mydrv_fops);
        printk(KERN_INFO "%s successfully loaded\n", DRV_NAME);
	return 0;
}

static void __exit cleanup_hello_4(void)
{
	printk(KERN_INFO "Goodbye, world 4.\n");

        free_irq(IRQ_EINT0, NULL);

    printk(KERN_INFO "%s successfully removed\n", DRV_NAME);

}

module_init(init_hello_4);
module_exit(cleanup_hello_4);
/* Get rid of taint message by declaring code as GPL. */
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);		/* Who wrote this module? */
MODULE_DESCRIPTION(DRIVER_DESC);	/* What does this module do */
MODULE_SUPPORTED_DEVICE("testdevice");
