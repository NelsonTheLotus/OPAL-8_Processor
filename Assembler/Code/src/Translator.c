#include "../headers/Translator.h"

#include <stdlib.h>

#include "../headers/Error.h"
#include "../headers/INFO.h"
#include "../headers/Adapter.h"
#include "../headers/Instructions.h"





//STRUCT FUNCTIONS

ByteCode* createByte(int posAddr, byte code)
{
    ByteCode* newByte = (ByteCode*)malloc(sizeof(ByteCode));
    setErrorIf(newByte == NULL, ALLOC_ERR, NULL);

    newByte->posAddr = posAddr;
    newByte->code = code;
    newByte->nextByte = NULL;

    return newByte;
}


ByteCode* addByteCode(ByteCode* byteTable, int posAddr, byte code)
{
    setErrorIf(byteTable == NULL, NULLARG_ERR, NULL);

    ByteCode* lastByte = getLastByte(byteTable);

    /*if(posAddr < 0)
    {
        posAddr = lastByte->posAddr+1;
    }*/

    ByteCode* newByte = createByte(posAddr, code);
    lastByte->nextByte = newByte;
    return newByte;
}


ByteCode* getLastByte(ByteCode* byteTable)
{
    exitIf(byteTable == NULL, NULL);

    ByteCode* lastByte = byteTable;
    while(lastByte->nextByte != NULL)
    {
        lastByte = lastByte->nextByte;
        continue;
    }

    return lastByte;
}


ByteCode* searchByte(ByteCode* byteTable, int posAddr)
{
    ByteCode* currentByte = byteTable;
    int currentAddr = -1;

    while(currentByte != NULL)
    {
        if(currentByte->posAddr < 0)
        {
            currentAddr++;
        }
        else{
            currentAddr = currentByte->posAddr;
        }

        if(currentAddr == posAddr)
        {
            printf("Found at curAd %d looking for %d\n", currentAddr, posAddr);
            break;
        }

        currentByte = currentByte->nextByte;
    }

    return currentByte;
}


void linkBytes(ByteCode* bytes1, ByteCode* bytes2)
{
    setErrorIf(bytes1 == NULL, NULLARG_ERR, );

    ByteCode* lastByte = getLastByte(bytes1);

    lastByte->nextByte = bytes2;
    //CHECK IF BYTES ARE LOOPING

    return;
}


void alignBytes(ByteCode* bytes)
{
    setErrorIf(bytes == NULL, NULLARG_ERR, );

    ByteCode* currentByte = bytes;
    int lastAdrr = -1;

    while(currentByte != NULL)
    {
        if(currentByte->posAddr < 0)
        {
            currentByte->posAddr = lastAdrr+1;
        }
        lastAdrr = currentByte->posAddr;
        currentByte = currentByte->nextByte;
    }

    return;
}


void freeByteCode(ByteCode* byteTable)
{
    ByteCode* currentByte = byteTable;
    ByteCode* nextByte;

    while(currentByte != NULL)
    {
        nextByte = currentByte->nextByte;
        free(currentByte);
        currentByte = nextByte;
    }
    return;
}



Table* createTable(char* name, char* value)
{
    setErrorIf(name == NULL, NULLARG_ERR, NULL);
    setErrorIf(value == NULL, NULLARG_ERR, NULL);

    Table* newTable = (Table*)malloc(sizeof(Table));
    setErrorIf(newTable == NULL, ALLOC_ERR, NULL);

    int nameLength = strlen(name);
    newTable->name = (char*)malloc(sizeof(char)*(nameLength+1));
    setErrorIf(newTable->name == NULL, ALLOC_ERR, NULL);

    int valueLength = strlen(value);
    newTable->value = (char*)malloc(sizeof(char)*(valueLength+1));
    setErrorIf(newTable->value == NULL, ALLOC_ERR, NULL);

    strcpy(newTable->name, name);
    strcpy(newTable->value, value);
    newTable->nextRow = NULL;

    return newTable;
}


Table* addRow(Table* tablePTR, char* name, char* value)
{
    setErrorIf(tablePTR == NULL, NULLARG_ERR, NULL);

    Table* lastRow = getLastRow(tablePTR);

    Table* newRow = createTable(name, value);
    lastRow->nextRow = newRow;
    return newRow;
}


Table* getLastRow(Table* table)
{
    exitIf(table == NULL, NULL);

    Table* lastRow = table;
    while(lastRow->nextRow != NULL)
    {
        lastRow = lastRow->nextRow;
        continue;
    }

    return lastRow;
}


Table* searchTable(Table* table, char* name)
{
    exitIf(table == NULL, NULL);
    setErrorIf(name == NULL, NULLARG_ERR, NULL);

    Table* currentRow = table;

    while(currentRow != NULL)
    {
        if(!strcmp(currentRow->name, name))
        {
            break;
        }
        currentRow = currentRow->nextRow;
    }

    return currentRow;
}


void freeTable(Table* table)
{
    Table* currentRow = table;
    Table* nextRow;

    while(currentRow != NULL)
    {
        //printf("Freed one row\n");
        nextRow = currentRow->nextRow;
        free(currentRow->name);
        free(currentRow->value);
        free(currentRow);
        currentRow = nextRow;
    }
    return;
}



MissingValue* createMisValue(char* name, int destAddr, argType targetType)
{
    setErrorIf(name == NULL, NULLARG_ERR, NULL);

    MissingValue* newValue = (MissingValue*)malloc(sizeof(MissingValue));
    setErrorIf(newValue == NULL, ALLOC_ERR, NULL);

    int nameLength = strlen(name);
    newValue->name = (char*)malloc(sizeof(char)*(nameLength+1));
    setErrorIf(newValue->name == NULL, ALLOC_ERR, NULL);

    strcpy(newValue->name, name);
    newValue->destPosition = destAddr;
    newValue->targetType = targetType;
    newValue->nextValue = NULL;

    return newValue;
}


MissingValue* addMisValue(MissingValue* misValues, char* name, int destAddr, argType targetType)
{
    setErrorIf(misValues == NULL, NULLARG_ERR, NULL);

    MissingValue* lastValue = getLastMisValue(misValues);

    MissingValue* newValue = createMisValue(name, destAddr, targetType);
    lastValue->nextValue = newValue;
    return newValue;
}


