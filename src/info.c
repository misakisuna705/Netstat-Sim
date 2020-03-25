#include <string.h>
#include <regex.h>

#include "ip.h"
#include "process.h"
#include "info.h"

void list_info(const int protocol, const char filter[]) {
    FILE * fd_v4 = NULL, * fd_v6 = NULL;
    char proto[SIZE] = {'\0'};

    switch (protocol) {
        case TCP:
            fd_v4 = fopen("/proc/net/tcp", "r");
            fd_v6 = fopen("/proc/net/tcp6", "r");
            strcpy(proto, "tcp");
            break;

        case UDP:
            fd_v4 = fopen("/proc/net/udp", "r");
            fd_v6 = fopen("/proc/net/udp6", "r");
            strcpy(proto, "udp");
            break;

        default:
            break;
    }

    parse_info(fd_v4, proto, IPv4, filter);
    parse_info(fd_v6, proto, IPv6, filter);

    fclose(fd_v4);
    fclose(fd_v6);
}

void parse_info(FILE * fd, char proto[], int version, const char filter[]) {
    char line[SIZE];
    Ip_Info ip_info;
    Process_Info process_info;

    fgets(line, sizeof(line), fd);

    while (fgets(line, sizeof(line), fd) != NULL) {
        parse_ip(&ip_info, version, line);
        parse_process(&process_info, &ip_info.inode);
        parse_filter(proto, version, &ip_info, &process_info, filter);
    }
}

void parse_filter(const char proto[], const int version, Ip_Info * ip_info, Process_Info * process_info, const char filter[]) {
    regex_t regex;
    regmatch_t match[1];

    switch (filter[0]) {
        case 0:
            print_info(proto, version, ip_info, process_info);
            break;

        default:

            regcomp(&regex, filter, REG_EXTENDED);

            if (!regexec(&regex, process_info->pname, 1, match, 0)) {
                print_info(proto, version, ip_info, process_info);
            }
            break;
    }
}

void print_info(const char proto[], const int version, Ip_Info * ip_info, Process_Info * process_info) {
    char local_addr[SIZE], foreign_addr[SIZE];

    sprintf(local_addr, "%s:%d", ip_info->local_ip, ip_info->local_port);
    sprintf(foreign_addr, "%s:%d", ip_info->foreign_ip, ip_info->foreign_port);

    switch (process_info->pid) {
        case '-':
            printf("%s%-2c %-45s %-45s %c/%s\n", proto, version, local_addr, foreign_addr, process_info->pid, process_info->pname);
            break;

        default:
            printf("%s%-2c %-45s %-45s %d/%s\n", proto, version, local_addr, foreign_addr, process_info->pid, process_info->pname);
            break;
    }
}
