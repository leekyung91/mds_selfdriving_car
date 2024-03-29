#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/gpio.h>
//#include <linux/workqueue.h>

#define DRV_NAME		"keyint"
static int p;
static int i=0;


struct rebis_key_detection
{
    int             irq;
    int             pin;
    int             pin_setting;
    char            *name;
    int             last_state;
};

static struct rebis_key_detection rebis_gd = {
   IRQ_EINT7, S3C2410_GPF(7), S3C2410_GPF7_EINT7, "key-detect", 0
};


static irqreturn_t
rebis_keyevent(int irq, void *dev_id, struct pt_regs *regs)
{
    //struct rebis_key_detection *gd = (struct rebis_key_detection *) dev_id;
    //int             state;
	printk("\nkeypad was pressed \n");


    return IRQ_HANDLED;

}

static int __init rebis_keyint_init(void)
{
	int ret;

    gpio_request(S3C2410_GPF(2), "led 1");
    gpio_request(S3C2410_GPF(3), "led 2");
    gpio_request(S3C2410_GPF(4), "led 3");
    gpio_request(S3C2410_GPF(5), "led 4");
    gpio_request(S3C2410_GPF(6), "led 5");

    // set output mode
    s3c_gpio_cfgpin(S3C2410_GPF(2), S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(S3C2410_GPF(3), S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(S3C2410_GPF(4), S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(S3C2410_GPF(5), S3C_GPIO_SFN(1));
    s3c_gpio_cfgpin(S3C2410_GPF(6), S3C_GPIO_SFN(1));
    // set data
    gpio_direction_output(S3C2410_GPF(2), 1);
    gpio_direction_output(S3C2410_GPF(3), 1);
    gpio_direction_output(S3C2410_GPF(4), 1);
    gpio_direction_output(S3C2410_GPF(5), 1);
    gpio_direction_output(S3C2410_GPF(6), 1); 

    s3c_gpio_cfgpin(S3C2410_GPF(7), S3C_GPIO_SFN(2));



#if 0
	writel(readl(S3C2410_EXTINT0) & (~(0xf << 12)), S3C2410_EXTINT0);	
	writel(readl(S3C2410_EXTINT0) | (0x2 << 12), S3C2410_EXTINT0); // Falling Edge interrupt
	
#endif 


    if( request_irq(IRQ_EINT7, (void *)rebis_keyevent, IRQF_DISABLED | IRQF_TRIGGER_RISING, DRV_NAME, &rebis_gd) )     
    {   
                printk("failed to request external interrupt.\n");
                ret = -ENOENT;
                return ret;
    }  
	printk(KERN_INFO "%s successfully loaded\n", DRV_NAME);

    return 0;
    
}

static void __exit rebis_keyint_exit(void)
{
   gpio_free(S3C2410_GPF(2));
   gpio_free(S3C2410_GPF(3));
   gpio_free(S3C2410_GPF(4));
   gpio_free(S3C2410_GPF(5));
   gpio_free(S3C2410_GPF(6));

    free_irq(rebis_gd.irq, &rebis_gd);

    printk(KERN_INFO "%s successfully removed\n", DRV_NAME);
}


module_init(rebis_keyint_init);
module_exit(rebis_keyint_exit);

MODULE_LICENSE("GPL");

