OBJS = ./ls \
       ./date \
	   ./mkdir \
	   ./rm \
	   ./cat

all: $(OBJS) myshell
	
myshell: myshell.c 
	gcc -o myshell myshell.c

./ls: ./ls.c
	gcc ./ls.c -o ./ls

./date: ./date.c
	gcc ./date.c -o ./date

./mkdir: ./mkdir.c
	gcc ./mkdir.c -o ./mkdir

./rm: ./rm.c
	gcc ./rm.c -o ./rm

./cat: ./cat.c
	gcc ./cat.c -o ./cat


clean:
	rm myshell $(OBJS) history.txt
