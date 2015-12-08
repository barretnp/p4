#ifndef state_c
#define state_c

#include <stdlib.h>
#include <stdio.h>
#include "state.h"

state_t * default_event_handler()
{
   default_action();

   return NULL;
}


void default_action()
{
   printf("Invalid event from current state.\n");
}

#endif
