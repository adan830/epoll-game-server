#include <stdio.h>

#include "buffer_h.h"


int main(int argc, char *argv[])
{
  buffer * bf = new_buffer(1024);
  char str = 'a';
  put_buffer(bf, str);
  flip_buffer(bf);
  char *str1 = get_buffer(bf);
  printf("str1 = %s\n",str1);
  clear_buffer(bf);

  char * str2 = "abcdefg";
  int size = put_buffers(bf, str2);
  printf("size = %d\n",size);
  printf("get_buffer_position = %d\n",get_buffer_position(bf));
  printf("get_buffer_remaining = %d\n",get_buffer_remaining(bf));
  flip_buffer(bf);
  printf("get_buffer_position1 = %d\n",get_buffer_position(bf));
  printf("get_buffer_remaining1 = %d\n",get_buffer_remaining(bf));
  char * str3 = (char *)malloc(sizeof(char)*strlen(str2));
  int size1 = sizeof(char)*strlen(str2);
  get_buffers(bf, str3, size1);
  printf("str3 = %s\n", str3);
  clear_buffer(bf);
  free(str3);

  char * str4 = "abcdefg";
  put_buffers(bf, str4);
  flip_buffer(bf);
  char * str5 = (char *)malloc(sizeof(char)*strlen(str2));
  int size2 = sizeof(char)*strlen(str4);
  int i;
  memset(str5, 0, size2);
  for(i = 0; i < size2; i++) {
    printf("%d = %c \n",i, *(str5+i));
  }
  get_buffers_len(bf, str5, size2, 2, 4);
  for(i = 0; i < size2; i++) {
    printf("%d = %c \n",i, *(str5+i));
  }
  printf("str4 = %s\n", str5);
  return 0;
}
