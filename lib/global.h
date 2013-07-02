#include "global.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <netdb.h>


#define MAXLINE         4096    /* max text line length */
#define OPEN_MAX 	100
#define LISTENQ 	20
#define SERV_PORT 	5000
#define INFTIM 		1000

/**
*level表示打印日志的等级
*buf表示存放数据
*/
typedef struct{
        int level;
        char *buf;
}log_unit_data;

void     err_sys(const char *, ...);
void     err_dump(const char *, ...);
void     err_msg(const char *, ...);
void     err_quit(const char *, ...);
void	 log_print(int, const char *, ...);
