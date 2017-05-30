#include "lib.h"

void debug_print_mask(uint32_t mask)
{
    if (mask & IN_DELETE_SELF) PRINT("WATCHING DIR WAS DELETED ");
    if (mask & IN_MOVE_SELF)   PRINT("WATCHING DIR WAS MOVED ");
    if (mask & IN_MOVED_FROM)  PRINT("FILE WAS MOVED OUTSIDE WATCHING DIR ");
    if (mask & IN_MOVED_TO)    PRINT("FILE WAS MOVED INSIDE WATCHING DIR ");
    if (mask & IN_CLOSE_WRITE) PRINT("FILE WAS CLOSED ");
    if (mask & IN_MODIFY)      PRINT("FILE WAS MODIFIED ");
    if (mask & IN_IGNORED)     PRINT("WATCH WAS IGNORED ");
    if (mask & IN_ISDIR)       PRINT("OBJECT IS A DIR ");
    if (mask & IN_Q_OVERFLOW)  PRINT("OVERFLOW OF EVENT QUERY ");
    if (mask & IN_UNMOUNT)     PRINT("WATCHING FS WAS UNMOUNTED");
    if (mask & IN_CREATE)      PRINT("FILE WAS CREATED IN WATCHING DIR ");
    if (mask & IN_ACCESS)      PRINT("ACCESSING TO THE FILE ");
    if (mask & IN_DELETE)      PRINT("FILE WAS DELETED ");
    if (mask & IN_OPEN)        PRINT("FILE WAS OPENED ");
}


void wd_path(int wd, char *path) {
    if (wd == 0) {
        strcpy(path, watch_dir);
        strcat(path, "/");
        return;
    }

    if (wd < 0 || !watches[wd].name) {
        DEBUG_PRINT("Recusive %d, %x\n", wd, watches[wd].name);
        ERROR("Memory corrupted: asked path of deleted event\n");
        exit(1);
    }

    wd_path(watches[wd].parent_wd, path);
    if (watches[wd].name[0] == 0) return;

    strcat(path, watches[wd].name);
    strcat(path, "/");
}

