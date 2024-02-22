/**
 * list data structure containing the tasks in the system
 */

#ifndef LIST_H
#define LIST_H

#include "task.h"

struct node
{
    Task *task;
    struct node *next;
};

struct node *create_node(char *name, int tid, int priority, int burst);
void destroy_node(struct node *node);

// insert and delete operations.
void list_insert(struct node **head, Task *task);
void list_remove(struct node **head, Task *task);
void list_traverse(struct node *head);

#endif