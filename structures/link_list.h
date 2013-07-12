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

#ifndef _LINK_LIST_H
#define _LINK_LIST_H

#include <stdlib.h>

typedef struct _list_node
{
    struct _list_node * next;
}list_node;

typedef struct _link_list
{
    int32_t size;
    list_node * head;
    list_node * tail;
}link_list;

link_list * create_link_list();
void    free_link_list(link_list * list);
list_node * link_list_head(link_list * list);
list_node * link_list_tail(link_list * list);
void    link_list_contract(link_list * to, link_list * from);
void    link_list_clear(link_list * list);
void    link_list_push_tail(link_list * list, list_node * node);
void    link_list_push_front(link_list * list, list_node * node);
list_node * link_list_pop(link_list * list);
int32_t     link_list_is_empty(link_list * list);
int32_t     link_list_size(link_list * list);

#endif