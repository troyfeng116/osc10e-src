#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

Queue *create_queue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    q->sz = 0;
    return q;
}

struct node *poll_queue(Queue *q)
{
    if (q->sz == 0)
    {
        // fprintf(stderr, "[Queue] cannot poll empty queue\n");
        return NULL;
    }

    struct node *temp = q->head;
    q->head = temp->next;
    q->sz--;
    if (q->sz == 0)
    {
        q->tail = NULL;
    }

    temp->next = NULL;
    return temp;
}

void push_queue(Queue *q, struct node *new_node)
{
    if (q->tail != NULL)
    {
        q->tail->next = new_node;
    }
    else
    {
        q->head = new_node;
    }

    q->tail = new_node;
    q->sz++;
}

// drain contents of queue to array; queue will be empty
// array will contain q->sz tasks, in queued FIFO order
// NOTE: array and all allocated tasks in array must be freed
Task **drain_to_array(Queue *q)
{
    struct node *polled_node;
    int sz = q->sz;

    Task **task_arr = (Task **)malloc(sizeof(Task *) * sz);
    for (int i = 0; i < sz; i++)
    {
        polled_node = poll_queue(q);
        task_arr[i] = polled_node->task;
        free(polled_node);
    }

    return task_arr;
}

void print_queue(Queue *q)
{
    struct node *temp = q->head;
    while (temp != NULL)
    {
        print_task(temp->task);
        temp = temp->next;
    }
}

void destroy_queue(Queue *q)
{
    struct node *prev;
    struct node *temp = q->head;

    while (temp != NULL)
    {
        prev = temp;
        temp = temp->next;
        destroy_node(prev);
    }

    free(q);
}