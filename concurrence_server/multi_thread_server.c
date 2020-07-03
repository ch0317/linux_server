#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

typedef struct SOCKINFO
{
	int fd;
	struct sockaddr_in addr;
	pthread_t id;
}SockInfo;

void worker(void *arg){
	char ip[64];
	char buff[1024];
	SockInfo *info = (SockInfo*)arg;
	while(1){
		printf("client IP:%s, port:%d\n",
			inet_ntop(AF_INET, &info->addr.sin_addr.s_addr, ip, sizeof(ip)),
			ntohs(info->addr.sin_port));
		int len = read(info->fd, buff, sizeof(buff));
		if(len == -1){
			perror("read error");
			pthread_exit(NULL);
		}
		else if(len == 0){
			printf("客户端断开连接\n");
			close(info->fd);
			break;
		}
		else{
			printf("recv buf: %s\n", buff);
			write(info->fd, buff, len);
		}
		
	}
}

int main(int argc, char *argv[]) {
	int lfd = socket(PF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_addr;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(54322);
	server_addr.sin_family = AF_INET; //address family
	
	bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	listen(lfd, 36);
	struct sockaddr client_addr;
	socklen_t socklen = sizeof(client_addr);
	SockInfo client_info[256];
	for(unsigned int i = 0; i < sizeof(client_info) / sizeof(SockInfo); i++){
		client_info[i].fd = -1;
	}
	unsigned int i = 0;
	while(1){
		
		for(i = 0; i < sizeof(client_info) / sizeof(SockInfo); i++){
			if(client_info[i].fd == -1){
				break;
			}
		}
		if(i == 256){
			break;
		}
		
		client_info[i].fd = accept(lfd, (struct sockaddr *)&client_info[i].addr, &socklen);
		
		pthread_t tid;
		pthread_create(&tid, NULL, worker, &client_info[i]);
		pthread_detach(tid);
		
	}
	
	close(lfd);
	
}