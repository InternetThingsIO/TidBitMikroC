#include "stdint.h"

#define MAX_COMMAND_LENGTH 20
#define MAX_PARAMETERS 10
#define MAX_PARAMETER_LENGTH 20
#define MAX_REPEAT_COMMANDS 10
#define MAX_LINE_LENGTH 50
#define NUM_AVAILABLE_COMMANDS 12

#define TRUE 1
#define FALSE 0

void TAction_ClearDisplay();

typedef struct{
    uint8_t Name[MAX_COMMAND_LENGTH]; //name of the command
    uint8_t Parameters[MAX_PARAMETERS][MAX_PARAMETER_LENGTH]; //parameters for the command
    uint8_t CommandIndex; //index of the command in availableCommands
    uint8_t Depth;
} SingleCommand;

typedef struct{
    uint8_t name[MAX_COMMAND_LENGTH];
    unsigned int NumberParameters;
    void (*function)(SingleCommand *pCommand);
} CommandDef;

typedef struct{
    uint8_t count; //number of times to repeat commands
    SingleCommand command[MAX_REPEAT_COMMANDS];
} RepeatCommand;