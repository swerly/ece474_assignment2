#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "lists.h"
#include "fileWriter.h"

#define INPUT "input"
#define OUTPUT "output"
#define WIRE "wire"
#define REGISTER "register"
#define COMP 5
#define MUX 6
#define MULT 7

int getWidth(char* token);
int isDataType(char* token);
void parseInputLine(varNode** list, char* inputLine);
void parseCircuitComponent(listContainer* lists, char* componentLine);

char* searchFor(varNode* node, char* token);
char* checkForOut(listContainer* lists, char* token);
char* checkForIn(listContainer* lists, char* token);
int checkWidth(listContainer* lists, char* token);

void beginParsingLine(listContainer* lists, char* line) {
	char* token;
	char* commentCheck;

	//if the string is a return, 
	//then its empty and we can move to the next line
	/*if(strcmp(line,"\n") || strcmp(line,"\r\n")){
	   fprintf(verilogOut, "NEWLINE\n");
	   return;
	} else {*/
	//get first token
	token = strtok(line, " \t");
	if (token == NULL) return;

	//allocate string to check and see if the first 2 chars
	//of the current line are a comment
	commentCheck = (char*)malloc(sizeof(char) * 3);
	//copy the first 2 chars from the string to our commentCheck var
	strncpy(commentCheck, token, 2);
	//set the null terminating flag
	commentCheck[2] = '\0';

	//check if commentCheck is equal to a comment
	//if so free and exit, else free and continue
	if ((strcmp(commentCheck, "//") == 0) || (strcmp(commentCheck, "\n") == 0)) {
		//this is a comment line or a new line and we can skip it
		free(commentCheck);
		return;
	}
	free(commentCheck);

	//while our current token isn't NULL
	while (token != NULL) {
		//check what we need to do with the current line
		if (strcmp(token, INPUT) == 0) {
			parseInputLine(&(lists->inputHead), line);
			return;
		}
		else if (strcmp(token, OUTPUT) == 0) {
			parseInputLine(&(lists->outputHead), line);
			return;
		}
		else if (strcmp(token, WIRE) == 0) {
			parseInputLine(&(lists->wireHead), line);
			return;
		}
		else if (strcmp(token, REGISTER) == 0) {
			parseInputLine(&(lists->regHead), line);
			return;
		}
		else {
			if (!lists->headerAndVarsWritten) {
				writeHeaderAndVars(*lists);
				lists->headerAndVarsWritten = 1;
			}
			//else we are past the inputs and outputs and we need to start handling the actual circuit
			parseCircuitComponent(lists, line);
			return;
		}


		token = strtok(NULL, " \t\n");
	}

	//we are done with all the tokens in the string
	return;
	//}
}

void endPrint(listContainer lists)
{
	printEndMod(lists);

	return;
}

int getWidth(char* token) {
	switch (token[strlen(token) - 1]) {
	case '1':
		return 1;
	case '2':
		return (token[strlen(token) - 2] == '3') ? 32 : 2;
	case '4':
		return 64;
	case '6':
		return 16;
	case '8':
		return 8;
	default:
		return -1;
	}
}

int isDataType(char* token) {
	return (strncmp(token, "Int", 3) == 0) || (strncmp(token, "UIn", 3) == 0);
}

void parseInputLine(varNode** list, char* inputLine) {
	varNode* tempVar;
	char* token = strtok(NULL, " ,\t\n");
	int currentIsSigned = -1;
	int currentWidth = -1;
	char* tempName;

	//while we are still pulling things from the line
	while (token != NULL) {
		//if we need to set the initial attributes
		if (isDataType(token)) {
			//set attributes based on dataType token
			currentIsSigned = (token[0] == 'I') ? 1 : 0;
			currentWidth = getWidth(token);
		}
		else {
			//make a new node and add it to the list
			tempVar = (varNode*)malloc(sizeof(varNode));
			tempVar->isSigned = currentIsSigned;
			tempName = (char*)malloc(sizeof(char)*strlen(token));
			strcpy(tempName, token);
			tempVar->name = tempName;
			tempVar->width = currentWidth;
			tempVar->next = NULL;
			addToList(list, tempVar);
		}
		token = strtok(NULL, " ,\t\n");
	}
}

