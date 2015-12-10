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

   status.status = 3;
   status.work = -1;
   status.id = 0;

   socketlen = sizeof(clientAddr);


   
   //Creates socket
   soc = socket(AF_INET, SOCK_DGRAM, 0);

   //Sets port, IP, etc for server struct
   memset((char *)&serverAddr, 0, sizeof(serverAddr));
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(7891);
   //IP of the client machine
   serverAddr.sin_addr.s_addr = inet_addr("134.126.141.81"); 


   /*//Sets port, IP, etc for own socket
   clientAddr.sin_family = AF_INET;
   clientAddr.sin_port = htons(7800);
   clientAddr.sin_addr.s_addr = inet_addr(argv[1]);*/

   
 
   //Set message that client will send
   message = NEW_CLIENT;
   
   //Send to client
   sendto(soc, &status, sizeof(status_struct), 0, (struct sockaddr *) &serverAddr, socketlen);

   //Receive From server
   recvfrom(soc,(order_struct*) order, sizeof(order_struct), 0, (struct sockaddr *)&serverAddr, &socketlen);

   printf ("Host: %s\n", getenv("HOSTNAME"));
   printf("Capacity: %d\n", order->capacity);
   printf("Duration: %d ms\n", order->duration);
   printf("Id Num: %d\n", order->id_num);
   duration = order->duration;

   status.id = order->id_num;
   fflush(stdout);



   while( work_status != 0 )
   {
      status.status = 1;
      
      sendto(soc, &status, sizeof(status_struct), 0, (struct sockaddr *) &serverAddr, socketlen);
      recvfrom(soc, &status, sizeof(status_struct), 0, NULL, NULL);
      work_status = status.status;

      if(work_status != 0) 
      {
         iterations++;
         num_orders += status.work;
        sleep(duration/1000);
        status.status = 2;
        status.id = order->id_num;
        sendto(soc, &status, sizeof(status_struct), 0, (struct sockaddr *) &serverAddr, socketlen);
	
      }

   }

   printf("Orders completed: %" PRIu32 "\n", num_orders);
   printf("Number of iterations: %d\n", iterations);

   free(order);
   close(soc);

    fflush(stdout);
    for(;;);



   return 0;
}
