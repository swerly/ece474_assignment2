#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "lists.h"

void writeHeaderAndVars(listContainer lists);
void printOp(listContainer lists, int errorCode, int op, int width, int inWidth1, int inWidth2, int outWidth, int signedFlag, char* in1, char* in2, char* in3, char* out);
void printEndMod(listContainer lists);
#endif