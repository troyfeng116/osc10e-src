#include "collatz.h"

#include <linux/init_task.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/moduleparam.h>

#define PROC_NAME "collatz"

static int start = 25;
module_param(start, int, 0);

static LIST_HEAD(collatz_list);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
};

static void create_and_print_collatz_list(int start_val)
{
    struct collatz *collatz_node, *ptr;

    printk("[create_and_print_collatz_list] start_val=%d\n", start_val);

    while (start_val > 1)
    {
        collatz_node = create_collatz_node(start_val);
        list_add_tail(&collatz_node->list, &collatz_list);

        if (start_val % 2 == 0)
        {
            start_val /= 2;
        }
        else
        {
            start_val = start_val * 3 + 1;
        }
    }

    if (start_val == 1)
    {
        collatz_node = create_collatz_node(start_val);
        list_add_tail(&collatz_node->list, &collatz_list);
    }

    list_for_each_entry(ptr, &collatz_list, list)
    {
        print_collatz(ptr);
    }
}

static void delete_collatz_list(void)
{
    struct collatz *ptr, *next;

    list_for_each_entry_safe(ptr, next, &collatz_list, list)
    {
        printk(KERN_INFO "deleting collatz node: %d\n", ptr->val);

        list_del(&ptr->list);
        kfree(ptr);
    }
}

/* This function is called when the module is loaded. */
static int proc_init(void)
{
    // creates the /proc/procfs entry
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    create_and_print_collatz_list(start);

    return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{
    delete_collatz_list();

    // removes the /proc/procfs entry
    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");