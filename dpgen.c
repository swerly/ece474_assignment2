#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lists.h"


#define LINE_SIZE 80

int main(int argc, char** argv){
    FILE *netlistIn;
    char currentLine[LINE_SIZE];
    
    listContainer lists = {
        .inputHead = NULL,
        .outputHead = NULL,
        .regHead = NULL,
        .wireHead = NULL
    };
    
    //check for correct argument count
    if (argc < 3){
        printf("\nUsage:\n\ndpgen inputNetlist outputVerilog\n\n");
        return 0;
    }
    
    //open input file for reading
    netlistIn = fopen(argv[1], "r");
    
    //check if our input can be found, if not exit
    if (netlistIn == NULL){
        printf("\nFile \"%s\" not found. Exiting...\n\n", argv[1]);
        return 0;
    }
    
    //start scanning line by line
    while(fgets(currentLine, LINE_SIZE, netlistIn)){
        //do things with the current line
        // printf("%s\n", currentLine);
        beginParsingLine(&lists, currentLine);
    }
    
    //close our files
    fclose(netlistIn);
    return 0;
    
}