void parseCircuitComponent(listContainer* lists, char* componentLine)
{
	varNode* node = NULL;
	int currop = -1;														//this variable denotes what the current operation being performed is. -1 = invalid, 0 = reg, 1 = add, 2 = sub, 3 = shiftr,
																			//4 = shiftl, 5-7 = comp, 8 = mux, 9 = mul
	int errorCode = -1;														//error code to be transmitted to the printer, will correspond to the type of invalid input, -1 means no error
	int opFound = 0;														//indicates if the operation has been found which determines if the operation is a register or not
	char* token;															//creates a token string for first string before the whitespace
	char* out = NULL;														//temporary strings created to hold a name prior to printing
	char* in1 = NULL;
	char* in2 = NULL;
	char* in3 = NULL;
	int width = 0;

	int loopCount = 0;														//keeps track of the position in the loop for printing purposes
	int validCheck = 1;														//the line is valid unless the parsing finds an invalid character in which case this flag goes low

	token = componentLine;

	while(token != NULL && validCheck == 1)
	{
		if((*token > 64 && *token < 90) || (*token > 96 && *token < 123))	//if the ascii value of the first character falls into these catagories then it isnt an operation
		{																	//works under the assumption that verilog can only have variables that start with a letter and special characters aren't 
																			//alowed for the first character. (ex. _var1, 1var2, /var3, %var4, etc are not acceptable)
			if (loopCount == 0)
			{
				out = checkForOut(lists, token);							//checks wires, outputs, and regs
				if (out == NULL)											//if out is still NULL then the output/register/wire couldnt be found
				{
					validCheck = 0;
					errorCode = 102;										//error code 102 corresponds to output/register/wire not found
					break;
				}
				else
				{
					width = checkWidth(lists, token);
				}
			}
			else if(loopCount == 2)
			{
				in1 = checkForIn(lists, token);
				if (in1 == NULL)
				{
					validCheck = 0;
					errorCode = 103;										//103 implies that an input wasnt found differs from 102 in that a different portion of the netlist is incorrect
				}
			}
			else if(loopCount > 2)
			{
				//check inputs and wires
				if (currop != -1)											//there are only other inputs if the operation isnt a register which isnt determined until the end
				{
					in2 = checkForIn(lists, token);
					if(in2 == NULL)
					{
						validCheck = 0;
						errorCode = 103;									//input wire/input couldnt be found
					}
					if(currop == 8)
					{
						token = strtok(NULL, " ,\t\n");						//grabs the next string
						if(strcmp(token,":") != 0)							//if the string isnt a colon then it isnt a correct conditional statement
						{
							validCheck = 0;
							errorCode = 101;								//error 101 implies an incorrect conditional
						}
						else												//otherwise continue on and get the third input
						{
							token = strtok(NULL, " ,\t\n");
							in3 = checkForIn(lists, token);
							if(in3 == NULL)
							{
								validCheck = 0;
								errorCode = 103;							//input wire/input couldnt be found
							}
						}
					}
				}
			}
		}
		else																//the token is either an operation or an invalid character
		{
			if(loopCount == 1)
			{
				if(strcmp(token,"=") == 0)
				{
					validCheck = 1;
				}
			}
			else if(loopCount == 3)
			{
				if(strcmp(token,"+") == 0)										//line is addition
				{
					currop = 1;													//current operation is set to addtion
					lists->opCount[currop]++;									//the number of adders is incrememnted
					opFound = 1;												//an operation has been found
				}
				else if(strcmp(token,"-") == 0)									//line is subtraction
				{
					currop = 2;
					lists->opCount[currop]++;
					opFound = 1;
				}
				else if(strcmp(token,">>") == 0)								//line is right shift
				{
					currop = 3;
					lists->opCount[currop]++;
					opFound = 1;
				}
				else if(strcmp(token,"<<") == 0)								//line is left shift
				{
					currop = 4;
					lists->opCount[currop]++;
					opFound = 1;
				}
				else if(strcmp(token,"<") == 0)									//line is a less than comparison
				{																//opcount uses COMP because comparators are the same module in verilog but need to be treated differently
					currop = 5;													//based on what port is needed for a GT,LT, or ET comparison
					lists->opCount[COMP]++;
					opFound = 1;
				}
				else if(strcmp(token,"==") == 0)								//line is a equal to comparison
				{
					currop = 6;
					lists->opCount[COMP]++;
					opFound = 1;
				}
				else if(strcmp(token,">") == 0)									//line is a greater than comparison
				{
					currop = 7;
					lists->opCount[COMP]++;
					opFound = 1;
				}
				else if(strcmp(token,"?") == 0)									//line is a mux
				{
					currop = 8;
					lists->opCount[MUX]++;										//MUX is defined as 6 to accomodate for 3 types of comparison
					opFound = 1;
				}
				else if(strcmp(token,"*") == 0)									//line is multiplication
				{
					currop = 9;
					lists->opCount[MULT]++;										//MUX is defined as 6 to accomodate for 3 types of comparison
					opFound = 1;
				}
				else															//operation is invalid
				{
					currop = -1;
					validCheck = 0;
					errorCode = 100;											//error code 100 coresponds to invalid operator
				}
			}
			else																//an operation/reserved character was found outside of the second token meaning the line is invalid
			{
				if(loopCount == 5 && currop != 6 && *token != 58 )				//accounts for a mux, if the 6th token isn't a colon then the line is invalid
				{
					validCheck = 0;
					errorCode = 101;											//incorrect conditional statement
				}
				else if(loopCount != 5)											//an operator has been found where it shouldnt be
				{
					validCheck = 0;
					errorCode = 100;											//invalid operator
				}
			}
		}

		loopCount++;
		token = strtok(NULL, " ,\t\n");

	}
	if(opFound == 0 && validCheck == 1)
	{
		currop = 0;																//register operation
		lists->opCount[currop]++;
	}
	printOp(*lists, errorCode, currop, width, in1, in2, in3, out);
	
	return;
}

