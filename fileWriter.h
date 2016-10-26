#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "lists.h"

void writeHeaderAndVars(listContainer lists);
void printOp(listContainer lists, int errorCode, int op, int width, char* in1, char* in2, char* in3, char* out);
void printEndMod(listContainer lists);
#endif