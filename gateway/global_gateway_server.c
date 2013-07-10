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
    
#ifndef _HEADERNAME_H
#define _HEADERNAME_H 1
#include "global.h"
#endif

#include "socket_event.h"

#define DRIVE_RECV_HEAD 0
#define DRIVE_RECV_DATA 1
#define DRIVE_FD_CLOSE 2
#define DRIVE_FD_ERROR 3

#define MAX_EVENTS 500

/**
struct epoll_event
{
  uint32_t events;   //事件
  epoll_data_t data;    
} __attribute__ ((__packed__));

typedef union epoll_data
{
  void *ptr; //用户指针
  int fd;  //描述符
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
*/
struct Head
{
    int data_len;
    int cmd;
};
struct Message
{
    struct Head head;
    char *data;
  //  short check_code; //校验码
};

//解析包状态
typedef struct{
    int packet_head_len; //包头总长
    int packet_body_len; //包数据长度
    int packet_head_index; //包头目前索引
    int packet_body_index; //包数据目前索引
    char *head;            //包头指针
    char *body;            //包数据指针
    int state;             //解包状态
}parse_pagket_status;

int e_pollFd;  
socket_event g_Events[MAX_EVENTS+1];
parse_pagket_status parse_packet[MAX_EVENTS+1];

void RecvData(int fd, int events, void *arg);  
void SendData(int fd, int events, void *arg);

void 
setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0){
	   perror("fcntl(sock,GETFL)");
    }
   	opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0){
	   perror("fcntl(sock,SETFL,opts)");
    }
}

//初始化客户数据包状态结构体
void
init_peer_packet_status(parse_pagket_status *packet_status){
    packet_status->packet_head_index = 0;
    packet_status->packet_body_index = 0;
    packet_status->state = DRIVE_RECV_HEAD; //设置当前为解包头状态
}

