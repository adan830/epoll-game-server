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

#include "global.h"

static void fdevent_linux_sysepoll_free(fdevents *ev) {
	close(ev->epoll_fd);
	free(ev->epoll_events); //释放掉epoll事件集合
}

static int fdevent_linux_sysepoll_event_set(fdevents *ev, int fde_ndx, int fd, int events) {
	struct epoll_event ep;
	int add = 0;

	if(fde_ndx == -1) add = 1;

	memset(&ep, 0, sizeof(ep));

	ep.events = 0;

	if (events & FDEVENT_IN)	ep.events |= EPOLLIN;
	if (events & FDEVENT_OUT)	ep.events |= EPOLLOUT;

	//ep.events |= EPOLLERR | EPOLLHUP;
	ep.events |= EPOLLET;

	ep.data.ptr = NULL;
	ep.data.fd = fd;
	if (0 != epoll_ctl(ev->epoll_fd, add ? EPOLL_CTL_ADD : EPOLL_CTL_MOD, fd, &ep)) {
		// log_error_write(ev->srv, __FILE__, __LINE__, "SSS",
		// 	"epoll_ctl failed: ", strerror(errno), ", dying");

		SEGFAULT();

		return 0;
	}

	return fd;

}

//epoll等待事件发生
static int fdevent_linux_sysepoll_poll(fdevents *ev, int timeout_ms) {
	return epoll_wait(ev->epoll_fd, ev->epoll_events, ev->maxfds, timeout_ms);
}

//获取epoll_events[ndx]事件
static int fdevent_linux_sysepoll_event_get_revent(fdevents *ev, size_t ndx) {
	int events = 0, e;

	e  = ev->epoll_events[ndx].events;
	if (e & EPOLLIN)  events |= FDEVENT_IN; //标记该位为EPOLLIN事件
	if (e & EPOLLOUT) events |= FDEVENT_OUT;
	if (e & EPOLLERR) events |= FDEVENT_ERR;
	if (e & EPOLLHUP) events |= FDEVENT_HUP;
	if (e & EPOLLPRI) events |= FDEVENT_PRI;

	return events;
}

static int fdevent_linux_sysepoll_event_get_fd(fdevents *ev, size_t ndx) {
// # if 0
// 	log_error_write(ev->srv, __FILE__, __LINE__, "SD, D",
// 		"fdevent_linux_sysepoll_event_get_fd: ", (int) ndx, ev->epoll_events[ndx].data.fd);
// # endif

	return ev->epoll_events[ndx].data.fd;
}

static int fdevent_linux_sysepoll_event_next_fdndx(fdevents *ev, int ndx) {
	size_t i;

	//UNUSED(ev);

	i = (ndx < 0) ? 0 : ndx + 1;

	return i;
}

static int fdevent_linux_sysepoll_event_del(fdevents *ev, int fde_ndx, int fd) {
	return 0;
}

//linux平台epoll的初始化
int fdevent_linux_sysepoll_init(fdevents *ev) {
	ev->type = FDEVENT_HANDLER_LINUX_SYSEPOLL;
#define SET(x) \
	ev->x = fdevent_linux_sysepoll_##x;

	SET(free);
	SET(poll);

	SET(event_del);
	SET(event_set);

	SET(event_next_fdndx);
	SET(event_get_fd);
	SET(event_get_revent);

	if(-1 == (ev->epoll_fd = epoll_create(ev->maxfds))) {
		//日志记录

		return -1;
	}

	if(-1 == fcntl(ev->epoll_fd, F_SETFD, FD_CLOEXEC)) {
		//日志记录

		close(ev->epoll_fd);
		return -1;
	}

	ev->epoll_events = malloc(ev->maxfds * sizeof(*ev->epoll_events));

	return 0;
}