MissingValue* getLastMisValue(MissingValue* misValues)
{
    exitIf(misValues == NULL, NULL);

    MissingValue* lastValue = misValues;
    while(lastValue->nextValue != NULL)
    {
        lastValue = lastValue->nextValue;
        continue;
    }

    return lastValue;
}


void freeMisValues(MissingValue* misValues)
{
    MissingValue* currentValue = misValues;
    MissingValue* nextValue;

    while(currentValue != NULL)
    {
        nextValue = currentValue->nextValue;
        free(currentValue->name);
        free(currentValue);
        currentValue = nextValue;
    }
    return;
}



//FUNCTIONS

ByteCode* compileFile(newFILE* inputFile, Table* globalVars, Rules* rulesTable, MissingValue* missingVals, int* startPos, Table* conversionTable)
{
    setErrorIf(inputFile == NULL, NULLARG_ERR, NULL);
    setCompiledFile(inputFile);

    ByteCode* byteTable = createByte(-1, 0x00);
    ByteCode* newBytes = NULL;
    Inst* instTable = initInstTable();
    Table* labelTable = createTable("Labels", "-1");
    Table* defineTable = createTable("Define", "-1");
    Table* globalTable;
    if(globalVars != NULL)
    {
        globalTable = globalVars;
    }
    else
    {
        globalTable = createTable("Global", "-1");
    }
    Rules* ruleTable;
    if(rulesTable != NULL)
    {
        ruleTable = rulesTable;
    }
    else
    {
        ruleTable = createRule("template", NULL, "none", 0);
    }
    Table* includeTable = createTable("Include", "-1");
    MissingValue* missingValues = createMisValue("MisValues", -1, UDEF);
    int nextBytePos = 0;
    if(startPos != NULL)
    {
        nextBytePos = *startPos;
    }

    char* fileExtension = strchr(inputFile->fileName, '.');
    bool isNotRule = strcmp(fileExtension, RULES_EXTENSION);
    char* openRule = NULL;
    char* closeRule = NULL;

    while(readFileLine(inputFile))
    {
        if(!isNotRule)
        {
            closeRule = strchr(inputFile->fileLine, CLOSERULE_CHAR);
            if(openRule == NULL)
            {
                openRule = strchr(inputFile->fileLine, OPENRULE_CHAR);
                setErrorIf(openRule == NULL, 100, NULL);
                strcpy(inputFile->fileLine, openRule+1);
            }
            else if(closeRule != NULL)
            {
                if(closeRule == inputFile->fileLine)
                {
                    printf("\n\n\n\n BORKE OUT \n\n\n\n\n");
                    break;
                }
                closeRule[0] = '\0';
            }
        }
        removeComment(inputFile->fileLine);
        removeExtraSpace(inputFile->fileLine);
        if(inputFile->fileLine[0] == '\0')
        {
            continue;
        }
        printf("New string: %s|\n", inputFile->fileLine);
        sysCallError(kill, "stop.\n");

        if(isLabel(inputFile->fileLine))
        {
            addLabel(inputFile->fileLine, labelTable, nextBytePos);
            sysCallError(kill, "stop.\n");
        }
        else if(isPragma(inputFile->fileLine))
        {
            newBytes = assemblePragma(inputFile->fileLine, defineTable, globalTable, includeTable, inputFile, ruleTable, missingValues, &nextBytePos);
            asmCallError(kill, "Could not resolve pragma\n");
        }
        else if(isDirective(inputFile->fileLine))
        {
            Table* lastDefine = getLastRow(defineTable);
            lastDefine->nextRow = globalTable->nextRow;
            newBytes = getDirectiveCode(inputFile->fileLine, &nextBytePos, missingValues, defineTable);
            asmCallError(kill, "Could not extract directive bytes\n");
            lastDefine->nextRow = NULL;
        }
        else
        {
            printf("Next byte pos is %d\n", nextBytePos);
            newBytes = translateInstLine(inputFile->fileLine, &nextBytePos, instTable, missingValues, ruleTable->nextRule, conversionTable);
            asmCallError(kill, "Could not translate line.\n");
        }

        sysCallError(kill, "stop.\n");
        printf("Got bytes\n");
        ByteCode* currentByte = newBytes;
        ByteCode* nextByte;
        while(currentByte != NULL)
        {
            nextByte = currentByte->nextByte;
            printf("New byte at pos %d: 0x%x\n", currentByte->posAddr, currentByte->code);
            currentByte = nextByte;
        }
        linkBytes(byteTable, newBytes);
        printf("Wrote bytes\n");
        newBytes = NULL;
        if(closeRule != NULL)
        {
            break;
        }
        continue;
    }

    Table* lastLabel = getLastRow(labelTable);
    lastLabel->nextRow = defineTable->nextRow;
    Table* lastDefine = getLastRow(labelTable);
    lastDefine->nextRow = globalTable->nextRow;
    Table* definedVars = labelTable->nextRow;

    //resolveLayeredMacros(definedVars);
    asmCallError(kill, "could not resolve layered macros.\n");
    if(!strcmp(fileExtension, RULES_EXTENSION))
    {
        /*MissingValue* returnValues = missingVals;
        MissingValue* currentMisVal = missingValues;*/
        MissingValue* previousLocalMisVal = missingValues;
        MissingValue* currentLocalMisVal = missingValues->nextValue;
        MissingValue* currentReturnMisVal = getLastMisValue(missingVals);

        while(currentLocalMisVal != NULL)
        {
            //if(typeVar)
            printf("Searching for row matching name |%s|\n", currentLocalMisVal->name);
            Table* resolvedValue = searchTable(conversionTable, currentLocalMisVal->name);
            if(resolvedValue != NULL)
            {
                printf("Found value\n");
                if(typeVar(resolvedValue->value) == UDEF)
                {
                    setErrorValue(NO_ERR);
                    printf("Var type is udef, will look further up the satck\n");
                    if(previousLocalMisVal != NULL)
                    {
                        previousLocalMisVal->nextValue = currentLocalMisVal->nextValue;
                    }
                    /*else
                    {
                        missingValues->nextValue = currentLocalMisVal->nextValue;
                    }*/

                    if(currentReturnMisVal != NULL)
                    {
                        currentReturnMisVal->nextValue = currentLocalMisVal;
                        currentReturnMisVal = currentReturnMisVal->nextValue;
                        strcpy(currentReturnMisVal->name, resolvedValue->value);
                    }
                }
            }

            printf("var |%s| will be replaced with value |%s| at position %d\n", currentLocalMisVal->name, resolvedValue->value, currentLocalMisVal->destPosition);
            previousLocalMisVal = currentLocalMisVal;
            currentLocalMisVal = currentLocalMisVal->nextValue;
        }
    }
    printf("Replacing missing values\n");
    resolveLayeredMacros(definedVars);
    replaceMissingValues(byteTable->nextByte, missingValues->nextValue, definedVars);
    asmCallError(kill, "could not replace value.\n");
    
    printf("ALIGN\n");
    alignBytes(byteTable);
    sysCallError(kill, "could not align bytes.\n");
    printf("Done\n");

    lastDefine->nextRow = NULL;

    freeInstTable(instTable);
    //printf("Done - freeing labels\n");
    freeTable(labelTable);
    printf("Done\n");
    //freeTable(defineTable); => linked to labelTable so it was freed previously
    if(globalVars == NULL)
    {
        freeTable(globalTable);
    }
    printf("Done\n");
    freeTable(includeTable);
    freeMisValues(missingValues);
    if(startPos != NULL)
    {
        *startPos = nextBytePos;
    }
    return byteTable->nextByte;
}


