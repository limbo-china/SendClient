#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>

#include "conn.h"
#include "times.h"
#include "proto.h"
#include "cycle.h"

int system_shutdown = 0;
#define PKT_LEN 1024*1024*10
#define G_RET_SUCCESS 0
#define STD_FILE_SIZE 251
#define MAX_FILE_NAME 1024
//#define KEYS_DIR "./keys/"

int get_file_len(const char *filename)
{
	int ret = 0;
	struct stat statbuf;
    ret = stat(filename, &statbuf);
	if(ret == -1)
	{
		if(errno == ENOENT)
			return -1;
	}
    ret = statbuf.st_size;
	printf("%d\n", ret);
	return ret;
}

int get_usr_status(const char *id)
{
	int ret = 0;
	char key_file_path[MAX_FILE_NAME] = { 0 };
	sprintf(key_file_path, "%s/%s.pub", g_cycle.keys_path, id);
	ret = get_file_len(key_file_path);
	if(ret != STD_FILE_SIZE)
		return -1;
	return ret;
}

void doVerify(int fd, char* buff)
{
	int ck = get_usr_status(buff);
	char tag = 0x01;
	printf("%d\n", ck);
	char* rsp_buff = (char*)calloc(PKT_LEN, 1);
	int cur = 0;
	memcpy(rsp_buff + cur, &tag, sizeof(tag));
	cur += sizeof(tag);
	int nleft_len = sizeof(ck);
	memcpy(rsp_buff + cur, &nleft_len, sizeof(nleft_len));
	cur += sizeof(nleft_len);
	memcpy(rsp_buff + cur, &ck, sizeof(ck));
	conn_write(fd, rsp_buff, sizeof(tag) + sizeof(nleft_len) + sizeof(ck), 0);

}

int server_cnf_loop(char* ip, int port)
{
	int fd = -1, lsfd = -1;
	//int ck = 0;
	//int cur = 0;

	fd_set fds;
	struct sockaddr_in addr;
	socklen_t socklen = 0;

	char *buff = (char *)calloc(PKT_LEN, sizeof(char));
	if(buff == NULL){
		printf("No free memory, alloc buff failed!\n");
		return -1;
	}
	size_t bufflen = sizeof(char) * PKT_LEN;

	while(!system_shutdown)
	{
		while((lsfd=conn_open(ip, port, CONN_REUSE)) == -1)
		{
			printf("Create server listen failed, rebuild listen socket.");
			sleep(1);
		}
again:
		fd = accept(lsfd, (struct sockaddr *)&addr, &socklen);
		if(fd == -1){
			usleep(100);
			goto again;
		}

		if(addr.sin_addr.s_addr == 0){
			getpeername(fd, (struct sockaddr *)&addr, &socklen);
		}

		printf("User link in, ip is %u.\n", addr.sin_addr.s_addr);
		fflush(stdout);

		for(;;)
		{
			FD_ZERO(&fds);
			FD_SET(fd, &fds);
			struct timeval tv = { 1, 0 };
			int ret = select(fd+1, &fds, NULL, NULL, &tv);
			if(ret == -1 && errno != EAGAIN)
			{
				close(fd);
				goto again;
			}

			if(ret == 0)
			{
				close(fd);

				/* check if listen fd error */
				if(conn_check(lsfd) == -1){
					close(lsfd);
					break;
				}
				goto again;
			}

			/* processing read event */
			if(FD_ISSET(fd, &fds))
			{
				printf("Recv new packet!\n");
				memset(buff, '\0', sizeof(char)*bufflen);
				int tag;
				int nleft_len;
				if(conn_read(fd, &tag, sizeof(tag), 0) != G_RET_SUCCESS)
				{
					close(fd);

					goto again;
				}
				tag = htonl(tag);

				if(conn_read(fd, &nleft_len, sizeof(nleft_len), 0) != G_RET_SUCCESS)
				{
					close(fd);

					goto again;
				}

				nleft_len = ntohl(nleft_len);
				/* packet header correct */
				if(nleft_len > bufflen)
				{
					buff = (char *)realloc(buff, nleft_len);
					if(buff == NULL)
					{
						close(fd);
						free(buff);
						goto error;
					}
					bufflen = nleft_len;
				}

				if(conn_read(fd, buff, nleft_len, 0) != G_RET_SUCCESS)
				{
					printf("Read packet body error.");
					close(fd);
					goto again;
				}

				if(tag == 0x01)
				{
					doVerify(fd, buff);
				}
				else
				{
					char data_file_path[MAX_FILE_NAME] = { 0 };
					char data_file_name[20] = { 0 };
					time2str(data_file_name, MAX_FILE_NAME, "%m%d%H%M%S");

					sprintf(data_file_path, "%s/%s.txt", g_cycle.target_path, data_file_name);
					printf("%s\n", data_file_path);
					FILE* fp = fopen(data_file_path, "a");
					if(!fp)
					{
						close(fd);
						goto again;
					}
					fwrite(buff, strlen(buff), 1, fp);
					fclose(fp);
					//printf("%s\n", buff);

					fflush(stdout);
				}
				close(fd);
				goto again;
			}
		}
	}
error:
	free(buff);
	close(lsfd);
	return -1;
}
