#include "lib.h"

char events_buf[PATH_MAX + sizeof(struct inotify_event) + 1];
_watchstruct *watches;
int ifd = 0, max_watches;
char *watch_dir;

int main(int argc, char *argv[])
{
    int fd, n;
    char buf[12];
    argc = 2;

    if (argc != 2) {
        ERROR("ERROR: No directory to watch\n");
        return 1;
    }

    fd = open("/proc/sys/fs/inotify/max_user_watches", O_RDONLY);
    if (fd < 0) {
        perror("Cannot open /proc/sys/fs/inotify/max_user_watches");
        return 1;
    }

    if ( (n = read(fd, buf, sizeof(buf) - 1)) < 0) {
        perror("Cannot read() /proc/sys/fs/inotify/max_user_watches");
        return 1;
    }

    buf[n] = 0;
    max_watches = atoi(buf) * 2;
    if (max_watches <= 0) {
        ERROR("Incorrect number of watches: ");
        ERROR(buf);
        ERROR("\n");
        return 1;
    } else {
        DEBUG_PRINT("Max watches: %d\n", max_watches);
    }

    watch_dir = argv[1];
    do_watch(max_watches);

    return 0;
}
