#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char**argv)
{
  int sockfd,n,i;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char bit, bitmap;
   char payload[2000];
   char *p = payload;

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(atoi(argv[1]));
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,payload,2000,0,(struct sockaddr *)&cliaddr,&len);
      printf("received %d bytes\n", n);
      printf("%x %x %x %x\n", payload[0]&0xff,payload[1]&0xff,payload[2]&0xff,payload[3]&0xff);
      if ((payload[0] & 0xff) == 0x12 && (payload[1] & 0xff) == 0x34 && (payload[2] & 0xff) == 0xab && (payload[3] & 0xff) == 0xcd){
	printf("TS packet detected \n");
	bitmap = payload[4];
	p = payload + 5;
	for(i = 7; 0 <= i; i --){
	  bit = (bitmap >> i) & 0x01;
	  printf("%d",(int)bit);
	}
	printf("\n");
      }
   }

      return 0;
}
