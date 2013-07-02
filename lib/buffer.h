#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>


#define SEGFAULT() do { fprintf(stderr, "%s.%d: aborted\n", __FILE__, __LINE__); abort(); } while(0)
#define UNUSED(x) ( (void)(x) )
