#include "../headers/Adapter.h"

#include <string.h>
#include <stdio.h>

#include "../headers/INFO.h"
#include "../headers/Error.h"
#include "../headers/Adapter.h"





//STRUCT FUNCTIONS

//FILES

newFILE* openFILE(char* filePath, const char* openMode, const char* extensionTarget)
{
    //STD checking
    setErrorIf(strlen(filePath) > FILEPATH_MAX, FILEPATH_ERR, NULL);
    newFILE* returnFile = (newFILE*)malloc(sizeof(newFILE));
    setErrorIf(returnFile == NULL, ALLOC_ERR, NULL);

    //ADDING the variables to the newFILE struct for easier manipulation
    strcpy(returnFile->filePath, filePath);
    returnFile->fileName = getFileName(returnFile->filePath);
    returnFile->extension = strchr(returnFile->fileName, '.');
    strcpy(returnFile->fileOpenMode, openMode);
    returnFile->fileLine = NULL;
    returnFile->lineNumber = 0;    //Line number indicates the number of the line contained in file->fileLine
    returnFile->cursorPos = 0;
    returnFile->filePTR = NULL;

    setErrorIf(returnFile->extension == NULL, NLLFILEXT_ERR, NULL);
    if(extensionTarget != NULL)
    {
        setErrorIf(strcmp(returnFile->extension, extensionTarget), EXTTARG_ERR, NULL)
    }
    returnFile->filePTR = fopen(returnFile->filePath, openMode);

    if(returnFile->filePTR == NULL)
    {
        setErrorIf(openMode[0] != 'w', FILEUKNW_ERR, NULL);
        setErrorIf(returnFile->fileName == returnFile->filePath, FILEACCS_ERR, NULL);

        //CREATING FILE PATH
        returnFile->fileName[-1] = '\0';
        char createCommand[7+FILEPATH_MAX];
        strcpy(createCommand, "mkdir ");
        strcat(createCommand, returnFile->filePath);
        system(createCommand);
        returnFile->fileName[-1] = systemFolderSeperator;

        //ATTEMPTING TO REOPEN FILE
        returnFile->filePTR = fopen(returnFile->filePath, openMode);
        setErrorIf(returnFile->filePTR == NULL, FILEACCS_ERR, NULL);
    }

    return returnFile;
}


char* readFileLine(newFILE* fileStruct)
{
    setErrorIf(fileStruct == NULL, NULLARG_ERR, NULL);
    setErrorIf(fileStruct->filePTR == NULL, NULLARG_ERR, NULL);
    exitIf(feof(fileStruct->filePTR), NULL);

    if(fileStruct->fileLine != NULL)
    {
        free(fileStruct->fileLine);
    }

    char currentChar = fgetc(fileStruct->filePTR);
    int numCharsRead = 0;
    char* newLine = (char*)malloc(sizeof(char));
    setErrorIf(newLine == NULL, ALLOC_ERR, NULL);

    while(currentChar != '\n' && currentChar != EOF)
    {
        newLine[numCharsRead] = currentChar;
        fileStruct->cursorPos++;
        numCharsRead++;
        newLine = (char*)realloc(newLine, (numCharsRead+1)*sizeof(char));
        setErrorIf(newLine == NULL, ALLOC_ERR, NULL);
        currentChar = fgetc(fileStruct->filePTR);
    }
    newLine[numCharsRead] = '\0';
    //setErrorIf(strlen(newLine) > FILELINE_MAX, LINELENG_ERR, NULL);
    fileStruct->fileLine = newLine;
    fileStruct->lineNumber++;

    if(fileStruct->fileLine[0] == '\0')
    {
        readFileLine(fileStruct);
    }

    return newLine;
}


void closeFILE(newFILE* fileStruct)
{
    exitIf(fileStruct == NULL, );
    setErrorIf(fileStruct->filePTR == NULL, NULLARG_ERR, );

    fclose(fileStruct->filePTR);
    free(fileStruct->fileLine);
    free(fileStruct);

    return;
}


