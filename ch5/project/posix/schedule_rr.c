#include "list.h"
#include "schedulers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct node *list_head = NULL;

// add a task to the list
void add(char *name, int priority, int burst)
{
    printf("add name=%s, prio=%d, burst=%d\n", name, priority, burst);

    if (list_head == NULL)
    {
        list_head = (struct node *)malloc(sizeof(struct node));
        list_head->task = create_task(name, -1, priority, burst);
        list_head->next = NULL;
    }
    else
    {
        Task *task = create_task(name, -1, priority, burst);
        insert(&list_head, task);
    }
}

// invoke the scheduler
void schedule()
{
    struct node *temp;

    printf("schedule\n");

    traverse(list_head);

    while (list_head != NULL)
    {
        temp = list_head;
        Task *task = list_head->task;
        delete (&list_head, task);
        free(task->name);
        free(task);
        free(temp);
    }
}