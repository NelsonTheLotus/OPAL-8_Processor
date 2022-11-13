#pragma once

#include "../headers/INFO.h"





//STRUCTS

typedef enum argType
{
    BIT8,
    IMM8,
    REG,
    BIT16,
    HL,
    IMM16,
    IMM1,
    UDEF
}argType;

typedef struct Args
{
    char name[10];
    argType type;
    struct Args* nextArg;
}Args;

typedef struct Inst
{
    char name [5];
    Args* argList;
    byte opCode;
    struct Inst* nextInst;
}Inst;


typedef struct Rules
{
    char* name;
    Args* argList;
    char* filePath;
    int fileLine;
    struct Rules* nextRule;
}Rules;



//STRUCT FUNCTIONS

Inst* createInst(const char* name, Args* argList, byte opCode, Inst* nextInst);
Inst* initInstTable();
Inst* searchInst(Inst* instTable, char* input);
void freeInstTable(Inst* instTable);

Args* createArg(const char* name, argType type, Args* nextArg);
void freeArgTable(Args* argTable);

Rules* createRule(char* name, Args* argList, char* filePath, int fileLine);
Rules* searchRule(Rules* ruleTable, char* name);
Rules* getLastRule(Rules* list);
void freeRules(Rules* ruleTable);



//FUNCTIONS

argType typeVar(char* varString);
bool typeMatch(argType type, argType target);
argType typeCast(argType varType, argType targetType);

argType getTypeString(char* typeString);
