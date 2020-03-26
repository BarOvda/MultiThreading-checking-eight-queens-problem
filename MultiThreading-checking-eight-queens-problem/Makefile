#make file:

CC=gcc
FLAGS= -c -Wall
OBJ=func

all: $(OBJ)
$(OBJ): v1.o v2.o
	$(CC) $(FLAGS)  v1.c v2.c -pthread -o $(OBJ)
	
v1.o: v1.c
	$(CC) $(FLAGS) v1.c	
v2.o: v2.c
	$(CC) $(FLAGS) v2.c		
exec:
	./func
clean:
	rm -rf func
	rm -rf *.exe
	rm -rf *.o
	
