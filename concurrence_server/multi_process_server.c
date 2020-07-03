#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

void recycle(int num){
	pid_t pid;
	while((pid = waitpid(-1, NULL, WNOHANG)) > 0){
		printf("child recycle. pid = %lu\n", pid);
	}
}

int main(int argc, char *argv[]) {
	
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; //address family
	server_addr.sin_port = htons(54321);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int ret = bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret < 0){
		perror("bind fail.\n");
		exit(1);
	}
	
	listen(fd, 36);
	struct sigaction act;
	act.sa_handler = recycle;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD, &act, NULL);
	printf("start accept...\n");
	
	while(1){
		struct sockaddr_in client_addr;
		socklen_t socklen = sizeof(client_addr);

		int cfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
		while(cfd == -1 && errno == EINTR){
			cfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
			printf("interrupt.\n");
		}
		printf("connect successfully.\n");
		
		pid_t pid = fork();
		if(pid == 0)
		{
			close(fd); //child process no long need listen.
			char ip[64];
			while(1){
				printf("client IP: %s, port: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr,ip, sizeof(ip)), ntohs(client_addr.sin_port));
				
				char buff[1024];
				memset(buff, 0, sizeof(buff));
				int len = read(cfd, buff, sizeof(buff));
				if(len == -1){
					perror(1);
					exit(1);
				}
				else if (len == 0) {
					printf("与服务器断开连接。\n");
					close(cfd);
					break;
				}
				else
				{
					printf("received: %s\n", buff);
					write(cfd, buff, len);
				}
			}
			
			//child should be terminate
			return 0;
		}
		else if(pid > 0)
		{
			close(cfd);
		}
	}
	
	close(fd);
	return 0;
	
}