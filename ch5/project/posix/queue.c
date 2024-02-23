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
// array will contain q->sz nodes, in queued FIFO order
// NOTE: array and all allocated nodes in array must be freed
struct node **drain_to_array(Queue *q)
{
    int sz = q->sz;

    struct node **node_arr = (struct node **)malloc(sizeof(struct node *) * sz);
    for (int i = 0; i < sz; i++)
    {
        node_arr[i] = poll_queue(q);
    }

    return node_arr;
}

// copy contents of queue to array
// note nodes are not copied/reallocated: changes to queue or array copy will be mutually reflected
struct node **copy_to_array(Queue *q)
{
    int sz = q->sz;

    // drain, then push all nodes back
    struct node **node_arr = drain_to_array(q);
    for (int i = 0; i < sz; i++)
    {
        push_queue(q, node_arr[i]);
    }

    return node_arr;
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

void destroy_node_array(struct node **nodes, int n)
{
    for (int i = 0; i < n; i++)
    {
        destroy_node(nodes[i]);
    }

    free(nodes);
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