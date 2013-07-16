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

#include "buffer_h.h"

static buffer * init_buffer(buffer * bf, int32_t capacity) {
    bf->capacity = capacity;
    bf->limit = capacity;
    bf->position = 0;
    bf->mark = 0;
    bf->data = (char *)malloc(sizeof(char) * capacity);
    memset(bf->data, 0, capacity);
    return bf;
}

buffer * new_buffer(int32_t capacity) {
    buffer * bf = (buffer *)malloc(sizeof(*bf));
    bf = init_buffer(bf, capacity);
    return bf;
}

void free_buffer(buffer * bf) {
    free(bf->data);
    free(bf);
}

int32_t put_buffers_len(buffer * bf, char *str, int32_t offset, int32_t length) {
    int i;
    int res = 0;
    int sum = 0;
    for (i = 0; i < length; i++) {
        res = put_buffer(bf, *(str + offset + i));
        if (res == -1 && sum == 0) {
            return -1;
        }else if (res == -1 && sum != 0) {
            return sum;
        }else {
            sum += res;
        }
    }
    return sum;
}

int32_t put_buffers(buffer * bf, char * str) {
    int i;
    int res = 0;
    int sum = 0;
    for(i = 0; i < strlen(str); i++) {
        res = put_buffer(bf, *(str+i));
        if (res == -1 && sum == 0) {
            return -1;
        }else if(res == -1 && sum != 0) {
            return sum;
        }else {
            sum += res;
        }
    }
    return sum;
}

int32_t put_buffer(buffer * bf, char ch) {
    if (get_buffer_limit(bf) > get_buffer_capacity(bf)) {
        return -1;
    }else {
        *(bf->data + bf->position) = ch;
        bf->position++;
        return 1;
    }
}

char * get_buffers_len(buffer * bf, char * str, int32_t size, int32_t offset, int32_t length) {
    if (offset >= 0 && offset < size) {
        if (length >= 0 && length < size - offset) {
            int i;
            for (i = 0; i < length; i++) {
                char * tmp = get_buffer(bf);
                if (tmp != NULL) *(str + offset + i) = *tmp;
            }
        }else {
            return NULL;
        }
    }else {
        return NULL;
    }
    return str;
}

char * get_buffers(buffer * bf, char * str, int32_t size) {
    if (get_buffer_remaining(bf) < size) {
        return NULL;
    }else {
        int i;
        for (i = 0; i < size; i++) {
            char * tmp = get_buffer(bf);
            // printf("tmp = %c\n",*tmp);
            if (tmp != NULL) *(str + i) = *tmp;
        }
    }
    return str;
}

char * get_buffer(buffer * bf) {
    if (get_buffer_position(bf) > get_buffer_limit(bf)) {
        return NULL;
    }
    else {
        char * a = bf->data + bf->position;
        bf->position++;
        return a;
    }
}

void flip_buffer(buffer * bf) {
    bf->limit = bf->position;
    bf->position = 0;
}

void clear_buffer(buffer * bf) {
    bf->limit = bf->capacity;
    bf->position = 0;
}

int32_t get_buffer_limit(buffer * bf) {
    return bf->limit;
}

int32_t get_buffer_position(buffer * bf) {
    return bf->position;
}

int32_t get_buffer_capacity(buffer * bf) {
    return bf->capacity;
}

int32_t get_buffer_remaining(buffer * bf) {
    return bf->limit - bf->position;
}

bool buffer_has_remaining(buffer * bf) {
    return bf->limit - bf->position ? true : false;
}
