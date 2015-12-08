#ifndef processing_h
#define processing_h

#include "state.h"

static state_t * payment_valid();
static state_t * payment_invalid();
static void get_payment_method();
static void entry_to();
static void exit_from();
static uint8_t reset_attempts();

#endif
