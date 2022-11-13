#pragma once

//#

#include "INFO.h"
#include "Instructions.h"
#include "Adapter.h"





//STRUCTS

typedef enum byteHalf
{
    LOW_HALF,
    HIGH_HALF
}byteHalf;

typedef struct ByteCode
{
    int posAddr;
    byte code;
    struct ByteCode* nextByte;
}ByteCode;

typedef struct Table
{
    char* name;
    char* value;
    struct Table* nextRow;
}Table;

typedef struct MissingValue
{
    char* name;
    int destPosition;
    argType targetType;
    struct MissingValue* nextValue;
}MissingValue;



//STRUCT FUNCTIONS

ByteCode* createByte(int posAddr, byte code);
ByteCode* addByteCode(ByteCode* byteTable, int posAddr, byte code);
ByteCode* getLastByte(ByteCode* byteTable);
ByteCode* searchByte(ByteCode* byteTable, int posAddr);
void linkBytes(ByteCode* bytes1, ByteCode* bytes2);
void alignBytes(ByteCode* bytes);
void freeByteCode(ByteCode* byteTable);

Table* createTable(char* name, char* value);
Table* addRow(Table* tablePTR, char* name, char* value);
Table* getLastRow(Table* table);
Table* searchTable(Table* table, char* name);
void freeTable(Table* table);

MissingValue* createMisValue(char* name, int destAddr, argType targetType);
MissingValue* addMisValue(MissingValue* misValues, char* name, int destAddr, argType targetType);
MissingValue* getLastMisValue(MissingValue* misValues);
void freeMisValues(MissingValue* misValues);



//FUNCTIONS

ByteCode* compileFile(newFILE* inputFile, Table* globalVars, Rules* rulesTable, MissingValue* missingVals, int* startPos, Table* conversionTable);
Rules* getRules(newFILE* ruleFile, Rules* rulesTable);
byte getVarValue(char* varString, argType targetType, byteHalf bitSelector);

ByteCode* translateInstLine(char* instructionLine, int* startPos, Inst* instructionTable, MissingValue* missingValues, Rules* ruleTable, Table* convTable);
ByteCode* copmileRule(char* ruleInstLine, Rules* rules, int* startPos);
Table* getConversionTable(char* ruleLine, Rules* usedRule);

void resolveLayeredMacros(Table* definedValues);
void replaceMissingValues(ByteCode* bytes, MissingValue* missingValues, Table* definedValues);

ByteCode* assemblePragma(char* lineString, Table* definedVars, Table* globalVars, Table* includes, newFILE* compilingFile, Rules* ruleTable, MissingValue* missingVals, int* startPos);
Table* addLabel(char* lineString, Table* labelTable, int posAddr);
ByteCode* getDirectiveCode(char* lineString, int* firstBytePos, MissingValue* misValues, Table* definedVars);
char* reduceExpression(char* inputString, int currentPos, argType targetType);
