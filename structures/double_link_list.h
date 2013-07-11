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

#ifndef _DOUBLE_LINK_LIST_H
#define _DOUBLE_LINK_LIST_H

#include <stdlib.h>

struct double_link_node
{
    struct double_link_node * pre;
    struct double_link_node * next;
};

struct double_link_list
{
    struct double_link_node head;
    struct double_link_node tail;
};

int32_t     double_link_list_empty(struct double_link_list *dl);
struct      double_link_node * double_link_list_first(struct double_link_list *dl);
struct      double_link_node * double_link_list_last(struct double_link_list *dl);
int32_t     double_link_list_remove(struct double_link_node * node);
struct      double_link_node * double_link_list_pop(struct double_link_list * dl);
int32_t     double_link_list_push(struct double_link_list * dl, struct double_link_node * ndoe);
void        double_link_list_init(struct double_link_list * dl);

#endif