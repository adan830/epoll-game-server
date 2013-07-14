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

#ifndef _SERVER_H
#define _SERVER_H

#include "engine.h"
#include "connect.h"

#define IP "127.0.0.1"
#define PORT 1234
#define MAXBACKLOG 256

typedef struct _socket_t
{
    int32_t status;
    int32_t fd;
    int32_t fdx;
    connection * ct;  //用于数据包的处理
    //link_list * pending_send; //待发送列表
    //link_list * pending_recv; //待接收列表
}socket_t;

void	server_work();
void	tcp_listen(engine_t * e, const char * ip, uint16_t port, struct sockaddr_in *servaddr, int backlog);
socket_t * 	open_socket(int32_t family, int32_t type, int32_t protocol);
int32_t	    socket_bind(int32_t fd, const struct sockaddr * servaddr, socklen_t addrlen);
int32_t	    socket_listen(int32_t fd, int32_t backlog);
int32_t     socket_accept(int32_t sockfd, struct sockaddr *sa, socklen_t *len);
handler_t	recv_data(void * e, void * fd);
socket_t *  init_socket(int32_t sockfd);

#endif



