
//#include "global.h"

#include <stdio.h>

#include "double_link_list.h"

struct A{
  int a;
  int b;
};

int main(int argc, char *argv[])
{
  struct double_link_list *dl = create_double_link_list();
  double_link_list_init(dl);
  struct A a = {1,2};
  struct double_link_node * node = create_double_link_node(&a);
  double_link_list_push(dl, node);
  struct A b = {3,4};
  struct double_link_node * node1 = create_double_link_node(&b);
  double_link_list_push(dl, node1);
  while (!double_link_list_empty(dl)) {
    struct double_link_node * node2;
    node2 = double_link_list_pop(dl);
    printf("A a = %d\n",((struct A *)node2->data)->a);
    printf("A b = %d\n",((struct A *)node2->data)->b);
  }
  double_link_list_free(dl);
  return 0;
}
