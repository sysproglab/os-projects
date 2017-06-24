SRCS = chardev.c mykeyboard_workqueue.c myusbmouse.c
OBJS = $(SRCS:.c=.o)
obj-m += $(OBJS)
EXTRA_CFLAGS = -O3

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
