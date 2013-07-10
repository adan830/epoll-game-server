
//#include "global.h"

#include <stdio.h>

#include "link_list.h"

struct A{
  list_node node;
  int a;
};

int main(int argc, char *argv[])
{
  struct link_list * list = create_link_list();
  list_node node;
  node.next = NULL;
  struct A * a = malloc(sizeof(*a));
  a->node = node;
  a->a = 1;
  printf("list_node = %p\n",((list_node *)a)->next);
  printf("a = %p\n",a);
  // printf("pointer = %p\n",a->next);
  link_list_push_tail(list, (list_node *)a);
  list_node node1;
  node1.next = NULL;
  // struct A b = {node1, 2};
  struct A * b = malloc(sizeof(*b));
  b->node = node;
  b->a = 2;
  link_list_push_tail(list, (list_node *)b);
  while (link_list_is_empty(list) == 0) {
    list_node * node;
    node = link_list_pop(list);
    printf("node = %d\n",((struct A*)node)->a);
  }
  return 0;
}
