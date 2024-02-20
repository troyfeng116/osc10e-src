/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

struct node
{
    Task *task;
    struct node *next;
};

Task *create_task(char *name, int tid, int priority, int burst);
void print_task(Task *task);
struct node *create_node(char *name, int tid, int priority, int burst);
void destroy_node(struct node *node);

// insert and delete operations.
void list_insert(struct node **head, Task *task);
void list_remove(struct node **head, Task *task);
void list_traverse(struct node *head);
