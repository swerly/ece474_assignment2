#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lists.h"

void printVars(FILE* outp, varNode* head, char* type);

void writeHeaderAndVars(listContainer lists){
    FILE* outp = fopen(lists.filename, "a");
    varNode* tNode;
    int curWidth = -1;
    int moduleNameSize = strchr(lists.filename, '.')-lists.filename;
    char* moduleName = (char*)malloc(sizeof(char)*moduleNameSize+1);
    
    strncpy(moduleName, lists.filename, moduleNameSize);
    moduleName[moduleNameSize]=0;
    
    if (outp == NULL){
        printf("Couldn't open \"%s\" for writing", lists.filename);
        return;
    }
    
    fprintf(outp, "`timescale 1ns / 1ps\n\n");
    fprintf(outp, "module %s( ", moduleName);
    
	free(moduleName);
    //print the inputs and outputs in the module definition
    tNode = lists.inputHead;
    while(tNode != NULL){
        fprintf(outp, "%s, ", tNode->name);
        tNode = tNode->next;
    }
    
    tNode = lists.outputHead;
    while(tNode != NULL){
		if (tNode->next != NULL)
			fprintf(outp, "%s, ", tNode->name);
		else
			fprintf(outp, "%s );\n", tNode->name);
        tNode = tNode->next;
    }
    
    //type inputs
    printVars(outp, lists.inputHead, "input");
    printVars(outp, lists.outputHead, "output");
    printVars(outp, lists.wireHead, "wire");
    
    fclose(outp);
}

void printVars(FILE* outp, varNode* head, char* type){
    //type inputs
    varNode* tNode = head;
    
    fprintf(outp, "    %s ", type);
    if (tNode->isSigned)
        fprintf(outp, "signed ");
    if (tNode->width != 1)
        fprintf(outp, "[%d:0] ", (tNode->width)-1);
    while(tNode != NULL){
		fprintf(outp, "%s", tNode->name);
        if (tNode->next == NULL)
            fprintf(outp, ";\n\n");
        else if ((tNode->width) != (tNode->next->width)){
			fprintf(outp, ";\n    %s ", type);
            if (tNode->next->isSigned)
                fprintf(outp, "signed ");
            if (tNode->next->width != 1)
                fprintf(outp, "[%d:0] ", (tNode->next->width) - 1);
        }
        else
            fprintf(outp, ", ");
        tNode = tNode->next;
    }
}