Rules* getRules(newFILE* ruleFile, Rules* rulesTable)
{
    char* fileExtension = strchr(ruleFile->fileName, '.');
    setErrorIf(strcmp(fileExtension, RULES_EXTENSION), ASMRCOMP_ERR, NULL);

    Rules* ruleList;
    if(rulesTable == NULL)
    {
        ruleList = createRule("template", NULL, "none", 0);
    }
    else
    {
        ruleList = getLastRule(rulesTable);
    }
    Rules* currentRule = ruleList;

    while(readFileLine(ruleFile))
    {
        removeComment(ruleFile->fileLine);
        removeExtraSpace(ruleFile->fileLine);
        if(ruleFile->fileLine[0] == '\0')
        {
            continue;
        }
        printf("New string: %s|\n", ruleFile->fileLine);
        sysCallError(kill, "stop.\n");

        char* startOfArg = strchr(ruleFile->fileLine, ' ');
        if(startOfArg != NULL){startOfArg[0] = '\0';}
        char* endOfArg;
        char* argTypeNameSep;
        char* ruleDefStart = strchr(ruleFile->fileLine, OPENRULE_CHAR);
        if(ruleDefStart != NULL){ruleDefStart[0] = '\0';}

        printf("String start at %p and args start at %p\n", ruleFile->fileLine, startOfArg);
        printf("\n\n\n\nCreating rule of name: |%s|\n\n\n\n\n", ruleFile->fileLine);
        printf("\n\n\n\nCreating rule of name: |%s|\n\n\n\n\n", startOfArg+1);
        currentRule->nextRule = createRule(ruleFile->fileLine, NULL, ruleFile->filePath, 0);
        currentRule = currentRule->nextRule;
        Args* ruleArgList = createArg("start", UDEF, NULL);
        Args* currentArg = ruleArgList;

        printf("Got here\n\n");
        while(startOfArg != NULL)
        {
            printf("Got ");
            startOfArg++;
            endOfArg = strchr(startOfArg, ARGSEP_CHAR);
            if(endOfArg != NULL)
            {
                endOfArg[0] = '\0';
            }
            printf("here ");
            argTypeNameSep = strchr(startOfArg, ' ');
            setErrorIf(argTypeNameSep == NULL, ARGNAMESPEC_ERR, NULL);
            argTypeNameSep[0] = '\0';

            printf("Arg name is |%s|\n", argTypeNameSep+1);
            printf("here ");
            currentArg->nextArg = createArg(argTypeNameSep+1, getTypeString(startOfArg), NULL);
            currentArg = currentArg->nextArg;
            startOfArg = endOfArg;
        }

        printf("\nGot here\n");
        if(ruleDefStart == NULL)
        {
            while(readFileLine(ruleFile))
            {
                removeComment(ruleFile->fileLine);
                removeExtraSpace(ruleFile->fileLine);

                if(ruleFile->fileLine[0] == '\0')
                {
                    continue;
                }
                setErrorIf((ruleFile->fileLine[0] != OPENRULE_CHAR), RULEDEF_ERR, NULL);
                break;
            }
        }

        currentRule->argList = ruleArgList->nextArg;
        currentRule->fileLine = ruleFile->lineNumber-1;

        char* ruleEnd = strchr(ruleFile->fileLine, CLOSERULE_CHAR);
        while(ruleEnd == NULL)
        {
            if(!readFileLine(ruleFile))
            {
                break;
            }
            ruleEnd = strchr(ruleFile->fileLine, CLOSERULE_CHAR);
        }
    }

    return ruleList->nextRule;
}


byte getVarValue(char* varString, argType targetType, byteHalf bitSelector)
{
    setErrorIf(varString == NULL, NULLARG_ERR, 0x00);
    setErrorIf(!typeMatch(typeVar(varString), targetType), TYPEMATCH_ERR, 0x00);
    
    targetType = typeCast(typeVar(varString), targetType);

    byte returnValue = 0;
    short shortValue = 0;

    if(targetType == REG)
    {
        switch(varString[0])
        {
            case 'f': shortValue += 0x01;
            case 'h': shortValue += 0x01;
            case 'l': shortValue += 0x01;
            case 'e': shortValue += 0x01;
            case 'd': shortValue += 0x01;
            case 'c': shortValue += 0x01;
            case 'b': shortValue += 0x01;
            case 'a': break;
            default: return 0x00;
        }
    }
    else if(targetType == HL)
    {
        return 0x00;
    }
    else if(targetType == IMM8 || targetType == IMM16 || targetType == IMM1)
    {
        int tempInt;
        if(isChar(varString))
        {
            tempInt = (int)varString[1];
        }
        else
        {
            getIntValue(varString, '\0', &tempInt);
            setErrorIf(tempInt >= 65536, INTOVFLW_ERR, 0x00);
        }

        shortValue = tempInt%65536;
        if(targetType == IMM1 && tempInt)
        {
            shortValue = 1;
        }
    }

    if(bitSelector){
        returnValue = shortValue/256;
    }
    else{
        returnValue = shortValue%256;
    }

    return returnValue;
}


