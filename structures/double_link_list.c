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

#include "double_link_list.h"

#include <stdio.h>

int32_t double_link_list_empty(struct double_link_list *dl) {
    return dl->head.next == &dl->tail ? 1:0;
}

struct double_link_node *double_link_list_first(struct double_link_list *dl) {
    if(double_link_list_empty(dl))
        return NULL;
    return dl->head.next;
}

struct double_link_node *double_link_list_last(struct double_link_list *dl) {
    if(double_link_list_empty(dl))
        return NULL;
    return dl->tail.pre;
}

int32_t double_link_list_remove(struct double_link_node * node) {
    if ( !node->pre || !node->next ) return -1;
    node->pre->next = node->next;
    node->next->pre = node->pre;
    node->pre = node->next = NULL;
    return 0;
}

struct double_link_node * double_link_list_pop(struct double_link_list * dl) {
    if (double_link_list_empty(dl)) return NULL;
    else {
        struct double_link_node * node = dl->head.next;
        double_link_list_remove(node);
        return node;
    }
}

int32_t double_link_list_push(struct double_link_list * dl, struct double_link_node * node) {
    if (node->pre || node->next) {
        return -1;
    }else {
        dl->tail.pre->next = node;
        node->pre = dl->tail.pre;
        dl->tail.pre = node;
        node->next = &dl->tail;
        return 0;
    }
}

void double_link_list_init(struct double_link_list * dl) {
    dl->head.pre = dl->head.next = NULL;
    dl->head.next = &dl->tail;
    dl->tail.pre = &dl->head;
}

struct double_link_list * create_double_link_list() {
    struct double_link_list * dl = (struct double_link_list *)malloc(sizeof(*dl));
    return dl;
}

void double_link_list_free(struct double_link_list * dl) {
   while (!double_link_list_empty(dl)) {
        struct double_link_node * node;
        node = double_link_list_pop(dl);
        free(node);
    }
    free(dl);
}

struct double_link_node * create_double_link_node(void * data) {
    struct double_link_node * node = (struct double_link_node *)malloc(sizeof(*node));
    node->data = data;
    return node;
}