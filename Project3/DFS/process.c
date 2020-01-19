#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

#include <linux/sched/signal.h>


int DFS(struct task_struct *p,int d)
{   

    struct task_struct *child;
    struct list_head *list;
    int i;
    char space[100]={""};
    
    for(i=0;i<d;i++){
    	space[i]= '\t';
    }
	//printk("DEPTH:%d",d);
	//printk("%s",space);
   	printk("%s|-%s(id:%d state:%ld)\n",space,p->comm,p->pid,p->state);
    list_for_each(list, &p->children) {
        child = list_entry(list, struct task_struct, sibling);
        DFS(child,d+1);
	}
	return 0;
}


static int tasks_init(void)
{
        struct task_struct *task;
        task = &init_task;
	DFS(task,0);
       
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
MODULE_DESCRIPTION("List process(DFS)");
MODULE_ALIAS("List process(DFS)");
