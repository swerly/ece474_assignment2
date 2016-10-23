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
} listContainer;

void addToList(varNode* list, varNode* varToAdd);
//other functions as needed
//searching for name, etc

#endif