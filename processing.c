#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "processing.h"
#include "statemodel.h"
#include "state.h"

state_t processing = {
   default_event_handler,
   payment_valid,
   payment_invalid,
   default_event_handler,
   default_event_handler,
   default_event_handler,
   default_event_handler,
   entry_to,
   exit_from
};

uint8_t attempts;
state_t * payment_valid()
{
   exit_from();
   return &manufacturing;
}

state_t * payment_invalid()
{
   printf("Payment invalid\n");
   if(attempts >= 3)
   {
      printf("Too many Invalid attempts...\n");
      exit_from();
      return &accepting;
   }

   return &processing;
}

void entry_to()
{
   printf("In processing state...\n");
   attempts = reset_attempts();
   get_payment_method();
}

void exit_from()
{
   printf("Leaving processing...\n");
}

void get_payment_method()
{
   printf("Please enter payment method\n");
}
     
uint8_t reset_attempts()
{
   return  0;
}
