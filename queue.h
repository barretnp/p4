//---------------------------------------------------------------
// File: Queue.h
// Purpose: Header file for a demonstration of a queue implemented 
//        as an array.  Data type: int
// Programming Language: C
// Based on the Code130_Queue implementation by Dr. Rick Coleman
// 
// Modified: Paul Easterbrooks
// Date: December 10, 2015
//
//---------------------------------------------------------------
#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>

#define MAX_SIZE    5        // Define maximum length of the queue

// List Function Prototypes
void InitQueue();             // Initialize the queue
void ClearQueue();            // Remove all items from the queue
int Enqueue(int id);         // Enter an item in the queue
char Dequeue();               // Remove an item from the queue
int isEmpty();                // Return true if queue is empty
int isFull();                 // Return true if queue is full

// Define TRUE and FALSE if they have not already been defined
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#endif // End of queue header
