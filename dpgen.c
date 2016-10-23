#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"


#define LINE_SIZE 80

int main(int argc, char** argv){
    FILE *netlistIn;
    FILE *verilogOut;
    char currentLine[LINE_SIZE];
    
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
    //open file for output
    //do this after checking the input so we dont needlessly create this file
    verilogOut = fopen(argv[2], "w");
    
    
    //start scanning line by line
    while(fgets(currentLine, LINE_SIZE, netlistIn)){
        //do things with the current line
        // printf("%s\n", currentLine);
        beginParsingLine(currentLine, verilogOut);
    }
    
    
    //close our files
    fclose(netlistIn);
    fclose(verilogOut);
    return 0;
    
}