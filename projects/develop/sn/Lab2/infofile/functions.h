#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

static char ftypelet (mode_t bits);
void strmode (mode_t mode, char *str);
void printFileType(struct stat fStat);
void printInfo(struct stat fStat);
