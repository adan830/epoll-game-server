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
#include "engine.h"
#include "server.h"
#include "epoll.h"

#include <stdlib.h>
#include <assert.h>

engine_t * create_engine() {
    engine_t * e = malloc_engine();
    init_engine(e);
    return e;
}

engine_t * malloc_engine() {
    engine_t * e = (engine_t *)malloc(sizeof(*e));
    return e;
}

void start_engine(engine_t * e) {
    e->engine_server_work(e); //启动服务器
    e->engine_epoll_loop(e); //epoll循环检测事件
}

void free_engine(engine_t * e) {
    assert(e);
    fdevent_free(e->_fdevents); //释放事件结构体
    free(e->socket_actived_list); //释放socket_t循环链表
    free(e); //是否engine_t
}

engine_t * init_engine(engine_t * e) {
    e->engine_server_work = server_work;
    e->engine_fdevent_init = fdevent_init;
    e->engine_epoll_loop = epoll_loop;
    fdevents * events = e->engine_fdevent_init(MAXFD, FDEVENT_HANDLER_LINUX_SYSEPOLL); //初始化epoll
    e->_fdevents = events;
    e->socket_actived_list = create_double_link_list();
    double_link_list_init(e->socket_actived_list); //初始存放活动中的socket_t
    return e;
}