ByteCode* translateInstLine(char* instructionLine, int* startPos, Inst* instructionTable, MissingValue* missingValues, Rules* ruleTable, Table* convTable)
{
    setErrorIf(instructionLine == NULL, NULLARG_ERR, NULL);
    setErrorIf(startPos == NULL, NULLARG_ERR, NULL);
    setErrorIf(instructionTable == NULL, NULLARG_ERR, NULL);

    Inst* currentInst = NULL;
    char* argsStart = strchr(instructionLine, ' ');
    char* startOfArg = argsStart;
    char* endOfArg = argsStart;
    int lastBytePos = *startPos;
    printf("New instruction at byte %d\n", lastBytePos);

    if(argsStart != NULL){argsStart[0] = '\0'; printf("INST ARG START NNULL: %s\n", instructionLine);}

    ByteCode* instBytes = NULL;
    currentInst = searchInst(instructionTable, instructionLine);

    if(currentInst == NULL)
    {
        /*instBytes = copmileRule(instructionLine, ruleTable, startPos);
        asmCallError(kill, "could not compile called rule\n");*/
        Rules* usedRule = searchRule(ruleTable, instructionLine);
        setErrorIf(usedRule == NULL, UKNWINST_ERR, NULL);
        newFILE* usedRuleFile = openFILE(usedRule->filePath, "r", RULES_EXTENSION);
        asmCallError(kill, "Rules can only be defined in rule files\n");
        while(usedRuleFile->lineNumber != usedRule->fileLine)
        {
            readFileLine(usedRuleFile);
        }
        if(argsStart != NULL){argsStart[0] = ' ';}
        Table* argConversionTable = createTable("convT", "null");
        argConversionTable->nextRow = getConversionTable(instructionLine, usedRule);
        asmCallError(kill, "couldn't execute rule");
        instBytes = compileFile(usedRuleFile, NULL, usedRule, missingValues, &lastBytePos, argConversionTable->nextRow);
        closeFILE(usedRuleFile);
        freeTable(argConversionTable);
        return instBytes;
    }

    //setErrorIf(currentInst == NULL, UKNWINST_ERR, NULL);
    instBytes = createByte(lastBytePos, currentInst->opCode);
    Args* templateArg = currentInst->argList;
    char* currentArgString;

    int regSet = 0;
    byte argCode;
    argType varTargetType;

    sysCallError(kill, "could not compile\n");
    while(startOfArg != NULL)
    {
        setErrorIf(templateArg == NULL, ARGNUMHIGH_ERR, NULL);
        startOfArg++;
        endOfArg[0] = ARGSEP_CHAR;
        endOfArg = strchr(startOfArg, ARGSEP_CHAR);
        if(endOfArg != NULL)
        {
            endOfArg[0] = '\0';
        }

        currentArgString = startOfArg;

        argType currentArgType = typeVar(currentArgString);
        if(currentArgType == UDEF)
        {
            Table* argReplace = searchTable(convTable, currentArgString);
            if(argReplace == NULL)
            {
                setErrorIf((templateArg->type == HL || templateArg->type == REG), MACROVAL_ERR, NULL);
                //It is not wrong to set an error above, but only if macros can only represent immedite values
                //(which currently is the case in version 1.0.0)
                currentArgType = IMM8; //IMM8 because IMM8 can be casted to IMM1 or IMM16 if needed
            }
            else if(typeVar(argReplace->value) != UDEF)
            {
                currentArgString = argReplace->value;
                currentArgType = typeVar(argReplace->value);
                setErrorValue(NO_ERR);
            }
            else
            {
                setErrorIf((templateArg->type == HL || templateArg->type == REG), MACROVAL_ERR, NULL);
                //It is not wrong to set an error above, but only if macros can only represent immedite values
                //(which currently is the case in version 1.0.0)
                currentArgType = IMM8;
            }
        }

        //Convert
        setErrorIf(!typeMatch(currentArgType, templateArg->type), TYPEMATCH_ERR, NULL);
        varTargetType = typeCast(currentArgType, templateArg->type);
        argCode = getVarValue(currentArgString, varTargetType, LOW_HALF);

        if(templateArg->type == BIT8 || templateArg->type == BIT16)
        {
            instBytes->code += (varTargetType%2)*8; 
        }

        if((varTargetType == REG || varTargetType == IMM1) && !regSet)
        {
            instBytes->code += argCode;
            regSet = 1;
        }
        else if(varTargetType != HL)
        {
            lastBytePos++;
            addByteCode(instBytes, -1, argCode);
            if(varTargetType == IMM16)
            {
                argCode = getVarValue(currentArgString, varTargetType, HIGH_HALF);
                addByteCode(instBytes, -1, argCode);
            }
        }

        if(typeVar(currentArgString) == UDEF)
        {
            setErrorValue(NO_ERR);
            addMisValue(missingValues, currentArgString, lastBytePos, varTargetType);
            printf("Filed missing value at bytes %d\n", lastBytePos);
        }
        if(varTargetType == IMM16)
        {
            lastBytePos++;
        }
        templateArg = templateArg->nextArg;
        startOfArg = endOfArg;
        
    }

    setErrorIf(templateArg != NULL, ARGNUMLOW_ERR, NULL);

    if(argsStart != NULL){argsStart[0] = ' ';}

    *startPos = lastBytePos+1;
    printf("Stored value %d\n", *startPos);

    return instBytes;
}


Table* getConversionTable(char* ruleLine, Rules* usedRule)
{
    setErrorIf(ruleLine == NULL, NULLARG_ERR, NULL);
    setErrorIf(usedRule == NULL, NULLARG_ERR, NULL);

    char* startOfArg = strchr(ruleLine, ' ');
    char* endOfArg = NULL;
    Table* conversionTable = createTable("argName", "defVal");
    Args* currentArg = usedRule->argList;

    if(startOfArg != NULL){startOfArg[0] = '\0';}
    if(endOfArg != NULL){endOfArg[0] = '\0';}

    setErrorIf(strcmp(ruleLine, usedRule->name), UKNWINST_ERR, NULL);

    while(startOfArg != NULL)
    {
        startOfArg++;
        endOfArg = strchr(startOfArg, ARGSEP_CHAR);
        if(endOfArg != NULL)
        {
            endOfArg[0] = '\0';
        }
        argType currentArgType = typeVar(startOfArg);
        if(currentArgType != UDEF)
        {
            setErrorIf(!typeMatch(currentArgType, currentArg->type), TYPEMATCH_ERR, NULL);
        }
        else
        {
            setErrorValue(NO_ERR);
        }
        addRow(conversionTable, currentArg->name, startOfArg);
        startOfArg = endOfArg;
    }

    return conversionTable->nextRow;
}


