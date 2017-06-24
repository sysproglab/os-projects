obj-m += mykeyboard.o
obj-m += mod_tasklet.o
obj-m += mod_workqueue.o
obj-m += mykeyboard_workqueue.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
