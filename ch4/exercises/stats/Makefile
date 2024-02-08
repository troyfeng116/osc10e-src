include $(VAR_MAKEFILE)

stats: stats.c $(COMMON_H)
	$(CC) -o stats stats.c $(CFLAGS) -pthread

clean:
	rm stats