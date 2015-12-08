#include <stdio.h>

#include "accepting.h"
#include "statemodel.h"

state_t accepting = {
   order_received,
   default_event_handler,
   default_event_handler,
   default_event_handler,
   default_event_handler,
   default_event_handler,
   default_event_handler,
   entry_to,
   exit_from
};

state_t * order_received()
{
   exit_from();
   return &processing;
}

void entry_to()
{
   printf("In accepting state...\nReady to process...\n");
}

void exit_from()
{
   printf("Received order now...\n");
}
