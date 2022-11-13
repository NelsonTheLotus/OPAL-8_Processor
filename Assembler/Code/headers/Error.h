#pragma once

#include <stdlib.h>

#include "../headers/Adapter.h"





//FUNCTION MACROS

#define setErrorIf(condition, errCode, returnValue) if(condition){setErrorValue(errCode); return returnValue;}
#define exitIf(condition, returnValue) if(condition){return returnValue;}

#define sysCallError(function, message) if(getErrorCode()){function(message);}
#define asmCallError(function, message) if(getErrorCode()){printFileLoc(); function(message);}

#define warnUserIf(condition, errCode, message) if(condition){setErrorValue(errCode); printFileLoc(); warn(message);}

//FUNCTIONS

void setErrorValue(int errCode);
void setCompiledFile(newFILE* asmFile);

int getErrorCode();

void printFileLoc();

void warn(char* message);
void kill(char* message);



//SYSTEM ERROR CODES            //Used for unexpected compiler errors if the program crashed or failed a task

#define NO_ERR 0                //NO ERROR
#define ALLOC_ERR 1             //ALLOCATION ERROR - failed to alocate
#define NULLARG_ERR 2           //NULL ARGUMENT ERROR - function pointer argument is null
#define USETCOMFILE_ERR 3       //UNSET COMPILING FILE - current compilling file is not set
#define FILEACCS_ERR 4          //FILE ACCESS - file could not be accessed
#define MISVALREF_ERR 5         //MISSING VALUE REFFERENCING - missing value could not be found in use
#define WRTFILEACCS_ERR 6       //WRITE FILE ACCESS - file could not be accessed in write mode
#define ASMRCOMP_ERR 7          //ASEMBLY RULE COPMILLATION - attempted to compile an assembly file in rule mode



//ASSEMBLY ERROR CODES          //Used for language errors and warnings preventing healthy compilation

#define TYPEMATCH_ERR 101       //TYPE MISMATCH - argument type does not match template type
#define INVNUM_ERR 102          //INVALID NUMBER - fed number is invalid and could not be converted to string
#define INTOVFLW_ERR 103        //INTEGER OVERFLOW - value is bigger the the 16bit maximum integer allowed
#define ARGNUMHIGH_ERR 104      //ARGUMENT NUMBER HIGH - number of fed arguments exceeds number of arguments provided in the declaration
#define ARGNUMLOW_ERR 105       //ARGUMENT NUMBER LOW - call to instruction is missing arguments
#define FILEPATH_ERR 106        //FILE PATH - file path is too long
#define FILEUKNW_ERR 107        //FILE UKNOWN - file could not be found
#define LINELENG_ERR 108        //LINE LENGTH - read file line exceeds maximum character count
#define UKNWINST_ERR 109        //UKNOWN INSTRUCTION - given instruction is unknown
#define IDNAMEMX_ERR 110        //IDENTIFICATION MAX - given name is too large
#define UKNWARG_ERR 111         //UNKNOW ARGUMENT - given argument is not known (COULD NOT BE IDENTIFIED)
#define MACROVAL_ERR 112        //MACRO VALUE - Macro cannot represent a register value such as ax, bx or hl
#define INVPRAGMA_ERR 113       //INVALID PRAGMA - pragama could not be interpreted
#define PRAGIDUDF_ERR 114       //PRAGMA IDENTIFICATION UNDEFINED - pragma identification field is not given
#define PRAGVALUDF_ERR 115      //PRAGMA VALUE UNDEFINED - pragma value field is undefined
#define UDFDIROPA_ERR 116       //UNDEFINED DIRECTIVE OPERAND - directive operand is undefined
#define INVDBNUM_ERR 117        //INVALID DEFINE BYTE NUMBER - define byte number is too large
#define PRAGREDEF_ERR 118       //PRAGMA REDEFINITION - pragma is redefined (valid for includes, defines and globals)
#define RECPRAGDEF_ERR 119      //RECURING PRAGAMA DEFINITION - pragma definition is defined in a loop
#define NOENDIF_ERR 120         //NO ENDIF - missing endif pragma
#define REINCFILE_ERR 121       //REINCLUDED FILE - included file has already been included
#define CFILEINC_ERR 122        //CURRENT FILE INCLUDE - included file is the same file performing the include
#define BYTEREDEF_ERR 123       //BYTE REDEFINITION - byte position is already used
#define ARGNAMESPEC_ERR 124     //ARGUMENT NAME SPECIFIER - rule argument has no specific name
#define RULEDEF_ERR 125         //RULE DEFINITION ERROR - named rule has no body
#define NLLFILEXT_ERR 126       //NULL FILE EXTENSION - no specified extension
#define EXTTARG_ERR 127         //EXTENSTION TARGET - specified extension does not match target extension
#define UDEFMACERR 128          //UNDEFINED MACRO ERR - macro is used to represent an undefined macro
#define OPERATOR_ERR 129        //UNDEFINED OPERATOR - Used operator is not known
#define UNDEFPRIOR_ERR 130      //UNDEFINED PRIORITY - Missing closing priority char
#define PROGARGDEF_ERR 131      //PROGRAM ARG DEFINITION - Program argument is invalid !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define ARGBOOLMISDEF_ERR 132   //ARGUMNET TRUTH MISDEFINED - Argument value is not true or false!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define UDEFPROGARG_ERR 133     //UNDEFINED PROGRAM ARGUMENT - program argument is undefined !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define OUTFILEREDEF_ERR 134    //OUTPUT FILE REDEFINED - output file is defined two or more times
#define INFILEREDEF_ERR 135     //INPUT FILE REDEFINED - input file is defined two or more times
#define INFILEUDEF_ERR 136      //INPUT FILE UNDEFINED - no input file provided
