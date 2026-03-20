#include "stack.h"

void init(stack_t* stack)
{
	stack->top = NULL;
}

int push(stack_t* stack, int n)
{
	node_t* node = malloc(sizeof(node_t));
	if (NULL == node)
	{
		return 0;
	}

	node->value = n;
	node->next = stack->top;
	stack->top = node;
	return 1;
}

int pop(stack_t* stack, int* out)
{
	if (NULL == stack->top)
	{
		return 0;
	}

	(*out) = stack->top->value;

	node_t* node = stack->top;
	stack->top = stack->top->next;

	free(node);

	return 1;
}

unsigned int kill(stack_t* stack)
{
	unsigned int deallocated_nodes = 0;
	while (NULL != stack->top)
	{
		node_t* node = stack->top;
		stack->top = stack->top->next;
		free(node);
		deallocated_nodes++;
	}

	return deallocated_nodes;
}