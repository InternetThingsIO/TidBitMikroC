#include "stdint.h"
#include "UART_Ext.h"
#include "TScriptActions.h"

SingleCommand *FillCommand(uint8_t cIndex, uint8_t *currPos);
uint8_t* NextLine(uint8_t *currPos);
uint8_t* RemoveTabs(uint8_t *currPos);
unsigned int GetNumberLines(uint8_t *script);
unsigned int LineLength(uint8_t *currPos);
uint8_t* GetCurrLine(uint8_t *script);
short GetCommand(uint8_t *currLine);
SingleCommand *GetEmptyCommand();
uint8_t* DeTab(uint8_t *currPos);
void ExecuteCommand(SingleCommand *command);
void ExecuteRepeat(SingleCommand *repeatCommand);
void TAction_repeat(SingleCommand *pCommand);
uint8_t TabDepth(uint8_t *currPos);
uint8_t *DoRepeat(SingleCommand *pCommand, uint8_t *currPos);
RepeatCommand *GetEmptyRepeat();
SingleCommand *GetNextCommand();

const CommandDef availableCommands[NUM_AVAILABLE_COMMANDS] = {
                                    //command name, number of parameters, pointer to function
                                    {"delay_ms",          1, &TAction_delay_ms},
                                    {"led_fade_complete", 0, &TAction_led_fade_complete},
                                    {"led_fade",          1, &TAction_led_fade},
                                    {"led_range",         3, &TAction_led_range},
                                    {"led_rotate",        1, &TAction_led_rotate},
                                    {"led_rotate_range",  3, &TAction_led_rotate_range},
                                    {"repeat",            1, &TAction_empty},
                                    {"led_set",           2, &TAction_led_set},
                                    {"show_bmp",          1, &TAction_show_bmp},
                                    {"clear_lcd",         0, &TAction_clear_lcd}
                                  };
                                  
//holds the pointer to the current position in the script
uint8_t *currPos;

//must be called first
//initializes any devices available for the action scripts
void TEngine_InitDevices(){
    TAction_InitDevices();
}

//Responsible for parsing the script and performing actions
char A2_ProcessScript(uint8_t *script){
    SingleCommand *pCommand, command;
    currPos = script;

    while (currPos != 0){

        pCommand = GetNextCommand();
        
        if (currPos == 0)
            return 1;
        
        if (pCommand != 0){
        
        command = *pCommand;
        
        //add if isRepeat here so that we don't do ExecuteCommand after a repeat
          if (strcmp(command.Name, "repeat") == 0)
            ExecuteRepeat(pCommand);
          else
            ExecuteCommand(pCommand);
          
        }
    }
    
    return 1;
}

//get the next command or return 0
SingleCommand *GetNextCommand(){
  short commandIndex;
  uint8_t *currLine;
  SingleCommand *pCommand;

  //parse out the current line
  currLine = GetCurrLine(currPos);
  //stop if there is nothing on this line
  if (currLine == 0){
    writeStr("currLine == 0");
    currPos = 0;
    return 0;
  }

  //get index current line if it's a command
  commandIndex  = GetCommand(currLine);

  //if we found a command parse it into a command and execute it
  if (commandIndex != -1){
    pCommand = FillCommand(commandIndex, currPos);
  }

 //move to next line
  currPos = NextLine(currPos);

  if (commandIndex != -1)
    return pCommand;

  return 0;
}

//do what the command asks for
void ExecuteCommand(SingleCommand *pCommand){
    SingleCommand command;
    command = *pCommand;
    writeStr("Found Command:");
    writeStr(command.Name);
    
    //TODO protect this if there is no function
    availableCommands[command.CommandIndex].function(pCommand);
}

void ExecuteRepeat(SingleCommand *repeatCommand){
    SingleCommand *pCommand;
    SingleCommand command, repCommand;
    RepeatCommand *pRepeat;
    RepeatCommand repeat;
    uint8_t i, y, numRepeats;
    uint8_t *prePos;
    
    repCommand = *repeatCommand;
    
    if (strcmp(repCommand.Name, "repeat") != 0)
        return;

    WriteStr("Found a REPEAT!");

    pRepeat = GetEmptyRepeat();

    repeat = *pRepeat;
    
    //TODO: number of repeats is 0 for some reason...  Fix it!!
    writeStr("Number Repeats:");
    writeStr(repCommand.Parameters[0]);
    writeStr(repCommand.Name);
    writeShort(repCommand.CommandIndex);
    writeShort(repCommand.Depth);

    //fill the repeat
    for (i = 0; i < MAX_REPEAT_COMMANDS; i++){

        WriteShort(i);

        prePos = currPos;
        
        pCommand = 0;

        while (pCommand == 0 && currPos != 0){
            pCommand = GetNextCommand();
        }
        command = *pCommand;

        if (currPos == 0 || pCommand == 0){
            writeStr("Ran out of commands mid repeat");
            return;
        }
            
        if (!(command.Depth > repCommand.Depth)){
            currPos = prePos;
            writeStr("Repeat ended, breaking");
            writeStr("Depths:");
            writeShort(command.Depth);
            writeShort(repCommand.Depth);
            break;
        }
        
        //add the command to the repeat
        repeat.command[i] = *pCommand;
        writeStr("Adding to repeat:");
        writeStr(command.Name);
    }
    
    //execute the repeat
    numRepeats = atoi(repCommand.Parameters[0]);
    writeStr("Number Repeats:");
    writeStr(repCommand.Parameters[0]);
    writeShort(numRepeats);
    
    for (i = 0; i < numRepeats; i++){
      for (y = 0; y < MAX_REPEAT_COMMANDS; y++){
        command = repeat.command[y];
        if(strlen(command.Name) > 0)
            ExecuteCommand(&command);
      }
    }

}

