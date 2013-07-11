/*  
    Copyright (C) lighttpd

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fdevent.h"

#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>


fdevents * fdevent_init(size_t maxfds, fdevent_handler_t type) {
    fdevents *ev;

    ev = calloc(1, sizeof(*ev));
    ev->fdarray = calloc(maxfds, sizeof(*ev->fdarray));
    ev->maxfds = maxfds;

    switch(type) {
        case FDEVENT_HANDLER_POLL:
            return ev;
        case FDEVENT_HANDLER_SELECT:
            return ev;
        case FDEVENT_HANDLER_LINUX_SYSEPOLL: //epoll模型，现在只支持epoll模型
            if(0 != fdevent_linux_sysepoll_init(ev)){
                //后期增加日志打印
                return NULL;
            }
            return ev;
        case FDEVENT_HANDLER_SOLARIS_DEVPOLL:
            return ev;
        case FDEVENT_HANDLER_SOLARIS_PORT:
            return ev;
        case FDEVENT_HANDLER_FREEBSD_KQUEUE: //freebsd 的queue模型
            return ev;
        case FDEVENT_HANDLER_LIBEV:
            return ev;
        case FDEVENT_HANDLER_LINUX_RTSIG:
            return ev;
        case FDEVENT_HANDLER_UNSET:
            break;
    }
    return NULL;
}

void fdevent_free(fdevents *ev) {
    size_t i;
    if (!ev) return;

    if(ev->free) ev->free(ev); //先释放到epoll_fd和epoll_events内存

    for(i = 0; i < ev->maxfds; i++) {
        if(ev->fdarray[i]) free(ev->fdarray[i]); //释放文件描述符数组
    }

    free(ev->fdarray); //释放fdarray指针内存
    free(ev); //释放ev指针内存
}

int fdevent_reset(fdevents *ev) {
    if (ev->reset) return ev->reset(ev);

    return 0;
}

//初始话文件描述符节点信息，私有方法，不供本文件外面的调用
static fdnode *fdnode_init(void) {
    fdnode *fdn;

    fdn = calloc(1, sizeof(*fdn));
    fdn->fd = -1;
    return fdn;
}

static void fdnode_free(fdnode *fdn) {
    free(fdn);
}

int fdevent_register(fdevents *ev, int fd, fdevent_handler handler, void *ctx) {
    fdnode *fdn;

    fdn = fdnode_init();
    fdn->handler = handler;
    fdn->fd = fd;
    fdn->ctx = ctx;
    fdn->handler_ctx = NULL;
    fdn->events = 0;
    ev->fdarray[fd] = fdn;

    return 0;
}

int fdevent_unregister(fdevents *ev, int fd) {
    fdnode *fdn;

    if (!ev) return 0;
    fdn = ev->fdarray[fd];

    assert(fdn->events == 0); //events事件不为0,表示该fd描述符还不能被回收

    fdnode_free(fdn);

    ev->fdarray[fd] = NULL;

    return 0;
}

int fdevent_event_del(fdevents *ev, int *fde_ndx, int fd) {
    int fde = fde_ndx ? *fde_ndx : -1;

    if (NULL == ev->fdarray[fd]) return 0;

    if(ev->event_del) fde = ev->event_del(ev, fde, fd);
    ev->fdarray[fd]->events = 0;

    if (fde_ndx) *fde_ndx = fde;

    return 0;
}

int fdevent_event_set(fdevents *ev, int* fde_ndx, int fd, int events) {
    int fde = fde_ndx ? *fde_ndx : -1;
    if(ev->event_set) fde = ev->event_set(ev, fde, fd, events);
    ev->fdarray[fd]->events = events;

    if(fde_ndx) *fde_ndx = fde;

    return 0;
}

int fdevent_poll(fdevents *ev, int timeout_ms) {
    if (ev->poll == NULL)   SEGFAULT();
    return ev->poll(ev, timeout_ms);
}

int fdevent_event_get_revent(fdevents *ev, size_t ndx) {
    if (ev->event_get_revent == NULL) SEGFAULT();
    return ev->event_get_revent(ev, ndx); 
}

int fdevent_event_get_fd(fdevents *ev, size_t ndx) {
    if (ev->event_get_fd == NULL) SEGFAULT();

    return ev->event_get_fd(ev, ndx);
}

fdevent_handler fdevent_get_handler(fdevents *ev, int fd) {
    if (ev->fdarray[fd] == NULL) SEGFAULT();
    if (ev->fdarray[fd]->fd != fd) SEGFAULT();

    return ev->fdarray[fd]->handler;
}

void * fdevent_get_context(fdevents *ev, int fd) {
    if (ev->fdarray[fd] == NULL) SEGFAULT();
    if (ev->fdarray[fd]->fd != fd) SEGFAULT();

    return ev->fdarray[fd]->ctx;
}

int fdevent_fcntl_set(fdevents *ev, int fd) {
#ifdef FD_CLOEXEC
    /* close fd on exec (cgi) */
    fcntl(fd, F_SETFD, FD_CLOEXEC);
#endif
    if ((ev) && (ev->fcntl_set)) return ev->fcntl_set(ev, fd);
#ifdef O_NONBLOCK
    return fcntl(fd, F_SETFL, O_NONBLOCK | O_RDWR);
#else
    return 0;
#endif
}

int fdevent_event_next_fdndx(fdevents *ev, int ndx) {
    if (ev->event_next_fdndx) return ev->event_next_fdndx(ev, ndx);

    return -1;
}