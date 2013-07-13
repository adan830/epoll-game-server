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

#ifndef _BUFFER_H_H
#define _BUFFER_H_H

#include "global.h"

typedef struct _buffer {
    int32_t capacity;
    int32_t limit;
    int32_t position;
    int32_t mark;
    char * data;
}buffer;

buffer *    new_buffer(int32_t capacity);
void        free_buffer(buffer * bf);
// static buffer *     init_buffer(buffer * bf, int32_t capacity);
int32_t     put_buffer(buffer * bf, char ch);
int32_t     put_buffers(buffer * bf, char * str);
int32_t     put_buffers_len(buffer * bf, char *str, int32_t offset, int32_t length);
char *      get_buffer(buffer * bf);
char *      get_buffers(buffer * bf, char * str, int32_t size);
char *      get_buffers_len(buffer * bf, char * str, int32_t size, int32_t offset, int32_t length);
void        flip_buffer(buffer * bf);
void        clear_buffer(buffer * bf);
int32_t     get_buffer_limit(buffer * bf);
int32_t     get_buffer_position(buffer * bf);
int32_t     get_buffer_capacity(buffer * bf);
int32_t     get_buffer_remaining(buffer * bf);
bool        buffer_has_remaining(buffer * bf);

#endif