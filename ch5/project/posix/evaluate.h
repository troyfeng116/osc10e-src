#ifndef EVALUATE_H
#define EVALUATE_H

#include "list.h"

// given final task state nodes, report evaluation metrics (response, wait, turnaround times)
void evaluate_and_report(struct node **nodes, int n);

#endif