#include "stdint.h"
#include "SDCard.h"

//this is the entry point to check each connector
//returns a single action script. Returns 0 on an error or if there is no action script
void checkFile(uint8_t *ptrFileBody){

  SDCard_getTestScript(ptrFileBody);

}