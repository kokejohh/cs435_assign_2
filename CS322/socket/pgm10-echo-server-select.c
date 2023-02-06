/*
 A simple echo server program 
 Kasidit Chanchio (kasiditchanchio@gmail.com)
*/
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP		"127.0.0.1"
#define SERV_PORT 	18800

#define MAXLINE	100

#include <sys/select.h>

int accept_cr(int fd, struct sockaddr *addr, socklen_t *len);
int write_full(int fd, const void *buf, size_t count);
int read_full(int fd, void *buf, size_t count);

int lis_fd;
int conn_fd;
struct sockaddr_in serv_addr;

int main(int argc, char *argv[]){

        int m, n, i;
        char line[MAXLINE];

	fd_set base_rfds; // base read fd set
	fd_set rfds; // read fd set to be passed as a parameter of select() 
	int fdmax;

	lis_fd = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	bind(lis_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); 

	listen(lis_fd, 5);

	FD_ZERO(&base_rfds);
	FD_ZERO(&rfds);

	FD_SET(lis_fd, &base_rfds);
	fdmax = lis_fd;

	for(;;){
	  memcpy(&rfds, &base_rfds, sizeof(fd_set));
	  if(select(fdmax+1, &rfds, NULL, NULL, NULL) < 0){
	    printf("select error!\n");
	    fflush(stdout);
	    exit(1);
	  }
  
	  for(i = 0; i <= fdmax; i++){
	    if(FD_ISSET(i, &rfds)){
		if(i == lis_fd){
		  if((conn_fd = accept_cr(lis_fd, NULL, NULL)) < 0){
		    printf("Accept: Error occured\n");
		    exit(1);
		  }
	          printf("a new connection %d is made!\n", conn_fd);
		  FD_SET(conn_fd, &base_rfds);
		  if(conn_fd > fdmax){
		    fdmax = conn_fd;
		  }
		}
		else{
		  n = read_full(i, line, MAXLINE);
		  if (n <= 0){
		    if(n == 0){
		        printf("read: close connection %d\n", i);
			FD_CLR(i, &base_rfds);
			close(i);
		    }
		    else{
		        printf("read: Error occured\n");
		        exit(1);
		    }
		  }
		  else{
                    printf("line = %s with n = %d characters\n", line, n);
                    fflush(stdout);
                    m = write_full(i, line, n);
                    printf("write line = %s for m = %d characters\n", line, m);
                    fflush(stdout);
        	  }
		}
	    }
	  }
	}
	close(lis_fd);

}

// -----Basic Communication Utilities-----
int accept_cr(int fd, struct sockaddr *addr, socklen_t *len){
	int ret;
repeat_accept:
        ret = accept(fd, addr, len);
        if (ret < 0) {
            if (errno == EINTR){
                goto repeat_accept;
	    }
	    printf("accept error errno=%d fd=%d\n", errno, fd);
        }
	return ret;
}

int write_full(int fd, const void *buf, size_t count){
    ssize_t ret = 0;
    ssize_t total = 0;

    while (count) {
        ret = write(fd, buf, count);
        if (ret < 0) {
            if (errno == EINTR){
                continue;
	    }
	    printf("write error errno=%d fd=%d\n", errno, fd);
            return ret;
        }
        else if (ret == 0){
            return ret; 
        }

        count -= ret;
        buf += ret;
        total += ret;
    }

    return total;
}

int read_full(int fd, void *buf, size_t count){
    ssize_t ret = 0;
    ssize_t total = 0;

    while (count) {
        ret = read(fd, buf, count);
        if (ret < 0) {
            if (errno == EINTR){ 
	        printf("read INTERRUPTED! %d\n", errno);
                continue;
	    } 
	    printf("read error errno=%d fd=%d\n", errno, fd);
            return ret;
        }
        else if (ret == 0){
            return ret; 
        }

        count -= ret;
        buf += ret;
        total += ret;
    }

    return total;
}
