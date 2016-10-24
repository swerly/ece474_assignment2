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

int getWidth(char* token);
int isDataType(char* token);
void parseInputLine(varNode** list, char* inputLine);
void parseCircuitComponent(listContainer* lists, char* componentLine);

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

//TODO: FILL OUT THESE METHODS
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

void parseCircuitComponent(listContainer* lists, char* componentLine){  }