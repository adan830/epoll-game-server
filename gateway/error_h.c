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

