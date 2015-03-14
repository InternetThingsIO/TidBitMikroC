#include "CC3000/CC3000_Driver.h"
#include "stdint.h"
#include "UART_Ext.h"
//#include "JSON.h"


const uint8_t hubHost[] = "dev-test";
uint8_t *PtrEValue;

//this is the entry point to check each connector
//returns a single action script. Returns 0 on an error or if there is no action script
uint8_t *checkConnectors(uint8_t *device_id){
  uint8_t *HTTPBody;
  uint8_t device_path[200] = "/ConnectorService/device/";

  writeStr("Checking connectors");

  //put the deviceID on the path
  strcat(device_path, device_id);
  writeStr(device_path);

  HTTPBody = Wifi_Get(hubHost, device_path, 8081);

  if (HTTPBody != 0){
    //download and return the action script
    //writeStr(HTTPBody);
    
    //see if we have a result
    if (strlen(HTTPBody) > 0)
        return HTTPBody;
    else
        return 0;
  }
  else{
    WriteStr("Failed to get action script HTTP response for some reason");
    return 0;
  }

return 0;

}