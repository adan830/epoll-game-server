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
#include "engine.h"

void server_work(engine_t * e) {
    create_acceptor();
    add_listener(e, "127.0.0.1", 1234);
   // acceptor_run();
}

int32_t open_socket(int32_t family, int32_t type, int32_t protocol) {
    int32_t sockfd;
    if ( (sockfd = socket(family, type, protocol)) < 0 ) {
        //日志打印
    }
    return sockfd;
}

int32_t Bind(int32_t sockfd, const struct sockaddr *myaddr, socklen_t addrlen)  {
    if ( bind(sockfd, myaddr, addrlen) < 0 ) {
            printf("%s\n",strerror(errno));
            return -1;
        }
    return 0;
}

int32_t Listen(int32_t sockfd,int32_t backlog)  {
    if( listen(sockfd, backlog) < 0 ) {
            printf("%s\n",strerror(errno));
            return -1;
        }
    return 0;
}

void tcp_listen(engine_t * e, const char * ip, uint16_t port, struct sockaddr_in *servaddr, int backlog) {
    int32_t listenfd;
    listenfd = open_socket(AF_INET, SOCK_STREAM, 0);

    int tmp = -1;
    //给sockfd注册事件
    fdevent_register(e->_fdevents, listenfd, acceptor_run, e);
    fdevent_event_set(e->_fdevents, &tmp, listenfd, FDEVENT_IN);
    printf("listenfd=%d\n",listenfd);
    if (listenfd) {
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
    printf("port = %d\n",port);
    if (Bind(listenfd, (const struct sockaddr*)servaddr, sizeof(*servaddr)) < 0) {
        //日志打印
        printf("%s\n",strerror(errno));
    }

    if (Listen(listenfd, backlog) == 0) {
        printf("%s\n","server listen successful!");
    }
    // struct sockaddr_in sin;  
    // socklen_t len = sizeof(struct sockaddr_in);  
    // int nfd;
    // while(1) {
    //     printf("listenfd=%d\n",listenfd);
    // if((nfd = accept(listenfd, (struct sockaddr*)&sin, &len)) == -1) {
    //         if(errno != EAGAIN && errno != EINTR) {  
    //             printf("%s: bad accept\n", __func__);  
    //         }  
    //         printf("%s\n",strerror(errno));
    //         printf("%d\n",nfd);
    //     }  
    // }
}