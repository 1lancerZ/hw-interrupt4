#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
//#include <linux/time.h>

static int irq;					//中断号变量
static char* devname;				//注册中断的设备名
static struct tasklet_struct mytasklet;		//声明tasklet结构体
static int count = 0;				//中断次数
//static struct timer_list my_timer;		//计时器

module_param(irq, int, 0644);			//用户向内核传递irq参数
module_param(devname, charp, 0644);		//用户向内核传递devname参数

MODULE_LICENSE("GPL");

struct myirq
{
	int devid;
};

struct myirq mydev = {1900};

/* tasklet结构体的处理函数*/
static void mytasklet_handler(unsigned long data)
{
	printk("=== tasklet is working...\n");
}

//DECLARE_TASKLET(mytasklet, mytasklet_handler, 0);	//初始化tasklet

/*中断处理函数*/
static irqreturn_t myirq_handler(int irq,void* dev)
{
	mydev = *(struct myirq*)dev;
	printk("=== count: %d\n", ++count);
	tasklet_init(&mytasklet, mytasklet_handler, 0);	//初始化tasklet
	tasklet_schedule(&mytasklet);
	return IRQ_HANDLED;
}

//void my_timer_callback(struct timer_list *timer)
//{
//    printk(KERN_INFO "Timer expired, generating IRQ %d\n", irq);
//    generic_handle_irq(irq);
//    mod_timer(timer, jiffies + msecs_to_jiffies(1000)); // 每秒触发一次
//}


static int __init myirq_init(void)
{
	int ret;
	printk("=== Module starts...\n");
	ret = request_irq(irq, myirq_handler, IRQF_SHARED, devname, &mydev);
	printk("=== ret is %d\n", ret);
	if(ret != 0){
		printk("=== %s request IRQ:%d failed...\n", devname, irq);
		return -1;
	}
	else printk("=== %s request IRQ:%d success...", devname, irq);

	// 初始化并启动定时器
	//timer_setup(&my_timer, my_timer_callback, 0);
	//mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000)); // 1秒后触发

	return 0;
}

static void __exit myirq_exit(void)
{
	printk("=== Module exits...\n");
	//del_timer(&my_timer);
	tasklet_kill(&mytasklet);
	free_irq(irq,&mydev);
	printk("=== %s request IRQ:%d leaving success...\n",devname,irq);
}

module_init(myirq_init);
module_exit(myirq_exit);

