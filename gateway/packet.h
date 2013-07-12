/*  
    Copyright (C) <2013>  <zhanweilong1992@gmail.com>

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
#ifndef _PACKET_H_
#define _PACKET_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "link_list.h"

typedef struct packet
{
    int32_t  epoll_fd; //epoll fd
    uint32_t pack_head_len;
    uint8_t  head_index; //标记头的数据
    uint32_t packet_index;
    uint32_t max_packet_len; //最大的数据量
    char *p_pack; //包数据

    link_list * pending_send; //待发送列表
    link_list * pending_recv; //待接收列表
    //void (*Init)(packet *dt, int32_t fd);
    void (*init)(struct packet * pt, int32_t fd);
    void (*read_hard)(struct packet *pt, char *pack, uint32_t *index, uint32_t pack_len);
    void (*read_pack)(struct packet *pt, char *pack, uint32_t *index, uint32_t pack_len);
    void (*read)(struct packet *pt, char *pack, uint32_t pack_len);
    void (*free)(struct packet *pt);
}packet;

//创建一个packet, 指针函数的初始化
//void packet_create(packet *pt, int32_t fd);
packet * packet_create(int32_t fd);
//初始化packet
void packet_init(packet *pt, int32_t fd);
//读取包头
void packet_read_hard(packet *pt, char *pack, uint32_t *index, uint32_t pack_len);
//读取包的数据
void packet_read_pack(packet *pt, char *pack, uint32_t *index, uint32_t pack_len);
//读取包
void packet_read(packet *pt, char *pack, uint32_t pack_len);
//复制包的内容到结构体
void packet_copy(packet *pt, char *pack, uint32_t *start_index, uint32_t end_index);
// 释放pack??等待完善
void packet_free(packet *pt);
#endif
