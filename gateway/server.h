#ifndef _SERVER_H
#define _SERVER_H

#include "global.h"
#include "fdevent.h"

void	server_work();
void	tcp_listen(engine_t * e, const char * ip, uint16_t port, struct sockaddr_in *servaddr, int backlog);
int32_t 	open_socket(int32_t family, int32_t type, int32_t protocol);
int32_t	Bind(int32_t fd, struct sockaddr_in * servaddr, socklen_t addrlen);
int32_t	Listen(int32_t fd, int32_t backlog);
int32_t Accept(int32_t sockfd, struct sockaddr *sa, socklen_t *len);

#endif



