#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define PRINT(str) write(1, str, strlen(str))
#define ERROR(str) write(2, str, strlen(str))

#define EVENT_MASK (IN_CLOSE_WRITE|IN_CREATE|IN_DELETE|IN_DELETE_SELF|IN_MODIFY|IN_MOVE_SELF\
    |IN_MOVED_FROM|IN_MOVED_TO|IN_DONT_FOLLOW|IN_ONLYDIR|IN_ATTRIB)

#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

typedef struct {
    int wd;
    int parent_wd;
    char *name;
} _watchstruct;

extern char events_buf[PATH_MAX + sizeof(struct inotify_event) + 1];
extern _watchstruct *watches;
extern int ifd, max_watches;
extern char *watch_dir;


void debug_print_mask(uint32_t mask);
int do_watch(int max_watches);


int add_dir_watch(int parent_wd, char *dir, char *dir_name, int no_print);
void add_dir(int dir_wd, char *dir, int errors_fatal, int no_print);
void wd_path(int wd, char *path);