void resetFILE(newFILE* fileStruct)
{
    setErrorIf(fileStruct == NULL, NULLARG_ERR, );
    setErrorIf(fileStruct->filePTR == NULL, NULLARG_ERR, );

    fseek(fileStruct->filePTR, 0, SEEK_SET);
    fileStruct->cursorPos = 0;
    fileStruct->lineNumber = 0;

    return;
}



//FUNCTIONS
//FILES

void removePathRedundancy(char* filePath)
{
    setErrorIf(filePath == NULL, NULLARG_ERR, );

    printf("Reducing %s\n", filePath);
    printf("end address: %p\n", filePath);

    char* currentFolderName = filePath;
    char* nextFolderSeperator = strchr(filePath, systemFolderSeperator);

    printf("Searching for last goBack\n");
    while(nextFolderSeperator != NULL)
    {
        nextFolderSeperator[0] = '\0';
        if(strcmp(currentFolderName, ".."))
        {
            break;
        }

        nextFolderSeperator[0] = systemFolderSeperator;
        currentFolderName = nextFolderSeperator+1;
        nextFolderSeperator = strchr(currentFolderName, systemFolderSeperator);
    }
    if(currentFolderName[0] == '\0')
    {
        return;
    }

    int definedFolders = 1;
    char* previousFolderName = currentFolderName;

    printf("Removing files\n");
    while(nextFolderSeperator != NULL)
    {
        nextFolderSeperator[0] = '\0';
        printf("Current folder name is %s\n", currentFolderName);
        if(!strcmp(currentFolderName, ".."))
        {
            if(definedFolders != 0)
            {
                definedFolders--;
                strcpy(previousFolderName, nextFolderSeperator+1);
                currentFolderName = previousFolderName;
                nextFolderSeperator = strchr(currentFolderName, systemFolderSeperator);
                if(nextFolderSeperator == NULL)
                {
                    break;
                }
            }
        }
        else
        {
            definedFolders++;
        }
        printf("Current folder name is %s\n", currentFolderName);

        if(!strcmp(currentFolderName, "."))
        {
            strcpy(currentFolderName, nextFolderSeperator+1);
            nextFolderSeperator = strchr(currentFolderName, systemFolderSeperator);
            continue;
        }

        previousFolderName = currentFolderName;
        nextFolderSeperator[0] = systemFolderSeperator;
        currentFolderName = nextFolderSeperator+1;
        nextFolderSeperator = strchr(currentFolderName, systemFolderSeperator);
    }

    printf("Final path is %s\n", filePath);
    printf("end address: %p\n", filePath);
    return;
}



//STRINGS

void removeExtraSpace(char* string/*, int maxLength*/)
{
    setErrorIf(string == NULL, NULLARG_ERR, );

    char* currentPos = string;
    int spacePlaced = 1;        //To offset the first space without further modification.

    while(currentPos[0] != '\0')
    {
        //printf("Space is %d at char %c\n", spacePlaced, currentPos[0]);
        if(currentPos[0] <= ' ')       //newline is ignored, a string should not be multiple lines.
        {
            if(spacePlaced)
            {
                strcpy(currentPos, currentPos+1);
                continue;
            }
            currentPos[0] = ' ';
            spacePlaced = 1;
        }
        else if(currentPos[0] == ARGSEP_CHAR)
        {
            //if(spacePlaced && (currentPos != string));
            if((currentPos != string))
            {
                if(spacePlaced)
                {
                    printf("Removing char: %c\n", currentPos[0]);
                    strcpy(currentPos-1, currentPos);
                    currentPos--;
                }
            }
            spacePlaced = 1;
        }
        else{spacePlaced = 0;}

        //printf("Space is %d at char %c\n", spacePlaced, currentPos[0]);
        currentPos++;
    }

    
    if(string[strlen(string)-1] == ' ' || string[strlen(string)-1] == '\n')
    {
        strcpy(currentPos-1, currentPos);
    }

    return;
}


