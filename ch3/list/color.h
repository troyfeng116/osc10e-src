#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/random.h>

struct color
{
    int red;
    int blue;
    int green;
    struct list_head list;
};

void print_color(struct color *color)
{
    printk(KERN_INFO "rgb(%d, %d, %d)\n", color->red, color->blue, color->green);
}

int rand_rgb(void)
{
    int rand;
    get_random_bytes(&rand, sizeof(int));
    if (rand < 0)
    {
        rand = ~rand;
    }
    return rand % (1 << 8);
}

struct color *create_color(int r, int g, int b)
{
    struct color *color;

    color = kmalloc(sizeof(struct color), GFP_KERNEL);
    color->red = r;
    color->green = g;
    color->blue = b;

    INIT_LIST_HEAD(&color->list);

    return color;
}