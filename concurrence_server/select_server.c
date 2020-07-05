#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int lfd = socket(PF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_addr;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(54323);
	server_addr.sin_family = AF_INET;
	socklen_t socklen = sizeof(server_addr);
	
	int flag = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
	int ret = bind(lfd, (struct sockaddr *)&server_addr, socklen);
	if (ret == -1){
		perror("bind error");
		exit(1);
	}
	
	listen(lfd, 36);
	
	int max_fd = lfd;
	printf("Start accept...\n");
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	fd_set reads, temp;
	FD_ZERO(&reads);
	FD_SET(lfd, &reads);	
	
	while(1){

		temp = reads;
		int ret = select(max_fd + 1, &temp, NULL, NULL, NULL);
		if(ret == -1){
			perror("select fail.");
			exit(1);
		}
		
		if(FD_ISSET(lfd, &temp)){
			int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_len);
			if(cfd == -1){
				perror("client error.");
				continue;
			}
			char ip[64];
			printf("new client IP:%s, Port: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(client_addr.sin_port));
			FD_SET(cfd, &reads);
			
			max_fd = cfd > max_fd ? cfd : max_fd;
		}
		
		for(int i = lfd + 1; i <= max_fd; i++){
			if(FD_ISSET(i, &temp))
			{
				char buff[1024] = {0};
				int len = recv(i, buff, sizeof(buff), 0);
				
				if(len == -1){
					perror("recv error");
					exit(1);
				}
				else if(len == 0){
					printf("客户端断开\n");
					close(i);
					FD_CLR(i, &reads);
				}
				else{
					printf("recv buf: %s\n", buff);
					send(i, buff, sizeof(buff), 0);
				}
			}
		}
	}
	close(lfd);
	return 0;
}