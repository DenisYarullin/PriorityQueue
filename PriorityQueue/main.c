#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>


int main()
{
	PrQueue** pp_queue = (PrQueue **)malloc(sizeof(pp_queue));
	prQueueCreate(18, pp_queue);

	int arr[18] = { 13030, 16413, 29168, 900, 32591, 18762, 1655, 17410, 6359, 27624, 20537, 21548, 6483, 27595, 4041, 3602, 24350, 10291 };
	for (int i = 0; i < 18; i++)
	{
		prQueuePush(*pp_queue, arr[i], &arr[i]);
		prQueuePrint(*pp_queue);
	}

	int priority = 0;
	int* item = NULL;
	size_t size = 0;

	prQueueGetCount(*pp_queue, &size);
	while (size > 0)
	{
		prQueuePop(*pp_queue, &priority, (void*)&item);
		printf("%d%20d\n", priority, *item);
		prQueueGetCount(*pp_queue, &size);
	}

	prQueueRelease(*pp_queue);
	free(pp_queue);

	return 0;
}
