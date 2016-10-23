#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "lists.h"

#define INPUT "input"
#define OUTPUT "output"
#define WIRE "wire"
#define REGISTER "register"

void parseInputLine(varNode* list, char* inputLine);
void parseOutputLine(varNode* list, char* outputLine);
void parseWireLine(varNode* list, char* wireLine);
void parseRegisterLine(varNode* list, char* registerLine);
void parseCircuitComponent( listContainer* lists, char* componentLine);

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
		if (strcmp(token, INPUT) == 0){
			parseInputLine(lists->inputHead, line);
			return;
		}
		else if (strcmp(token, OUTPUT) == 0) {
			parseOutputLine(lists->outputHead, line);
			return;
		}
		else if (strcmp(token, WIRE) == 0) {
			parseWireLine(lists->wireHead, line);
			return;
		}
        else if (strcmp(token, REGISTER) == 0) {
            parseRegisterLine(lists->regHead, line);
            return;
        }
		else {
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

//TODO: FILL OUT THESE METHODS
void parseInputLine(varNode* list, char* inputLine) {  }
void parseOutputLine(varNode* list, char* outputLine){  }
void parseWireLine(varNode* list, char* wireLine){ }
void parseRegisterLine(varNode* list, char* registerLine){  }
void parseCircuitComponent(listContainer* lists, char* componentLine){  }