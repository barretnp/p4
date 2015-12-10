#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include "manufacturing.h"

#define NEW_CLIENT 6

int
main(int argc, char* argv[])
{
   struct sockaddr_in clientAddr, serverAddr, receiveAddr;
   int soc, message, bytes, iterations, work_status, duration;
   socklen_t socketlen;
   order_struct *order;
   status_struct status;

   struct timespec time, time1;

   uint32_t num_orders;

   bytes = 0;
   iterations = 0;
   
   num_orders = 0;
   work_status = -1;

   order = malloc(sizeof(order_struct));
   order->capacity = 0;
   order->duration = 0;
   order->id_num = 0;

   status.status = -1;
   status.work = 0;

   socketlen = sizeof(clientAddr);
   
   //Creates socket
   soc = socket(AF_INET, SOCK_DGRAM, 0);

   //Sets port, IP, etc for server struct
   memset((char *)&serverAddr, 0, sizeof(serverAddr));
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(7891);
   //IP of the client machine
   serverAddr.sin_addr.s_addr = inet_addr("134.126.141.154"); 

   //Sets port, IP, etc for own socket
   clientAddr.sin_family = AF_INET;
   clientAddr.sin_port = htons(7800);
   clientAddr.sin_addr.s_addr = inet_addr(argv[1]);

   //Bind socket to port
   /*if(connect(soc, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) < 0)
   {
      printf("failed to connect\n");
      exit(0);
   }*/
 
   //Set message that client will send
   message = NEW_CLIENT;
   
   //Send to client
   sendto(soc, &message, sizeof(int), 0, (struct sockaddr *) &serverAddr, socketlen);
   printf("Sent message to server\n");

   //Receive From server
   bytes = recvfrom(soc,(order_struct*) order, sizeof(order_struct), 0, (struct sockaddr *)&serverAddr, &socketlen);

   printf("Capacity: %d\n", order->capacity);
   printf("Duration: %d\n", order->duration);
   printf("Id Num: %d\n", order->id_num);


   while( work_status != 0 )
   {
      printf("Top of loop\n");
      sendto(soc, &(order->id_num), sizeof(uint32_t), 0, (struct sockaddr *) &serverAddr, socketlen);
      recvfrom(soc, &status, sizeof(status_struct), 0, NULL, NULL);
      printf("Message received\n");
      work_status = status.status;

      if(work_status != 0) 
      {
         iterations++;
         num_orders += status.work;
         sleep(order->duration/1000 );
      }
      printf("work-status: %d\n", work_status);
   }

   printf("Orders completed: %" PRIu32 "\n", num_orders);
   printf("Number of iterations: %d\n", iterations);

   free(order);
   close(soc);

   return 0;
}
