#include "lib.h"

int do_watch(int max_watches) {
    struct inotify_event *ev;// = (struct inotify_event *) events_buf;
    ssize_t n = 0, wd;
    char path[PATH_MAX + 1];

    watches = (_watchstruct *) calloc(max_watches, sizeof(_watchstruct));
    if (!watches) {
        writeLog("Cannot allocate memory\n");
        exit(1);
    }

    writeLog("Doing initial watches setup\n");

    ifd = inotify_init();
    if (ifd == -1) {
        perror("Cannot init inotify");
        writeLog("Cannot init inotify");
        exit(1);
    }

    wd = add_dir_watch(0, watch_dir, "", 1);
    if (wd < 0) {
        /*ERROR*/writeLog("Cannot add dir watch\n");
        exit(1);
    }
    add_dir(wd, watch_dir, 1, 1);

    writeLog("while loop");
    //while(1){
    while ((n = read(ifd, events_buf, sizeof(events_buf))) > 0) {
        ev = (struct inotify_event *) events_buf;
        while (n > 0) {
            if (ev->mask & IN_Q_OVERFLOW) {
                /*ERROR*/writeLog("Queue overflow, restart needed\n");
                exit(3);
            }

            if (ev->mask & IN_IGNORED) {
                free(watches[ev->wd].name);
                watches[ev->wd].parent_wd = -1;
                watches[ev->wd].name = NULL;
                goto loop_end;
            }

            wd_path(ev->wd, path);
            char pr[256] = "Path: ";
            strcat(pr, path);
            strcat(pr, "\nFile: ");
            strcat(pr, ev->name);
            strcat(pr, "\nEvent: ");
            strcat(pr, debug_print_mask(ev->mask));
            writeLog(pr);

#ifdef DEBUG
            if (ev->len) {
                DEBUG_PRINT(" | ");
                DEBUG_PRINT("%s", ev->name);
            }
            DEBUG_PRINT(" | ");
            debug_print_mask(ev->mask);
#endif

            if ((ev->mask & IN_DELETE) || (ev->mask & IN_MOVED_FROM)) {
                goto loop_end;
            }

            if (ev->mask & IN_ISDIR && ev->len > 0 && strcmp(ev->name, ".")) {
                if (ev->len + strlen(path) > sizeof(path) - 1) {
                    char errorMsg[21] = "Too deep directory: ";
                    strcat(errorMsg, path);
                    strcat(errorMsg, ev->name);
                    writeLog(errorMsg);
                    /*ERROR("Too deep directory: ");
                    ERROR(path);
                    ERROR(ev->name);
                    ERROR("\n");*/
                    goto loop_end;
                }
                strcat(path, ev->name);
                wd = add_dir_watch(ev->wd, path, ev->name, 0);
                if (wd < 0) goto loop_end;
                add_dir(ev->wd, path, 0, 0);
            }

            loop_end:
            n -= sizeof(struct inotify_event) + ev->len;
            ev = (struct inotify_event *) ((char *) ev + sizeof(struct inotify_event) + ev->len);
        }

        PRINT("-\n");
    }

    perror("Cannot read() inotify queue");
    exit(1);
}

int add_dir_watch(int parent_wd, char *dir, char *dir_name, int no_print) {
        int wd = inotify_add_watch(ifd, dir, EVENT_MASK);
        if (wd < 0) {
            ERROR("Cannot add watch to '");
            ERROR(dir);
            ERROR("' using inotify: ");
            if (errno == ENOSPC) {
                ERROR("too many watches\nYou can increase number of user watches using /proc/sys/fs/inotify/max_user_watches");
            } else {
                ERROR(strerror(errno));
            }
            ERROR("\n");
            if (errno != EACCES && errno != ENOENT) exit(1);
            return wd;
        }

        if (wd >= max_watches) {
            ERROR("\nToo many events; restart required to prevent watch descriptor overflow.\n");
            exit(3);
        }

        dir_name = strdup(dir_name);
        if (!dir_name) {
            ERROR("Cannot strdup(dir_name)\n");
            exit(1);
        }

        watches[wd].wd = wd;
        watches[wd].parent_wd = parent_wd;
        if (watches[wd].name) free(watches[wd].name);
        watches[wd].name = dir_name;

        if (!no_print) {
            PRINT(dir);
            PRINT("\n");
        }

        return wd;
    }

void add_dir(int dir_wd, char *dir, int errors_fatal, int no_print) {
        char path[PATH_MAX + 1];
        DIR *dh = opendir(dir);
        struct dirent *ent;
        struct stat st;
        int dirl = strlen(dir), n = sizeof(path) - 1 - dirl, had_errors = 0, wd;

        if (dirl > sizeof(path) - 3) {
            /*ERROR("Too long path (not watched): ");
            ERROR(dir);
            ERROR("\n");*/
            writeLog(strcat("Too long path (not watched): ", dir));

            if (errors_fatal) exit(1);
            return;
        }

        if (!dh) {
            ERROR("Cannot opendir(");
            ERROR(dir);
            ERROR("): ");
            ERROR(strerror(errno));
            ERROR("\n");

            if (errors_fatal) exit(1);
            return;
        }

        strcpy(path, dir);

        while ((ent = readdir(dh)) != NULL) {
            if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") || !strcmp(ent->d_name, ".unrealsync"))
                continue;

            path[dirl] = '/';
            path[dirl + 1] = 0;
            strncat(path + dirl, ent->d_name, n);
            path[sizeof(path) - 1] = 0;
            if (lstat(path, &st)) {
                ERROR("Cannot lstat(");
                ERROR(path);
                ERROR("): ");
                ERROR(strerror(errno));
                ERROR("\n");
                had_errors = 1;
                continue;
            }

            if (S_ISDIR(st.st_mode)) {
                wd = add_dir_watch(dir_wd, path, ent->d_name, no_print);
                if (wd < 0) continue;
                add_dir(wd, path, errors_fatal, no_print);
            }
        }

        closedir(dh);

        if (errors_fatal && had_errors) exit(1);
    }

