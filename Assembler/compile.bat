cd ../../Code/src

gcc newOASIS.c -c -o ../../bin/OASIS.o
gcc Instructions.c -c -o ../../bin/Instruction.o
gcc Adapter.c -c -o ../../bin/Adapter.o
gcc Translator.c -c -o ../../bin/Translator.o
gcc Error.c -c -o ../../bin/Error.o

cd ../../bin

gcc OASIS.o Adapter.o Error.o Instruction.o Translator.o -o main.exe



cd ../Makerules/windows