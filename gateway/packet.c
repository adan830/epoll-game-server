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
    void (*init)(struct packet * pt, int32_t fd);
    void (*read_hard)(struct packet *pt, char *pack, uint32_t *index, uint32_t pack_len);
    void (*read_pack)(struct packet *pt, char *pack, uint32_t *index, uint32_t pack_len);
    void (*read)(struct packet *pt, char *pack, uint32_t pack_len);
    void (*free)(struct packet *pt);
}packet;
*/

//创建一个packet结构体
packet * packet_create(int32_t fd)
{
    packet * pt;
    pt  = (packet *)malloc(sizeof(*pt));
    if (pt) {   
        pt->init = packet_init;
        pt->read_hard = packet_read_hard;
        pt->read_pack = packet_read_pack;
        pt->read = packet_read;
        pt->free = packet_free;
        pt->init(pt, fd);
        //初始化收发数据链表
        pt->pending_send = create_link_list();
        pt->pending_recv = create_link_list();
        link_list_clear(pt->pending_send);
        link_list_clear(pt->pending_recv);
    }
    return pt;
}

void packet_init(packet *pt, int32_t fd)
{
	pt->epoll_fd = fd;
    pt->pack_head_len = 0;
    pt->head_index = 0; //标记头的数据
    pt->packet_index = 0; //已读到数据的索引
    pt->max_packet_len = 0; //最大的数据量
}

void packet_read_hard(packet *pt, char *pack, uint32_t *index, uint32_t pack_len)
{
	//uint32_t len = strlen(pack); //计算当前读到数据的长度
    //int pack_index = 0;
    while(pt->head_index < 4 && (*index) < pack_len) {
        pt->pack_head_len = (pt->pack_head_len << 8) + pack[(*index)];
        pt->head_index ++;
        (*index) ++;
    }

    //读完包头
    if (pt->head_index == 4) {
        pt->max_packet_len = pt->pack_head_len; 
        pt->p_pack = (char *)malloc(sizeof(char) * pt->max_packet_len);
        pt->packet_index = 0;
        pt->head_index ++;
    }
}

void packet_read_pack(packet *pt, char *pack, uint32_t *index, uint32_t pack_len)
{
    //uint32_t len = strlen(pack);
    if ((*index) < pack_len && pt->packet_index < pt->max_packet_len) {
        uint32_t len1 = pack_len - (*index); //收到包剩余的数据长度
        uint32_t len2 = pt->max_packet_len - pt->packet_index; //剩余需要填充的包数据
        uint32_t end_index = 0;
        if (len1 > len2) {
            end_index = len2 + (*index);
        }
        else {
            end_index = len1 + (*index);
        }
        //copy pack
        packet_copy(pt, pack, index, end_index);
        (*index) = end_index;
    }
}

void packet_read(packet *pt, char *pack, uint32_t pack_len)
{
    uint32_t index = 0;
    //uint32_t len = strlen(pack);
    //解决粘包情况
    while(index < pack_len) {
        pt->read_hard(pt, pack, &index, pack_len);
        pt->read_pack(pt, pack, &index, pack_len);
        if (pt->packet_index == pt->max_packet_len
            && pt->packet_index != 0) {
            /*
            //单个包读完写到buf里面
            还未完善，发完一个包需对包头重新初始化 
            */
            pt->head_index = 0;
            pt->pack_head_len = 0;
            pt->packet_index = 0;
        }
    }
}

void packet_copy(packet *pt, char * pack, uint32_t *start_index, uint32_t end_index)
{
    uint32_t i = (*start_index);
    for ( ; i < end_index; i ++)
    {
        pt->p_pack[pt->packet_index ++] = pack[i];
    }
}

void packet_free(packet *pt)
{
    if (!pt) return;
    if (pt->p_pack){
        free(pt->p_pack);
    }
    free(pt);
}