#include "../headers/OASIS.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../headers/Instructions.h"
#include "../headers/INFO.h"
#include "../headers/Adapter.h"
#include "../headers/Translator.h"
#include "../headers/Error.h"





void writeBytesToFile(newFILE* outputFile, ByteCode* bytes, int hexDump)
{
    setErrorIf(outputFile == NULL, NULLARG_ERR, );
    setErrorIf(outputFile->fileOpenMode[0] != 'w', WRTFILEACCS_ERR, )

    ByteCode* currentByte = bytes;
    int currentPos = 0;
    byte zeroByte = 0x00;
    ByteCode* overWrittenByte = NULL;

    while(currentByte != NULL)
    {
        overWrittenByte = searchByte(bytes, currentByte->posAddr);      //BACKWARDS SEARCH
        setErrorIf(overWrittenByte != currentByte, BYTEREDEF_ERR, );
        setErrorValue(NO_ERR);

        while(currentPos<currentByte->posAddr)
        {
            fwrite(&zeroByte, sizeof(byte), 1, outputFile->filePTR);
            currentPos++;
        }
        if(currentPos>currentByte->posAddr)
        {
            fseek(outputFile->filePTR, 0, currentByte->posAddr);
            currentPos = currentByte->posAddr;
        }
        fwrite(&currentByte->code, sizeof(byte), 1, outputFile->filePTR);
        currentByte = currentByte->nextByte;
        currentPos++;

        
    }

    return;
}



void showSections()
{

}



void printHexDump(char* filename)
{
    if(filename == NULL)
    {
        return;
    }

    newFILE* hexFile = openFILE(filename, "rb", BINARY_EXTENSION);
    setErrorIf(hexFile == NULL, getErrorCode(), );

    int vTracker = 0x0000;
    int hTracker = 0x00;

    SETC_COMMENTARY;
    printf("FILE: %s\n", hexFile->fileName);
    SETC_HEXFRAME;
    printf("       ");
    for( ; hTracker<0x10; hTracker++) printf("%02x ", hTracker);
    hTracker = 0x00;
    SETC_NORMAL;

    byte singleByte = fgetc(hexFile->filePTR);
    while(!feof(hexFile->filePTR))
    {
        hexFile->cursorPos++;

        if(hTracker == 0)
        {
            printf("\n");
            SETC_HEXFRAME;
            printf("%04x   ", vTracker);
            SETC_NORMAL;
            vTracker += 0x0010;
        }

        printf("%02x ", singleByte);
        hTracker++;
        hTracker %= 16;
        singleByte = fgetc(hexFile->filePTR);
    }

    closeFILE(hexFile);
}



//int main(int argc, char* argv[])
//{
//
//    char inputFile[50];
//    printf("Enter input file: ");
//    fgets(inputFile, 50, stdin);
//    removeComment(inputFile);
//    removeExtraSpace(inputFile);
//    printf("You entered: |%s|\n", inputFile);
//
//    /*printf("Start address: %p\n", inputFile);
//    removePathRedundancy(inputFile);
//    printf("Start address: %p\n", inputFile);
//    printf("Your redeced path is now: |%s|\n", inputFile);*/
//
//    char outFile[50];
//    printf("Enter input file: ");
//    fgets(outFile, 50, stdin);
//    removeComment(outFile);
//    removeExtraSpace(outFile);
//    printf("You entered: |%s|\n", outFile);
//
//    newFILE* newInputFile = openFILE(inputFile, "r", ASSEMBLY_EXTENSION);
//    printf("opened File\n");
//    sysCallError(kill, "CAN ONLY COMPILE ASSEMBLY FILES\n");
//
//    ByteCode* byteTable = compileFile(newInputFile, NULL, NULL, NULL, NULL, NULL);
//    printf("Done compilling\n");
//    //sysCallError(kill, "none\n");
//
//    printf("\nPrinting byte code:\n");
//    ByteCode* currentByte = byteTable;
//    ByteCode* nextByte;
//    while(currentByte != NULL)
//    {
//        nextByte = currentByte->nextByte;
//        printf("New byte at pos %d: 0x%x\n", currentByte->posAddr, currentByte->code);
//        currentByte = nextByte;
//    }
//
//    printf("FINAL FORMAT:\n\n");
//    newFILE* newOutputFile = openFILE(outFile, "wb", BINARY_EXTENSION);
//    sysCallError(kill, "CAN ONLY WRITE TO BINARY FILES\n");
//    printf("Opened file\n");
//    currentByte = byteTable->nextByte;
//    printf("Opened file\n");
//    while(currentByte != NULL)
//    {
//        nextByte = currentByte->nextByte;
//        printf(" 0x%x ", currentByte->code);
//        //fwrite(&currentByte->code, sizeof(byte), 1, newOutputFile->filePTR);
//        currentByte = nextByte;
//    }
//
//    writeBytesToFile(newOutputFile, byteTable);
//    asmCallError(kill, "Failed\n");
//
//    printf("\n\nDone with compillation\n");
//    freeByteCode(byteTable);
//    return 0;
//
//
//}





