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
#include "acceptor.h"
#include "server.h"

void create_acceptor() {

}

void add_listener(engine_t * e, const char * ip, uint32_t port) {
    struct sockaddr_in serveraddr;
   // int socketfd;
    tcp_listen(e, ip, port, &serveraddr, 256);
}

handler_t acceptor_run(void * e, int revents, int fd) {
    struct sockaddr_in client_address;
    socklen_t len = sizeof(struct sockaddr_in); 
    int nfd = -1;
    if (revents & FDEVENT_IN) {
        if((nfd = Accept(fd, (struct sockaddr*)&client_address, &len)) == -1) {  
            if(errno != EAGAIN && errno != EINTR) {  
                printf("%s: bad accept\n", __func__);  
            }  
            return HANDLER_GO_ON;  
        }  
    }
    printf("***********8nfd************=%d\n",nfd);
    int tmp = -1;
    // //给listenfd注册事件

    fdevent_register(((engine_t *)e)->_fdevents, nfd, recv_data, (engine_t *)e);
    fdevent_event_set(((engine_t *)e)->_fdevents, &tmp, nfd, FDEVENT_IN);

    return HANDLER_GO_ON;
}