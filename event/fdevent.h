#ifndef _FDEVENT_H_
#define _FDEVENT_H_

#include "settings.h"

#include <sys/epoll.h>

#define FDEVENT_IN     BV(0)
#define FDEVENT_PRI    BV(1)
#define FDEVENT_OUT    BV(2)
#define FDEVENT_ERR    BV(3)
#define FDEVENT_HUP    BV(4)
#define FDEVENT_NVAL   BV(5)

typedef handler_t (*fdevent_handler)(int revents);

//定义系统使用的并发模型,方便以后做移植
typedef enum {
    FDEVENT_HANDLER_UNSET,                 //未定义
    FDEVENT_HANDLER_SELECT,             //select
    FDEVENT_HANDLER_POLL,                 //poll
    FDEVENT_HANDLER_LINUX_RTSIG,         //rtsig
    FDEVENT_HANDLER_LINUX_SYSEPOLL,     //sysepoll
    FDEVENT_HANDLER_SOLARIS_DEVPOLL,     //devpoll
    FDEVENT_HANDLER_FREEBSD_KQUEUE,     //kqueue
    FDEVENT_HANDLER_SOLARIS_PORT         //port
} fdevent_handler_t;

typedef struct _fdnode {
     fdevent_handler handler; //处理函数指针
     void *ctx;                //文件描述符的context
     void *handler_ctx;
     int fd;                  //文件描述符
     int events;               //事件
}fdnode;

typedef struct fdevents {
	fdevent_handler_t type; //多路IO类型
	fdnode **fdarray;   //文件描述符数组
	size_t maxfds;      //最大的文件描述符数

	#ifdef USE_LINUX_EPOLL
		int epoll_fd;  //epoll_create返回的描述符
		struct epoll_event *epoll_events;  //保存fd及对应的事件
	#endif

	int (*reset) (struct fdevents * ev);
	void (*free) (struct fdevents * ev);
	int (*event_add) (struct fdevents *ev, int fde_ndx, int fd, int events);
	int (*event_del) (struct fdevents *ev, int fde_ndx, int fd);
	int (*event_get_revent) (struct fdevents *ev, size_t ndx);
	int (*event_get_fd) (struct fdevents *ev, size_t ndx);
	int (*event_next_fdndx) (struct fdevents *ev, int ndx);
	int (*poll) (struct fdevents *ev, int timeout_ms);
	int (*fcntl_set) (struct fdevents *ev, int fd);

}fdevents;

fdevents *fdevent_init(size_t maxfds, fdevent_handler_t type);

/*
* 重置和释放fdevent系统。
*/
int fdevent_reset(fdevents * ev);
void fdevent_free(fdevents * ev);

/*
* 将fd增加到fd event系统中。events是要对fd要监听的事件。
* fde_ndx是fd对应的fdnode在ev->fdarray中的下标值的指针。
* 如果fde_ndx==NULL，则表示在fd event系统中增加fd。如果不为NULL，则表示这个
* fd已经在系统中存在，这个函数的功能就变为将对fd监听的事件变为events。
*/
int fdevent_event_add(fdevents * ev, int *fde_ndx, int fd, int events);

/*
* 从fd event系统中删除fd。 fde_ndx的内容和上面的一致。
*/
int fdevent_event_del(fdevents * ev, int *fde_ndx, int fd);

/*
* 返回ndx对应的fd所发生的事件。
* 这里的ndx和上面的fde_ndx不一样，这个ndx是ev->epoll_events中epoll_event结构体的下标
* 第一次调用的时候，通常ndx为-1
* 这个ndx和其对应的fd没有关系。而fde_ndx等于其对应的fd。
*/
int fdevent_event_get_revent(fdevents * ev, size_t ndx);

/*
* 返回ndx对应的fd。
*/
int fdevent_event_get_fd(fdevents * ev, size_t ndx);

/*
* 返回下一个发生IO事件的fd。
*/
int fdevent_event_next_fdndx(fdevents * ev, int ndx);

/*
* 开始等待IO事件。timeout_ms是超时限制。
*/
int fdevent_poll(fdevents * ev, int timeout_ms);

/**
* 设置fd的状态，通常是设置为运行exec在子进程中关闭和非阻塞。
*/
int fdevent_fcntl_set(fdevents * ev, int fd);

/*
* 返回fd对应的事件处理函数地址。也就是fdnode中handler的值。
*/
fdevent_handler fdevent_get_handler(fdevents * ev, int fd);

/*
* 返回fd对应的环境。也就是fdnode中ctx的值。
*/
void *fdevent_get_context(fdevents * ev, int fd);

/*
  * 注册和取消注册fd。
  * 就是生成一个fdnode，然后保存在ev->fdarray中。或者删除之。
*/
int fdevent_register(fdevents * ev, int fd, fdevent_handler handler, void *ctx);
int fdevent_unregister(fdevents * ev, int fd);

/**
 * 初始化各种多路IO。
*/
int fdevent_linux_sysepoll_init(fdevents *ev);



