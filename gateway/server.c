/*  
    Copyright (C) <2013>  <jjchen.lian@gmail.com> <zhanweilong1992@gmail.com>

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
#include "connect.h"

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
        //st->pending_send = create_link_list();
        //st->pending_recv = create_link_list();
        //link_list_clear(st->pending_send);
        //link_list_clear(st->pending_recv);
        st->ct = create_connection(st->fd);
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
    char buff[1024];
    int32_t len = 0;
    int32_t offset = 0;
    socket_t * st = (socket_t *)s;
    connection * ct = st->ct;
    if (st->status == ISWRITE) {
        while (1) {
            memset(buff, 0, sizeof(buff));
            len = recv(st->fd, buff, sizeof(buff) - 1, 0);
            if (len == 0) {
                break; //木有数据了
            }else if (len == -1) {
                //出错了
            }else {
                while (len > 0) {
                // printf("len = %d\n",len);
                // printf("get_buffer_remaining(ct->packet) = %d\n",get_buffer_remaining(ct->packet));
                if (buffer_has_remaining(ct->packet)) {
                    int32_t remaining = get_buffer_remaining(ct->packet);
                    if (remaining >= len) {
                        put_buffers_len(ct->packet, buff, offset, len);
                        if (buffer_has_remaining(ct->packet) == false) {
                            flip_buffer(ct->packet);
                            read_all_packet(ct);
                            clear_buffer(ct->packet);
                        }
                        offset = 0;
                        len = 0;
                    }
                    else {
                        put_buffers_len(ct->packet, buff, offset, remaining);
                        flip_buffer(ct->packet);
                        read_all_packet(ct);
                        clear_buffer(ct->packet);
                        offset += remaining;
                        len -= remaining;
                        }
                    }
                }
            }
        }
    }
    return HANDLER_GO_ON;
}