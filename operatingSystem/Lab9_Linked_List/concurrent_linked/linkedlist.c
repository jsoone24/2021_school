#include "linkedlist.h"

int linked_list_init(linked_list_t **ll)
{
	if (ll == NULL)
	{
		printf("linked list pointer error \n");
		return -1;
	}

	/* do program */
	/* allocate linked list */
	/* initialize list head */
	/* initialize mutex lock for BLINKED_LIST version */

	(*ll) = (linked_list_t *)malloc(sizeof(linked_list_t));
	(*ll)->list_head = (node_t *)malloc(sizeof(node_t));
	(*ll)->list_head->key = 0;
	(*ll)->list_head->value = 0;
	(*ll)->list_head->level = 0;
	(*ll)->list_head->next = NULL;

#ifdef BLINKED_LIST
	int rc = pthread_mutex_init(&((*ll)->list_lock), NULL);
	assert(rc == 0);
#endif
	srand((unsigned)time(NULL));

	return 0;
}

void linked_list_destroy(linked_list_t *ll)
{
	/* do program */
	/* free all nodes in the linked list */
	/* free the linked list */

	node_t **ptr;
	node_t **tmp;
	node_t *_tmp;

	ptr = ll->list_head->next;

	while (ptr != NULL)
	{
		tmp = ptr;
		_tmp = *ptr;
		ptr = (*ptr)->next;

		free(_tmp);
		free(tmp);
	}

	free(ll->list_head);
	free(ll);

	return;
}

long linked_list_get(long key, linked_list_t *ll)
{
	/* do program */
	/* if key is found, return value */
	/* if key is not found, return -1 */

	node_t **ptr;

	ptr = &(ll->list_head);
	while (ptr != NULL)
	{
		if ((*ptr)->key != key)
		{
			ptr = (*ptr)->next;
			continue;
		}

		return (*ptr)->value;
	}

	return -1;
}

long linked_list_put(long key, long value, linked_list_t *ll)
{
	/* do program */
	/* if succeeds, return 0 */
	/* if fails, return -1 */

#ifdef BLINKED_LIST
	// lock
	pthread_mutex_lock(&(ll->list_lock));

	node_t **ptr;
	node_t **tmp;

	ptr = ll->list_head->next;

	tmp = (node_t **)malloc(sizeof(node_t *));
	if (tmp == NULL)
	{
		return -1;
	}
	*tmp = (node_t *)malloc(sizeof(node_t));
	if (*tmp == NULL)
	{
		return -1;
	}
	(*tmp)->key = key;
	(*tmp)->value = value;
	(*tmp)->level = 0;
	(*tmp)->next = ptr;

	(ll->list_head->next) = tmp;

	// unlock
	pthread_mutex_unlock(&(ll->list_lock));
#else
	while (1)
	{
		node_t **ptr;
		node_t **tmp;

		ptr = ll->list_head->next;

		tmp = (node_t **)malloc(sizeof(node_t *));
		if (tmp == NULL)
		{
			return -1;
		}
		*tmp = (node_t *)malloc(sizeof(node_t));
		if (*tmp == NULL)
		{
			return -1;
		}
		(*tmp)->key = key;
		(*tmp)->value = value;
		(*tmp)->level = 0;
		(*tmp)->next = ptr;

		if (CAS(&(ll->list_head->next), ptr, tmp) == 1)
		{
			break;
		}

		free(*tmp);
		free(tmp);
	}
#endif

	return 0;
}
