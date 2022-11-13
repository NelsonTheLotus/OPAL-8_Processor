#pragma once

#include <stdio.h>





//SYSTEM VARIABLES AND FUNCTIONS

#ifndef systemSeperatorDefined
#define systemSeperatorDefined
#ifdef _WIN32 
#define systemFolderSeperator '\\'
#else 
#define systemFolderSeperator '/';
#endif
#endif

typedef int bool;
typedef unsigned char byte;
#define true 1
#define false 0

#define TEST(message) printf(message);
#define INFO(command) SETC_STDINFO; command; SETC_NORMAL;



//VERSION

#define printVersion \
SETC_COMMENTARY; printf("VERSION:\n");\
SETC_SYSINFO; printf("Assembler for the "); WRITE_OPAL8; SETC_SYSINFO; printf("architecture, version 0.9.8.\n"); SETC_NORMAL;



//COMPILLER ARGUMENTS HELP

#define print_PROGRAM_ARGUMENTS \
SETC_COMMENTARY; printf("HELP:\n");\
SETC_HCOMMAND; printf("FLAG         "); SETC_HINFO; printf("DESCRIPTION                               "); SETC_HSYNTAX; printf("SYNTAX\n\n");\
SETC_HCOMMAND; printf("-help"); SETC_DOTS; printf(" ...... "); SETC_HINFO; printf("prints this argument helper list."); SETC_DOTS; printf(" ....... "); SETC_HSYNTAX; printf("-help\n");\
SETC_HCOMMAND; printf("-sections"); SETC_DOTS; printf(" .. "); SETC_HINFO; printf("selects the amount of bytes written"); SETC_DOTS; printf(" ..... "); SETC_HSYNTAX; printf("-sections=<numFiles>:<numBytes>;...\n");\
SETC_HINFO; printf("             to a file and the number of files\n\
             of that kind. Chainable several times.\n");\
SETC_HCOMMAND; printf("-nomap"); SETC_DOTS; printf(" ..... "); SETC_HINFO; printf("disables all warnings linked to the"); SETC_DOTS; printf(" ..... "); SETC_HSYNTAX; printf("-nomap=<\"true\"/\"false\">\n");\
SETC_HINFO; printf("             preprogrammed memory mapping of the\n\
             target computer.\n");\
SETC_HCOMMAND; printf("-output"); SETC_DOTS; printf(" .... "); SETC_HINFO; printf("selects a defined output file."); SETC_DOTS; printf(" .......... "); SETC_HSYNTAX; printf("-output=<filename>.<extension>\n");\
SETC_HCOMMAND; printf("-version"); SETC_DOTS; printf(" ... "); SETC_HINFO; printf("prints the version of this assembler."); SETC_DOTS; printf(" ... "); SETC_HSYNTAX; printf("-version\n");\
SETC_HCOMMAND; printf("-lean"); SETC_DOTS; printf(" ...... "); SETC_HINFO; printf("disables compilling of rule files."); SETC_DOTS; printf(" ...... "); SETC_HSYNTAX; printf("-lean=<\"true\"/\"false\">\n");\
SETC_HCOMMAND; printf("-hexdump"); SETC_DOTS; printf(" ... "); SETC_HINFO; printf("displays the final compilled file"); SETC_DOTS; printf(" ....... "); SETC_HSYNTAX; printf("-hexdump=<\"true\"/\"false\">\n");\
SETC_HINFO; printf("             in hexadecimal format.\n");\
SETC_HCOMMAND; printf("-config"); SETC_DOTS; printf(" .... "); SETC_HINFO; printf("display the settings of the compiler."); SETC_DOTS; printf(" ... "); SETC_HSYNTAX; printf("-config=<\"true\"/\"false\">\n");\
SETC_NORMAL; printf("\nAll commands can be shortened to their first letter, with the exception of -help.\n\
Not defining a true/false variable will set it to true.\n\
Setting a -sections number to 0 will create the amount needed.\n\
More information can be found at: "); SETC_HYPERLINK; printf("https://...\n"); SETC_NORMAL; printf("\n");



//COMPILLER RESERVED CHARS AND CHARACTERISTICS

#define COMMENT_CHAR ';'
#define OPEN_DIRECTIVE_CHAR '<'
#define CLOSE_DIRECTIVE_CHAR '>'
#define START_PRAGMA_CHAR '#'
#define END_LABEL_CHAR ':'
#define CHARINT_CHAR '\''
#define ARGSEP_CHAR ','
#define MEMADDR_CHAR '$'
#define OPENRULE_CHAR '{'
#define CLOSERULE_CHAR '}'
#define MEMDEREFOPEN_CHAR '['
#define MEMDEREFCLOSE_CHAR ']'
#define PRIORITYOPEN_CHAR '('
#define PRIORITYCLOSE_CHAR ')'

#define FILELINE_MAX 50
#define IDNAME_MAX 30
#define FILEPATH_MAX FILENAME_MAX

#define RULES_EXTENSION ".asmr"
#define ASSEMBLY_EXTENSION ".asm"
#define BINARY_EXTENSION ".bin"



//CMDLINE COLORS

/*  
*   NOTE: for the colors to work in the terminal, a DWORD pointer named VirtualTerminalLevel has to be created
*   and set to 0x1 in the HKCU/Console folder of the registry editor.
*   A color palette wit hthe corresponding codes can be found at: https://talyian.github.io/ansicolors
*/
#define SETC_NORMAL printf("\x1b[38;5;7m");         //WHITE on BLACK
#define SETC_WARN printf("\x1b[38;5;201m");         //MAGENTA on BLACK
#define SETC_ERROR printf("\x1b[38;5;196m");        //RED on BLACK
#define SETC_SYSINFO printf("\x1b[38;5;14m");       //OCEAN on BLACK
#define SETC_HCOMMAND printf("\x1b[38;5;34m");      //DARK GREEN on BLACK
#define SETC_HINFO printf("\x1b[38;5;153m");        //LIGHT BLUE on BLACK
#define SETC_HSYNTAX printf("\x1b[38;5;202m");      //DARK ORANGE on BLACK
#define SETC_OPAL8 printf("\x1b[38;5;118m");        //LIGHT GREEN on BLACK
#define SETC_DOTS printf("\x1b[38;5;241m");         //DARK GREY on BLACK
#define SETC_STDINFO printf("\x1b[38;5;237m");      //DARK BROWN on BLACK
#define SETC_HYPERLINK printf("\x1b[38;5;39m")      //DEEP BLUE on BLACK
#define SETC_COMMENTARY printf("\x1b[38;5;189m");   //SILICA BLUE on BLACK
#define SETC_SUCCESS printf("\x1b[38;5;46m")        //FLUO GREEN on BLACK
#define SETC_VARIABLE printf("\x1b[38;5;45m")       //CYAN on BLACK
#define SETC_VALUE printf("\x1b[38;5;195m")         //LIGTH CYAN on BLACK
#define SETC_HEXFRAME printf("\x1b[38;5;2m")        //FOREST GREEN on BLACK

#define WRITE_OPAL8 SETC_OPAL8; printf("OPAL-8 "); SETC_NORMAL;