void removeComment(char* string)
{
    setErrorIf(string == NULL, NULLARG_ERR, );

    char* currentPos = string;

    while(currentPos[0] != '\0')
    {
        if(currentPos[0] == COMMENT_CHAR)
        {
            currentPos[0] = '\0';
            break;
        }
        currentPos++;
    }
}


bool getIntValue(char* intString, char endChar, int* resultLoc)
{
    setErrorIf(intString == NULL, NULLARG_ERR, 0);

    int resultInt = 0;
    int stringPTR = 0;
    int decodeMode = 10;     //2 = binary, 10 = decimal, 16 = hex
    char currentChar;

    if(intString[0] == '0' && strlen(intString) >= 3)
    {
        stringPTR += 2;
        if(intString[1] == 'b')
        {
            decodeMode = 2;
        }
        else if(intString[1] == 'd')
        {
            decodeMode = 10;
        }
        else if(intString[1] == 'x')
        {
            decodeMode = 16;
        }
        else
        {
            stringPTR -= 2;
        }
    }

    currentChar = intString[stringPTR];

    while(currentChar != endChar)
    {   
        setErrorIf((currentChar < '0'), INVNUM_ERR, false);
        setErrorIf((currentChar > '1' && decodeMode == 2), INVNUM_ERR, false);
        setErrorIf((currentChar > '9' && decodeMode == 10), INVNUM_ERR, false);

        if(decodeMode==16)
        {
            setErrorIf((currentChar > '9' && (currentChar < 'A' || currentChar > 'F') && (currentChar < 'a' || currentChar > 'f')), INVNUM_ERR, false);
            
            if(!((currentChar < 'A' || currentChar > 'F') && (currentChar < 'a' || currentChar > 'f')))
            {
                currentChar -= 0x07;
                if(currentChar > 0x3F)
                {
                    currentChar -= 0x20;
                }
            }
        }

        resultInt *= decodeMode;
        resultInt += currentChar-0x30;

        currentChar = intString[++stringPTR];
        continue;
    }

    if(resultLoc != NULL)
    {
        *resultLoc = resultInt;
    }
    return true;
}



//TYPES

bool isLabel(char* lineString)
{
    exitIf(lineString == NULL, false);
    int stringLen = strlen(lineString);
    exitIf(lineString[stringLen-1] != END_LABEL_CHAR, false);
    return true;
}


bool isDirective(char* lineString)
{
    exitIf(lineString[0] != OPEN_DIRECTIVE_CHAR, false);
    exitIf(lineString[strlen(lineString)-1] != CLOSE_DIRECTIVE_CHAR, false);

    return true;
}


bool isPragma(char* lineString)
{
    exitIf(lineString[0] != START_PRAGMA_CHAR, false);

    return true;
}


bool isChar(char* string)
{
    exitIf(string[0] != CHARINT_CHAR, false);
    exitIf(string[strlen(string)-1] != CHARINT_CHAR, false);

    return true;
}


bool isModMemChar(char* string)
{
    exitIf(string == NULL, false);

    if(string[0] == MEMADDR_CHAR && string[1] == MEMDEREFOPEN_CHAR && string[strlen(string)-1] == MEMDEREFCLOSE_CHAR)
    {
        return true;
    }

    return false;
}


char* searchClosingPriority(char* inputString)
{
    exitIf(inputString == NULL, NULL);

    int priorCount = 0;
    char currentChar = '$';
    int index = 0;
    bool openEncountered = false;

    while(currentChar != '\0')
    {
        currentChar = inputString[index++];

        if(currentChar == PRIORITYOPEN_CHAR)
        {
            priorCount++;
            openEncountered = true;
        }
        else if(currentChar == PRIORITYCLOSE_CHAR)
        {
            priorCount--;
        }

        if(priorCount == 0 && openEncountered)
        {
            return inputString+index-1;
        }
    }

    return NULL;
}