#include <stdio.h>

#include "ip.h"

void parse_ip(Ip_Info * info, const int version, const char line[]) {
    switch (version) {
        case IPv4:
            parse_ipv4(info, line);
            break;

        case IPv6:
            parse_ipv6(info, line);
            break;

        default:
            break;
    }
}

void parse_ipv4(Ip_Info * info, const char line[]) {
    struct in_addr local_addr;
    struct in_addr foreign_addr;

    sscanf(line, "%*d: %X:%X %X:%X %*X %*X:%*X %*X:%*X %*d %*d %*d %d",
           &local_addr.s_addr, &info->local_port, &foreign_addr.s_addr, &info->foreign_port, &info->inode);

    inet_ntop(AF_INET, &local_addr, info->local_ip, sizeof(info->local_ip));
    inet_ntop(AF_INET, &foreign_addr, info->foreign_ip, sizeof(info->foreign_ip));
}

void parse_ipv6(Ip_Info * info, const char line[]) {
    struct in6_addr local_addr;
    int * local_ip = (int *)local_addr.s6_addr;
    struct in6_addr foreign_addr;
    int * foreign_ip = (int *)foreign_addr.s6_addr;

    sscanf(line, "%*d: %8X%8X%8X%8X:%X %8X%8X%8X%8X:%X %*X %*X:%*X %*X:%*X %*d %*d %*d %d",
           &local_ip[0], &local_ip[1], &local_ip[2], &local_ip[3], &info->local_port,
           &foreign_ip[0], &foreign_ip[1], &foreign_ip[2], &foreign_ip[3], &info->foreign_port, &info->inode);

    inet_ntop(AF_INET6, &local_addr, info->local_ip, sizeof(info->local_ip));
    inet_ntop(AF_INET6, &foreign_addr, info->foreign_ip, sizeof(info->local_ip));
}
