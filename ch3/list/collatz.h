#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>

struct collatz
{
    int val;
    struct list_head list;
};

void print_collatz(struct collatz *collatz)
{
    printk(KERN_INFO "%d\n", collatz->val);
}

struct collatz *create_collatz_node(int val)
{
    struct collatz *collatz_node;

    collatz_node = kmalloc(sizeof(struct collatz), GFP_KERNEL);
    collatz_node->val = val;

    INIT_LIST_HEAD(&collatz_node->list);

    return collatz_node;
}