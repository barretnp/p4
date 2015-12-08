#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "manufacturing.h"
#include "statemodel.h"
#include "shared.h"

#define CLIENTS 1

state_t manufacturing = {
   default_event_handler, 
   default_event_handler, 
   default_event_handler,
   manu_success,
   manu_fail,
   default_event_handler, 
   default_event_handler,
   entry_to,
   exit_from 
};

state_t * manu_success()
{
   exit_from();
   charge_client();
   return &shipping;
}

state_t * manu_fail()
{
   printf("Failed to manufacture all parts...\n");
   updateStats(FAIL);
   exit_from();
   return &accepting;
}

void entry_to()
{
   printf("In manufacturing state...\n");
   dispatch_factory_lines();
}

void exit_from()
{
   shutdown_factory_lines();
}

void charge_client()
{
   printf("Successfully manufactured parts...\nCharging clients...\n");
}

void dispatch_factory_lines()
{
   int i;
   int counter = 0;
   int udpSocket, nBytes;

   int return_status;

   uint32_t order_size, order_size2;

   line_response response;

   line_stats *stats[5];

   order_struct *orders[5];

   int ip_list[5];

   ip_list[0] = 0x867e8d5e;
   /*ip_list[1] = 0x867e8d9b;
   ip_list[2] = 0x867e8d9c;
   ip_list[3] = 0x867e8d9d;
   ip_list[4] = 0x867e8d9e;

   printf("Factory starting up...\n");

   stats = malloc(sizeof(line_stats) * 5);*/
   for(i = 0; i < CLIENTS; i++)
   {
      orders[i] = malloc(sizeof(order_struct));
   }

   //Seed random number generator
   srandom(time(NULL));

   order_size = random() % 40001 + 10000;

   for(i = 0; i < CLIENTS; i++)
   {
      orders[i]->capacity = random() % 41 + 10;
      orders[i]->duration = random() % 4001 + 1000;
      orders[i]->id_num = i + 1; 
   }

   struct sockaddr_in serverAddr, clientAddr;
   struct sockaddr_storage serverStorage;
   socklen_t addr_size, clinet_addr_size;
   int clientlen_t = sizeof(clientAddr);

   udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
   
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(7891);
   serverAddr.sin_addr.s_addr = inet_addr("134.126.141.94");
   memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

   bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

   for(i = 0; i < CLIENTS; i++)
   {
      clientAddr.sin_family = AF_INET;
      clientAddr.sin_port = htons(7892);
      clientAddr.sin_addr.s_addr = inet_addr("134.126.141.20");
      memset(clientAddr.sin_zero, '\0', sizeof clientAddr.sin_zero); 
      sendto(udpSocket, orders[i], sizeof(orders[i]), 0, (struct sockaddr *)&clientAddr, addr_size);
   }
   order_size2 = 1;
   printf("Order size is: %d\n", order_size);

   while(order_size2 > 0)
   {
      nBytes = recvfrom(udpSocket, &return_status, 4, 0, (struct sockaddr *) &clientAddr, &clientlen_t);
      printf("Message recieved: %d\n", return_status);
      order_size -= orders[return_status]->capacity;
      printf("IP address of client: %X\n", clientAddr.sin_addr.s_addr);
      printf("Order size after client did work: %d\n", order_size);
      order_size2--;
   }
   close(udpSocket);

}

void shutdown_factory_lines()
{
   printf("Factory shutting down...\n");
}
