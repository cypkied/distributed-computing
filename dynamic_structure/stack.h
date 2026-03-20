#pragma once
#include "stdlib.h"

typedef struct node_t node_t;
typedef struct stack_t stack_t;

typedef struct node_t
{
	int value;
	struct node_t* next;
} node_t;

typedef struct stack_t
{
	node_t* top;
} stack_t;

void init(stack_t* stack);
int push(stack_t* stack, int n);
int pop(stack_t* stack, int* out);
unsigned int kill(stack_t* stack);