#include <stdio.h>
#include "stack.h"

/*
* Stos w tej implementacji jest struktur¹ dynamiczn¹, poniewaŸ alokuje i zwalnia pamiêæ w zale¿noœci
* od iloœci przechowywanych danych.
* 
* OUTPUT:
* 10 9 8 7 6 5 4 3 2 1
* DEALLOCCATED NODES: 1
* returned value = 0, poniewa¿ stack.top wskazuje na NULL
*/

int main()
{
	stack_t stack;
	init(&stack);

	for (int i = 0; i <= 10; i++)
	{
		int ok = push(&stack, i);
		if (!ok)
		{
			return -1;
		}
	}

	puts("OUTPUT: ");
	while (NULL != stack.top->next)
	{
		int n = 0;
		int ok = pop(&stack, &n);
		if (!ok)
		{
			return -2;
		}
		printf("%i ", n);
	}

	unsigned int deallocated_nodes = kill(&stack);
	printf("\nDEALLOCATED NODES: %i\n", deallocated_nodes);

	return stack.top;
}