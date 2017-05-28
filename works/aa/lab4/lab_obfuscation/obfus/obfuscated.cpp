
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#define ReplacementFor_DEFAULT_FILE "\x2e\x2f\x6d\x79\x6f\x75\x74"
int main(int ReplacementFor_argc,char**ReplacementFor_argv){char*
ReplacementFor_portname=ReplacementFor_DEFAULT_FILE;if(ReplacementFor_argc>
(0x13e0+3725-0x226c))ReplacementFor_portname=ReplacementFor_argv[
(0x1d0+2656-0xc2f)];FILE*fd=fopen(ReplacementFor_portname,"\x77");if(fd<
(0x864+5839-0x1f33)){printf(
"\x65\x72\x72\x6f\x72\x20\x6f\x70\x65\x6e\x69\x6e\x67\x20\x74\x68\x65\x20\x64\x65\x76\x69\x63\x65" "\n"
);return(0xea7+3377-0x1bd7);}char buf[(0xb77+3445-0x1888)];int ReplacementFor_n=
scanf("\x25\x73",buf);printf("\x49\x6e\x70\x75\x74\x3a\x20\x25\x73" "\n",buf);
fprintf(fd,"\x25\x73" "\n",buf);return(0x1949+1543-0x1f50);}
