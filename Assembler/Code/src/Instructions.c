#include "../headers/Instructions.h"

#include <string.h>

#include "../headers/Error.h"





//STRUCT FUNCTIONS

Inst* createInst(const char* name, Args* argList, byte opCode, Inst* nextInst)
{
    if(strlen(name) > 5)
    {
        return NULL;
    }

    Inst* newInst = (Inst*)malloc(sizeof(Inst));
    setErrorIf(newInst == NULL, ALLOC_ERR, NULL);

    strcpy(newInst->name, name);
    newInst->argList = argList;
    newInst->opCode = opCode;
    newInst->nextInst = nextInst;

    return newInst;
}


Inst* initInstTable()
{
    Args* sourceBIT8 = createArg("source", BIT8, NULL);
    Args* destREG = createArg("dest", REG, sourceBIT8);

    Args* sourceBIT16 = createArg("source", BIT16, NULL);
    Args* destREG16 = createArg("dest", REG, sourceBIT16);

    Args* destBIT16 = createArg("dest", BIT16, NULL);
    Args* sourceREG = createArg("source", REG, destBIT16);

    Args* boolean = createArg("bool", IMM1, NULL);

    Args* destREGDEF = createArg("dest", REG, NULL);
    Args* sourceBIT8DEF = createArg("source", BIT8, NULL);
    Args* destBIT16DEF = createArg("dest", BIT16, NULL);


    Inst* MVB = createInst("mvb", destREG, 0xF0, NULL);
    Inst* CMP = createInst("cmp", destREG, 0xE0, MVB);
    Inst* SUB = createInst("sub", destREG, 0xD0, CMP);
    Inst* ADD = createInst("add", destREG, 0xC0, SUB);
    Inst* ADC = createInst("adc", destREG, 0xB0, ADD);
    Inst* OR = createInst("or", destREG, 0xA0, ADC);
    Inst* AND = createInst("and", destREG, 0x90, OR);
    Inst* NOT = createInst("not", destREG, 0x80, AND);

    Inst* IET = createInst("iet", boolean, 0x78, NOT);
    Inst* INT = createInst("int", NULL, 0x70, IET);

    Inst* OUB = createInst("oub", destREGDEF, 0x68, INT);
    Inst* INB = createInst("inb", destREGDEF, 0x60, OUB);

    Inst* POP = createInst("pop", destREGDEF, 0x50, INB);
    Inst* PUSH = createInst("push", sourceBIT8DEF, 0x40, POP);

    Inst* STB = createInst("stb", sourceREG, 0x30, PUSH);

    Inst* JNE = createInst("jne", destBIT16DEF, 0x20, STB);

    Inst* LDB = createInst("ldb", destREG16, 0x10, JNE);

    Inst* NOP = createInst("nop", NULL, 0x08, LDB);

    return NOP;
}


Inst* searchInst(Inst* instTable, char* input)
{
    Inst* currentInst = instTable;

    while((currentInst != NULL))
    {
        if(!strcmp(currentInst->name, input))
        {
            break;
        }
        currentInst = currentInst->nextInst;
        continue;
    }

    return currentInst;
}


void freeInstTable(Inst* instTable)
{
    Inst* previousInst = instTable;
    Inst* nextInst = NULL;
    Args* nullArg = createArg("pointer", UDEF, NULL);
    Args* currentNullArg = nullArg;

    printf("FREEING InsT\n");
    while(previousInst != NULL)
    {
        printf("FREED ONE: %s\n", previousInst->name);
        nextInst = previousInst->nextInst;
        if(previousInst->argList != NULL)
        {
            if(strcmp(previousInst->argList->name, ""))
            {
                freeArgTable(previousInst->argList->nextArg);
                strcpy(previousInst->argList->name, "");
                previousInst->argList->nextArg = NULL;
                currentNullArg->nextArg = previousInst->argList;
                currentNullArg = previousInst->argList;
            }
        }
        free(previousInst);
        previousInst = nextInst;
    }
    freeArgTable(nullArg);
    printf("DONE INST\n");
    return;
}



Args* createArg(const char* name, argType type, Args* nextArg)
{
    printf("Assigning\n");
    if(strlen(name) > 9)
    {
        return NULL;
    }

    printf("Assigning\n");
    Args* newArg = (Args*)malloc(sizeof(Args));
    setErrorIf(newArg == NULL, ALLOC_ERR, NULL);

    printf("Assigning\n");
    strcpy(newArg->name, name);
    newArg->type = type;
    newArg->nextArg = nextArg;

    return newArg;
}


void freeArgTable(Args* argTable)
{
    Args* previousArg = argTable;
    Args* nextArg = NULL;

    while(previousArg != NULL)
    {
        nextArg = previousArg->nextArg;
        free(previousArg);
        previousArg = nextArg;
    }
    return;
}



