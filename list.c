#include "archive.h"

Pnode Linsert(Pnode list, fileHeader newElem)
{
	Pnode newNode = (Pnode)malloc_s(sizeof(node));

	newNode->elem = newElem;
	newNode->next = list;

	return newNode;
}

Pnode Lsearch(Pnode list, char *searchName)
{
	Pnode tempNode;

	for (tempNode = list; tempNode != NULL; tempNode = tempNode->next)
		{
			if (!strcmp((char*)(tempNode->elem.fileName), searchName))
				return tempNode;
		}

	return NULL;
}

int Lfree(Pnode list)
{
	Pnode temp = list;

	while((list = temp))
	{
		temp = list->next;
		free(list);
	}

	return 1;
}
