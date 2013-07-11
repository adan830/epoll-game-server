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

#include "link_list.h"

#include <stdio.h>

link_list * create_link_list() {
    link_list * list = calloc(1, sizeof(*list));
    return list;
}

void free_link_list(link_list ** list) {
    free(*list);
    *list = NULL;
}

list_node * link_list_head(link_list * list) {
    return list->head;
}

list_node * link_list_tail(link_list * list) {
    return list->tail;
}

void link_list_contract(link_list * to, link_list * from) {
    if (from->head && from->tail) {
        if (to->head && to->tail) {
            to->tail->next = from->head;
        }else {
            to->head = from->head;
        }
        to->tail = from->tail;
        from->head = from->tail = NULL;
        to->size += from->size;
        from->size = 0;
    }
}

void link_list_clear(link_list * list) {
    list->size = 0;
    list->head = list->tail = NULL;
}

void link_list_push_tail(link_list * list, list_node * node) {
    if (node->next) {
        return;
    }
    node->next = NULL;
    if (list->size == 0) { //表示链表为空
        list->head = list->tail = node;
    }else {
        list->tail->next = node;
        list->tail = node;
    }
    ++list->size;
}

void link_list_push_front(link_list * list, list_node * node) {
    if (node->next) {
        return;
    }
    node->next = NULL;
    if (list->size == 0) {
        list->head = list->tail = node;
    }else {
        node->next = list->head;
        list->head = node;
    }
    ++list->size;
}

list_node * link_list_pop(link_list * list) {
    if (list->size == 0) return NULL;
    list_node * node = list->head;
    list->head = list->head->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }
    --list->size;
    node->next = NULL;
    return node;
}

int32_t link_list_is_empty(link_list * list) {
    return list->size == 0;
}

int32_t link_list_size(link_list * list) {
    return list->size;
}

//暂时还未实现可以遍历链表进行回调匹配的实现，后面需要再实现