int main(int argc, char* argv[])
{

    int inputFileSet = 0;
    int outputFileSet = 0;
    int inputFileNeeded = 1;
    int isCompilation = 0;
    int valueNotFound = 0;
    int trueValue = 0;

    char* inputFileName = NULL;
    char* outputFileName = NULL;
    int noMap = 0;
    int compileLean = 0;
    int hexDump = 0;
    int showHelp = 0;
    int showVersion = 0;
    //sectionToken* firstOfSections = NULL;

    newFILE* inputFile;
    newFILE* outputFile;
    setErrorValue(NO_ERR);

    //Parsing for input variables and changing settings if needed.
    for(int i=1; i<argc; i++)
    {
        INFO(printf("You entered: %s\n\n", argv[i]);)
        if(strlen(argv[i]) < 2)
        {
            setErrorValue(PROGARGDEF_ERR);
            break;
        }

        //Setting inputfilename, then checking for multiple input files
        if(argv[i][0] != '-')
        {
            if(inputFileSet == 1)
            {
                setErrorValue(INFILEREDEF_ERR);
                break;
            }

            inputFileName = (char*)malloc(strlen(argv[i])+1);
            if(inputFileName == NULL)
            {
                setErrorValue(ALLOC_ERR);
                break;
            }
            strcpy(inputFileName, argv[i]);
            inputFileSet = 1;
            isCompilation = 1;
            continue;
        }

        //Setting variables to all lowercase and defining the variable/value couple for better processing.
        char* variable = argv[i]+1;
        char* value = strchr(variable, '=');
        if(value != NULL)
        {   
            *value = '\0';
            value++;
        }
        char* str = variable;
        for ( ; *str; ++str){*str = tolower(*str);}

        //Parsing the program arguments
        if(!strcmp(variable, "output") || !strcmp(variable, "o"))
        {
            if(outputFileSet)
            {
                setErrorValue(OUTFILEREDEF_ERR);
                break;
            }

            outputFileName = (char*)malloc(strlen(value)+1);
            if(outputFileName == NULL)
            {
                setErrorValue(ALLOC_ERR);
                break;
            }
            strcpy(outputFileName, value);
            outputFileSet = 1;
            continue;
        }

        if(!strcmp(variable, "sections") || !strcmp(variable, "s"))
        {
            //GetSections
            continue;
        }

        if(!strcmp(variable, "help"))
        {
            showHelp = 1;
            inputFileNeeded = 0;
            print_PROGRAM_ARGUMENTS;
            continue;
        }

        if(!strcmp(variable, "version") || !strcmp(variable, "v"))
        {
            showVersion = 1;
            inputFileNeeded = 0;
            printVersion;
            continue;
        }

        int resultValue = 0;
        if(value == NULL)
        {
            resultValue = 1;
        }
        else if(value != NULL)
        {
            str = value;
            for ( ; *str; ++str){*str = tolower(*str);}
            if(!strcmp(value, "true"))
            {
                resultValue = 1;
            }
            else if(strcmp(value, "false"))
            {
                setErrorValue(ARGBOOLMISDEF_ERR);
                asmCallError(warn, ("Could not resolve variable value of %s. Continuing with default (false).\n", variable));
                setErrorValue(NO_ERR);
                continue;
            }
        }

        if(!strcmp(variable, "nomap") || !strcmp(variable, "n"))
        {
            noMap = resultValue;
            continue;
        }

        if(!strcmp(variable, "lean") || !strcmp(variable, "l"))
        {
            compileLean = resultValue;
            continue;
        }

        if(!strcmp(variable, "hexdump") || !strcmp(variable, "h"))
        {
            hexDump = resultValue;
            continue;
        }

        if(!strcmp(variable, "config") || !strcmp(variable, "c"))
        {
            inputFileNeeded = 0;
            SETC_COMMENTARY; printf("CONFIGURATION:\n");
            SETC_VARIABLE; printf("Input File: "); SETC_DOTS; printf(".... "); SETC_VALUE; inputFileName != NULL? printf("%s\n", inputFileName):printf("-\n");
            SETC_VARIABLE; printf("Output File: "); SETC_DOTS; printf("... "); SETC_VALUE; outputFileName != NULL? printf("%s\n", outputFileName):printf("-\n");
            SETC_VARIABLE; printf("Show Help: "); SETC_DOTS; printf("..... "); showHelp? SETC_SUCCESS:SETC_ERROR; showHelp? printf("True\n"):printf("False\n");
            SETC_VARIABLE; printf("Show Version: "); SETC_DOTS; printf(".. "); showVersion? SETC_SUCCESS:SETC_ERROR; showVersion? printf("True\n"):printf("False\n");
            SETC_VARIABLE; printf("Sections: "); SETC_DOTS; printf("...... "); SETC_VALUE; showSections();
            SETC_VARIABLE; printf("Nomap: "); SETC_DOTS; printf("......... "); noMap? SETC_SUCCESS:SETC_ERROR; noMap? printf("True\n"):printf("False\n");
            SETC_VARIABLE; printf("Compile Lean: "); SETC_DOTS; printf(".. "); compileLean? SETC_SUCCESS:SETC_ERROR; compileLean? printf("True\n"):printf("False\n");
            SETC_VARIABLE; printf("Show Hexdump: "); SETC_DOTS; printf(".. "); hexDump? SETC_SUCCESS:SETC_ERROR; hexDump? printf("True\n"):printf("False\n");
            SETC_VARIABLE; printf("Show Config: "); SETC_DOTS; printf("... "); SETC_SUCCESS; printf("True\n");
            SETC_NORMAL;
            continue;
        }

        setErrorValue(UDEFPROGARG_ERR);
        asmCallError(warn, ("Undefined argument %s. Ignoring.\n", variable));
        setErrorValue(NO_ERR);
    }

    asmCallError(kill, "Couldn't resolve compiller parameters")
    INFO(printf("Finished parsing parameters\n");)

    //Checking if the input file argument is present if needed
    setErrorIf((inputFileName == NULL) && inputFileNeeded, INFILEUDEF_ERR, 0);

    if(hexDump && !strcmp(strrchr(inputFileName, '.'), BINARY_EXTENSION))
    {
        isCompilation = 0;
        printHexDump(inputFileName);
    }
    
    if(isCompilation)
    {
        //Setting output filename if the output filename is not set already.
        if(!outputFileSet)
        {
            char* extension = strrchr(outputFileName, '.');
            int extLen = strlen(extension);

            outputFileName = (char*)malloc(strlen(inputFileName)-extLen+strlen(".bin"));
            strcpy(outputFileName, inputFileName);
        }

        //HELP      - done
        //VERSION   - done 
        //INPUT     - done
        //OUTPUT    - done
        //CONFIG    - done

        newFILE* newInputFile = openFILE(inputFileName, "r", ASSEMBLY_EXTENSION);
        printf("opened File\n");
        sysCallError(kill, "CAN ONLY COMPILE ASSEMBLY FILES\n");
        
        //DEBUG
        //LEAN
        ByteCode* byteTable = compileFile(newInputFile, NULL, NULL, NULL, NULL, NULL);
        printf("Done compilling\n");

        newFILE* newOutputFile = openFILE(outputFileName, "wb", BINARY_EXTENSION);
        sysCallError(kill, "CAN ONLY WRITE TO BINARY FILES\n");
        printf("Opened file\n");

        //CHECK MEM SECTIONS  -
        //ResolveFileSections -
        //HEXDUMP             -done

        writeBytesToFile(newOutputFile, byteTable, hexDump);
        asmCallError(kill, "Failed\n");

        printf("\n\nDone with compillation\n");
        freeByteCode(byteTable);
    }

    SETC_SUCCESS;
    printf("\n\nFinished !\n");
    SETC_NORMAL;
    return 0;
}