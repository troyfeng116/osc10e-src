/**
 * Design a kernel module that iterates through all tasks in the system using the
 * for each process() macro. In particular, output the task command, state,
 * and process id of each task. (You will probably have to read through the
 * task struct structure in <linux/sched.h> to obtain the names of these
 * fields.) Write this code in the module entry point so that its contents will appear
 * in the kernel log buffer, which can be viewed using the dmesg command.
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "proc-list-linear"

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
};

/* This function is called when the module is loaded. */
static int proc_init(void)
{
    struct task_struct *task;

    // creates the /proc/procfs entry
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    for_each_process(task)
    {
        printk(KERN_INFO "pid = [%d] command = [%s] state = [%ld]\n", task->pid, task->comm, task->state);
    }

    return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{
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