void resolveLayeredMacros(Table* definedValues)
{
    Table* previousMacro = NULL;
    Table* currentMacro = definedValues;
    Table* targetMacro = NULL;

    while(currentMacro != NULL)
    {
        targetMacro = currentMacro;
        while(typeVar(targetMacro->value) == UDEF)
        {
            targetMacro = searchTable(definedValues, targetMacro->value);
            //setErrorIf(targetMacro == NULL, UKNWARG_ERR, );
            //setErrorIf(targetMacro == NULL, UDEFMACERR, );
            if(targetMacro == NULL)
            {
                //char* invOperand = reduceExpression();
                previousMacro = currentMacro;
                currentMacro = currentMacro->nextRow;
                break;
            }
            setErrorIf(!strcmp(targetMacro->value, currentMacro->name), RECPRAGDEF_ERR, );
        }
        setErrorValue(NO_ERR);
        if(targetMacro == NULL){continue;}

        if(targetMacro != currentMacro)
        {
            Table* newDefVal = createTable(currentMacro->name, targetMacro->value);
            setErrorIf(newDefVal == NULL, getErrorCode(), );
            newDefVal->nextRow = currentMacro->nextRow;
            if(previousMacro != NULL)
            {
                previousMacro->nextRow = newDefVal;
            }
            currentMacro->nextRow = NULL;
            freeTable(currentMacro);
            currentMacro = newDefVal;
        }

        previousMacro = currentMacro;
        currentMacro = currentMacro->nextRow;
    }

    return;
}


void replaceMissingValues(ByteCode* bytes, MissingValue* missingValues, Table* definedValues)
{
    exitIf(bytes == NULL, );
    exitIf(missingValues == NULL, );
    exitIf(definedValues == NULL, );

    MissingValue* currentMisValue = missingValues;
    ByteCode* destByte = NULL;
    Table* valueRow = NULL;
    byte argCode = 0x00;

    //MISSING VALUE -> reference bytes
    while(currentMisValue != NULL)
    {
        destByte = searchByte(bytes, currentMisValue->destPosition);
        if(destByte == NULL)
        {
            currentMisValue = currentMisValue->nextValue;
            warnUserIf(true, MISVALREF_ERR, "");
            continue;
        }

        valueRow = searchTable(definedValues, currentMisValue->name);
        printf("Searching for variable named: |%s|\n", currentMisValue->name);

        //arithmetic code
        char* inputString = (char*)malloc(strlen(currentMisValue->name)*sizeof(char));
        setErrorIf(inputString == NULL, ALLOC_ERR, );

        if(valueRow == NULL)
        {
            strcpy(inputString, currentMisValue->name);
            char* pointerToInvalid = NULL;
            printf("Next1? Error is %d\n", getErrorCode());
            while((pointerToInvalid=reduceExpression(inputString, currentMisValue->destPosition, currentMisValue->targetType)))
            {
                asmCallError(kill, "could not resolve value\n");
                char* firstOP = NULL;
                int index = pointerToInvalid-inputString;
                while(!firstOP)
                {
                    if(inputString[index++] < '0')
                    {
                        firstOP = inputString+index-1;
                    }
                }

                index = pointerToInvalid-inputString;
                int sizeInputString = strlen(inputString);
                int sizeOfInvalid = strlen(pointerToInvalid);

                char firstOpChar = firstOP[0];
                firstOP[0] = '\0';

                //setErrorIf(!strcmp(currentMisValue->name, pointerToInvalid), RECPRAGDEF_ERR, );
                printf("Mis Value is |%s|\n", currentMisValue->name);
                Table* tempPtr = searchTable(definedValues, pointerToInvalid);
                //Check redundancy + add parentheses
                printf("Searched Tables for |%s|\n", pointerToInvalid);
                setErrorIf(tempPtr == NULL, UDEFMACERR, );

                //Redundancy check

                pointerToInvalid = tempPtr->value;
                int reSize = sizeInputString-sizeOfInvalid+strlen(pointerToInvalid)+1+2;//+nullchar+'()'



                char* tempInput = (char*)malloc(reSize*sizeof(char));
                firstOP[0] = firstOpChar;

                strcpy(tempInput, inputString);
                tempInput[index++] = PRIORITYOPEN_CHAR;
                strcpy(tempInput+index, pointerToInvalid);
                tempInput[strlen(tempInput)] = PRIORITYCLOSE_CHAR;
                index++;
                strcpy(tempInput+index+strlen(pointerToInvalid), firstOP);

                free(inputString);
                inputString = tempInput;
            }
        }
        else
        {
            strcpy(inputString, valueRow->value);
        }
        //setErrorIf(valueRow == NULL, UKNWARG_ERR, );

        /*while(typeVar(intValueReplace) == UDEF)               ===> resolved in resolveLayerMacros();
        {
            valueRow = searchTable(definedValues, intValueReplace);
            setErrorIf(valueRow == NULL, UKNWARG_ERR, );
        }
        setErrorValue(NO_ERR);*/      

        argType targetType = currentMisValue->targetType;
        argType currentArgType = typeVar(inputString);

        printf("Arg type of %s is %d\n", inputString, currentArgType);
        setErrorIf(currentArgType == UDEF, UKNWARG_ERR, );

        printf("Typing %s\n", inputString);
        setErrorIf(!typeMatch(currentArgType, targetType), MACROVAL_ERR, );

        argCode = getVarValue(inputString, targetType, LOW_HALF);
        destByte->code += argCode;
        if(targetType == IMM16)
        {
            argCode = getVarValue(inputString, targetType, HIGH_HALF);
            destByte->nextByte->code += argCode;
        }

        currentMisValue = currentMisValue->nextValue;
        free(inputString);
        printf("Next var? Error is %d\n", getErrorCode());
    }

    return;

}


