#ifndef shared_h
#define shared_h

#include <semaphore.h>
#include <pthread.h>

typedef struct {
   uint32_t items;
   uint32_t duration;
} line_info;

typedef struct {
   uint32_t capacity;
   uint32_t duration;
} line_attr;

typedef struct {
   uint32_t order_size;
   line_attr line_attr_arr[5]; 
   pthread_mutex_t lock;
   sem_t  order_sem;
   sem_t  message_sem;
   sem_t pause_parent;
   sem_t parent_lock;
} shared_data;

typedef struct {
   long msg_type;
   struct info {
      uint32_t id;
      uint32_t duration;
      uint32_t items;
   } info;
} msg_buf;

#define SHMEM_KEY 0x100
#define SHMEM_SIZE sizeof( shared_data )

#endif
