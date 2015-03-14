#include "__Net_Wireless_CC3000_PIC32.h"
#include "UART_Ext.h"
//#include "stdlib.h"

const uint8_t ELEMENT_VALUE_SIZE = 25;

uint8_t elementValue[ELEMENT_VALUE_SIZE];

uint8_t* removeQuotes( uint8_t *eValue);

uint8_t* getJSONElement(uint8_t HTTPBody[], uint8_t eName[]){
  //uint8_t
  uint8_t *tmp1, *tmp2, *eValue;

  memset(elementValue, '\0', ELEMENT_VALUE_SIZE);
  
  eValue = elementValue;
  
  WriteStr("test2");

	//get first instance of element name
  tmp1 = strstr(HTTPBody, eName);
  if (tmp1 == 0)
  	return 0;

  writeStr(tmp1);

	//then find the : which indicates the value is next
  tmp1 = strchr(tmp1, ':');
  if (tmp1 == 0)
  	return 0;

  //increment pointer by 1 to get rid of : character
  tmp1 = &tmp1[1];

  writeStr(tmp1);

  //get pointer to next comma
  tmp2 = strchr(tmp1, ',');
	if (tmp2 == 0)
	  tmp2 = strchr(tmp1, '}');
	  if (tmp2 == 0)
	    return 0;
	    
	writeStr("");
  writeStr(tmp2);
  writeStr(tmp1);

  //copy distance between first and second pointers to eValue
  memcpy(eValue, tmp1, tmp2-tmp1);
  writeStr(eValue);

  //remove quotes if there are any and return
  if(strlen(eValue) > 2)
  	eValue = removeQuotes(eValue);
  	
  WriteStr("test6");
  
  return eValue;
}

uint8_t* removeQuotes( uint8_t *eValue){

  //if the value is encapsulated in quotes
  if(eValue[0] == '\"' && eValue[strlen(eValue)-1] == '\"'){
    WriteStr("Need to remove quotes");
    eValue = &eValue[1];
    WriteStr(eValue);
    eValue[strlen(eValue)-1] = '\0';
    WriteStr(eValue);
  }

 return eValue;

}