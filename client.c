#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "manufacturing.h"

int
main(int argc, char* argv[])
{
   struct sockaddr_in si_self, si_other;
   int s, message;
   socklen_t slen;
   //char host[6];
   order_struct *order;

   order = malloc(sizeof(order_struct));

   //host = getenv("HOSTNAME");

   slen = sizeof(si_other);
   
   s=socket(AF_INET, SOCK_DGRAM, 0);
   memset((char *) &si_other, 0, sizeof(si_other));
   si_self.sin_family = AF_INET;
   si_self.sin_port = htons(7892);
   //IP of the client machine
   si_self.sin_addr.s_addr = inet_addr("134.126.141.94"); 
   bind(s, (struct sockaddr *) &si_self, sizeof(si_self));
   printf("Waiting for respnse\n");
   fflush(stdout);
   recvfrom(s, order, sizeof(order_struct), 0, (struct sockaddr *)&si_other, &slen);
   printf("Capacity: %d\n", order->capacity);
   printf("Duration: %d\n", order->duration);
   printf("Id Num: %d\n", order->id_num);

   //printf("Message sent from: %s\n", host);

   sendto(s, &order->id_num, 4, 0, (struct sockaddr *) &si_other, slen);
   close(s);  

   return 0;
}
