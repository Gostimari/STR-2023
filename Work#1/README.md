Example of the command line to compile a program in C in the file "program.c" (to be developed by you) that links to the object file, creating the executable file "program":

For Ex1, Ex3, Ex4, Ex6:

    gcc -Wall -O2 -D_REENTRANT -lpthread -lrt program.c func.o -o program
    OR
    gcc -Wall -O2 -D_REENTRANT program.c func.o -lpthread -lrt -o program

    The program.c file must include the func.h header (#include "func.h").

For Ex5:

    gcc -Wall -O2 -D_REENTRANT -lpthread -lrt program.c func2.c -o program
    OR
    gcc -Wall -O2 -D_REENTRANT program.c func2.c -lpthread -lrt -o program

    The program.c file must include the func2.h header (#include "func2.h").


