#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
	int lfd = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(54324);
	server_addr.sin_family = AF_INET;
	
	
	int flag = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
	int ret = bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret == -1)
	{
		perror("bind fail.");
		exit(1);
	}
	
	listen(lfd, 36);
	
	struct pollfd allfd[1024];
	int max_index = 0;
	for(int i = 0; i < 1024; i++)
	{
		allfd[i].fd = -1;
		allfd[i].events = POLLIN;
	}
	allfd[0].fd = lfd;
	allfd[0].events = POLLIN;
	
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	printf("Start accept...\n");
	
	while(1)
	{
		int i = 0;
		int ret = poll(allfd, max_index + 1, -1);
		if(ret == -1)
		{
			perror("poll error");
			exit(1);
		}
		
		if(allfd[0].revents & POLLIN)
		{
			int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_len);
			char ip[64];
			printf("client addr:%s, Port: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(client_addr.sin_port));
			for(i = 0; i < 1024; i++)
			{
				if(allfd[i].fd == -1)
				{
					allfd[i].fd = cfd;
					break;
				}
			}
			
			max_index = max_index < i ? i : max_index;
			printf("max_index: %d\n", max_index);
			
		}
		
		for(i=1; i <= max_index; i++)
		{
			int fd = allfd[i].fd;
			if(fd == -1)
			{
				continue;
			}
			if(allfd[i].revents & POLLIN) // !!!!revents!!!!!
			{
				printf("fd:%d\n",fd);
				char buff[1024] = {0};
				int len = recv(fd, buff, sizeof(buff), 0);
				if(len == -1)
				{
					perror("recv error");
					exit(1);
				}
				else if(len == 0)
				{
					allfd[i].fd = -1;
					close(fd);
					printf("客户端断开。\n");
				}
				else
				{
					printf("recv: %s",buff);
					send(fd, buff, strlen(buff) + 1, 0);
				}
			}
		}
		
	}
	
	close(lfd);
	return 0;
}