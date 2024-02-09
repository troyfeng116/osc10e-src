include $(VAR_MAKEFILE)

sudoku: sudoku.c $(COMMON_H)
	$(CC) -o sudoku sudoku.c $(CFLAGS) -pthread

clean:
	rm sudoku