#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

#include <linux/sched/signal.h>

static int tasks_init(void)
{
        struct task_struct *task;
        task = &init_task;
        printk(KERN_INFO "Pid\t\t State\t\t Task Name\t");
        for_each_process(task)
        {
        	printk(KERN_INFO "pid:%5d    state:%5ld    task_name:%s\n",task->pid,task->state,task->comm);
        }
        return 0;
}

static void tasks_exit(void)
{
        printk(KERN_INFO "Module Exit\n");
}

module_init(tasks_init);
module_exit(tasks_exit);        


MODULE_LICENSE("GPL");
MODULE_AUTHOR("DuYe 222017321102034");
MODULE_DESCRIPTION("List process");
MODULE_ALIAS("List process");
