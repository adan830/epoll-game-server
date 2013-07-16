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
#ifndef _CONNECT_H_
#define _CONNECT_H_

#include "global.h"
#include "buffer_h.h"
// #include "link_list.h"

typedef struct connection
{
    int32_t  fd; //fd
    uint32_t pack_head_len;
    uint8_t  head_index; //标记头的数据
    uint32_t packet_index;
    uint32_t max_packet_len; //最大的数据量
    char * packet_data; //包数据
    uint8_t bit_index; //包头的位移标识
    buffer * packet;
    // link_list * pending_send; //待发送列表
    // link_list * pending_recv; //待接收列表
    void (*init)(struct connection * ct, int32_t fd);
    void (*read_head)(struct connection * ct, char * data, uint32_t *index, uint32_t size);
    void (*read_data)(struct connection *pt, char * data, uint32_t *index, uint32_t size);
    void (*read)(struct connection * ct);
    void (*free)(struct connection * ct);
}connection;

//void packet_create(packet *pt, int32_t fd);
connection * create_connection(int32_t fd);

void init_connection(connection * ct, int32_t fd);
//读取包头
void read_packet_head(connection * ct, char * data, uint32_t *index, uint32_t size);
//读取包的数据
void read_packet_data(connection * ct, char * data, uint32_t *index, uint32_t size);
//读取包
// void read_all_packet(connection * ct, buffer * data, uint32_t size);
void read_all_packet(connection * ct);

//复制包的内容到结构体
void copy_packet_data(connection * ct, char * data, uint32_t * start_index, uint32_t end_index);

void free_connection(connection * ct);
#endif
