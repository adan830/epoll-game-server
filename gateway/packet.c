#include "packet.h"
#include <string.h>
/*
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
    void (*read_hard)(struct packet *dt, char *pack);
    void (*read)(struct  packet *dt, char *pack);
}packet;
*/

//创建一个packet结构体
void packet_create(packet *dt, int32_t fd)
{
	dt->init = packet_init;
	dt->read_hard = packet_read_hard;
    dt->read = packet_read;
	dt->init(dt, fd);
}

void packet_init(packet *dt, int32_t fd)
{
	dt->epoll_fd = fd;
    dt->pack_head_len = 0;
    dt->head_index = 0; //标记头的数据
    dt->packet_index = 0; //已读到数据的索引
    dt->max_packet_len = 0; //最大的数据量
}

void packet_read_hard(packet *dt, char *pack, uint32_t *index)
{
	uint32_t len = strlen(pack); //计算当前读到数据的长度
    //int pack_index = 0;
    while(dt->head_index < 4 && (*index) < len) {
        dt->pack_head_len = (dt->pack_head_len << 8) + pack[(*index)];
        dt->head_index ++;
        (*index) ++;
    }

    //读完包头
    if (dt->head_index == 4)
    {
        dt->max_packet_len = dt->pack_head_len; 
        dt->p_pack = (char *)malloc(sizeof(char) * dt->max_packet_len);
        dt->pack_index = 0;
        dt->head_index ++;
    }
}

void packet_read_pack(packet *dt, char *pack, uint32_t *index)
{
    uint32_t len = strlen(pack);
    if ((*index) < len && dt->pack_index < dt->max_packet_len) {
        uint32_t len1 = len - (*index); //收到包剩余的数据长度
        uint32_t len2 = dt->max_packet_len - dt->pack_index; //剩余需要填充的包数据
        uint32_t end_index = 0;
        if (len1 > len2) {
            end_index = len2 + (*index);
        }
        else {
            end_index = len1 + (*index);
        }
        //copy pack
        packet_copy(dt, pack, index, end_index)
    }
}

void packet_read(packet *dt, char *pack)
{
    uint32_t index = 0;
    uint32_t len = strlen(pack);
    //解决粘包情况
    while(index < len) {
        dt->read_hard(dt, pack, &index);
        dt->read_pack(dt, pack, &index);
        if (dt->pack_index == dt->max_packet_len
            && dt->pack_index != 0) {
           //单个包读完写到buf里面
        }
    }
}

void packet_copy(packet *dt, char * pack, uint32_t *start_index, uint32_t end_index)
{
    for (uint32_t i = (*start_index); i < end_index; i ++)
    {
        dt->p_pack[dt->pack_index ++] = pack[i];
    }
}