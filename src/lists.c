#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists.h"

//adds a node to the specified list
void addToList(varNode** list, varNode* varToAdd){
	varNode* tNode = *list;
	
	//if our list is null, set the new variable as the head
	if (tNode == NULL) {
		*list = varToAdd;
		return;
	}

	//loop through until we find the last node
	while (tNode->next != NULL)
		tNode = tNode->next;

	//make the last node point to the new last node
	tNode->next = varToAdd;
}

//prints the list starting at the header
void printList(varNode* head) {
	varNode* tNode = head;

	while (tNode != NULL) {
		printf("\n    %s\t%d\t\t%s", tNode->name, tNode->width, tNode->isSigned ? "Signed" : "Unsigned");
		tNode = tNode->next;
	}
}

//checks a list to see if one of its nodes has the specified name
//params:
//  head: pointer to the head node
//  name: name we are looking for in the list
int containsVar(varNode* head, char* name) {
	varNode* temp = head;

	//while we still have values to check
	while (temp != NULL) {
		//if the name of the var in the list = the name we are checking
		if (strcmp(temp->name, name) == 0) {
			return 1;
		}
		//move to next node;
		temp = temp->next;
	}
	//we didnt find the variable in the list
	return 0;
}

//frees all the memory for the list. 
//haven't checked if it works, but idk why it wouldn't
void freeList(varNode* head) {
	varNode* tNode;

	while (head != NULL) {
		tNode = head->next;
		free(head);
		head = tNode;
	}
}