//loops through the available commands, returns the index of the command or -1 if no command
short GetCommand(uint8_t *currLine){
    uint8_t i;
    
    for (i = 0; i < NUM_AVAILABLE_COMMANDS; i++){
        if (strcmp(DeTab(currLine), availableCommands[i].Name) == 0 && strlen(currLine) > 1){
            return i;
        }
    }
    return -1;
}

//Fills out a command struct with name and parameters
SingleCommand *FillCommand(uint8_t cIndex, uint8_t *currPos){
    uint8_t i;
    uint8_t *currLine;
    SingleCommand *pCommand, command;

    //fill with null
    pCommand = GetEmptyCommand();
    
    command = *pCommand;
    
    //get the command's name
    command.Depth = TabDepth(currPos);
    writeStr("TabDepth:");
    writeShort(command.Depth);
    currLine = DeTab(GetCurrLine(currPos));
    memcpy(command.Name, currLine, strLen(currLine));

    //store the command's index
    command.CommandIndex = cIndex;

    //advance to the next line which has the parameters
    currPos = NextLine(currPos);

    //loop through each expected parameter updating currPos at the end of each loop
    for (i = 0; i < availableCommands[cIndex].NumberParameters; i++, currPos = NextLine(currPos)){
        //protect this incase the script ends prematurely
        if (currPos != 0){
            currLine = DeTab(GetCurrLine(currPos));
            //take each parameter and place it in the command's parameters
            memcpy(command.Parameters[i], currLine, strLen(currLine));
        }
    }

    *pCommand = command;

    return pCommand;
}

SingleCommand *GetEmptyCommand(){
    static SingleCommand command;
    uint8_t i;

    memset(command.Name, '\0', MAX_COMMAND_LENGTH);
    
    for (i = 0; i < MAX_PARAMETERS; i++){
        memset(command.Parameters[i], '\0', MAX_PARAMETER_LENGTH);
    }
    
    return &command;
}

RepeatCommand *GetEmptyRepeat(){
    static RepeatCommand currRepeat;
    uint8_t i;

    currRepeat.count = 0;

    for (i = 0; i < MAX_REPEAT_COMMANDS; i++){
        currRepeat.command[i] = *GetEmptyCommand();
    }

    return &currRepeat;
}

//returns the number of lines in the script
unsigned int GetNumberLines(uint8_t *script){
    unsigned int lines = 0;

    while (script != 0){;
        script = NextLine(script);
        lines++;
    }
    
    writeStr("Number of lines in script:");
    writeInt(lines);
    
    return lines;
}

//returns the value of the text up until the CR LF indicating the start of a new line
uint8_t* GetCurrLine(uint8_t *script){
    static uint8_t currLine[MAX_LINE_LENGTH];

    uint8_t *begin, *end;
    memset(currLine, '\0', MAX_LINE_LENGTH);

    //writeStr("Get Line");

    begin = script;
    end = strstr(script, "\r\n");

    if (end != 0){
        return memcpy(currLine, begin, end-begin);
    }
    //this is the last line, we need to handle it
    writeStr("didn't find end of line");
    if (strlen(script) > 0){
        return memcpy(currLine, script, strlen(script));
    }


    return 0;


}

//returns a pointer to the next line or 0 if there isn't one
uint8_t* NextLine(uint8_t *currPos){

    currPos = strstr(currPos, "\r\n");

    if (currPos != 0)
        return currPos + 2;
        
    return 0;
}

//returns pointer after tabs
uint8_t* DeTab(uint8_t *currPos){
    while(currPos[0] == '\t')
        currPos += 1;
    return currPos;
}

uint8_t TabDepth(uint8_t *currPos){
    uint8_t tabs = 0;
    
     while(currPos[0] == '\t'){
        currPos += 1;
        tabs++;
    }
    return tabs;
}