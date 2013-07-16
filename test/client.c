#include "global.h"
#define MAXDATASIZE 100

struct Head
{
    int data_len;
    int cmd;
};
// struct Message
// {
//     struct Head head;
//     char data[10];
//   //  short check_code; //校验码
// };
int main(int argc, char *argv[])
{
    int sockfd;
    // char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; // connector'saddress information
 
    if (( argc == 1) || (argc==2) ) 
    {
        fprintf(stderr,"usage: client hostname\nEx:\n$./client01 ip port\n");
        exit(1);
    }
 
    if ((he = gethostbyname(argv[1])) == NULL) 
    { // get the host info
        printf("gethostbyname");
        exit(1);
    }
 
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
    {
        printf("socket");
        exit(1);
    }
 
    their_addr.sin_family = AF_INET; // host byte order
    their_addr.sin_port = htons(atoi(argv[2])); // short, network byte order
    //their_addr.sin_addr = *(struct in_addr *)(*(he->h_addr_list));
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
   // memset(their_addr.sin_zero, '', sizeof(their_addr.sin_zero));
    int status;
    if ( (status = connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) 
    {
        printf("status=%d\n",status);
        perror("connect");
        exit(1);
    }
    printf("--------------status=%d\n",status);
   //  struct Head head;
   // // struct Message message = (struct Message*)malloc(sizeof(struct Message));
   //  char *data = "111";
   //  head.data_len = strlen(data);
   //  head.cmd = 1;
    //message.head = head;
    //message.data = data;
    // int i;
    // char *str = "0005aa";
    // char *str1 = "1111";
    // char *str2 = "2";
    // char str[20] = {0,0,0,6,'a','a','a','a','a','a',0,0,0,1,'e'};
    // char *str = "0006aaaaaa0001e";
    // char *str1 = "er";
    // i = 6;
    // printf("%d\n",strlen(str));
    int len1 = 6;
    char * str = "aaaaaa";
    // int len2 = 1;
    // char * str = (char *)malloc(sizeof(char) * (4+6+4+1));
    // memcpy(str, &len1, 4);
    //printf("------------=%s\n",str);
    // char *s1 = "aaaaaa";
    // memcpy(str + 5, s1, 6);
    // memcpy(str + 11, &len2, 4);
    // char *s2 = "e";
    // memcpy(str + 15, s2, 1);
    // int i;
    // for(i = 0;i < 15; i++){
    //   printf("i = %c\n",*(str+i));
    // }
    // printf("%s\n",str);
    send(sockfd, &len1, sizeof(len1), 0);
   send(sockfd, str, 6, 0 );
    // while( 1 )
    // {
      // send(sockfd, str1, strlen(str1), 0 );
      // send(sockfd, str2, strlen(str2), 0 );
       // // send(sockfd, &i, sizeof(i), 0 );
       //  printf("----------------------------%d\n",send(sockfd, &i, sizeof(i), 0 ));
        // send(sockfd, str, strlen(str), 0 );
       //  printf("----------------------------%d\n",send(sockfd, str, strlen(str), 0 ));
       //   printf("----------------------------%d\n",send(sockfd, str1, strlen(str1), 0 ));
       //  // if(send(sockfd, &i, sizeof(i), 0 ) == -1)
       //  // {
       //  //     printf("come in-------111111111");
       //  //     perror("send");
       //  // }
       //  // if(send(sockfd, str, strlen(str), 0 ) == -1)
       //  // {
       //  //     printf("come in-------111111111");
       //  //     perror("send");
       //  // }
       //  // printf("%s\n",data);
       //  // if(send(sockfd, data, strlen(data), 0 ) == -1)
       //  // {
       //  //             printf("come in-------111111111");
       //  //             perror("send");
       //  // }
       //  printf("come in-------2222222222");
       //  if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
       //  {
       //      perror("recv");
       //  }
       //  else if( numbytes == 0 )
       //  {
       //      printf("Remote server has shutdown!\n");
       //      break;
       //  }
 
       //  buf[numbytes] = '\0';
 
       //  printf("Received: %s \n",buf);
       //  sleep(1);
    // }
 
    close(sockfd);
    return 0;
}
