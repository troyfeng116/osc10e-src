#include "color.h"

#include <linux/init_task.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "color"

static LIST_HEAD(color_list);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
};

static void create_color_list(int len)
{
    int i;
    struct color *new_color, *ptr;

    for (i = 0; i < len; i++)
    {
        new_color = create_color(rand_rgb(), rand_rgb(), rand_rgb());
        list_add_tail(&new_color->list, &color_list);
    }

    list_for_each_entry(ptr, &color_list, list)
    {
        print_color(ptr);
    }
}

static void delete_color_list(void)
{
    struct color *ptr, *next;

    list_for_each_entry_safe(ptr, next, &color_list, list)
    {
        printk(KERN_INFO "deleting color node: ");
        print_color(ptr);

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

    create_color_list(4);

    return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{
    delete_color_list();

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