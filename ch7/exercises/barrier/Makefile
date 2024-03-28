include $(VAR_MAKEFILE)

barrier_test: barrier_test.c barrier.o $(COMMON_H)
	$(CC) $(CFLAGS) -pthread -o barrier_test barrier_test.c barrier.o

barrier.o: barrier.h barrier.c
	$(CC) $(CFLAGS) -pthread -c barrier.c 

clean:
	rm *.o barrier_test