Rules* createRule(char* name, Args* argList, char* filePath, int fileLine)
{
    setErrorIf(name == NULL, NULLARG_ERR, NULL);
    setErrorIf(filePath == NULL, NULLARG_ERR, NULL);

    Rules* newRule = (Rules*)malloc(sizeof(Rules));
    setErrorIf(newRule == NULL, ALLOC_ERR, NULL);

    char* ruleName = (char*)malloc(sizeof(char)*(strlen(name)+1));
    setErrorIf(ruleName == NULL, ALLOC_ERR, NULL);
    strcpy(ruleName, name);

    char* rulePath = (char*)malloc(sizeof(char)*(strlen(filePath)+1));
    setErrorIf(rulePath == NULL, ALLOC_ERR, NULL);
    strcpy(rulePath, filePath);

    newRule->name = ruleName;
    newRule->argList = argList;
    newRule->filePath = rulePath;
    newRule->fileLine = fileLine;
    newRule->nextRule = NULL;
}


Rules* searchRule(Rules* ruleTable, char* name)
{
    setErrorIf(name == NULL, NULLARG_ERR, NULL);

    Rules* currentRule = ruleTable;
    printf("Searching for string matching |%s|\n", name);

    while(currentRule != NULL)
    {
        printf("looking at string |%s|\n", currentRule->name);
        if(!strcmp(currentRule->name, name))
        {
            break;
        }
        currentRule = currentRule->nextRule;
    }

    return currentRule;
}


Rules* getLastRule(Rules* list)
{
    Rules* currentRule = list;
    Rules* previousRule = list;

    while(currentRule != NULL)
    {
        previousRule = currentRule;
        currentRule = currentRule->nextRule;
    }

    return previousRule;
}


void freeRules(Rules* ruleTable)
{
    Rules* currentRule = ruleTable;
    Rules* nextRule;

    while(currentRule != NULL)
    {
        nextRule = currentRule->nextRule;
        free(currentRule->name);
        free(currentRule->filePath);
        free(currentRule);
        currentRule = nextRule;
    }
    return;
}



//FUNCTIONS

argType typeVar(char* varString)
{
    setErrorIf(varString == NULL, NULLARG_ERR, UDEF);

    int varValue;
    //printf("Typing %s\n", varString);

    if(isChar(varString))
    {
        return IMM8;
    }
    if(strlen(varString) == 2)
    {
        if(varString[1] == 'x')
        {
            if(varString[0] <= 'f' && varString[0] >= 'a')
            {
                return REG;
            }
            if(varString[0] == 'h' || varString[0] == 'l')
            {
                return REG;
            }
        }
    }

    if(!strcmp(varString, "hl"))
    {
        return HL;
    }

    if(getIntValue(varString, '\0', &varValue))
    {
        if(varValue < 256)
        {
            return IMM8;
        }
        if(varValue < 65536)
        {
            return IMM16;
        }
    }

    return UDEF;
}


bool typeMatch(argType type, argType target)
{
    if(target == BIT8)
    {
        if (type == REG || type == IMM8)
        {
            return true;
        }
        return false;
    }

    if(target == BIT16)
    {
        if (type == IMM16 || type == IMM8 || type == HL)
        {
            return true;
        }
        return false;
    }

    if(target == IMM16 || target == IMM1)
    {
        if(type == IMM16 || type == IMM8)
        {
            return true;
        }
        return false;
    }

    if(type == target)
    {
        return true;
    }
    return false;

}


argType typeCast(argType varType, argType targetType)
{
    //setErrorIf(!typeMatch(varType, targetType), TYPEMATCH_ERR, UDEF);

    argType resultType = varType;
    if(targetType == BIT16)             //TYPE CASTING
    {
        if(varType == IMM8)
        {
            resultType = IMM16;
        }
    }

    if(targetType == IMM1)              //TYPE CASTING
    {
        if(varType == IMM8 || varType == IMM16)
        {
            resultType = IMM1;
        }
    }

    return resultType;
}



argType getTypeString(char* typeString)
{
    argType newType = UDEF;

    printf("Getting Type of %s\n", typeString);

    if(!strcmp(typeString, "BIT8"))
    {
        newType = BIT8;
    }
    else if(!strcmp(typeString, "IMM8"))
    {
        newType = IMM8;
    }
    else if(!strcmp(typeString, "REG"))
    {
        newType = REG;
    }
    else if(!strcmp(typeString, "BIT16"))
    {
        newType = BIT16;
    }
    else if(!strcmp(typeString, "IMM16"))
    {
        newType = IMM16;
    }
    else if(!strcmp(typeString, "HL"))
    {
        newType = HL;
    }
    else if(!strcmp(typeString, "IMM16"))
    {
        newType = IMM16;
    }
    else if(!strcmp(typeString, "IMM1"))
    {
        newType = IMM1;
    }

    printf("Got it\n");
    return newType;
}
