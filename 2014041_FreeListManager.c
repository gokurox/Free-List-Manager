#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**********************************************************************************************************************/
/****************************************	GURSIMRAN SINGH __ 2014041	***************************************/
/**********************************************************************************************************************/
/****************************************	Implementing a Linked List	***************************************/
/**********************************************************************************************************************/

typedef struct _FreeList
{
	uint8_t *memoryContainer;
	int allocatedMemory;

	struct _FreeList *next;
	struct _FreeList *prev;
} FreeList;

void insertAtEnd (FreeList **, FreeList **, unsigned int );
FreeList *deleteNode (FreeList **, FreeList **, FreeList *);
FreeList *freeMem (FreeList *);
void deleteFromPosition (FreeList **, FreeList **, unsigned int);
void printFreeList (FreeList *, FreeList *);
void _exit_ (FreeList **, FreeList **);

int main ()
{
	FreeList *FL_START = NULL, *FL_END = NULL;
	char tempBuffer[80];
	char *inputCommand, *tempBuffer2;
	unsigned int inputArgument;					// Number of bytes requested by the user

	printf ("********** IMPORTANT **********\n");
	printf ("This program implements a Linked List to store the allocated and de-allocated memory.. ..\n");
	printf ("The size of LinkedList Structure == %lu (bytes)\n\n", sizeof (FreeList));
	printf ("***** Made By: GURSIMRAN SINGH __ 2014041 *****\n\n");
	printf ("Enter one of the following (without quotes) to operate\n");
	printf ("\t\"malloc\" <int>\t\tTo allocate <int> bytes\n");
	printf ("\t\"free\" <int>\t\tTo free the entry at <int> in the list\n");
	printf ("\t\"printlist\"\t\tTo print the FreeList\n");
	printf ("\t\"exit\"\t\t\tTo quit\n\n");

	while (1)
	{
		fgets (tempBuffer, 80, stdin);
		tempBuffer [strlen (tempBuffer) -1] = '\0';

		inputCommand = strtok (tempBuffer, " ");
		tempBuffer2 = strtok (NULL, " ");
		if (tempBuffer2 != NULL)
			inputArgument = atoi (tempBuffer2);
		else
			inputArgument = -1;

		// printf ("%s %d\n", inputCommand, inputArgument);

		if (!strcmp (inputCommand, "malloc"))
		{
			if (inputArgument > 0)
				insertAtEnd (&FL_START, &FL_END, inputArgument);
			continue;
		}
		else if (!strcmp (inputCommand, "free"))
		{
			deleteFromPosition (&FL_START, &FL_END, inputArgument);
			continue;
		}
		else if (!strcmp (inputCommand, "printlist"))
		{
			if (inputArgument == -1)
				printFreeList (FL_START, FL_END);
			continue;
		}
		else if (!strcmp (inputCommand, "exit"))
		{
			_exit_ (&FL_START, &FL_END);
		}
		else
		{
			printf ("INVALID_COMMAND: Enter one of the following (without quotes)\n");
			printf ("\t\"malloc\" <int>\t\tTo allocate <int> bytes\n");
			printf ("\t\"free\" <int>\t\tTo free the entry at <int> in the list\n");
			printf ("\t\"printlist\"\t\tTo print the FreeList\n\n");
		}
	}

	return 0;
}

