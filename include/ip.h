#ifndef _IP_H_
#define _IP_H_

#include "main.h"

void parse_ip(Ip_Info * info, const int version, const char line[]);
void parse_ipv4(Ip_Info * info, const char line[]);
void parse_ipv6(Ip_Info * info, const char line[]);

#endif
