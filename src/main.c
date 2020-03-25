#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <regex.h>
#include <string.h>

#include "main.h"
#include "ip.h"
#include "process.h"

void parse_filter(const int * argc, char * argv[], char filter[]);
void list_info(const int protocol, const char filter[]);
void print_info(const char proto[], const int version, Ip_Info * ip_info, Process_Info * process_info, const char filter[]);

int main(int argc, char * argv[]) {
    const char * short_opts = "tu";
    const struct option long_opts[] = {{"tcp", no_argument, NULL, 't'}, {"udp", no_argument, NULL, 'u'}, {NULL, 0, NULL, 0}};
    char ch;
    int t = 0, u = 0;
    char filter[SIZE] = {'\0'};

    while ((ch = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
        switch (ch) {
            case 't':
                t = 1;
                break;

            case 'u':
                u = 1;
                break;

            default:
                break;
        }
    }

    argc -= optind;
    argv += optind;

    parse_filter(&argc, argv, filter);

    switch (t - u) {
        case 1:
            list_info(TCP, filter);
            break;

        case -1:
            list_info(UDP, filter);
            break;

        default:
            list_info(TCP, filter);
            list_info(UDP, filter);
            break;
    }

    return 0;
}

void parse_filter(const int * argc, char * argv[], char filter[]) {
    switch (*argc) {
        case 0:
            break;

        default:
            strcpy(filter, argv[0]);

            for (int i = 1; i <= *argc - 1; i++) {
                strcat(filter, " ");
                strcat(filter, argv[i]);
            }
            break;
    }
}

void list_info(const int protocol, const char filter[]) {
    char line[SIZE];
    FILE * fd = NULL;
    Ip_Info ip_info;
    Process_Info process_info;
    char proto[SIZE] = {'\0'};

    switch (protocol) {
        case TCP:
            fd = fopen("/proc/net/tcp", "r");
            strcpy(proto, "tcp");
            break;

        case UDP:
            fd = fopen("/proc/net/udp", "r");
            strcpy(proto, "udp");
            break;

        default:
            break;
    }

    fgets(line, sizeof(line), fd);

    while (fgets(line, sizeof(line), fd) != NULL) {
        parse_ip(&ip_info, IPv4, line);
        parse_process(&process_info, &ip_info.inode);
        print_info(proto, IPv4, &ip_info, &process_info, filter);
    }

    fclose(fd);

    switch (protocol) {
        case TCP:
            fd = fopen("/proc/net/tcp6", "r");
            break;

        case UDP:
            fd = fopen("/proc/net/udp6", "r");
            break;

        default:
            break;
    }

    fgets(line, sizeof(line), fd);

    while (fgets(line, sizeof(line), fd) != NULL) {
        parse_ip(&ip_info, IPv6, line);
        parse_process(&process_info, &ip_info.inode);
        print_info(proto, IPv6, &ip_info, &process_info, filter);
    }

    fclose(fd);
}

void print_info(const char proto[], const int version, Ip_Info * ip_info, Process_Info * process_info, const char filter[]) {
    if (filter[0]) {
        regex_t regex;
        regmatch_t match[1];

        regcomp(&regex, filter, REG_EXTENDED);

        if (!regexec(&regex, process_info->pname, 1, match, 0)) {
            char local_addr[SIZE], foreign_addr[SIZE];

            sprintf(local_addr, "%s:%d", ip_info->local_ip, ip_info->local_port);
            sprintf(foreign_addr, "%s:%d", ip_info->foreign_ip, ip_info->foreign_port);

            printf("%s%-2c %-45s %-45s %d/%s\n", proto, version, local_addr, foreign_addr, process_info->pid, process_info->pname);
        }
    } else {
            char local_addr[SIZE], foreign_addr[SIZE];

            sprintf(local_addr, "%s:%d", ip_info->local_ip, ip_info->local_port);
            sprintf(foreign_addr, "%s:%d", ip_info->foreign_ip, ip_info->foreign_port);

            printf("%s%-2c %-45s %-45s %d/%s\n", proto, version, local_addr, foreign_addr, process_info->pid, process_info->pname);
    }
}
