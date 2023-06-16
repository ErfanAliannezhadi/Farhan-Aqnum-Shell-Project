#include "shell_start.h"


struct CommandBlock** initializeArrayOfCommandBlocks(void)
{
    struct CommandBlock** commandBlockArray = calloc(countList+1, sizeof(struct CommandBlock*));

    if(commandBlockArray == NULL)
        return NULL;

    for(size_t i = 0; i < countList+1; ++i) {
        commandBlockArray[i] = constructCommandBlock(globalSpecialCharIndexArray, i, NULL);
        parseCommandBlock(commandBlockArray[i]);
    }

    return commandBlockArray;
}

void destroyArrayOfCommandBlocks(struct CommandBlock** commandBlocks, size_t sizeOfCommandBlocks)
{
    if(commandBlocks == NULL || sizeOfCommandBlocks == 0)
        return;

    for(size_t i = 0; i < sizeOfCommandBlocks; ++i) {
        destroyCommandBlock(commandBlocks[i]);
    }

    free(commandBlocks);
    commandBlocks = NULL;
}

void start_shell(void)
{
    char* command = read_storeCommands();
    if(command == NULL)
        return;

    pid_t childPID = 0;
    GlobalSpecialCharCounter();

    size_t* globalSpecialCharIndexArray = GlobalSpecialCharIndexArray();

    struct CommandBlock** commandBlockArray = initializeArrayOfCommandBlocks();

    if(commandBlockArray == NULL)
        return;

    if(globalSpecialCharIndexArray != NULL) {
        
        childPID = executeCommandBlock(commandBlockArray[0]);

        for(size_t i = 0; i < countList; ++i) {

            if(strcmp(argv[globalSpecialCharIndexArray[i]], "&&") == 0) {
                   if(childPID == -1)
                        break;
                   childPID =  andList(commandBlockArray[i+1], childPID);
                   continue;
                }

            if(strcmp(argv[globalSpecialCharIndexArray[i]], "||") == 0) {
                    if(childPID == -1)
                        break;
                    childPID = orList(commandBlockArray[i+1], childPID);
                    continue;
            }
            if(strcmp(argv[globalSpecialCharIndexArray[i]], ";") == 0) {
                    if(childPID == -1)
                        break;
                    childPID  = semicolonList(commandBlockArray[i+1], childPID);
            }
        }
        
    }

    else 
        executeCommandBlock(commandBlockArray[0]);
    
        
    free(globalSpecialCharIndexArray);
    destroyArrayOfCommandBlocks(commandBlockArray, countList + 1);
}