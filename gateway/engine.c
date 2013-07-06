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
#include "engine.h"
#include "server.h"

#include <stdlib.h>
#include <assert.h>

engine_t * create_engine() {
    engine_t * e = (engine_t *)malloc(sizeof(engine_t *));
    if (e) {
        init_engine(e);
    }
    return e;
}

void start_engine(engine_t * e) {
    e->engine_server_start(); //启动服务器
}

void free_engine(engine_t * e) {
    assert(e);
    free(e);
}

engine_t * init_engine(engine_t * e) {
    e->engine_server_start = start_server;
    return e;
}