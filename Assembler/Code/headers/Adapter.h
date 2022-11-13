#pragma once

#include <string.h>

#include "INFO.h"






//STRUCTS

typedef struct newFILE
{
    char filePath[FILEPATH_MAX];
    char* fileName;
    char* extension;
    char fileOpenMode[3];
    char* fileLine;
    int lineNumber;
    int cursorPos;
    FILE* filePTR;
}newFILE;



//STRUCT FUNCTIONS

newFILE* openFILE(char* filePath, const char* openMode, const char* extensionTarget);
char* readFileLine(newFILE* fileStruct);
void closeFILE(newFILE* fileStruct);
void resetFILE(newFILE* fileStruct);



//FUNCTIONS MACROS

#define getFileName(filePath) (strrchr(filePath, systemFolderSeperator) ? strrchr(filePath, systemFolderSeperator) + 1 : filePath)



//FUNCTIONS
//FILES

void removePathRedundancy(char* filePath);


//STRINGS

void removeExtraSpace(char* string);
void removeComment(char* string);
bool getIntValue(char* intString, char endChar, int* resultLoc);


//TYPES

bool isLabel(char* lineString);
bool isDirective(char* lineString);
bool isPragma(char* lineString);
bool isChar(char* string);
bool isModMemChar(char* string);
char* searchClosingPriority(char* inputString);
