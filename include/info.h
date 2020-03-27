#ifndef _INFO_H_
#define _INFO_H_

#include <stdio.h>

#include "main.h"

void list_info(const int protocol, const char filter[]);
void parse_info(FILE * fd, char proto[], int version, const char filter[]);
void print_info(const char proto[], const int version, Ip_Info * ip_info, Process_Info * process_info, const char filter[]);

#endif
