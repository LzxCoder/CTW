#include "../lib/csapp.h"


#include "recv_mem.h"
#include "to_client.h"
#include "sort_mem.h"


void *output_mem(){
    int i,j,n;

    while(1){

        n = num;
        for(i=0;i<n;i++){
            printf("NO.%s server has %s kb free memory\n",uses[i].ipaddr, uses[i].mems);
        }
        if (num>0) printf("\n");
        usleep(10000000);
    }
}

/*
void *to_client(){

        int server_sockfd,client_sockfd;   
        int server_len,client_len;
        char tmp[CSIZE];
        struct sockaddr_in server_sockaddr,client_sockaddr;
        char buf[CSIZE];

        server_sockfd = socket(AF_INET,SOCK_STREAM, 0); // 定义套接字类型
   
        server_sockaddr.sin_family=AF_INET;
        server_sockaddr.sin_port=htons(PORTCT);
        server_sockaddr.sin_addr.s_addr=INADDR_ANY;
   
        server_len=sizeof(server_sockaddr);
        int j=1;
        setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&j,sizeof(j));
   
    //绑定端口
        if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,server_len)==-1){
                perror("bind:");
                exit(1);       
        }
   
   
        if(listen(server_sockfd,5)==-1){
                perror("listen:");
                exit(1);   
        }
        printf("Listening from client\n");
   
        client_len=sizeof(client_sockaddr);
        while(1) {  
            if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&client_sockaddr,&client_len))==-1) {
                        perror("accept error:");
                        exit(1);
                }
                memset(buf, 0, sizeof(buf));
                printf("%s收到请求\n",inet_ntoa(client_sockaddr.sin_addr));
                recv(client_sockfd,buf,CSIZE,0);
                printf("%s\n",buf);
                if (strcmp(buf,"getip") == 0){
                    if (num > 0){
                        int sendbytes = 0;
                        sort_mem(num);
                        memset(buf, 0, sizeof(buf));
                        strcpy(buf, uses[0].ipaddr);
                        printf("%s\n",buf);
                        usleep(1000000);
                        if ((sendbytes = write(client_sockfd, buf, CSIZE)) == -1){
                                perror("send to server failed!");
                            }    
                        printf("success!\n");
                        } else perror("No servers stand by!");  
                }
            usleep(100000);                   
            }
        close(client_sockfd);
}

*/

int main()
{  
              memset(uses,0,sizeof(uses));  
              num = 0;
             pthread_t clit;
             int cl;
             cl = pthread_create(&clit, NULL, to_client, NULL);

    	int server_sockfd,client_sockfd;   
    	int server_len,client_len;
   
    	struct sockaddr_in server_sockaddr,client_sockaddr;
   
   
    	server_sockfd = socket(AF_INET,SOCK_STREAM, 0); // 定义套接字类型
   
    	server_sockaddr.sin_family=AF_INET;
    	server_sockaddr.sin_port=htons(PORTTW);
   	server_sockaddr.sin_addr.s_addr=INADDR_ANY;
   
    	server_len=sizeof(server_sockaddr);
   
    //允许重复使用本地地址和套接字绑定
    	int j=1;
    	setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&j,sizeof(j));
   
    //绑定端口
    	if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,server_len)==-1){
        		perror("bind:");
        		exit(1);       
    	}
   
    	if(listen(server_sockfd,5)==-1){
        		perror("listen:");
        		exit(1);   
    	}
    	printf("Listening from worker\n");
   
    	client_len=sizeof(client_sockaddr);
   
    	pthread_t thread[CSIZE];
              int i,ret,temp;
              pthread_t outt;
               temp = pthread_create(&outt , NULL, output_mem, NULL); 
  	while(1) {  
    		if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&client_sockaddr,&client_len))==-1) {
             			perror("accept error:");
             			exit(1);
        		}
                            printf("%s登录服务器\n",inet_ntoa(client_sockaddr.sin_addr));
                            strcpy(uses[num].ipaddr, inet_ntoa(client_sockaddr.sin_addr));

                            struct mypara mp;
                            mp.num=num++;
                            mp.data=client_sockfd;

                            ret = pthread_create(&thread[num],NULL,recv_mem,&(mp));
                            usleep(10000);        
    	}
        close(client_sockfd);
        return 0;
}