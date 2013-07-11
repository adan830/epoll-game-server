#include <stdio.h>
#include "packet.h"

int main(){
	packet pk;
	int32_t fd = 0;
	packet_create(&pk, fd);
	//packet_init(&pk, fd);
	char ch[32] = {0, 0, 0, 6, 't', 'e', 's', 't', '\0'};
	pk.read(&pk, ch, 8);
	printf("第1个包，只收到一半的包:\n头：%u\n数据：%s\n",
		pk.pack_head_len, pk.p_pack);
	char ch2[10] = {'!', '!', 0, 0, '\0'};
	//strcpy(ch, "!!");
	pk.read(&pk, ch2, 4);
	printf("第2个包，粘包(数据包中有后一个包的头)：\n头：%u\n数据：%s\n", 
		pk.pack_head_len, pk.p_pack);
	char ch3[32] = {0, 6, 't', 'e', 's', 't', '2', '!','\0'};
	pk.read(&pk, ch3, 8);
	printf("第3个包：\n头：%u\n数据：%s\n", 
		pk.pack_head_len, pk.p_pack);
	return 0;
}