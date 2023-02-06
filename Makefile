all:
	gcc matrix_sequential.c -o seq.exe
	gcc matrix_thread.c -o thr.exe -pthread

