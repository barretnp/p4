#ifndef manufacturing_h
#define manufacturing_h
#include <semaphore.h>

#include "state.h"

static state_t * manu_success();
static state_t * manu_fail();
static void entry_to();
static void exit_from();
static void dispatch_factory_lines();
static void shutdown_factory_lines();
static void charge_client();
typedef struct {
   uint32_t capacity;
   uint32_t duration;
   uint32_t id_num;
} order_struct;

typedef struct {
   uint32_t request_num;
   uint32_t line_num;
}  line_response;

typedef struct {
   uint32_t iterations;
   uint32_t num_completed;
}  line_stats;


#endif