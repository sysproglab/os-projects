#include <string.h>
#include <unistd.h>
#include <stdio.h>
#define DEFAULT_FILE "\x2e\x2f\x6d\x79\x6f\x75\x74"
int main(int argc, char** argv) {char *portname = DEFAULT_FILE; if (argc > 1) portname = argv[1];FILE* fd = fopen (portname, "\x77");if (fd < 0){ printf("\x65\x72\x72\x6f\x72\x20\x6f\x70\x65\x6e\x69\x6e\x67\x20\x74\x68\x65\x20\x64\x65\x76\x69\x63\x65\xa"); return 1;} char buf [100]; int n = scanf ("\x25\x73", buf); printf("\x49\x6e\x70\x75\x74\x3a\x20\x25\x73\xa", buf);fprintf(fd, "\x25\x73\xa", buf); return 0;}
