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

#include "error_h.h"

char * buff;

void init_error_buff(int32_t Size) {
    buff = (char *)malloc(Size * sizeof(char));
}

void free_error_buff() {
    free(buff);
}

char * get_open_socket_error(int32_t error) {
    bzero(buff, strlen(buff));
    switch (error) {
        case EACCES:  buff = "没有权限建立指定domain type的socket"; break;
        case EAFNOSUPPORT: buff = "不支持所给的地址类型"; break;
        case EINVAL: buff = "不支持此协议或者协议不可用"; break;
        case EMFILE: buff = "进程文件表溢出"; break;
        case ENFILE: buff = "已经达到系统允许打开的文件数量,打开文件过多"; break;
        case ENOBUFS: buff = "内存不足"; break;
        case EPROTONOSUPPORT: buff = "指定的协议type在domain中不存在"; break;
    }
    return buff;
}

char * get_bind_socket_error(int32_t error) {
    bzero(buff, strlen(buff));
    switch (error) {
        case EADDRINUSE: buff = "给定地址已经使用"; break;
        case EBADF: buff = "sockfd不合法"; break;
        case EINVAL: buff = "sockfd已经绑定到其他地址"; break;
        case ENOTSOCK: buff = "sockfd是一个文件描述符,不是socket描述符"; break;
        case EACCES: buff = "地址被保护,用户的权限不足"; break;
    }
    return buff;
}

char * get_listen_socket_error(int32_t error) {
    bzero(buff, strlen(buff));
    switch (error) {
        case EADDRINUSE: buff = "另一个socket已经在同一个端口监听";
        case EBADF: buff = "sockfd不是合法的描述符";
        case ENOTSOCK: buff = "sockfd不是代表socket的文件描述符";
        case EOPNOTSUPP: buff = "socket不支持listen操作";
    }
    return buff;
}

char * get_accept_socket_error(int32_t error) {
    bzero(buff, strlen(buff));
    switch (error) {
        //case EAGAIN:
        case EWOULDBLOCK: buff = "当前没有可接收的连接"; break;
        case EBADF: buff = "描述符非法"; break;
        case ECONNABORTED: buff = "连接取消"; break;
        case EINTR: buff = "信号在合法连接到来之前打断accept系统调用"; break;
        case EINVAL: buff = "socket没有监听连接或者地址长度不合法"; break;
        case EMFILE: buff = "每个进程允许打开的文件描述符数量达到最大值"; break;
        case ENOTSOCK: buff = "文件描述符是一个文件,不是socket"; break;
        case ENOBUFS: buff = "内存不足"; break;
        case EPROTO: buff = "协议错误"; break;
        case EPERM: buff = "防火墙不允许连接"; break;
    }
    return buff;
}

char * get_recv_socket_error(int32_t error) {
    bzero(buff, strlen(buff));
    switch (error) {
        case EAGAIN:  buff = "套接字定义为非阻塞,而操作采用了阻塞方式"; break;
        case EBADF: buff = "socket不是合法描述符"; break;
        case ECONNREFUSED: buff = "远程主机不允许此操作"; break;
        case EFAULT: buff = "接收缓冲区指针在此进程之外"; break;
        case EINTR: buff = "接收到中断信号"; break;
        case EINVAL: buff = "传递了不合法参数"; break;
        case ENOTCONN: buff = "套接字表示流式套接字,此套接字没有连接"; break;
        case ENOTSOCK: buff = "参数不是套接字描述符"; break;
    }
    return buff;
}

