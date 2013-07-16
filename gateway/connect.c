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

#include "connect.h"


//创建一个packet结构体
connection * create_connection(int32_t fd) {
    connection * ct;
    ct  = (connection *)malloc(sizeof(*ct));
    if (ct) {   
        ct->init = init_connection;
        ct->read_head = read_packet_head;
        ct->read_data = read_packet_data;
        ct->read = read_all_packet;
        ct->free = free_connection;
        ct->init(ct, fd);
        //初始化收发数据链表
        // pt->pending_send = create_link_list();
        // pt->pending_recv = create_link_list();
        // link_list_clear(pt->pending_send);
        // link_list_clear(pt->pending_recv);
    }
    return ct;
}

void init_connection(connection * ct, int32_t fd) {
	ct->fd = fd;
    ct->pack_head_len = 0;
    ct->head_index = 0; //标记头的数据
    ct->packet_index = 0; //已读到数据的索引
    ct->max_packet_len = 0; //最大的数据量
    ct->bit_index = 0;
    ct->packet = new_buffer(5); //为每个连接开辟一个缓冲区
}

void read_packet_head(connection * ct, char * data, uint32_t *index, uint32_t size) {
    while(ct->head_index < 4 && (*index) < size) {
        ct->pack_head_len += ((uint32_t)data[(*index)]) << (ct->bit_index * 8);
        ct->head_index ++;
        (*index) ++;
    }
    //读完包头
    if (ct->head_index == 4) {
        // memcpy(&ct->pack_head_len, data, ct->head_index); //这里不要用memcpy来做，因为有可能第二个包的长度在第一个包里面了
        ct->max_packet_len = ct->pack_head_len;
        ct->packet_data = (char *)malloc(sizeof(char) * ct->max_packet_len);
        ct->packet_index = 0;
        ct->head_index ++;
        ct->bit_index = 0;
    }
}

void read_packet_data(connection * ct, char * data, uint32_t *index, uint32_t size)
{
    //uint32_t len = strlen(pack);
    if ((*index) < size && ct->packet_index < ct->max_packet_len) {
        uint32_t len1 = size - (*index); //收到包剩余的数据长度
        uint32_t len2 = ct->max_packet_len - ct->packet_index; //剩余需要填充的包数据
        uint32_t end_index = 0;
        if (len1 > len2) {
            end_index = len2 + (*index);
        }
        else {
            end_index = len1 + (*index);
        }
        //copy pack
        copy_packet_data(ct, data, index, end_index);
        (*index) = end_index;
    }
}

void read_all_packet(connection * ct) {
    uint32_t index = 0;
    int32_t size = get_buffer_remaining(ct->packet);
    char data[size+1];
    memset(data, 0, sizeof(data));
    get_buffers(ct->packet, data, size);
    //解决粘包情况
    while(index < size) {
        ct->read_head(ct, data, &index, size);
        ct->read_data(ct, data, &index, size);
        if (ct->packet_index == ct->max_packet_len
            && ct->packet_index != 0) {
            /*
            单个包读完写到buf里面
            还未完善，发完一个包需对包头重新初始化 
            */
            ct->head_index = 0;
            ct->pack_head_len = 0;
            ct->packet_index = 0;
        }
    }
}

void copy_packet_data(connection * ct, char * data, uint32_t * start_index, uint32_t end_index)
{
    uint32_t i = (*start_index);
    for ( ; i < end_index; i ++)
    {
        ct->packet_data[ct->packet_index ++] = data[i];
    }
    printf("收到的数据包=%s\n",ct->packet_data);
    // free(ct->packet_data);
}

void free_connection(connection * ct) {
    if (!ct) return;
    if (ct->packet_data){
        free(ct->packet_data);
    }
    free(ct);
}