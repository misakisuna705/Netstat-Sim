#define _GNU_SOURCE

#include <stdio.h>
#include <dirent.h>
#include <regex.h>
#include <unistd.h>
#include <string.h>

#include "process.h"

void parse_process(Process_Info * info, const int * inode) {
    info->pid = '-';
    info->pname[0] = '-';
    info->pname[1] = '\0';

    parse_pid(&info->pid, inode);

    switch (info->pid) {
        case '-':
            break;

        default:
            parse_pname(info->pname, &info->pid);
            break;
    }
}

void parse_pid(int * pid, const int * inode) {
    DIR * proc;
    struct dirent * proc_entry;

    proc = opendir("/proc");

    while ((proc_entry = readdir(proc)) != NULL) {
        regex_t regex;
        regmatch_t match[1];

        regcomp(&regex, "^[0-9]+$", REG_EXTENDED);

        if (!regexec(&regex, proc_entry->d_name, 1, match, 0)) {
            char path[SIZE];

            sprintf(path, "/proc/%s/fd", proc_entry->d_name);

            if (!eaccess(path, R_OK)) {
                DIR * procfd;
                struct dirent * procfd_entry;

                procfd = opendir(path);

                while ((procfd_entry = readdir(procfd)) != NULL) {
                    sprintf(path, "/proc/%s/fd/%s", proc_entry->d_name, procfd_entry->d_name);

                    if (!eaccess(path, R_OK) && procfd_entry->d_type == DT_LNK) {
                        char link[SIZE];
                        int len;
                        char socket1[SIZE], socket2[SIZE];

                        len = readlink(path, link, SIZE);
                        link[len] = '\0';

                        sprintf(socket1, "socket:[%d]", *inode);
                        sprintf(socket2, "0000:[%d]", *inode);

                        if (!strcmp(link, socket1) || !strcmp(link, socket2)) {
                            sscanf(proc_entry->d_name, "%d", pid);

                            return;
                        }
                    }
                }

                closedir(procfd);
            }
        }
    }

    closedir(proc);
}

void parse_pname(char pname[], const int * pid) {
    char cmd[SIZE], opt[SIZE] = {'\0'};

    parse_cmd(cmd, pid);
    parse_opt(opt, pid);

    sprintf(pname, "%s %s", cmd, opt);
}

void parse_cmd(char cmd[], const int * pid) {
    char comm_path[SIZE];
    FILE * comm_fd;

    sprintf(comm_path, "/proc/%d/comm", *pid);

    comm_fd = fopen(comm_path, "r");

    fgets(cmd, SIZE, comm_fd);
    cmd[strlen(cmd) - 1] = '\0';

    fclose(comm_fd);
}

void parse_opt(char opt[], const int * pid) {
    char cmdline_path[SIZE];
    FILE * cmdline_fd;
    char buf[SIZE] = {'\0'}, tmp[SIZE] = {'\0'};

    sprintf(cmdline_path, "/proc/%d/cmdline", *pid);

    cmdline_fd = fopen(cmdline_path, "r");

    fread(buf, sizeof(buf) - 1, 1, cmdline_fd);
    for (int i = 0; i < (sizeof(buf) - 1); i++) {
        if (!(buf[i] | buf[i + 1])) {
            break;
        }

        if (buf[i] == '\0') {
            buf[i] = ' ';
        }
    }
    strcpy(tmp, buf);
    sscanf(tmp, "%*[^ ]%*[ ]%[^\n]", opt);

    fclose(cmdline_fd);
}
