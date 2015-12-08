#ifndef statemodel_h
#define statemodel_h

#include <stdint.h>

#include "state.h"

//The states
extern state_t accepting;
extern state_t processing;
extern state_t manufacturing;
extern state_t shipping;

typedef enum{
   ORDER_RECEIVED,
   PAYMENT_VALID,
   PAYMENT_INVALID,
   MANU_SUCCESS,
   MANU_FAIL,
   SHIP_SUCCESS,
   SHIP_FAIL,
   ERROR
}event;

void handle_event(event current_event);
void updateStats(stats current_stat);

#endif
