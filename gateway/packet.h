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

typedef struct packet
{
    int32_t  epoll_fd; //epoll fd
    uint32_t pack_head_len;
    uint8_t  head_index; //标记头的数据
    uint32_t packet_index;
    uint32_t max_packet_len; //最大的数据量
    char *p_pack; //包数据
    //void (*Init)(packet *dt, int32_t fd);
    void (*init)(struct packet * dt, int32_t fd);
    void  (*read_hard)(struct packet *dt, char *pack, uint32_t *index);
    void (*read_pack)(struct packet *dt, char *pack, uint32_t *index);
    void (*read)(struct packet *dt, char *pack);
}packet;

void packet_create(packet *dt, int32_t fd);
void packet_init(packet *dt, int32_t fd);
void packet_read_hard(packet *dt, char *pack, uint32_t *index);
void packet_read_pack(packet *dt, char *pack, uint32_t *index);
void packet_read(packet *dt, char *pack);
void packet_copy(packet *dt, char * pack, uint32_t *start_index, uint32_t end_index);

#endif
