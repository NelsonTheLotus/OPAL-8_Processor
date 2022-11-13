#include "../headers/Error.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/Adapter.h"





//LOCAL VARIABLES

int errorCode = NO_ERR;
newFILE* currentCompiledFile = NULL;



//FUNCTIONS

void setErrorValue(int errCode)
{
    errorCode = errCode;
}

void setCompiledFile(newFILE* asmFile)
{
    currentCompiledFile = asmFile;
}

int getErrorCode()
{
    return errorCode;
}

void printFileLoc()
{
    currentCompiledFile? printf("FILE: %s\n", currentCompiledFile->filePath):printf("FILE: input arguments\n");
    currentCompiledFile? printf("In file: %s at line %d:\n", currentCompiledFile->fileName, currentCompiledFile->lineNumber): printf("In input arguments\n");
}



void warn(char* message)
{
    SETC_WARN;
    if(errorCode)
    {
        if(errorCode > 100)
        {
            printf("ASSEMBLY WARNING: ");
        }
        else{
            printf("UNEXPECTED COMPILER WARNING: ");
        }
    }
    SETC_NORMAL;

    switch(errorCode)
    {
        case NO_ERR: break;
        case ALLOC_ERR: printf("failed to allocated required space\n");
        case NULLARG_ERR: printf("function pointer argument is null\n"); break;
        case USETCOMFILE_ERR: printf("current compilling file has not been set\n"); break;
        case FILEACCS_ERR: printf("file could not be accessed\n"); break;
        case MISVALREF_ERR: printf("missing value could not be found in use\n"); break;
        case WRTFILEACCS_ERR: printf("file could not be accessed in write mode\n"); break;
        case ASMRCOMP_ERR: printf("attempted to compile an assembly file in rule mode\n"); break;

        case TYPEMATCH_ERR: printf("given argument type does not match declaration argument type\n"); break;
        case INVNUM_ERR: printf("given number could not be converted correctly\n"); break;
        case INTOVFLW_ERR: printf("given value is bigger than the 16bit maximum of 0d65536\n"); break;
        case ARGNUMHIGH_ERR: printf("number of fed arguments exceeds number of arguments provided in the declaration\n"); break;
        case ARGNUMLOW_ERR: printf("call to instruction is missing arguments\n"); break;
        case FILEPATH_ERR: printf("file path is too long\n"); break;
        case FILEUKNW_ERR: printf("file could not be found\n"); break;
        case LINELENG_ERR: printf("read file line exceeds maximum character count\n"); break;
        case UKNWINST_ERR: printf("given instruction is unknown\n"); break;
        case IDNAMEMX_ERR: printf("given identification name is too long\n"); break;
        case UKNWARG_ERR: printf("given argument could not be identified\n"); break;
        case MACROVAL_ERR: printf("macros can only represent immediate values, not registers\n"); break;
        case INVPRAGMA_ERR: printf("given pragama could not be interpreted\n"); break;
        case PRAGIDUDF_ERR: printf("pragma identification field is undefined\n"); break;
        case PRAGVALUDF_ERR: printf("pragma value field is undefined\n"); break;
        case UDFDIROPA_ERR: printf("directive has not value to operate with\n"); break;
        case INVDBNUM_ERR: printf("directive <db> can only interpret 8bit values\n"); break;
        case PRAGREDEF_ERR: printf("pragama is used twice with the same identification name\n"); break;
        case RECPRAGDEF_ERR: printf("pragma definitions are resulting in a loop\n"); break;
        case NOENDIF_ERR: printf("missing an endif statement\n"); break;
        case REINCFILE_ERR: printf("file has already been included within the scope of the current file\n"); break;
        case CFILEINC_ERR: printf("included file is the same file performing the include\n"); break;
        case BYTEREDEF_ERR: printf("byte position has already been used\n"); break;
        case ARGNAMESPEC_ERR: printf("rule argument has no specific name\n"); break;
        case RULEDEF_ERR: printf("named rule has no body\n"); break;
        case NLLFILEXT_ERR: printf("no specified extension\n"); break;
        case EXTTARG_ERR: printf("specified extension does not match target extension\n"); break;
        case UDEFMACERR: printf("macro is used to represent an undefined macro\n"); break;

        default: printf("Uknown error of type %d\n", errorCode); break;
    }
    printf(message);
    printf("Ignoring\n");
    return;
}


