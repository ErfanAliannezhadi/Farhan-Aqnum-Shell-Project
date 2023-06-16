#include "nonBuiltIn.h"
#include "basic.h"

void exec_nonBuiltInCommand(size_t indexArgv, size_t argc)
{
    if(argc == 1) {
            if(execvp(argv[indexArgv], NULL) == -1) {
                freeAllBlocks();
                errExit("execvp");
            }

        }
        
    else {
        size_t sizeOfNewArgv = argc+1;
        char** newArgv = malloc(sizeOfNewArgv*(sizeof(char*)));
        initializePointerBuffer((void*)newArgv, sizeOfNewArgv);

        for(size_t i = 0; i < sizeOfNewArgv - 1 && i < BUFFSIZE; ++i)
            newArgv[i] = argv[indexArgv+i];
        if(execvp(argv[indexArgv], newArgv) == -1) {
            freeAllBlocks();
            free(newArgv);
            newArgv = NULL;
            errExit("execvp");
        }
    }
}

pid_t nonBuiltInCommand(size_t indexArgv, size_t argc)
{
    pid_t childPID = 0;
    switch(fork()) {
        case 0:
            exec_nonBuiltInCommand(indexArgv, argc);
            break;
        default:
            childPID = wait(NULL);
            break;
    }

    return childPID;
}