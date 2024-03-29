# makefile for scheduling program
#
# make rr - for round-robin scheduling
# make fcfs - for FCFS scheduling
# make sjf - for SJF scheduling
# make priority - for priority scheduling
# make priority_rr - for priority with round robin scheduling

CC=gcc
CFLAGS=-Wall -latomic

clean:
	rm -rf *.o
	rm -rf fcfs
	rm -rf sjf
	rm -rf rr
	rm -rf priority
	rm -rf priority_rr

rr: driver.o list.o queue.o task.o CPU.o add.o tid_manager.o run_rr.o evaluate.o schedule_rr.o
	$(CC) $(CFLAGS) -o rr driver.o schedule_rr.o list.o queue.o task.o CPU.o add.o tid_manager.o run_rr.o evaluate.o

sjf: driver.o list.o queue.o task.o CPU.o add.o tid_manager.o evaluate.o schedule_sjf.o
	$(CC) $(CFLAGS) -o sjf driver.o schedule_sjf.o list.o queue.o task.o CPU.o add.o tid_manager.o evaluate.o

fcfs: driver.o list.o queue.o task.o CPU.o add.o tid_manager.o evaluate.o schedule_fcfs.o
	$(CC) $(CFLAGS) -o fcfs driver.o schedule_fcfs.o list.o queue.o task.o CPU.o add.o tid_manager.o evaluate.o

priority: driver.o list.o queue.o task.o CPU.o add.o tid_manager.o evaluate.o schedule_priority.o
	$(CC) $(CFLAGS) -o priority driver.o schedule_priority.o list.o queue.o task.o CPU.o add.o tid_manager.o evaluate.o

priority_rr: driver.o list.o queue.o task.o CPU.o add.o tid_manager.o run_rr.o evaluate.o schedule_priority_rr.o
	$(CC) $(CFLAGS) -o priority_rr driver.o schedule_priority_rr.o list.o queue.o task.o CPU.o add.o tid_manager.o run_rr.o evaluate.o

schedule_rr.o: schedule_rr.c
	$(CC) $(CFLAGS) -c schedule_rr.c

schedule_sjf.o: schedule_sjf.c
	$(CC) $(CFLAGS) -c schedule_sjf.c

schedule_fcfs.o: schedule_fcfs.c
	$(CC) $(CFLAGS) -c schedule_fcfs.c

schedule_priority.o: schedule_priority.c
	$(CC) $(CFLAGS) -c schedule_priority.c

schedule_priority_rr.o: schedule_priority_rr.c
	$(CC) $(CFLAGS) -c schedule_priority_rr.c

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

task.o: task.c task.h
	$(CC) $(CFLAGS) -c task.c

CPU.o: CPU.c cpu.h
	$(CC) $(CFLAGS) -c CPU.c

tid_manager.o: tid_manager.c tid_manager.h
	$(CC) $(CFLAGS) -c tid_manager.c

evaluate.o: evaluate.c evaluate.h
	$(CC) $(CFLAGS) -c evaluate.c

run_rr.o: run_rr.c run_rr.h
	$(CC) $(CFLAGS) -c run_rr.c