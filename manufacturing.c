#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "manufacturing.h"
#include "statemodel.h"
#include "shared.h"
#include "queue.h"

#define CLIENTS 1
#define WORKING 1
#define NO_MORE 0

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
   int line_id, line_cap, i, udpSocket, nBytes, curr_status, head, tail, order_fin;

   int theQueue[5];

   uint32_t order_size, return_status;

   line_stats *stats[5];

   order_struct *orders[5];

   status_struct status;

   struct sockaddr_in serverAddr, client_info;
   struct sockaddr_in *clientAddr[5];
   
   socklen_t addr_size, client_addr_size;
   
   client_addr_size = sizeof(clientAddr);
   order_fin = 0;

   printf("Factory starting up...\n");

   //Allocate array of structs for order info
   for(i = 0; i < CLIENTS; i++)
   {
      orders[i] = malloc(sizeof(order_struct));
      stats[i] = malloc(sizeof(line_stats));
      clientAddr[i] = malloc(sizeof(struct sockaddr_in));
   }

   //Seed random number generator
   srandom(time(NULL));

 
   //Generate oder size
   order_size = random() % 40001 + 10000;

   //Create capacity, duration, and id numbers for orders
   for(i = 0; i < CLIENTS; i++)
   {
      orders[i]->capacity = random() % 591 + 10;
      orders[i]->duration = random() % 4001 + 1000;
      orders[i]->id_num = i + 1; 
      printf("Capacity: %d\n", orders[i]->capacity);
      printf("Duration: %d\n", orders[i]->duration);
      printf("id_num: %d\n", orders[i]->id_num);
   }

   InitQueue();

   //Set up Socket
   udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
 
   if(udpSocket < 0)
   {
      printf("Failed to create file descriptor\n");
      exit(0);
   }

   //Create port and IP info for socket
   memset((char *)&serverAddr, 0, sizeof(serverAddr));
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(7891);
   serverAddr.sin_addr.s_addr = inet_addr("134.126.141.154");

   //Bind socket to port
   if((bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))) < 0)
   {
      printf("Failed to bind port\n");
      exit(0);
   }

   curr_status = WORKING;
   i = 0;
   printf("%d\n", order_size);
   //Receive message from new client and send order info
   system("./launch_clients.sh easterpa");
   while(order_size > 0)
   {  
      if(!isEmpty())
      {
         line_id = Dequeue() -1;
         line_cap = orders[line_id]->capacity;
         if(line_cap <= order_size)
         {
            sendto(udpSocket, &line_cap, sizeof(line_cap), 0, (struct sockaddr*)clientAddr[line_id], addr_size);
            order_size -= line_cap;
         }
         else
         {
            sendto(udpSocket, &order_size, sizeof(order_size), 0, (struct sockaddr*)clientAddr[line_id], addr_size);
            order_size = 0;
            order_fin = 2;
         }
      }

      while(order_fin != 2)
      {
         recvfrom(udpSocket, &status, sizeof(status), 0, (struct sockaddr*)&client_info, &addr_size);

         if(status.status == 1)
         {
            Enqueue(status.id);
         }
         else if(status.status == 2)
         {
            line_id = status.id -1;
            stats[line_id]->num_completed += status.work;
            stats[line_id]->iterations += status.iterations;
            order_fin = 2;
         }
         else
         {
            sendto(udpSocket, orders[i], sizeof(order_struct), 0, (struct sockaddr*)&client_info, addr_size);
            clientAddr[i]->sin_family = client_info.sin_family;
            clientAddr[i]->sin_port = client_info.sin_port;
            clientAddr[i]->sin_addr.s_addr = client_info.sin_addr.s_addr;
            i++;
         }
      }
      /*printf("top of loop\n");
      recvfrom(udpSocket, &return_status, sizeof(return_status), 0, (struct sockaddr *) &clientAddr, &client_addr_size);
      printf("Received: message: %d\n", return_status);
      if(return_status == 6 && curr_status == WORKING)
      {
         sendto(udpSocket, orders[i], sizeof(order_struct), 0, (struct sockaddr *)&clientAddr, client_addr_size);
         i++;
         printf("Sent message back to client\n");
      }
      else if(curr_status == WORKING)
      {
        printf("Made into working section\n");
        if(orders[return_status-1]->capacity <= order_size)
        {
           
           status.status = curr_status;
           status.work = orders[return_status-1]->capacity; 
           sendto(udpSocket, &status, sizeof(status_struct), 0, (struct sockaddr *)&clientAddr, client_addr_size);
           stats[return_status-1]->num_completed += status.work;
           stats[return_status-1]->iterations++;
           order_size -= status.work;
           printf("full order\n");
        }
        else
        {
           status.status = curr_status;
           status.work = order_size;
           sendto(udpSocket, &status, sizeof(status), 0, (struct sockaddr *)&clientAddr, client_addr_size);
           status.status = NO_MORE;
           stats[return_status-1]->num_completed += status.work;
           stats[return_status-1]->iterations++; 
           order_size -= status.work;
           printf("partial order\n");
           recvfrom(udpSocket, &return_status, sizeof(return_status), 0, (struct sockaddr *) &clientAddr, &client_addr_size);
           sendto(udpSocket, &status, sizeof(status), 0, (struct sockaddr *)&clientAddr, client_addr_size);
           printf("finished in server\n");
        } 
      }*/
   }

   //system call to kill all lines
   system("./killall.sh easterpa");

   for(i = 0; i < CLIENTS; i++)
   {
      printf("Line %d: %d orders completed, %d iterations\n", i+1, stats[i]->num_completed, stats[i]->iterations);
   }

   for(i = 0; i < CLIENTS; i++)
   {
      free(orders[i]);
      free(stats[i]);
      free(clientAddr[i]);
   }
   close(udpSocket);
}

void shutdown_factory_lines()
{
   printf("Factory shutting down...\n");
}
