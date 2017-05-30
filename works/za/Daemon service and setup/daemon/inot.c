#include "lib.h"

char events_buf[PATH_MAX + sizeof(struct inotify_event) + 1];
_watchstruct *watches;
int ifd = 0, max_watches;
char *watch_dir;

int main(int argc, char *argv[])
{
    int fd, n;
    char buf[12];

    if (argc != 2) {
        writeLog("ERROR: No directory to watch\n");
        return 1;
    }

    fd = open("/proc/sys/fs/inotify/max_user_watches", O_RDONLY);
    if (fd < 0) {
        perror("Cannot open /proc/sys/fs/inotify/max_user_watches");
        writeLog("Cannot open /proc/sys/fs/inotify/max_user_watches");
        return 1;
    }

    if ((n = read(fd, buf, sizeof(buf) - 1)) < 0) {
        perror("Cannot read() /proc/sys/fs/inotify/max_user_watches");
        writeLog("Cannot read() /proc/sys/fs/inotify/max_user_watches");
        return 1;
    }

    buf[n] = 0;
    max_watches = atoi(buf) * 2;
    if (max_watches <= 0) {
        /writeLog(strcat("Incorrect number of watches: ", buf));
        return 1;
    }

    switch (fork()){
        case -1:
            writeLog(strcat("Daemon start failed ", strerror(errno)));
            return -1;
        case 0:
            writeLog("Daemon is running");
            umask(0);
            setsid();
            chdir("/");

            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);

            watch_dir = argv[1];//"//home//arseny//DirForWatching";
            return do_watch(max_watches);
        default:
            return 0;
    }
}
