#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lists.h"
#define COMP 5
#define MUX 6
#define MULT 7

void printVars(FILE* outp, varNode* head, char* type);
void printRegs(FILE* outp, listContainer lists);

void writeHeaderAndVars(listContainer lists){
    FILE* outp = fopen(lists.filename, "w");
    varNode* tNode;
    int moduleNameSize = strchr(lists.filename, '.')-lists.filename;
    char* moduleName = (char*)malloc(sizeof(char)*moduleNameSize+1);
    
    strncpy(moduleName, lists.filename, moduleNameSize);
    moduleName[moduleNameSize]=0;
    
    if (outp == NULL){
        printf("Couldn't open \"%s\" for writing", lists.filename);
        return;
    }
    
    fprintf(outp, "`timescale 1ns / 1ps\n\n");
    fprintf(outp, "module %s( Clk, rst, ", moduleName);
    
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
    
    fprintf(outp, "    input Clk, rst;\n");
    //type inputs
    printVars(outp, lists.inputHead, "input");
    printVars(outp, lists.outputHead, "output");
    printVars(outp, lists.wireHead, "wire");
    printRegs(outp, lists);
    
    fclose(outp);
}

void printVars(FILE* outp, varNode* head, char* type){
    //type inputs
    varNode* tNode = head;
    
    if (head == NULL) return;
    
    fprintf(outp, "    %s ", type);
    if (tNode->isSigned)
        fprintf(outp, "signed ");
    if (tNode->width != 1)
        fprintf(outp, "[%d:0] ", (tNode->width)-1);
    while(tNode != NULL){
		fprintf(outp, "%s", tNode->name);
        if (tNode->next == NULL)
            fprintf(outp, ";\n\n");
        else if (
        (tNode->width) != (tNode->next->width) ||
        (tNode->isSigned) != (tNode->next->isSigned)){
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

void printRegs(FILE* outp, listContainer lists) {
	int startWritten = 0;

	varNode* tNode = lists.regHead;
	varNode* addWire;

	while (tNode != NULL) {
		if (!containsVar(lists.outputHead, tNode->name)) {
			
			if (!startWritten) {
				startWritten = 1;
				fprintf(outp, "    wire ");
				if (tNode->isSigned)
					fprintf(outp, "signed ");
				if (tNode->width != 1)
					fprintf(outp, "[%d:0] ", (tNode->width) - 1);
			}
			fprintf(outp, "%s", tNode->name);
			if (tNode->next == NULL)
				fprintf(outp, ";\n\n");
			else if (
            (tNode->width) != (tNode->next->width) ||
            (tNode->isSigned) != (tNode->next->isSigned)) {
				fprintf(outp, ";\n    wire ");
				if (tNode->next->isSigned)
					fprintf(outp, "signed ");
				if (tNode->next->width != 1)
					fprintf(outp, "[%d:0] ", (tNode->next->width) - 1);
			}
			else
				fprintf(outp, ", ");

			addWire = (varNode*)malloc(sizeof(varNode));
			addWire->isSigned = tNode->isSigned;
			addWire->name = (char*)malloc(sizeof(char)*strlen(tNode->name));
			strcpy(addWire->name, tNode->name);
			addWire->next = NULL;
			addWire->width = tNode->width;
			addToList(&lists.wireHead, addWire);
			tNode = tNode->next;

		}
	}

}

void printOp(listContainer lists, int errorCode, int op, int width, int inWidth1, int inWidth2, int outWidth, int signedFlag, char* in1, char* in2, char* in3, char* out)
{
	FILE* outp = fopen(lists.filename, "a");

	if(errorCode == -1)													//parsing occured without error
	{
		switch (op)
		{
			case 0:
				if (signedFlag == 1)
				{
					if( width == inWidth1)
					{
						fprintf(outp,"    SREG #(%d) sreg_%d(%s, Clk, rst, %s);\n", width, lists.opCount[op], in1, out);
					}
					else
					{
						fprintf(outp,"    SREG #(%d) sreg_%d({{%d{%s[%d]}},%s[%d:0]}, Clk, rst, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), out);
					}
				}
				else
				{
					if( width == inWidth1)
					{
						fprintf(outp,"    REG #(%d) reg_%d(%s, Clk, rst, %s);\n", width, lists.opCount[op], in1, out);
					}
					else
					{
						fprintf(outp,"    REG #(%d) reg_%d({{%d'b0},%s[%d:0]}, Clk, rst, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), out);
					}
				}
				break;
			case 1:
				if (signedFlag == 1)
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SADD #(%d) sadd_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    SADD #(%d) sadd_%d(%s, {{%d{%s[%d]}},%s[%d:0]}, %s);\n", width, lists.opCount[op], in1, abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SADD #(%d) sadd_%d({{%d{%s[%d]}},%s[%d:0]}, %s, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    SADD #(%d) sadd_%d({{%d{%s[%d]}},%s[%d:0]}, {{%d{%s[%d]}},%s[%d:0]}, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
				}
				else
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    ADD #(%d) add_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    ADD #(%d) add_%d(%s, {{%d'b0},%s[%d:0]}, %s);\n", width, lists.opCount[op], in1, abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    ADD #(%d) add_%d({{%d'b0},%s[%d:0]}, %s, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    ADD #(%d) add_%d({{%d'b0},%s[%d:0]}, {{%d'b0}},%s[%d:0]}, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
				}
				
				break;
			case 2:
				if (signedFlag == 1)
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SSUB #(%d) ssub_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    SSUB #(%d) ssub_%d(%s, {{%d{%s[%d]}},%s[%d:0]}, %s);\n", width, lists.opCount[op], in1, abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SSUB #(%d) ssub_%d({{%d{%s[%d]}},%s[%d:0]}, %s, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    SSUB #(%d) ssub_%d({{%d{%s[%d]}},%s[%d:0]}, {{%d{%s[%d]}},%s[%d:0]}, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
				}
				else
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SUB #(%d) sub_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    SUB #(%d) sub__%d(%s, {{%d'b0},%s[%d:0]}, %s);\n", width, lists.opCount[op], in1, abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SUB #(%d) sub_%d({{%d'b0},%s[%d:0]}, %s, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    SUB #(%d) sub_%d({{%d'b0},%s[%d:0]}, {{%d'b0}},%s[%d:0]}, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
				}
				break;
			case 3:
				if (signedFlag == 1)
				{
					if(width > inWidth1)
					{
						fprintf(outp,"    SSHR #(%d) sshr_%d({{%d{%s[%d]}},%s[%d:0]},%s[%d:0]}, %s, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), in2,out);
					}
					else
					{
						fprintf(outp,"    SSHR #(%d) sshr_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
					}
				}
				else
				{
					if(width > inWidth1)
					{
						fprintf(outp,"    SHR #(%d) shr_%d({{%d'b0},%s[%d:0]},%s[%d:0]}, %s, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in2,out);
					}
					else
					{
						fprintf(outp,"    SHR #(%d) shr_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
					}
				}

				break;
			case 4:
				if (signedFlag == 1)
				{
					if(width > inWidth1)
					{
						fprintf(outp,"    SSHL #(%d) sshl_%d({{%d{%s[%d]}},%s[%d:0]},%s[%d:0]}, %s, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), in2,out);
					}
					else
					{
						fprintf(outp,"    SSHL #(%d) sshl_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
					}
				}
				else
				{
					if(width > inWidth1)
					{
						fprintf(outp,"    SHL #(%d) shl_%d({{%d'b0},%s[%d:0]},%s[%d:0]}, %s, %s);\n", width, lists.opCount[op], abs(width - inWidth1), in1, in1, (inWidth1 - 1), in2,out);
					}
					else
					{
						fprintf(outp,"    SHL #(%d) shl_%d(%s, %s, %s);\n", width, lists.opCount[op],in1,in2,out);
					}
				}
				break;
			case 5:
				if (signedFlag == 1)
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d(%s, %s, ,%s, );\n", width, lists.opCount[COMP],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d(%s, {{%d{%s[%d]}},%s[%d:0]}, ,%s, );\n", width, lists.opCount[COMP], in1, abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d({{%d{%s[%d]}},%s[%d:0]}, %s, ,%s, );\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d({{%d{%s[%d]}},%s[%d:0]}, {{%d{%s[%d]}},%s[%d:0]}, ,%s, );\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
				}
				else
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp_%d(%s, %s, ,%s, );\n", width, lists.opCount[COMP],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp__%d(%s, {{%d'b0},%s[%d:0]}, ,%s, );\n", width, lists.opCount[COMP], in1, abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp_%d({{%d'b0},%s[%d:0]}, %s, ,%s, );\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    COMP #(%d) comp_%d({{%d'b0},%s[%d:0]}, {{%d'b0},%s[%d:0]}, ,%s, );\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
				}
				break;
			case 6:
				if (signedFlag == 1)
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d(%s, %s, , ,%s);\n", width, lists.opCount[COMP],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d(%s, {{%d{%s[%d]}},%s[%d:0]}, , ,%s);\n", width, lists.opCount[COMP], in1, abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d({{%d{%s[%d]}},%s[%d:0]}, %s, , ,%s);\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d({{%d{%s[%d]}},%s[%d:0]}, {{%d{%s[%d]}},%s[%d:0]}, , ,%s);\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
				}
				else
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp_%d(%s, %s, , ,%s);\n", width, lists.opCount[COMP],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp__%d(%s, {{%d'b0},%s[%d:0]}, , ,%s);\n", width, lists.opCount[COMP], in1, abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp_%d({{%d'b0},%s[%d:0]}, %s, , ,%s);\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    COMP #(%d) comp_%d({{%d'b0},%s[%d:0]}, {{%d'b0},%s[%d:0]}, , ,%s);\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
				}
				
				break;
			case 7:
				if (signedFlag == 1)
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d(%s, %s, %s, , );\n", width, lists.opCount[COMP],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d(%s, {{%d{%s[%d]}},%s[%d:0]}, %s, , );\n", width, lists.opCount[COMP], in1, abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d({{%d{%s[%d]}},%s[%d:0]}, %s, %s, , );\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    SCOMP #(%d) scomp_%d({{%d{%s[%d]}},%s[%d:0]}, {{%d{%s[%d]}},%s[%d:0]}, %s, , );\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
				}
				else
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp_%d(%s, %s, %s, , );\n", width, lists.opCount[COMP],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp__%d(%s, {{%d'b0},%s[%d:0]}, %s, , );\n", width, lists.opCount[COMP], in1, abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    COMP #(%d) comp_%d({{%d'b0},%s[%d:0]}, %s, %s, , );\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    COMP #(%d) comp_%d({{%d'b0},%s[%d:0]}, {{%d'b0}},%s[%d:0]}, %s, , );\n", width, lists.opCount[COMP], abs(width - inWidth1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
				}
				
				break;
			case 8:
				if (signedFlag == 1)
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SMUX2x1 #(%d) smux_%d(%s, %s, %s, %s);\n", width, lists.opCount[MUX],in2,in3,in1,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    SMUX2x1 #(%d) smux_%d(%s, {{%d{%s[%d]}},%s[%d:0]}, %s, %s);\n", width, lists.opCount[MUX], in2, abs(width - inWidth2), in3, (inWidth2 - 1), in3, (inWidth2 - 1), in1, out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SMUX2x1 #(%d) smux_%d({{%d{%s[%d]}},%s[%d:0]}, %s, %s, %s);\n", width, lists.opCount[MUX], abs(width - inWidth1), in2, (inWidth1 - 1), in2, (inWidth1 - 1), in3, in1, out);
					}
					else
					{
						fprintf(outp,"    SMUX2x1 #(%d) smux_%d({{%d{%s[%d]}},%s[%d:0]}, {{%d{%s[%d]}},%s[%d:0]}, %s, %s);\n", width, lists.opCount[MUX], abs(width - inWidth1), in2, (inWidth1 - 1), in2, (inWidth1 - 1), abs(width - inWidth2), in3, (inWidth2 - 1), in3, (inWidth2 - 1), in1, out);
					}
				}
				else
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    MUX2x1 #(%d) mux_%d(%s, %s, %s, %s);\n", width, lists.opCount[MUX],in2,in3,in1,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    MUX2x1 #(%d) mux_%d(%s, {{%d'b0},%s[%d:0]}, %s, %s);\n", width, lists.opCount[MUX], in2, abs(width - inWidth2), in3, (inWidth2 - 1), in1, out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    MUX2x1 #(%d) mux_%d({{%d'b0},%s[%d:0]}, %s, %s, %s);\n", width, lists.opCount[MUX], abs(width - inWidth1), in2, (inWidth1 - 1), in3, in1, out);
					}
					else
					{
						fprintf(outp,"    MUX2x1 #(%d) mux_%d({{%d'b0},%s[%d:0]}, {{%d'b0}},%s[%d:0]}, %s, %s);\n", width, lists.opCount[MUX], abs(width - inWidth1), in2, (inWidth1 - 1), abs(width - inWidth2), in3, (inWidth2 - 1),in1, out);
					}
				}
				//fprintf(outp,"    MUX2x1 #(%d) mux_%d(%s, %s, %s, %s);\n", width, lists.opCount[MUX],in2,in3,in1,out);
				break;
			case 9:
				if (signedFlag == 1)
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SMULT #(%d) smult_%d(%s, %s, %s);\n", width, lists.opCount[MULT],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    SMULT #(%d) smult_%d(%s, {{%d{%s[%d]}},%s[%d:0]}, %s);\n", width, lists.opCount[MULT], in1, abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    SMULT #(%d) smult_%d({{%d{%s[%d]}},%s[%d:0]}, %s, %s);\n", width, lists.opCount[MULT], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    SMULT #(%d) smult_%d({{%d{%s[%d]}},%s[%d:0]}, {{%d{%s[%d]}},%s[%d:0]}, %s);\n", width, lists.opCount[MULT], abs(width - inWidth1), in1, (inWidth1 - 1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), in2, (inWidth2 - 1), out);
					}
				}
				else
				{
					if (width == inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    MULT #(%d) mult_%d(%s, %s, %s);\n", width, lists.opCount[MULT],in1,in2,out);
					}
					else if (width == inWidth1 && width > inWidth2)
					{
						fprintf(outp,"    MULT #(%d) mult_%d(%s, {{%d'b0},%s[%d:0]}, %s);\n", width, lists.opCount[MULT], in1, abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
					else if (width > inWidth1 && width == inWidth2)
					{
						fprintf(outp,"    MULT #(%d) mult_%d({{%d'b0},%s[%d:0]}, %s, %s);\n", width, lists.opCount[MULT], abs(width - inWidth1), in1, (inWidth1 - 1), in2, out);
					}
					else
					{
						fprintf(outp,"    MULT #(%d) mult_%d({{%d'b0},%s[%d:0]}, {{%d'b0}},%s[%d:0]}, %s);\n", width, lists.opCount[MULT], abs(width - inWidth1), in1, (inWidth1 - 1), abs(width - inWidth2), in2, (inWidth2 - 1), out);
					}
				}
				//fprintf(outp,"    MULT #(%d) mult_%d(%s, %s, %s);\n", width, lists.opCount[MUX],in1, in2,out);
				break;
			default:
				fprintf(outp,"//AN UNEXPECTED ERROR HAS OCCURED\n");
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