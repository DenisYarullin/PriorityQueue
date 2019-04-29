#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	int n_priority;
	void* p_item;
} PrQueueNode;

struct _PrQueue
{
	int n_capacity;
	int n_size;
	PrQueueNode *p_nodes;
};

/*  Brief: Create priority queue with specified capacity.
	Input:
		n_capacity    - Queue capacity.
		pp_queue      - Pointer to the variable to fill with new queue pointer.
	Output:
		*pp_queue     - Will contain a pointer to a newly created queue.
	Return:
		STATUS_OK           - Operation completed successfully
		STATUS_ERR_INVALID  - Invalid arguments passed.
		STATUS_ERR_NOMEM    - Not enough memory to allocate the queue.
*/
int prQueueCreate(size_t n_capacity, PrQueue ** pp_queue)
{
	if (n_capacity <= 0 || pp_queue == NULL)
		return STATUS_ERR_INVALID;

	*pp_queue = (PrQueue *)calloc(1, sizeof(PrQueue));
	if (*pp_queue == NULL)
		return STATUS_ERR_NOMEM;

	(*pp_queue)->p_nodes = (PrQueueNode *)realloc((*pp_queue)->p_nodes, n_capacity * sizeof(PrQueueNode));
	if ((*pp_queue)->p_nodes == NULL)
		return STATUS_ERR_NOMEM;

	(*pp_queue)->n_capacity = n_capacity;
	(*pp_queue)->n_size = 0;

	return STATUS_OK;
}


/*  Brief: Push an item to the queue.
	Input:
		p_queue     - A pointer to the priority queue.
		n_priority  - New item priority.
		p_item      - New item data.
	Return:
		STATUS_OK           - Operation completed succesfully
		STATUS_ERR_INVALID  - Invalid arguments passed.
		STATUS_ERR_NOMEM    - The queue capacity exceeded.
*/
int prQueuePush(PrQueue * p_queue, int n_priority, void * p_item)
{
	int n_i, n_parent;
	if (p_queue == NULL || n_priority < 0 || p_item == NULL)
		return STATUS_ERR_INVALID;
	if (p_queue->n_capacity == p_queue->n_size)
		return STATUS_ERR_NOMEM;

	n_i = p_queue->n_size;
	n_parent = (n_i - 1) / 2;
	while (n_i > 0 && p_queue->p_nodes[n_parent].n_priority > n_priority)
	{
		// можно проверять на уникальность приоритета, но нужно ли?
		p_queue->p_nodes[n_i] = p_queue->p_nodes[n_parent];
		n_i = n_parent;
		n_parent = (n_parent - 1) / 2;
	}
	p_queue->p_nodes[n_i].n_priority = n_priority;
	p_queue->p_nodes[n_i].p_item = p_item;
	p_queue->n_size++;

	return STATUS_OK;
}


/*  Brief: Pop (get and remove) item with minimal priority from the queue.
	Input:
		p_queue      - A priority queue pointer.
		pn_priority  - A pointer to the variable to fill with item priority.
		pp_item      - A pointer to the variable to fill with item data pointer.
	Output:
		*pn_priority - Extracted item priority.
		*pp_item     - Extracted item data pointer.
	Return:
		STATUS_OK           - Operation completed successfully
		STATUS_ERR_INVALID  - Invalid arguments passed.
		STATUS_ERR_NODATA   - There are no items in the queue.
*/
int prQueuePop(PrQueue * p_queue, int * pn_priority, void ** pp_item)
{
	int n_i, n_child, n_k;
	if (p_queue == NULL)
		return STATUS_ERR_INVALID;
	if (p_queue->n_size == 0)
		return STATUS_ERR_NODATA;

	*pn_priority = p_queue->p_nodes[0].n_priority;
	*pp_item = p_queue->p_nodes[0].p_item;

	p_queue->p_nodes[0] = p_queue->p_nodes[p_queue->n_size - 1];
	p_queue->n_size--;

	n_i = 0;
	while (n_i != p_queue->n_size)
	{
		n_k = p_queue->n_size;
		n_child = 2 * n_i + 1;
		if (n_child <= p_queue->n_size - 1 && p_queue->p_nodes[n_child].n_priority < p_queue->p_nodes[n_k].n_priority)
		{
			n_k = n_child;
		}
		if (n_child + 1 <= p_queue->n_size - 1 && p_queue->p_nodes[n_child + 1].n_priority < p_queue->p_nodes[n_k].n_priority)
		{
			n_k = n_child + 1;
		}
		p_queue->p_nodes[n_i] = p_queue->p_nodes[n_k];
		n_i = n_k;
	}

	return STATUS_OK;
}


/*  Brief: Peek item with minimal priority from the queue.
	Same as prQueuePop(...) except that this function does not
	remove item from the queue.
*/
int prQueuePeek(PrQueue * p_queue, int * priority, void ** pp_item)
{
	if (p_queue == NULL) 
		return STATUS_ERR_INVALID;
	if (p_queue->n_size == 0)
		return STATUS_ERR_NODATA;

	*priority = p_queue->p_nodes[0].n_priority;
	*pp_item = p_queue->p_nodes[0].p_item;

	return STATUS_OK;
}


/*  Brief: Get number of elements in the queue.
	Input:
		p_queue      - A pointer to the priority queue.
		pn_count     - A pointer to the variable to fill with number of elements.
	Output:
		*pn_count    - Number of elements in the queue.
	Return:
		STATUS_OK           - Operation completed successfully
		STATUS_ERR_INVALID  - Invalid arguments passed.
*/
int prQueueGetCount(PrQueue * p_queue, size_t * pn_count)
{
	if (p_queue == NULL || pn_count == NULL)
		return STATUS_ERR_INVALID;

	*pn_count = p_queue->n_size;

	return STATUS_OK;
}


/*  Brief: Destroy queue and free the memory.
	Input:
		p_queue      - A pointer to the priority queue.
	Return:
		Nothing
*/
void prQueueRelease(PrQueue * p_queue)
{
	if (p_queue != NULL) 
	{
		free(p_queue->p_nodes);
		free(p_queue);
	}
}


/*  Brief: Print queue
	Input:
		p_queue      - A pointer to the priority queue.
	Return:
		Nothing
*/
void prQueuePrint(PrQueue * p_queue)
{
	for (int i = 0; i < p_queue->n_size; i++)
		printf("%d	", p_queue->p_nodes[i].n_priority);
	printf("\n");
}