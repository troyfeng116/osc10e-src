include $(VAR_MAKEFILE)

time_shmem: time_shmem.c $(COMMON_H)
	$(CC) -o time_shmem time_shmem.c $(CFLAGS) -lrt

time_pipe: time_pipe.c $(COMMON_H)
	$(CC) -o time_pipe time_pipe.c $(CFLAGS)

clean:
	rm time_shmem time_pipe