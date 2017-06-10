#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "functions.h"

int main(int argc, char **argv)
{
    struct stat fStat;
    if(argc != 2 || stat(argv[1],&fStat) < 0)    
        return 1;
    printInfo(fStat);	 
    return 0;
}
