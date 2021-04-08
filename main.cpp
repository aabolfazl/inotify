#include <iostream>
#include <sys/inotify.h>
#include <climits>
#include <unistd.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

static void displayInotifyEvent(struct inotify_event *i);

int main(int argc, char *argv[]) {

    int inotifyFd, wd, j;
    char buf[BUF_LEN];
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        std::cerr << "inotify_init" << std::endl;
    }

    for (j = 1; j < argc; j++) {
        wd = inotify_add_watch(inotifyFd, argv[j], IN_ALL_EVENTS);
        if (wd == -1) {
            std::cout << "inotify_add_watch" << std::endl;
        }

        printf("Watching %s using wd %d\n", argv[j], wd);
    }

    for (;;) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0) {
            std::cout << "read() from inotify fd returned 0!" << std::endl;
        }

        if (numRead == -1) {
            std::cout << "read" << std::endl;
        }
        printf("Read %ld bytes from inotify fd\n", (long) numRead);
        for (p = buf; p < buf + numRead;) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}

static void displayInotifyEvent(struct inotify_event *i) {
    printf("wd =%2d; ", i->wd);

    if (i->cookie > 0) {
        printf("cookie =%4d; ", i->cookie);
    }

    printf("mask = ");
    if (i->mask & IN_ACCESS) {
        printf("IN_ACCESS ");
    } else if (i->mask & IN_ATTRIB) {
        printf("IN_ATTRIB ");
    } else if (i->mask & IN_CLOSE_NOWRITE) {
        printf("IN_CLOSE_NOWRITE ");
    } else if (i->mask & IN_CLOSE_WRITE) {
        printf("IN_CLOSE_WRITE ");
    } else if (i->mask & IN_CREATE) {
        printf("IN_CREATE ");
    } else if (i->mask & IN_DELETE) {
        printf("IN_DELETE ");
    } else if (i->mask & IN_DELETE_SELF) {
        printf("IN_DELETE_SELF ");
    } else if (i->mask & IN_IGNORED) {
        printf("IN_IGNORED ");
    } else if (i->mask & IN_ISDIR) {
        printf("IN_ISDIR ");
    } else if (i->mask & IN_MODIFY) {
        printf("IN_MODIFY ");
    } else if (i->mask & IN_MOVE_SELF) {
        printf("IN_MOVE_SELF ");
    } else if (i->mask & IN_MOVED_FROM) {
        printf("IN_MOVED_FROM ");
    } else if (i->mask & IN_MOVED_TO) {
        printf("IN_MOVED_TO ");
    } else if (i->mask & IN_OPEN) {
        printf("IN_OPEN ");
    } else if (i->mask & IN_Q_OVERFLOW) {
        printf("IN_Q_OVERFLOW ");
    } else if (i->mask & IN_UNMOUNT) {
        printf("IN_UNMOUNT ");
    }
    printf("\n");
}