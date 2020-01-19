#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

struct birthday  
{
    int day;
    int month;
    int year;
    struct list_head list;   
};

struct birthday *person;      
//struct list_head person_list;  
//struct list_head *pos;

static LIST_HEAD(birthday_list);
struct birthday *ptr;
struct birthday *next;

int mylist_init(void)
{
	int i = 0;
	INIT_LIST_HEAD(&birthday_list);  
	//list_add_tail(&person->list, &birthday_list);

	person = kmalloc(sizeof(struct birthday)*5,GFP_KERNEL);
	memset(person,0,sizeof(struct birthday)*5); 
	for(i=0;i<5;i++)
	{ 
		person[i].day = i+1;
		person[i].month = i+1;
		person[i].year = i+1+1990; 
		list_add_tail( &(person[i].list), &birthday_list);
	} 

	//list_add_tail(&person->list, &birthday_list);
	
	list_for_each_entry(ptr,&birthday_list,list)
	{
		printk("<check>day:%d,month:%d,year:%d \n",ptr->day,ptr->month,ptr->year);
	}
	
	return 0;
}


void mylist_exit(void)
{	
	struct birthday *ptr;
	struct birthday *next;

	list_for_each_entry_safe(ptr, next, &birthday_list, list) {
		printk(KERN_INFO "<Removing>day:%d,month:%d,year:%d \n",ptr->day,ptr->month,ptr->year);
		list_del(&ptr->list);
		kfree(ptr);
	}
	
}

module_init(mylist_init);
module_exit(mylist_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DuYe 222017321102034");
MODULE_DESCRIPTION("List Module");
MODULE_ALIAS("List module");
