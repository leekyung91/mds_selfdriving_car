/***************************************
 * Filename: sk.c
 * Title: Skeleton Device
 * Desc: Implementation of system call
 ***************************************/
#if 0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#endif

#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <asm/page.h>
//#include <asm/hardware.h>
#include <linux/delay.h>
#include <asm/io.h>   // ioremap(),iounmap()
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <asm/signal.h>

MODULE_LICENSE("GPL");

static int sk_major = 0, sk_minor = 0;
static int result;
static dev_t sk_dev;
int  id;


static struct cdev sk_cdev;
int my_kill_proc(pid_t pid, int sig);
static int sk_register_cdev(void);
int my_kill_proc(pid_t pid, int sig);

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

	//copy_from_user(data, buf, count);
	//printk("data >>>>> = %s\n", data);

        get_user(id, (int *)buf);
        printk("\n [Kernel] id = %d", id);
        my_kill_proc(id,SIGUSR1);
	return count;
}

static int sk_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	char data[20] = "this is read func...";

	copy_to_user(buf, data, count);

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


int my_kill_proc(pid_t pid, int sig)
{
    int error = ESRCH;
    struct task_struct *p;
    struct task_struct *t = NULL;
    struct pid *pspid;
    rcu_read_lock();

    p = &init_task;
    do
    {
	if(p->pid == pid)
        {
            t=p;
            break;
        }
        p = next_task(p);
    }while(p != &init_task);

    if(t != NULL)
    {
        pspid = t-> pids[PIDTYPE_PID].pid;
        if(pspid != NULL)
            error = kill_pid(pspid,sig,1);
    }
    rcu_read_unlock();
    return error;
}



module_init(sk_init); 
module_exit(sk_exit); 
