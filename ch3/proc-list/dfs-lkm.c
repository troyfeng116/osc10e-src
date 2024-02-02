/**
 * Design a kernel module that iterates through all tasks in the system using the
 * for each process() macro. In particular, output the task command, state,
 * and process id of each task. (You will probably have to read through the
 * task struct structure in <linux/sched.h> to obtain the names of these
 * fields.) Write this code in the module entry point so that its contents will appear
 * in the kernel log buffer, which can be viewed using the dmesg command.
 */

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
#define PROC_NAME "proc-list-dfs"

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
};

static void dfs_tasks(struct task_struct *task)
{
    struct task_struct *child;
    struct list_head *list;

    printk(KERN_INFO "command = [%s] pid = [%d] state = [%ld] parent_pid = [%d]\n",
           task->comm, task->pid, task->state, task->real_parent != NULL ? task->real_parent->pid : -1);

    list_for_each(list, &(task->children))
    {
        child = list_entry(list, struct task_struct, sibling);
        dfs_tasks(child);
    }
}

/* This function is called when the module is loaded. */
static int proc_init(void)
{
    // creates the /proc/procfs entry
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    dfs_tasks(&init_task);

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