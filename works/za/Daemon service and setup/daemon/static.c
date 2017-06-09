#include <time.h>
#include "lib.h"

char* getTime() {
    time_t now;
    struct tm *ptr;
    static char tbuf[64];
    bzero(tbuf,64);
    time(&now);
    ptr = localtime(&now);
    strftime(tbuf,64, "%Y-%m-%e %H:%M:%S", ptr);
    return tbuf;
}

int writeLog(char msg[256]){
    FILE* pLog = fopen("/home/arseny/daemon.log", "a");

    if(pLog == NULL)
        return -1;

    char str[312];
    bzero(str,312);
    strcpy(str, getTime());
    strcat(str, "\n====================\n");
    strcat(str,msg);
    strcat(str, "\n\n");
    fputs(str, pLog);
    fclose(pLog);
    return 0;
}

char* debug_print_mask(uint32_t mask)
{
    if (mask & IN_DELETE_SELF) {
        PRINT("WATCHING DIR WAS DELETED ");
        return "WATCHING DIR WAS DELETED ";
    }
    if (mask & IN_MOVE_SELF) {
        PRINT("WATCHING DIR WAS MOVED ");
        return "WATCHING DIR WAS MOVED ";
    }
    if (mask & IN_MOVED_FROM) {
        PRINT("FILE WAS MOVED OUTSIDE WATCHING DIR ");
        return "FILE WAS MOVED OUTSIDE WATCHING DIR ";
    }
    if (mask & IN_MOVED_TO) {
        PRINT("FILE WAS MOVED INSIDE WATCHING DIR ");
        return "FILE WAS MOVED INSIDE WATCHING DIR ";
    }
    if (mask & IN_CLOSE_WRITE) {
        PRINT("FILE WAS CLOSED ");
        return "FILE WAS CLOSED ";
    }
    if (mask & IN_MODIFY) {
        PRINT("FILE WAS MODIFIED ");
        return "FILE WAS MODIFIED ";
    }
    if (mask & IN_IGNORED) {
        PRINT("WATCH WAS IGNORED ");
        return ("WATCH WAS IGNORED ");
    }
    if (mask & IN_ISDIR) {
        PRINT("OBJECT IS A DIR ");
        return "OBJECT IS A DIR ";
    }
    if (mask & IN_Q_OVERFLOW) {
        PRINT("OVERFLOW OF EVENT QUERY ");
        return "OVERFLOW OF EVENT QUERY ";
    }
    if (mask & IN_UNMOUNT) {
        PRINT("WATCHING FS WAS UNMOUNTED");
        return("WATCHING FS WAS UNMOUNTED");
    }
    if (mask & IN_CREATE) {
        PRINT("FILE WAS CREATED IN WATCHING DIR ");
        return("FILE WAS CREATED IN WATCHING DIR ");
    }
    if (mask & IN_ACCESS) {
        PRINT("ACCESSING TO THE FILE ");
        return("ACCESSING TO THE FILE ");
    }
    if (mask & IN_DELETE) {
        PRINT("FILE WAS DELETED ");
        return("FILE WAS DELETED ");
    }
    if (mask & IN_OPEN) {
        PRINT("FILE WAS OPENED ");
        return("FILE WAS OPENED ");
    }
}

void wd_path(int wd, char *path) {
    if (wd == 0) {
        strcpy(path, watch_dir);
        strcat(path, "/");
        return;
    }

    if (wd < 0 || !watches[wd].name) {
        char deb[256] = "Recursive ";
        strcat(deb, wd);
        strcat(deb, ", ");
        strcat(deb, watches[wd].name);
        strcat(deb, "\n");
        writeLog(deb);
        //DEBUG_PRINT("Recursive %d, %x\n", wd, watches[wd].name);
        /*DEBUG_PRINT*/writeLog("Memory corrupted: asked path of deleted event\n");
        exit(1);
    }

    wd_path(watches[wd].parent_wd, path);
    if (watches[wd].name[0] == 0) return;

    strcat(path, watches[wd].name);
    strcat(path, "/");
}