void insertAtEnd (FreeList **FL_START, FreeList **FL_END, unsigned int requestedMem)
{
	if ((*FL_END) == NULL && (*FL_START) == NULL)
	{
		// printf ("Heere\n");
		(*FL_END) = (FreeList *) malloc (sizeof (FreeList));						// Create a new Node
		if (*FL_END == NULL)
		{
			printf ("OUT_OF_MEMORY :: Can't create a new Node for FreeList\n");
			return;
		}
		// printf ("Heeresdsd\n");

		(*FL_END) -> memoryContainer = (uint8_t *) malloc (sizeof (uint8_t) * requestedMem);		// Allocate Memory
		if ((*FL_END) -> memoryContainer == NULL)
		{
			printf ("OUT_OF_MEMORY :: Can't allocate any more Memory... Allocation Failed !!\n");
			return;
		}

		(*FL_END) -> allocatedMemory = requestedMem;

		// printf ("Heereadad\n");

		(*FL_END) -> prev = NULL;
		(*FL_END) -> next = NULL;

		(*FL_START) = (*FL_END);
		// printf ("Heerhdghfe\n");
		return;
	}
	else
	{
		FreeList *TMP = (FreeList *) malloc (sizeof (FreeList));
		if (TMP == NULL)
		{
			printf ("OUT_OF_MEMORY :: Can't create a new Node for FreeList\n");
			return;
		}

		TMP -> memoryContainer = (uint8_t *) malloc (sizeof (uint8_t) * requestedMem);
		if (TMP -> memoryContainer == NULL)
		{
			printf ("OUT_OF_MEMORY :: Can't allocate any more Memory... Allocation Failed !!\n");
			return;
		}

		TMP -> allocatedMemory = requestedMem;

		TMP -> prev = (*FL_END);
		TMP -> next = NULL;
		(*FL_END) -> next = TMP;

		(*FL_END) = TMP;
		TMP = NULL;
		return;
	}

	printf ("INSERT_AT_END :: Something unexpected happened\n");
	return;
}

FreeList *deleteNode (FreeList **FL_START, FreeList **FL_END, FreeList *toDelete)
{
	FreeList *prev = toDelete -> prev;
	FreeList *next = toDelete -> next;

	if (prev == NULL && next == NULL)
	{
		// FL_START == FL_END | Only one Node
		toDelete = freeMem (toDelete);
		
		(*FL_START) = NULL;
		(*FL_END) = NULL;

		return NULL;
	}
	else if (prev == NULL)
	{
		// toDelete == FL_START
		(*FL_START) = (*FL_START) -> next;
		(*FL_START) -> prev = NULL;

		toDelete = freeMem (toDelete);

		return NULL;
	}
	else if (next == NULL)
	{
		// toDelete == FL_END
		(*FL_END) = (*FL_END) -> prev;
		(*FL_END) -> next = NULL;

		toDelete = freeMem (toDelete);

		return NULL;
	}
	else
	{
		// Node somewher in the List
		prev -> next = next;
		next -> prev = prev;

		toDelete = freeMem (toDelete);

		return NULL;
	}

	return NULL;
}

FreeList *freeMem (FreeList *TMP)
{
	free (TMP -> memoryContainer);

	TMP -> prev = NULL;
	TMP -> next = NULL;

	free (TMP);

	return NULL;
}

void deleteFromPosition (FreeList **FL_START, FreeList **FL_END, unsigned int position)
{
	if (position < 1)
	{
		printf ("ERROR: position must be >= 1\n");
		return;
	}

	FreeList *toDelete = NULL, *iterator = *FL_START;
	int counter = 1;

	while (counter < position && iterator != NULL)
	{
		counter ++;
		iterator = iterator -> next;
	}

	if (iterator == NULL)
		printf ("INVALID_POSITION :: No Node found at position %d\n", position);
	else
		iterator = deleteNode (FL_START, FL_END, iterator);

	return;
}

void printFreeList (FreeList *start, FreeList *end)
{
	int counter = 1;

	printf ("\n\tTHE FREE LIST ::\n");
	while (start != NULL)
	{
		printf ("\t%d\tSize:\t%d\t\tLocation: %p\n", counter ++, start -> allocatedMemory, start -> memoryContainer);
		start = start -> next;
	}
	printf ("\n");

	return;
}

void _exit_ (FreeList **FL_START, FreeList **FL_END)
{
	int counter = 1;

	printf ("\nEmptying the Allocated Memory.......................\n");
	while (*FL_START != NULL)
	{
		deleteFromPosition (FL_START, FL_END, 1);
	}
	printf ("Exiting !!! Bye\n");

	exit (0);
}