ByteCode* assemblePragma(char* lineString, Table* definedVars, Table* globalVars, Table* includes, newFILE* compilingFile, Rules* ruleTable, MissingValue* missingVals, int* startPos)
{
    setErrorIf(lineString == NULL, NULLARG_ERR, NULL);

    char* startPragma = lineString;
    char* endPragma = strchr(startPragma, ' ');
    char* startID = NULL;
    char* endID = NULL;
    char* startValue = NULL;
    ByteCode* newCode = NULL;

    if(endPragma != NULL)
    {
        endPragma[0] = '\0';
        startID = endPragma+1;
        endID = strchr(startID, ' ');
        if(endID != NULL)
        {
            endID[0] = '\0';
            startValue = endID+1;
        }
    }

    if(!strcmp(startPragma, "#define"))
    {
        setErrorIf(startID == NULL, PRAGIDUDF_ERR, NULL);
        if(startValue == NULL)
        {
            startValue = "0";
        }
        //setErrorIf(startValue == NULL, PRAGVALUDF_ERR, NULL);

        setErrorIf(searchTable(definedVars->nextRow, startID)!= NULL, PRAGREDEF_ERR, NULL);
        setErrorIf(searchTable(globalVars->nextRow, startID)!= NULL, PRAGREDEF_ERR, NULL);

        addRow(definedVars, startID, startValue);
    }
    else if(!strcmp(startPragma, "#global"))
    {
        setErrorIf(startID == NULL, PRAGIDUDF_ERR, NULL);
        if(startValue == NULL)
        {
            startValue = "0";
        }
        //setErrorIf(startValue == NULL, PRAGVALUDF_ERR, NULL);

        setErrorIf(searchTable(definedVars->nextRow, startID)!= NULL, PRAGREDEF_ERR, NULL);
        setErrorIf(searchTable(globalVars->nextRow, startID)!= NULL, PRAGREDEF_ERR, NULL);

        addRow(globalVars, startID, startValue);
    }
    else if(!strcmp(startPragma, "#include"))
    {
        //Append fileName to current file Path

        int currentFilePathLen = strlen(compilingFile->filePath);
        int targetFilePathLen = strlen(startID);

        char* newFilePath = (char*)malloc(sizeof(char)*(currentFilePathLen+targetFilePathLen+1));
        setErrorIf(newFilePath == NULL, ALLOC_ERR, NULL);
        strcpy(newFilePath, compilingFile->filePath);
        char* lastFolderSep = strrchr(newFilePath, systemFolderSeperator);
        if(lastFolderSep == NULL)
        {
            lastFolderSep = newFilePath;
        }
        lastFolderSep[0] = '\0';
        strcat(newFilePath, startID);

        removePathRedundancy(newFilePath);

        //Remove what can be removed (test/../test/bin => /bin || test/test1/../Test2 => test/Test2)
        //removePathRedundancy(newFilePath);

        //Check if file has not already been included in the current include stack (table)
        Table* includedFile = searchTable(includes, newFilePath);
        setErrorIf(includedFile != NULL, REINCFILE_ERR, NULL)
        setErrorIf(!strcmp(newFilePath, compilingFile->filePath), CFILEINC_ERR, NULL);
        addRow(includes, newFilePath, "1");

        //Open new file 

        newFILE* newIncludeFile = openFILE(newFilePath, "r", NULL);
        setErrorIf(newIncludeFile == NULL, getErrorCode(), NULL);
        setCompiledFile(newIncludeFile);

        char* fileExtension = strchr(newIncludeFile->fileName, '.');
        if(!strcmp(fileExtension, RULES_EXTENSION))
        {
            getRules(newIncludeFile, ruleTable);
            asmCallError(kill, "could not get rules\n");
            newCode = NULL;
        }
        else
        {
            newCode = compileFile(newIncludeFile, globalVars, ruleTable, missingVals, startPos, NULL);
            asmCallError(kill, "could compile include\n");
        }
        setCompiledFile(compilingFile);

        printf("IN PRAGMA\n");
        ByteCode* currentByte = newCode;
        ByteCode* nextByte;
        while(currentByte != NULL)
        {
            nextByte = currentByte->nextByte;
            printf("New byte at pos %d: 0x%x\n", currentByte->posAddr, currentByte->code);
            currentByte = nextByte;
        }
        printf("IN PRAGMA\n\n");

        closeFILE(newIncludeFile);

        //compileFile(newFile, &availableRules)
    }
    else if(!strcmp(startPragma, "#ifdef"))
    {
        setErrorIf(startID == NULL, PRAGIDUDF_ERR, NULL);

        Table* defValue = NULL;
        defValue = searchTable(definedVars->nextRow, startID);
        if(defValue == NULL)
        {
            defValue = searchTable(globalVars->nextRow, startID);
        }

        bool comparisonTrue = false;
        if(defValue != NULL)
        {
            comparisonTrue = true;
            if(startValue != NULL)
            {
                if(strcmp(defValue->value, startValue))
                {
                    comparisonTrue = false;
                }
            }
        }

        int numIfEnd = 0;
        if(!comparisonTrue)
        {
            while(readFileLine(compilingFile))
            {
                if(isPragma(compilingFile->fileLine))
                {
                    char* firstSpace = strchr(compilingFile->fileLine, ' ');
                    if(firstSpace != NULL)
                    {
                        firstSpace[0] = '\0';
                    }
                    if(!strcmp(compilingFile->fileLine, "#ifdef")){numIfEnd++;}
                    if(!strcmp(compilingFile->fileLine, "#else")){return NULL;}
                    if(!strcmp(compilingFile->fileLine, "#endif"))
                    {
                        if(numIfEnd == 0)
                        {
                            return NULL;
                        }
                        numIfEnd--;
                    }
                }
            }
            //ENDIF COUNT ERROR
            setErrorIf(true, NOENDIF_ERR, NULL);
        }
    }
    else if(!strcmp(startPragma, "#else"))
    {
        int numIfEnd = 0;
        while(readFileLine(compilingFile))
        {
            if(isPragma(compilingFile->fileLine))
                {
                    char* firstSpace = strchr(compilingFile->fileLine, ' ');
                    if(firstSpace != NULL)
                    {
                        firstSpace[0] = '\0';
                    }
                    if(!strcmp(compilingFile->fileLine, "#ifdef")){numIfEnd++;}
                    if(!strcmp(compilingFile->fileLine, "#endif"))
                    {
                        if(numIfEnd == 0)
                        {
                            return NULL;
                        }
                        numIfEnd--;
                    }
                }
        }
        setErrorIf(true, NOENDIF_ERR, NULL);
    }
    else if(strcmp(startPragma, "#endif"))
    {
        setErrorIf(true, INVPRAGMA_ERR, NULL);
    }

    return newCode;
}


