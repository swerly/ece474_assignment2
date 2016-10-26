#ifndef LISTS_H
#define LISTS_H

//Structure for each variable/node in our list
typedef struct node{
    int isSigned;
    int width;
    char* name;
    struct node* next;
} varNode;

//structure to hold all of our lists to pass easily between functions
typedef struct{
    varNode* inputHead;
    varNode* outputHead;
    varNode* regHead;
    varNode* wireHead;
    //0 if header/vars havent been printed, 1 if they have
    int headerAndVarsWritten;
    char* filename;

	int opCount[8];						//an array that tracks the number of times an operation has been used for printing purposes
} listContainer;

void addToList(varNode** list, varNode* varToAdd);
//other functions as needed
//searching for name, etc

void printList(varNode* head);
int containsVar(varNode* head, char* name);
void freeList(varNode* head);
#endif