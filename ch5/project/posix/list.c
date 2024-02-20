/**
 * Various list operations
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"

struct node *create_node(char *name, int tid, int priority, int burst)
{
    Task *task = create_task(name, tid, priority, burst);
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->task = task;
    node->next = NULL;
    return node;
}

void destroy_node(struct node *node)
{
    free(node->task->name);
    free(node->task);
    free(node);
}

// add a new task to the list of tasks
void list_insert(struct node **head, Task *newTask)
{
    // add the new task to the list
    struct node *newNode = malloc(sizeof(struct node));

    newNode->task = newTask;
    newNode->next = *head;
    *head = newNode;
}

// delete the selected task from the list
void list_remove(struct node **head, Task *task)
{
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name, temp->task->name) == 0)
    {
        *head = (*head)->next;
    }
    else
    {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name, temp->task->name) != 0)
        {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

// traverse the list
void list_traverse(struct node *head)
{
    struct node *temp;
    temp = head;

    while (temp != NULL)
    {
        print_task(temp->task);
        temp = temp->next;
    }
}
