/*  
    Copyright (C) <2013>  <jjchen.lian@gmail.com>

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

#include "server.h"
#include "epoll.h"

void epoll_loop(engine_t * e) {
    int n;
    while (1) {
        while(!double_link_list_empty(e->socket_actived_list)) {
            socket_t * st = (socket_t *)double_link_list_pop(e->socket_actived_list);
            fdevent_handler handler = fdevent_get_handler(e->_fdevents, st->fd);
            (*handler)(e, st);
        }
        if ( (n = fdevent_poll(e->_fdevents, 1000)) > 0) {
            int fd_ndx = -1;
            for(; fd_ndx < n; fd_ndx++) {
                fd_ndx  = fdevent_event_next_fdndx(e->_fdevents, fd_ndx);
                if (fd_ndx == -1) break;
                int events = fdevent_event_get_revent(e->_fdevents, fd_ndx);
                int fd = fdevent_event_get_fd(e->_fdevents, fd_ndx);
                if (events & FDEVENT_IN) {
                    socket_t * st = fdevent_get_context(e->_fdevents, fd);
                    st->status |= 0x0001;
                    double_link_list_push(e->socket_actived_list, (struct double_link_node *)st);
                }
            }
        }
    }
}