void 
AcceptConn(int fd, int events, void *arg)  
{  
    struct sockaddr_in sin;  
    socklen_t len = sizeof(struct sockaddr_in);  
    int nfd, i;  
    // accept  
    if((nfd = accept(fd, (struct sockaddr*)&sin, &len)) == -1)  
    {  
        if(errno != EAGAIN && errno != EINTR)  
        {  
            printf("%s: bad accept\n", __func__);  
        }  
        return;  
    }  
    do  
    {  
        for(i = 0; i < MAX_EVENTS; i++)  
        {  
            if(g_Events[i].status == 0)  
            {  
                break;  
            }  
        }  
        if(i == MAX_EVENTS)  
        {  
            printf("%s:max connection limit[%d].\n", __func__, MAX_EVENTS);  
            break;  
        }  
        // set nonblocking  
        if(fcntl(nfd, F_SETFL, O_NONBLOCK) < 0) break;  
        // add a read event for receive data  
        init_peer_packet_status(&parse_packet[i]);
        socket_event_set(&g_Events[i], nfd, RecvData, &g_Events[i]);  
        socket_event_add(&g_Events[i], e_pollFd, EPOLLIN|EPOLLET);  
        printf("new conn[%s:%d][time:%ld]\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port), g_Events[i].last_active);  
    }while(0);  
}  

void 
decode_head(int fd, socket_event *ev){

    // if(ev->state != DRIVE_RECV_HEAD){
    //     return;
    // }
    // int bytes = recv(fd, ev->head + ev->head_len, 4 - ev->head_len, 0);
    // printf("bytes = %d\n",bytes);
    // if(bytes > 0){
    //     ev->head_len += bytes;
    //     if(ev->head_len == 4){
    //         ev->state = DRIVE_RECV_DATA;
    //         ev->body = malloc(sizeof(char) * ev->recev_body_len);
    //         memcpy(&ev->recev_body_len, ev->head, ev->head_len);
    //         return;
    //     }
    //     else if(ev->head_len < 4){
    //         return;
    //     }
    // }
    // else if(bytes == 0){
    //     ev->state = DRIVE_FD_CLOSE;
    //     return;
    // }
    // else if(bytes == -1 && errno != EAGAIN){
    //     ev->state = DRIVE_FD_ERROR;
    //     return;
    // }
}

void decode_body(int fd, socket_event *ev){
   //  if(ev->state != DRIVE_RECV_DATA){
   //      return;
   //  }

   //  //int recev_body_len = *(int *)en->head - ev->head_len;
   //  //int recev_body_len;
   //  printf("ev->head_len = %d\n",ev->head_len);
   // // memcpy(&ev->recev_body_len, ev->head, ev->head_len);
   //  printf("recev body len = %d\n",ev->recev_body_len);
   //  int bytes = recv(fd, ev->body + ev->body_len, ev->recev_body_len - ev->body_len, 0);
   //  printf("body = %s\n",ev->body);
   //  if(bytes > 0){
   //      ev->body_len += bytes;
   //      if(ev->body_len == ev->recev_body_len){
   //          ev->state = DRIVE_RECV_HEAD;
   //          return;
   //      }
   //      else if(ev->body_len < ev->recev_body_len){
   //          return;
   //      }
   //      else if(bytes == 0){
   //          ev->state = DRIVE_FD_CLOSE;
   //          return;
   //      }
   //      else if(bytes == -1 && errno != EAGAIN){
   //          ev->state = DRIVE_FD_ERROR;
   //          return;
   //      }
   //  }
}

void
decode(int fd, socket_event *ev){
    // printf("ev->state = %d\n",ev->state);
    decode_head(fd, ev);
    decode_body(fd, ev);
    // struct Message message = ev->message;
    // char *index = ev->buff;
    // int data_len = message.head.data_len;
    // printf("data_len = %d\n",data_len);
    // int left_body_len = data_len;
    // int sum_body_len = 1;
    // int recev_body_len = 0;
    // printf("data_len11111111 = %d\n",data_len);
    // //while(sum_body_len != data_len){

    //     recev_body_len = recv(fd, ev->buff + sum_body_len, left_body_len, 0);
    //     printf("recev_body_len22222222 = %d\n",recev_body_len);
    //     printf("error = %d\n",errno);
    //     sum_body_len += recev_body_len;
    //     left_body_len -= recev_body_len;
    // //}
    // message.data = (char *)malloc(sum_body_len);
    // memcpy(message.data, index, sum_body_len);
    // printf("Data = %s",message.data);
    // ev->buff[sum_body_len] = '\0';
    // printf("Data = %s",ev->buff);
    // message.data = index;
}

// receive data  
void 
RecvData(int fd, int events, void *arg)  
{  
    socket_event *ev = (socket_event*)arg;  
    // int len;  
    // receive data  
   // decode(fd, ev);
   // len = recv(fd, ev->buff, sizeof(ev->buff) - 1, 0);    
    //EventDel(e_pollFd, ev);
    // if(ev->state == DRIVE_RECV_HEAD && ev->head_len != 4)  
    // {  
        // ev->len = len;  
        // ev->buff[len] = '\0';  
        // printf("C[%d]:%s\n", fd, ev->buff);  
        // change to send event  
        // EventSet(ev, fd, SendData, ev);  
        // EventAdd(e_pollFd, EPOLLOUT|EPOLLET, ev);  
        decode_head(fd, ev);
       // EventSet(ev, fd, RecvData, ev);  
       // EventAdd(e_pollFd, EPOLLIN|EPOLLET, ev);  
    // }  
    // else if(ev->state == DRIVE_RECV_DATA && ev->body_len != ev->recev_body_len){
        decode_body(fd, ev);
       // EventSet(ev, fd, RecvData, ev);  
       // EventAdd(e_pollFd, EPOLLIN|EPOLLET, ev);
    // }
    // else if(ev->state == DRIVE_FD_CLOSE)  
    // {  
    //     close(ev->fd);  
    //     printf("[fd=%d] closed gracefully.\n", fd);  
    // }  
    // else  
    // {  
    //     close(ev->fd);  
    //     printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));  
    // }  
} 

// send data  
void 
SendData(int fd, int events, void *arg)  
{  
    // struct myevent_s *ev = (struct myevent_s*)arg;  
    // int len;  
    // // send data  
    // len = send(fd, ev->buff, ev->len, 0);  
    // ev->len = 0;  
    // EventDel(e_pollFd, ev);  
    // if(len > 0)  
    // {  
    //     // change to receive event  
    //     EventSet(ev, fd, RecvData, ev);  
    //     EventAdd(e_pollFd, EPOLLIN|EPOLLET, ev);  
    // }  
    // else  
    // {  
    //     close(ev->fd);  
    //     printf("recv[fd=%d] error[%d]\n", fd, errno);  
    // }  
}

void
init_listen_socket(int e_pollFd, short port){
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    setnonblocking(listenFd); //设置为非阻塞
    socket_event_set(&g_Events[MAX_EVENTS], listenFd, AcceptConn, &g_Events[MAX_EVENTS]);
    socket_event_add(&g_Events[MAX_EVENTS], e_pollFd, EPOLLIN|EPOLLET);  //给listenfd注册时间
	struct sockaddr_in sin;  
    bzero(&sin, sizeof(sin));  
    sin.sin_family = AF_INET;  
    sin.sin_addr.s_addr = INADDR_ANY;  
    sin.sin_port = htons(port);  
    bind(listenFd, (const struct sockaddr*)&sin, sizeof(sin));  
    listen(listenFd, 5);
}


int
start_game_server(){
    short port = 12345;
    e_pollFd = epoll_create(256);
    if(e_pollFd <= 0){
        perror("e_pollFd < 0");
    }

	init_listen_socket(e_pollFd, port); //初始化服务器配置,并且给监听描述符注册事件
    printf("server running:port[%d]\n", port);  

	struct epoll_event events[MAX_EVENTS];
    int checkPos = 0;  
    while(1){  
        // a simple timeout check here, every time 100, better to use a mini-heap, and add timer event  
        long now = time(NULL);  
        int i;
        for(i = 0; i < 100; i++, checkPos++) // doesn't check listen fd  
        {  
            if(checkPos == MAX_EVENTS) checkPos = 0; // recycle  
            if(g_Events[checkPos].status != 1) continue;  
            long duration = now - g_Events[checkPos].last_active;  
            if(duration >= 60) // 60s timeout  
            {  
                close(g_Events[checkPos].fd);  
                printf("[fd=%d] timeout[%ld--%ld].\n", g_Events[checkPos].fd, g_Events[checkPos].last_active, now);  
                socket_event_delete(&g_Events[checkPos], e_pollFd);  
            }  
        }  
        // wait for events to happen  
        int fds = epoll_wait(e_pollFd, events, MAX_EVENTS, 1000);  
        if(fds < 0){  
            printf("epoll_wait error, exit\n");  
            break;  
        }  
        for(i = 0; i < fds; i++){  
            socket_event *ev = (socket_event*)events[i].data.ptr;  
            if((events[i].events & EPOLLIN) && (ev->events & EPOLLIN)) // read event  
            {  
                ev->call_back(ev->fd, events[i].events, ev->arg);  
            }  
            if((events[i].events & EPOLLOUT)&&(ev->events & EPOLLOUT)) // write event  
            {  
                ev->call_back(ev->fd, events[i].events, ev->arg);  
            }  
        }  
    }  
    // free resource  
    return 0;  
}
