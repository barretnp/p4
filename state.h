#ifndef state_h
#define state_h

#include <stdint.h>

typedef struct state state_t;

typedef state_t * event_handler(void);

typedef void action(void);

typedef enum
{
   FAIL,
   LOST,
   DONE

}stats;

struct state {
   event_handler * order_received;
   event_handler * payment_valid;
   event_handler * payment_invalid;
   event_handler * manu_success;
   event_handler * manu_fail;
   event_handler * ship_success;
   event_handler * ship_fail;
   action        * entry_to;
   action        * exit_from;

};

extern state_t * default_event_handler();
extern void default_action();
extern uint8_t attempts;

#endif
