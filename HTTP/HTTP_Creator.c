#include "UART_Ext.h"

//host: projectiris.cloudapp.net
//path: /api/contact.  Leading / is required
//returns a 200 length string that can be used to submit a GET request.
void HTTP_Generate_Get(char host[], char path[], char httpGetRequest[]){

  memset(httpGetRequest, '\0', sizeof(httpGetRequest));

  strcat(httpGetRequest, "GET http://");
  strcat(httpGetRequest, host);
  strcat(httpGetRequest, path);
  strcat(httpGetRequest, " HTTP/1.1\nHost: ");
  strcat(httpGetRequest, host);
  strcat(httpGetRequest, "\n\n");

}