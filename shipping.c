#include <stdio.h>

#include "shipping.h"
#include "statemodel.h"

state_t shipping = {
   default_event_handler,
   default_event_handler,
   default_event_handler,
   default_event_handler,
   default_event_handler,
   ship_success,
   ship_fail,
   entry_to,
   exit_from
};

state_t * ship_success()
{
   printf("Successfully shipped parts...\n");
   start_warranty();
   updateStats(DONE);
   exit_from();
   return &accepting;
}

state_t * ship_fail()
{
   printf("Failed to ship parts...\n");
   refund();
   updateStats(LOST);
   return &accepting;
   
}

void entry_to()
{
   printf("In shipping state...\n");
   get_address();
}

void exit_from()
{
   printf("Leaving shipping...\n");
}

void get_address()
{
   printf("Enter Address:\n");
}

void refund()
{
   printf("You are given a refund...\n");
}

void start_warranty()
{
   printf("Starting warranty...\n");
}
