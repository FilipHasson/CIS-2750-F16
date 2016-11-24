CC =  gcc
CFLAGS = -std=c11 -g
TFLAGS = -Wall -pedantic
LDFLAGS = -L./ -llistio
LDMYSQL = -L/usr/lib/x86_64-linux-gnu/ -lmysqlclient 

a4: a4.o liblistio.a listio.h
	$(CC) a4.o $(LDFLAGS) $(LDMYSQL) -o a4
a4.o: a4.c
	$(CC) $(CFLAGS) $(TFLAGS) a4.c -c -o a4.o
liblistio.a: listio.o
	ar cr liblistio.a listio.o
listio.o: listio.c listio.h
	$(CC) $(CFLAGS) $(TFLAGS) listio.c -c -o listio.o
python: main.py
	chmod +x main.py
db: db.o
	$(CC) db.o $(LDMYSQL) -o db
db.o: db.c
	$(CC) $(CFLAGS) $(TFLAGS) db.c -c -o db.o
clean: 
	rm *.o