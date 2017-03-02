#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "../../core/pilight.h"
#include "../../core/common.h"
#include "../../core/dso.h"
#include "../../core/log.h"
#include "../protocol.h"
#include "smartwares.h"
 
static void smartwaresPrintHelp(void) {
  printf("\t -c --code=\"raw\"\t\traw code devided by spaces\n\t\t\t\t\t
    (just like the output of debug)\n");
}


static int smartwaresCreateCode(JsonNode *code) {
  char *rcode = NULL;
  char *rncode = NULL;
  char *pch = NULL;
  int i=0;
 
  if(json_find_string(code, "code", &rcode) != 0) {
    logprintf(LOG_ERR, "dummy: insufficient number of arguments");
    return EXIT_FAILURE;
  }
 
  rncode = strdup(rcode);
  pch = strtok(rncode, " ");
  while(pch != NULL) {
    smartwares->raw[i]=atoi(pch);
    pch = strtok(NULL, " ");
    i++;
  }
  sfree((void *)&rncode);
  smartwares->rawlen=i;
  return EXIT_SUCCESS;
}


#if !defined(MODULE) && !defined(_WIN32)
__attribute__((weak))
#endif
void smartwaresInit(void) {
    protocol_register(&smartwares);
    protocol_set_id(smartwares, "smartwares");
    protocol_device_add(smartwares, "smartwares", "Smartwares protocol");
    smartwares->devtype = RAW;
    smartwares->createCode=&smartwaresCreateCode;
    smartwares->printHelp=&smartwaresPrintHelp;
    options_add(&smartwares->options, 'c', "code", OPTION_HAS_VALUE, 0, JSON_STRING, NULL, NULL);
}
 
#if defined(MODULE) && !defined(_WIN32)
void compatibility(struct module_t *module) {
    module->name = "smartwares";
    module->version = "0.1";
    module->reqversion = "5.0";
    module->reqcommit = NULL;
}
 
void init(void) {
    smartwaresInit();
}



#endif
