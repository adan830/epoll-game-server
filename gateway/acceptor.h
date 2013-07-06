#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "global.h"

void	create_acceptor();
void	add_listener(const char * ip, uint32_t port);
void	acceptor_run();


#endif



