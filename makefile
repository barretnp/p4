all: driver client

driver: driver.o statemodel.o state.o accepting.o processing.o manufacturing.o shipping.o queue.o
	gcc -pthread driver.o statemodel.o state.o accepting.o processing.o manufacturing.o shipping.o queue.o -o driver
driver.o: driver.c statemodel.h
	gcc -pthread -g -c driver.c

statemodel.o: statemodel.h statemodel.c
	gcc -pthread -g -c statemodel.c

state.o: state.h state.c
	gcc -pthread -g -c state.c -o state.o

accepting.o: state.h accepting.h accepting.c
	gcc -pthread -g -c accepting.c

processing.o: state.h processing.h processing.c
	gcc -pthread -g -c processing.c
queue.o: queue.h queue.c
	gcc -g -c queue.c

manufacturing.o: state.h manufacturing.h manufacturing.c queue.h
	gcc -pthread -g -c manufacturing.c

opening.o: state.h shipping.h shipping.c
	gcc -pthread -g -c shipping.c
client: client.o
	gcc -o client client.o

client.o: client.c
	gcc -g -c client.c

clean:
	rm *.o ; rm driver ; rm client

