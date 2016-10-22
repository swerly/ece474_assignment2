#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 80

int main(int charc, char** argv){
    //open files for i/o
    FILE *netlistIn = fopen(argv[1], "r");
    FILE *verilogOut = fopen(argv[2], "w");
    char* currentLine[LINE_SIZE];
    
    while(fgets(currentLine, LINE_SIZE, netlistIn)){
        //do things with the current line
    }
    
    fclose(netlistIn);
    fclose(verilogOut);
    return 0;
    
}