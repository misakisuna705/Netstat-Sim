#include <getopt.h>
#include <string.h>

#include "info.h"

void parse_arg(const int * argc, char * argv[], char filter[]);

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

    parse_arg(&argc, argv, filter);

    switch (t - u) {
        case 1:
            printf("List of TCP connections:\n");
            printf("%-5s %-45s %-45s %-50s\n", "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");
            list_info(TCP, filter);
            break;

        case -1:
            printf("List of UDP connections:\n");
            printf("%-5s %-45s %-45s %-50s\n", "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");
            list_info(UDP, filter);
            break;

        default:
            printf("List of TCP connections:\n");
            printf("%-5s %-45s %-45s %-50s\n", "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");
            list_info(TCP, filter);

            putchar('\n');

            printf("List of UDP connections:\n");
            printf("%-5s %-45s %-45s %-50s\n", "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");
            list_info(UDP, filter);
            break;
    }

    return 0;
}

void parse_arg(const int * argc, char * argv[], char filter[]) {
    switch (*argc) {
        case 0:
            break;

        default:
            strcpy(filter, argv[0]);

            for (int i = 1; i <= *argc - 1; i++) {
                strcat(filter, "[ ]+");
                strcat(filter, argv[i]);
            }
            break;
    }
}
