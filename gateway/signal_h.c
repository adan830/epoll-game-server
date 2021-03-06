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

#include "signal_h.h"
#include "engine.h"

engine_t * engine;

SigfunC * signal(int signo, SigfunC * func) {
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(signo, &act, &oact)) {
        return(SIG_ERR);
    }
    return (oact.sa_handler);
} 

//信号中断回调函数
void sig_free(int signo) {
    printf("-----------------");
    // free_engine(engine);
}

//TODO 套接字断开的信号处理函数
void sig_close_socket(int signo) {

}