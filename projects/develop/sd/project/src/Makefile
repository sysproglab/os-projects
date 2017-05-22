CC=/usr/bin/gcc

all: casio_system  casio_task

casio_system: casio_system.c 
	${CC} -lrt -lm casio_system.c -o casio_system


casio_task: casio_task.c 
	${CC} -lrt -lm casio_task.c -o casio_task

clean:
	rm -f *.o *.a *~ casio_task  casio_task
