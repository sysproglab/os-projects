#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/vfs.h>

char* getTime() {  //функция получение текущего времени
    time_t now;
    struct tm *ptr;
    static char tbuf[64];
    bzero(tbuf,64);
    time(&now);
    ptr = localtime(&now);
    strftime(tbuf,64, "%Y-%m-%d %H:%M:%S", ptr);
    return tbuf;
}

int writeLog(char msg[1024]) {  //функция логирования
    FILE * pLog;
    pLog = fopen("/home/nikita/Документы/OS/Lab2demon/daemon/daemon.log", "a");
    if(pLog == NULL) {
        return 1;
    }
    char str[1110];
    bzero(str, 1110);
    strcpy(str, getTime());
    strcat(str, "\n==========================\n");
    strcat(str, msg);
    strcat(str, "\n==========================\n");
    fputs(str, pLog);
    fclose(pLog);
    return 0;
}

int Daemon(int argc, char* argv[]) { //бесконечный цикл демона
    char *log;
    struct stat fStat;
    while(1) {
    	if(argc != 2 || stat(argv[1],&fStat) < 0)
	{   
		writeLog("Cycle terminated. Invalid arguments!");
        	return 1;
	}
    	log = printInfo(fStat);
        writeLog(log);
        sleep(60);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    writeLog("Daemon Start");
    pid_t parpid, sid; 
    parpid = fork();  // создание дочернего процесса
    if(parpid < 0) {
        exit(1);
    } else if(parpid != 0) {
        exit(0);
    } 
    sid = setsid(); //создание нового сеанса, не зависящего от родителя
    if(sid < 0) {
        exit(1);
    }
    if((chdir("/")) < 0) { //переход к корню диска
        exit(1);
    }

    //закрытие дескрипторов ввода/вывода/ошибок
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO); 
    return Daemon(argc, argv);
}
