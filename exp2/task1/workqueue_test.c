#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Theory&Model Group");
MODULE_DESCRIPTION("Use workqueue to print the time periodically");

int times;
module_param(times,int,0644);//用户向内核传递times参数
static int count = 0;

static struct workqueue_struct *queue = NULL;

//static struct work_struct work1;
static struct delayed_work work1;
static struct delayed_work work2;

//打印时间
static void print_time(void){
	struct timeval tv;
	struct rtc_time tm;

	do_gettimeofday(&tv);
	rtc_time_to_tm(tv.tv_sec, &tm);

	//UTC
	printk(KERN_INFO "%04d-%02d-%02d %02d:%02d:%02d\n",
		tm.tm_year + 1900,
        	tm.tm_mon + 1,
        	tm.tm_mday,
        	tm.tm_hour,
 		tm.tm_min,
          	tm.tm_sec);
}

//work handler
static void work_handler(struct work_struct *work){
	printk(KERN_INFO "%d:\n", count++);
	print_time();

	if(count < times){
		queue_delayed_work(queue, &work1, 5 * HZ);
	}
}

//delay work handler
static void work_handler_delay(struct work_struct *work){
	printk(KERN_INFO "This is a delay work: \n");
	printk(KERN_INFO "%d:\n", count++);
	print_time();
}

static int init_workqueue(void){
	printk(KERN_INFO "module init\n");

	queue = create_workqueue("my_queue");
	if(!queue){
		printk(KERN_ERR "Failed to create workqueue.\n");
		return -1;
	}

	INIT_DELAYED_WORK(&work1, work_handler);
	queue_delayed_work(queue, &work1, 0);

	INIT_DELAYED_WORK(&work2, work_handler_delay);
        queue_delayed_work(queue, &work2, 10 * HZ);

	return 0;
}

static void exit_workqueue(void){
	printk(KERN_INFO "module exit\n");

	cancel_delayed_work_sync(&work1);
	cancel_delayed_work_sync(&work2);
	if(queue){
		destroy_workqueue(queue);
	}

	printk(KERN_INFO "Unloading OK\n");
}
module_init(init_workqueue);
module_exit(exit_workqueue);