char* checkForOut(listContainer* lists, char* token)
{
	char* out = NULL;
	varNode* node = lists->wireHead;							//checks wires first
	out = searchFor(node, token);
	if (out == NULL)
	{
		node = lists->outputHead;								//checks outputs second
		out = searchFor(node, token);
		if(out == NULL)
		{
			node = lists->regHead;								//checks registers third
			out = searchFor(node, token);
		}
	}

	return out;
}

char* checkForIn(listContainer* lists, char* token)
{
	char* in = NULL;
	varNode* node = lists->inputHead;

	in = searchFor(node, token);
	if(in == NULL)
	{
		node = lists->wireHead;
		in = searchFor(node,token);
	}

	return in;
}

char* searchFor(varNode* node, char* token)										//function searches through the nodes of a given list to find a name
{
	char* out = NULL;															//if out stays NULL then the list did not contain the name
	int found = 0;

	while (node != NULL && found == 0)
	{
		if (strcmp((node->name), token) == 0)
		{
			found = 1;
			out = token;
		}
		node = node->next;
	}

	return out;
}

int checkWidth(listContainer* lists, char* token)
{
	int width = 0;
	int found = 0;
	char* match;
	varNode* node = lists->wireHead;
	match = searchFor(node, token);

	while (node != NULL && found == 0)						//checks the wires for the output width of the lines output
	{
		if (strcmp((node->name), token) == 0)
		{
			found = 1;
			width = node->width;
			return width;
		}
		node = node->next;
	}
	node = lists->outputHead;						//checks outputs next
	while (node != NULL && found == 0)
	{
		if (strcmp((node->name), token) == 0)
		{
			found = 1;
			width = node->width;
			return width;
		}
		node = node->next;
	}
	node = lists->regHead;							//checks registers last
	while (node != NULL && found == 0)
	{
		if (strcmp((node->name), token) == 0)
		{
			found = 1;
			width = node->width;
			return width;
		}
		node = node->next;							
	}
	return -1;												//returns -1 if the node doesn't exist however this is unlikely to occur
}