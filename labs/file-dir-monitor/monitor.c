#define _XOPEN_SOURCE 500
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/inotify.h>
#include <limits.h>
#include <ftw.h>
/*Resource: https://linux.die.net/man/3/nftw */

int inotifyFd;
int wd;


static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    if(tflag == FTW_D){
        wd = inotify_add_watch(inotifyFd, fpath, IN_CREATE | IN_DELETE | IN_MOVE);
        if (wd == -1)
            errorf("inotify_add_watch");
    }
    return 0; 
}

/*Resource: https://man7.org/tlpi/code/online/dist/inotify/demo_inotify.c.html */
static void displayInotifyEvent(struct inotify_event *i){
    if (i->mask & IN_CREATE)        infof("CREATED %s\n",i->name);
    if (i->mask & IN_DELETE)        infof("DELETED %s\n",i->name);
    if (i->mask & IN_MOVED_FROM)    infof("RENAMING %s ",i->name);
    if (i->mask & IN_MOVED_TO)      infof("TO %s\n", i->name);

}

int main(int argc, char** argv){
    int flags = 0;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    if (argc < 2){
        errorf("Error! Invalid Input\n");
        exit(EXIT_FAILURE);
    }

    /*Resource: https://man7.org/linux/man-pages/man7/inotify.7.html*/
    inotifyFd = inotify_init();                 
    if (inotifyFd == -1){
        errorf("inotify_init");
    }

    if (argc > 2 && strchr(argv[2], 'd') != NULL){
        flags |= FTW_DEPTH;
   	}
    if (argc > 2 && strchr(argv[2], 'p') != NULL){
        flags |= FTW_PHYS;
   	}

   	if (nftw((argc < 2) ? "." : argv[1], display_info, 20, flags) == -1) {
        errorf("nftw");
        exit(EXIT_FAILURE);
   	}

    for (;;) { 
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            errorf("read() from inotify fd returned 0!");

        if (numRead == -1)
            errorf("read");

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }
    exit(EXIT_SUCCESS);
    return 0;
}
