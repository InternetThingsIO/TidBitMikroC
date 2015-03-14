#include "UART_Ext.h"

int isHTTP(uint8_t sHTTPResult[]);
void getHTTPCode(uint8_t sHTTPResult[], uint8_t outHTTPCode[]);

//Validates HTTP response and extracts HTTP Body if possible.
//sHTTPResult: HTTP response from server
//outHTTPBody: Pointer to store extracted HTTP body
//Returns 0 if failed, 1 if succeeded
char HTTP_CheckResult(uint8_t sHTTPResult[]){
  uint8_t sBuffer[20];
  memset(sBuffer, '\0', sizeof(sBuffer));
  writeInt(*sHTTPResult);
  
  //if this is a correct HTTP response from the server
  if(isHTTP(sHTTPResult) == 1){
    //extract the HTTP code
    getHTTPCode(sHTTPResult, sBuffer);
    writeStr(sBuffer);
    
    //if the response is OK
    if (strcmp(sBuffer, "200") == 0){
            return 1;
    }
    else
      writeStr("HTTP Code was not 200");
    
  }
  else
          writeStr("Response is not HTTP");

        return 0;


}

//returns the HTTP response code
void getHTTPCode(uint8_t sHTTPResult[], uint8_t outHTTPCode[]){
  writeInt(*sHTTPResult);
  memcpy(outHTTPCode, &sHTTPResult[9], 3);
}


int isHTTP(uint8_t sHTTPResult[]){
  uint8_t sBuffer[15];
  memset(sBuffer, '\0', sizeof(sBuffer));
  
  writeInt(*sHTTPResult);

  if(strlen(sHTTPResult) > 10){
    memcpy(sBuffer, sHTTPResult, 8);
    writeStr("HTTP Header:");
    writeStr(sBuffer);

    //compare copied portion to what we exepect the first 8 characters to be
    if (strcmp(sBuffer, "HTTP/1.1") == 0)
            return 1;
    else
            return 0;

  }

}