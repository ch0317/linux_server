#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	int lfd = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(54325);
	server_addr.sin_family = AF_INET;

	int flag = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
	int ret = bind(lfd, (struct sockaddr *)&server_addr,sizeof(server_addr));
	if(ret == -1)
	{
		perror("bind fail.");
		exit(1);
	}

	listen(lfd, 36);

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = lfd;
	int epfd = epoll_create(2000);
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);

	struct epoll_event all_event[2000];
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	printf("Start epoll wait...\n");

		
	while(1){
		int ready = epoll_wait(epfd, all_event, sizeof(all_event) / sizeof(struct epoll_event), -1);	
		for(int i = 0; i < ready; ++i){
			int fd = all_event[i].data.fd;
			if(fd == lfd){
				int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_len);
				if(cfd == -1)
				{
					perror("accept error");
					exit(1);
				}
				
				int flag = fcntl(cfd, F_GETFL);
				flag |= O_NONBLOCK;
				fcntl(cfd, F_SETFL, flag);
				struct epoll_event temp;
				temp.data.fd = cfd;
				temp.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &temp);
				char ip[64];
				printf("client IP: %s , Port: %d\n",
				inet_ntop(AF_INET, &client_addr.sin_addr.s_addr,ip, sizeof(ip)), ntohs(client_addr.sin_port));

			}
			else{
				if(!all_event[i].events & EPOLLIN)
				{
					continue;
				}

				char buff[10] = {0};
				int len;
				while((len = recv(fd, buff, sizeof(buff),0)) > 0){
					
					write(STDOUT_FILENO, buff, len);
					send(fd, buff, len, 0);
				}
				if(len == -1){
					if(errno == EAGAIN){
						printf("read to end.\n");	
					}
					else{
						perror("recv error");
						exit(1);
					}
				}
				else if(len == 0){
					printf("client disconnected...\n");
					epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
					
					close(fd);
				}
				else{
					printf("recv buf: %s\n", buff);
					write(fd, buff, sizeof(buff));
				}
			}
		}
	}

	return 0;
}
