.PHONY: all clean install uninstall
all: infofile
clean:
	rm -rf infofile
infofile: main.c functions.c
	gcc -o infofile main.c functions.c
install:
	install ./infofile /usr/local/bin
uninstall:
	rm -rf /usr/local/bin/infofile