void kill(char* message)
{
    SETC_ERROR;
    if(errorCode)
    {
        if(errorCode > 100)
        {
            printf("FATAL ASSEMBLY ERROR: ");
        }
        else{
            printf("UNEXPECTED COMPILER ERROR: ");
        }
    }
    SETC_NORMAL;

    switch(errorCode)
    {
        case NO_ERR: break;
        case ALLOC_ERR: printf("failed to allocated required space\n");
        case NULLARG_ERR: printf("function pointer argument is null\n"); break;
        case USETCOMFILE_ERR: printf("current compilling file has not been set\n"); break;
        case FILEACCS_ERR: printf("file could not be accessed\n"); break;
        case MISVALREF_ERR: printf("missing value could not be found in use\n"); break;
        case WRTFILEACCS_ERR: printf("file could not be accessed in write mode\n"); break;
        case ASMRCOMP_ERR: printf("attempted to compile an assembly file in rule mode\n"); break;

        case TYPEMATCH_ERR: printf("given argument type does not match declaration argument type\n"); break;
        case INVNUM_ERR: printf("given number could not be converted correctly\n"); break;
        case INTOVFLW_ERR: printf("given value is bigger than the 16bit maximum of 0d65536\n"); break;
        case ARGNUMHIGH_ERR: printf("number of fed arguments exceeds number of arguments provided in the declaration\n"); break;
        case ARGNUMLOW_ERR: printf("call to instruction is missing arguments\n"); break;
        case FILEPATH_ERR: printf("file path is too long\n"); break;
        case FILEUKNW_ERR: printf("file could not be found\n"); break;
        case LINELENG_ERR: printf("read file line exceeds maximum character count\n"); break;
        case UKNWINST_ERR: printf("given instruction is unknown\n"); break;
        case IDNAMEMX_ERR: printf("given identification name is too long\n"); break;
        case UKNWARG_ERR: printf("given argument could not be identified\n"); break;
        case MACROVAL_ERR: printf("macros can only represent immediate values, not registers\n"); break;
        case INVPRAGMA_ERR: printf("given pragama could not be interpreted\n"); break;
        case PRAGIDUDF_ERR: printf("pragma identification field is undefined\n"); break;
        case PRAGVALUDF_ERR: printf("pragma value field is undefined\n"); break;
        case UDFDIROPA_ERR: printf("directive has not value to operate with\n"); break;
        case INVDBNUM_ERR: printf("directive <db> can only interpret 8bit values\n"); break;
        case PRAGREDEF_ERR: printf("pragama is used twice with the same identification name\n"); break;
        case RECPRAGDEF_ERR: printf("pragma definitions are resulting in a loop\n"); break;
        case NOENDIF_ERR: printf("missing an endif statement\n"); break;
        case REINCFILE_ERR: printf("file has already been included within the scope of the current file\n"); break;
        case CFILEINC_ERR: printf("included file is the same file performing the include\n"); break;
        case BYTEREDEF_ERR: printf("byte position has already been used\n"); break;
        case ARGNAMESPEC_ERR: printf("rule argument has no specific name\n"); break;
        case RULEDEF_ERR: printf("named rule has no body\n"); break;
        case NLLFILEXT_ERR: printf("no specified extension\n"); break;
        case EXTTARG_ERR: printf("specified extension does not match target extension\n"); break;
        case UDEFMACERR: printf("macro is used to represent an undefined macro\n"); break;

        default: printf("Uknown error of type %d\n", errorCode); break;
    }
    printf(message);
    printf("Could not continue\n");
    exit(-1);
    return;
}
