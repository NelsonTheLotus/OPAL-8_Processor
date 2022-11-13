#pragma once

#include "../headers/INFO.h"
#include "../headers/Adapter.h"
#include "../headers/Translator.h"
#include "../headers/Error.h"





void writeBytesToFile(newFILE* outputFile, ByteCode* bytes, int hexDump);
void showSections();
void printHexDump(char* filename);

