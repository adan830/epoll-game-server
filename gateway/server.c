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
#include "acceptor.h"

void server_work(engine_t * e) {
    create_acceptor();
    add_listener(e, IP, PORT);
}

socket_t * open_socket(int32_t family, int32_t type, int32_t protocol) {
    int32_t sockfd;
    if ( (sockfd = socket(family, type, protocol)) < 0 ) {
        //日志打印
    }
    socket_t * st = init_socket(sockfd);
    if (st == NULL) {
        close(sockfd);
        return NULL;
    }
    return st;
}

socket_t * init_socket(int32_t sockfd) {
    socket_t * st = (socket_t *)malloc(sizeof(* st));
    if (st) {
        st->status = ISAVTIVE;
        st->fd = sockfd;
        st->fdx = -1;
        st->pending_send = create_link_list();
        st->pending_recv = create_link_list();
        link_list_clear(st->pending_send);
        link_list_clear(st->pending_recv);
    }
    return st;
}

int32_t socket_listen(int32_t sockfd, int32_t backlog)  {
    if( listen(sockfd, backlog) < 0 ) {
            printf("%s\n",strerror(errno));
            return -1;
        }
    return 0;
}

int socket_accept(int32_t sockfd, struct sockaddr *sa, socklen_t *len) {
    int32_t nfd;
    if ( (nfd = accept(sockfd, sa, len)) < 0 ) {
        if(errno != EAGAIN && errno != EINTR) {  
            //日志打印
        }   
    }
    return nfd;
}

void tcp_listen(engine_t * e, const char * ip, uint16_t port, struct sockaddr_in *servaddr, int backlog) {

    socket_t * st = open_socket(AF_INET, SOCK_STREAM, 0);

    //给listenfd注册事件
    fdevent_register(e->_fdevents, st->fd, acceptor_run, st);
    fdevent_event_set(e->_fdevents, &st->fdx, st->fd, FDEVENT_IN);

    if (st->fd) {
        //日志打印
    }

    memset( (void *)servaddr, 0, sizeof(*servaddr));
    servaddr->sin_family = AF_INET;

    if (ip) {
        if (inet_pton(AF_INET, ip, &servaddr->sin_addr) < 0) {
            //日志打印
            printf("%s\n",strerror(errno));
        }
    }
    else {
        servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    }
    servaddr->sin_port = htons(port);

    if (socket_bind(st->fd, (const struct sockaddr *)servaddr, sizeof(*servaddr)) < 0) {
        //日志打印
        printf("%s\n",strerror(errno));
    }

    if (socket_listen(st->fd, backlog) == 0) {
        printf("%s\n","server listen successful!");
    }
}

int32_t socket_bind(int32_t sockfd, const struct sockaddr * myaddr, socklen_t addrlen) {
    if ( bind(sockfd, myaddr, addrlen) < 0 ) {
            printf("%s\n",strerror(errno));
            return -1;
        }
    return 0;
}

handler_t  recv_data(void * e, void * s) {
    char buff[100];
    printf("come in ------------ = %s\n",buff);
    if (((socket_t *)s)->status == ISWRITE) { //表示可以接收数据
        int len = recv(((socket_t *)s)->fd, buff, sizeof(buff) - 1, 0);
        printf("len=%d\n",len);
        printf("%s\n",buff);
    }
    //TODO 开始接收数据了
    return HANDLER_GO_ON;
}