Table* addLabel(char* lineString, Table* labelTable, int posAddr)
{
    setErrorIf(lineString == NULL, NULLARG_ERR, NULL);
    setErrorIf(labelTable == NULL, NULLARG_ERR, NULL);

    int lastCharPos = strlen(lineString)-1;
    if(lineString[lastCharPos-1] == ' ')
    {
        lastCharPos--;
    }
    lineString[lastCharPos] = '\0';
    char* labelAddress = (char*)malloc(sizeof(char)*(lastCharPos+1));
    setErrorIf(labelAddress == NULL, ALLOC_ERR, NULL);
    itoa(posAddr, labelAddress, 10);
    Table* returnRow = addRow(labelTable, lineString, labelAddress);
    free(labelAddress);

    return returnRow;
}


ByteCode* getDirectiveCode(char* lineString, int* firstBytePos, MissingValue* misValues, Table* definedVars)
{
    printf("Got here\n");
    setErrorIf(lineString == NULL, NULLARG_ERR, NULL);
    setErrorIf(firstBytePos == NULL, NULLARG_ERR, NULL);
    exitIf(!isDirective(lineString), NULL);

    int currentPosAddr = *firstBytePos;
    ByteCode* returnBytes = NULL;

    char* startDirectiveWord = lineString+1;
    if(startDirectiveWord[0] == ' ')
    {
        startDirectiveWord++;
    }
    char* endDirectiveWord = strchr(startDirectiveWord, ' ');
    setErrorIf(endDirectiveWord == NULL, UDFDIROPA_ERR, NULL);
    endDirectiveWord[0] = '\0';

    char* startOperand = endDirectiveWord+1;
    char* endOperand = strchr(startOperand, CLOSE_DIRECTIVE_CHAR);
    endOperand[0] = '\0';


    char* newOperand = (char*)malloc(strlen(startOperand+1));
    setErrorIf(newOperand == NULL, ALLOC_ERR, NULL);
    strcpy(newOperand, startOperand);

    char* endFirstOp = strchr(newOperand, ' ');
    if(endFirstOp != NULL)
    {
        endFirstOp[0] = '\0';
    }
    char* pointerToInvalid = NULL;
    while((pointerToInvalid=reduceExpression(newOperand, *firstBytePos, IMM16)))
    {
        asmCallError(kill, "could not resolve value\n");
        char* firstOP = NULL;
        int index = pointerToInvalid-newOperand;
        while(!firstOP)
        {
            if(newOperand[index++] < '0')
            {
                firstOP = newOperand+index-1;
            }
        }

        index = pointerToInvalid-newOperand;
        int sizeInputString = strlen(newOperand);
        int sizeOfInvalid = strlen(pointerToInvalid);

        char firstOpChar = firstOP[0];
        firstOP[0] = '\0';

        //setErrorIf(!strcmp(currentMisValue->name, pointerToInvalid), RECPRAGDEF_ERR, );
        Table* tempPtr = searchTable(definedVars, pointerToInvalid);
        //Check redundancy + add parentheses
        printf("Searched Tables for |%s|\n", pointerToInvalid);
        setErrorIf(tempPtr == NULL, UDEFMACERR, NULL);

        //Redundancy check

        pointerToInvalid = tempPtr->value;
        int reSize = sizeInputString-sizeOfInvalid+strlen(pointerToInvalid)+1+2;//+nullchar+'()'



        char* tempInput = (char*)malloc(reSize*sizeof(char));
        firstOP[0] = firstOpChar;

        strcpy(tempInput, newOperand);
        tempInput[index++] = PRIORITYOPEN_CHAR;
        strcpy(tempInput+index, pointerToInvalid);
        tempInput[strlen(tempInput)] = PRIORITYCLOSE_CHAR;
        index++;
        strcpy(tempInput+index+strlen(pointerToInvalid), firstOP);

        free(newOperand);
        newOperand = tempInput;
    }

    printf("Got here\n");
    startOperand = newOperand;
    printf("Got here\n");

    if(!strcmp(startDirectiveWord, "org"))
    {
        printf("Got here\n");
        setErrorIf(!getIntValue(startOperand, '\0', &currentPosAddr), getErrorCode(), NULL);
        printf("Got here\n");
    }

    else if(!strcmp(startDirectiveWord, "db"))
    {
        int definedByteCode;
        if(typeVar(startOperand) == UDEF)
        {
            setErrorValue(NO_ERR);
            addMisValue(misValues, startOperand, currentPosAddr, IMM8);
            definedByteCode = 0x00;
        }
        else
        {
            definedByteCode = getVarValue(startOperand, IMM8, LOW_HALF);
            setErrorIf(getErrorCode(), getErrorCode(), NULL);
            setErrorIf(definedByteCode > 255, INVDBNUM_ERR, NULL);
        }
        returnBytes = createByte(currentPosAddr, definedByteCode);
        currentPosAddr++;
    }

    else if(!strcmp(startDirectiveWord, "times"))
    {
        printf("Executing Times directive\n");
        int numTimes;
        char* endNumTimes = strchr(startOperand, ' ');
        printf("Got here\n");
        if(endNumTimes == NULL)
        {
            endNumTimes = startOperand+strlen(startOperand);
        }
        endNumTimes[0] = '\0';
        printf("Got here\n");
        setErrorIf(!getIntValue(startOperand, '\0', &numTimes), getErrorCode(), NULL);
        printf("Got here\n");

        endNumTimes[0] = OPEN_DIRECTIVE_CHAR;
        endOperand[0] = CLOSE_DIRECTIVE_CHAR;
        printf("Got here\n");

        for(int i=0; i<numTimes; i++)
        {
            ByteCode* subBytes = getDirectiveCode(endNumTimes, &currentPosAddr, misValues, definedVars);
            setErrorIf(getErrorCode(), getErrorCode(), NULL);

            if(returnBytes == NULL)
            {
                returnBytes = subBytes;
            }
            else
            {
                printf("Linked\n");
                linkBytes(returnBytes, subBytes);
            }
        }
    }

    free(startOperand);
    endDirectiveWord[0] = ' ';
    endOperand[0] = CLOSE_DIRECTIVE_CHAR;
    *firstBytePos = currentPosAddr;
    return returnBytes;
}


