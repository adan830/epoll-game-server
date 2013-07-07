#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "global.h"
#include "engine.h"
#include "fdevent.h"

void	create_acceptor();
void	add_listener(engine_t * e, const char * ip, uint32_t port);
handler_t	acceptor_run(void * e, int events, int fd);


#endif



