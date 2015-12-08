#include <stdio.h>
#include "statemodel.h"
#include "state.h"

int main(int argc, char **argv )
{
   int input;
  printf("In accepting state...\n"); 
   input = getchar();
   attempts = 0;
   uint8_t num_invalid = 0;

   while(input != 'X')
   {
      if(input != '\n')
      {

         if(input == 'O')
         {
            handle_event(ORDER_RECEIVED);
         }
         else if(input == 'V')
         {
            handle_event(PAYMENT_VALID);        
         }
         else if(input == 'I')
         {
            num_invalid++;
	    attempts = num_invalid % 4;
            handle_event(PAYMENT_INVALID);        
         }
         else if(input == 'F')
         {
            handle_event(MANU_FAIL);        
         }
         else if(input == 'C')
         {
            handle_event(MANU_SUCCESS);        
         }
         else if(input == 'R')
         {
            handle_event(SHIP_SUCCESS);        
         }
         else if(input == 'L')
         {
            handle_event(SHIP_FAIL);        
         }
         else
         {
            handle_event(ERROR);
         }
      }

      input = getchar();

      //if(input == '\n')
      //{
        // input = getchar();
      //}

   }

   return 0;
}