//puts integer string result in output if resolved, puts unidentified arg otherwise
char* reduceExpression(char* inputString, int currentPos, argType targetType)
{
    if(inputString == NULL)
    {
        return NULL;
    }
    printf("\n\nReducing |%s|\n", inputString);

    unsigned short result = 0;
    int stringIndex = 0;
    int isHexNum = 0;
    char* lastArgStart = inputString;
    char previousOperator = '+';
    char* closePriority = NULL;

    while((inputString[stringIndex] < 'A')||(isHexNum && ((inputString[stringIndex]>='A' && 'F'>=inputString[stringIndex])||(inputString[stringIndex]>='a' && 'f'>=inputString[stringIndex]))))
    {
        printf("REDUC: |%s|\n", inputString);
        printf("Tested Char is %c at address %p\n", inputString[stringIndex], inputString+stringIndex);
        if((inputString[stringIndex]>='0' && '9'>=inputString[stringIndex]) || (isHexNum && ((inputString[stringIndex]>='A' && 'F'>=inputString[stringIndex])||(inputString[stringIndex]>='a' && 'f'>=inputString[stringIndex]))))
        {
            printf("Got Here\n");
            stringIndex++;
            if(inputString[stringIndex-1] == '0' && stringIndex>0)
            {
                if(inputString[stringIndex] == 'b' || inputString[stringIndex] == 'd')
                {
                    stringIndex++;
                }
                else if(inputString[stringIndex] == 'x')
                {
                    isHexNum = 1;
                    stringIndex++;
                }
            }
            continue;
        }

        if(inputString[stringIndex] == ' ')
        {
            isHexNum = 0;
            continue;
        }

        int tempInt = 0;
        char newOperator = inputString[stringIndex];
        isHexNum = 0;

        if(inputString[stringIndex-1] == '0' && stringIndex>0)
        {
            if(newOperator == 'b' || newOperator == 'd' || newOperator == 'x')
            {
                continue;
            }
        }

        if(newOperator == MEMADDR_CHAR)
        {
            int startArgIndex = stringIndex;
            int offset = 0;
            while(inputString[stringIndex+(offset++)]!=MEMDEREFCLOSE_CHAR)
            {
                if(inputString[stringIndex+offset] == '\0')
                {
                    offset = 1;
                    break;
                }
                continue;
            }
            stringIndex += offset;
            newOperator = inputString[stringIndex];
            inputString[stringIndex] = '\0';
            tempInt = currentPos;
            printf("New operator is %c\n", newOperator);
            printf("Checking for |%s|\n", inputString+startArgIndex);
            if(isModMemChar(inputString+startArgIndex))
            {
                if(inputString[startArgIndex+2] == 'h')
                {
                    printf("Divided\n");
                    tempInt = tempInt/256;
                }
                else
                {
                    printf("Moduloed\n");
                    tempInt = tempInt%256;
                }
            }
        }
        else 
        {
            if(newOperator == PRIORITYOPEN_CHAR)
            {
                closePriority = searchClosingPriority(inputString+stringIndex);
                setErrorIf(closePriority == NULL, UNDEFPRIOR_ERR, NULL);

                char* tempAfterPrior = (char*)malloc(strlen(closePriority)*sizeof(char));
                setErrorIf(tempAfterPrior == NULL, ALLOC_ERR, NULL);
                strcpy(tempAfterPrior, closePriority+1);
                printf("STORED TEMP AFTER PRIOR: |%s|\n", tempAfterPrior);

                *closePriority = '\0';
                lastArgStart = reduceExpression(inputString+stringIndex+1, currentPos, targetType);
                if(lastArgStart != NULL)
                {
                    *closePriority = PRIORITYCLOSE_CHAR;
                    free(tempAfterPrior);
                    break;
                }

                strcpy(inputString+stringIndex, inputString+stringIndex+1);
                lastArgStart = inputString+stringIndex;
                stringIndex = strlen(inputString);
                strcpy(inputString+strlen(inputString), tempAfterPrior);
                newOperator = tempAfterPrior[0];

                //stringIndex = closePriority-inputString-1;//-1 because it is incremented later
                printf("STRING INDEX POINTS TO STRING: |%s|\n", inputString+stringIndex);
                free(tempAfterPrior);
                printf("\n");
            }

            inputString[stringIndex] = '\0';
            printf("Next2? Error is %d\n", getErrorCode());
            printf("Converting |%s|\n", lastArgStart);
            setErrorIf(!getIntValue(lastArgStart, '\0', &tempInt), INVNUM_ERR, lastArgStart);
            printf("Next3? Error is %d\n", getErrorCode());
        }

        printf("OPERATION IS : %d %c %d\n\n", result, previousOperator, tempInt);
        if(previousOperator == '+')
        {
            result += tempInt;
        }

        else if(previousOperator == '-')
        {
            result -= tempInt;
        }

        else if(previousOperator == '*')
        {
            result *= tempInt;
        }

        else if(previousOperator == '/')
        {
            result /= tempInt;
        }

        else
        {
            printf("Operator was %c\n", previousOperator);
            setErrorIf(true, OPERATOR_ERR, lastArgStart);
        }

        if(newOperator != '\0')
        {
            previousOperator = newOperator;
            printf("String before: |%s|\n", inputString);
            inputString[stringIndex] = newOperator;
            printf("\nNEXT OEPRATOR IS: %c\n", newOperator);
            printf("String before: |%s|\n", inputString);
            stringIndex++;
            lastArgStart = inputString+stringIndex;
            printf("Continued with  char %c\n", inputString[stringIndex]);
            
            continue;
        }
        else
        {
            if(targetType == IMM8)
            {
                result = result%256;
                warn("Resolved value is too large\n");
            }
            char resultStr[6];
            itoa(result, resultStr, 10);
            if(strlen(inputString) >= strlen(resultStr))
            {
                strcpy(inputString, resultStr);
            }
            printf("Return NULL with value %d\n", result);
            return NULL;
        }

    }

    /*if(closePriority != NULL)
    {
        *closePriority = PRIORITYCLOSE_CHAR;
    }*/
    printf("Returned uknown arg\n");
    return lastArgStart;

}
