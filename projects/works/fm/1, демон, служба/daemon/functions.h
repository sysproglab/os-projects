#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

static char ftypelet (mode_t bits);
void strmode (mode_t mode, char *str);
char* printFileType(struct stat fStat);
char* printInfo(struct stat fStat);
