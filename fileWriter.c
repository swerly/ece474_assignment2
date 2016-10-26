#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lists.h"
#define COMP 5
#define MUX 6
#define MULT 7

void printVars(FILE* outp, varNode* head, char* type);

void writeHeaderAndVars(listContainer lists){
    FILE* outp = fopen(lists.filename, "w");
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

void printOp(listContainer lists, int errorCode, int op, int width, char* in1, char* in2, char* in3, char* out)
{
	FILE* outp = fopen(lists.filename, "a");

	if(errorCode == -1)													//parsing occured without error
	{
		switch (op)
		{
			case 0:
				fprintf(outp,"    REG #(%d) reg_%d(%s, Clk, rst, %s);\n", width, lists.opCount[op], in1, out);
				break;
			case 1:
				fprintf(outp,"    ADD #(%d) add_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
				break;
			case 2:
				fprintf(outp,"    SUB #(%d) sub_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
				break;
			case 3:
				fprintf(outp,"    SHR #(%d) shr_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
				break;
			case 4:
				fprintf(outp,"    SHL #(%d) shl_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
				break;
			case 5:
				fprintf(outp,"    COMP #(%d) cmp_%d(%s, %s, ,%s, );\n", width, lists.opCount[COMP],in1,in2,out);
				break;
			case 6:
				fprintf(outp,"    COMP #(%d) cmp_%d(%s, %s, , , %s);\n", width, lists.opCount[COMP],in1,in2,out);
				break;
			case 7:
				fprintf(outp,"    COMP #(%d) cmp_%d(%s, %s, %s, , );\n", width, lists.opCount[COMP],in1,in2,out);
				break;
			case 8:
				fprintf(outp,"    MUX2x1 #(%d) mux_%d(%s, %s, %s, %s);\n", width, lists.opCount[MUX],in2,in3,in1,out);
				break;
			case 9:
				fprintf(outp,"    MULT #(%d) mult_%d(%s, %s, %s);\n", width, lists.opCount[MUX],in1, in2,out);
				break;
			default:
				fprintf(outp,"//AN UNEXPECTED ERROR HAS OCCURED");
				break;
		}
	}
	else
	{
		switch (errorCode)
		{
			case 100:
				fprintf(outp,"//INVALID OPERATOR IN NETLIST\n");
				break;
			case 101:
				fprintf(outp,"//INVALID CONDITIONAL STATEMENT IN NETLIST\n");
				break;
			case 102:
				fprintf(outp,"//DATA OUT FOR MODULE COULD NOT BE LOCATED\n");
				break;
			case 103:
				fprintf(outp,"//DATA IN FOR MODULE COULD NOT BE LOCATED\n");
				break;
			default:
				fprintf(outp,"//AN UNEXPECTED ERROR HAS OCCURED");
				break;
		}
	}

	fclose(outp);
	return;
}
void printEndMod(listContainer lists)
{
	FILE* outp = fopen(lists.filename, "a");

	fprintf(outp,"endmodule\n");

	fclose(outp);
}