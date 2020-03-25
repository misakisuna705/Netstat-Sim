#ifndef _MAIN_H_
#define _MAIN_H_

#define SIZE 1024

#include <arpa/inet.h>

enum {TCP, UDP};
enum {IPv4 = ' ', IPv6 = '6'};

typedef struct {
    char local_ip[SIZE];
    int local_port;
    char foreign_ip[SIZE];
    int foreign_port;
    int inode;
} Ip_Info;

typedef struct {
    int pid;
    char pname[SIZE];
} Process_Info;

#endif
