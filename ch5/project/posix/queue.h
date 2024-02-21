#include "list.h"

typedef struct queue
{
    struct node *head, *tail;
    int sz;
} Queue;

Queue *create_queue();
struct node *poll_queue(Queue *q);
void push_queue(Queue *q, struct node *new_node);
Task **drain_to_array(Queue *q);
void print_queue(Queue *q);
void destroy_queue(Queue *q);