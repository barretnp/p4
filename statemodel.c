#include <stdlib.h>
#include <stdio.h>
#include "statemodel.h"

static state_t * current_state = &accepting;

void handle_event( event current_event )
{
   state_t * next_state;

   next_state = NULL;

   switch( current_event )
   {
      case ORDER_RECEIVED:
         next_state = current_state->order_received();
         break;
      case PAYMENT_VALID:
         next_state = current_state->payment_valid();
         break;
      case PAYMENT_INVALID:
         next_state = current_state->payment_invalid();
         break;
      case MANU_SUCCESS:
         next_state = current_state->manu_success();
         break;
      case MANU_FAIL:
         next_state = current_state->manu_fail();
         break;
      case SHIP_SUCCESS:
         next_state = current_state->ship_success();
         break;
      case SHIP_FAIL:
         next_state = current_state->ship_fail();
         break;
      default:
         printf("ERROR: Not a valid state\n");
   }

   if(next_state != NULL)
   {
      current_state = next_state;
      current_state->entry_to();
   }
}

void updateStats(stats current_stat)
{
   if(current_stat == FAIL)
   {
      printf("Failure in manufacturing...\n");
   }
   else if(current_stat == LOST)
   {
      printf("Shipment was lost in transition...\n");
   }
   else if(current_stat == DONE)
   {
      printf("Shipment complete...\n");
   }
   else
   {
      printf("Invalid stat\n");
   }
}
