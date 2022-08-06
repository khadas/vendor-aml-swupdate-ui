/*
 * Copyright (C) 2017 Amlogic Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSEERROR_CODE_INVALID_OPERATION.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  DESCRIPTION
 *      This file implements a adaptor of audio decoder from Amlogic.
 *
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdbool.h>

#define SWUPDATE_PROGRESS_PATH  "/tmp/swupdateprog"
struct sockaddr_un address;

int sock_conn(void)
{
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd <= 0) {
        perror("socket failed: ");
        return sock_fd;
    }
    /* init & connect socket of progress notify server */
    address.sun_family = AF_UNIX;
    memset(address.sun_path, 0, sizeof(address.sun_path));
    strncpy(address.sun_path, SWUPDATE_PROGRESS_PATH, sizeof(address.sun_path) - 1);
    if (-1 == connect(sock_fd, (struct sockaddr *)&address, sizeof(address)))
        perror("connect failed: ");
    return sock_fd;
}

bool sock_recv_prog(int fd, void *data, int len)
{
    static int ret = 0;
    if (fd > 0) {
        /* block receiving progress data */
        while (( ret = recv(fd, data, len, 0)) <= 0) {
            /* if recv() returns 0, it means SWupdate is disconnected, retry to connect */
            if (ret == 0) {
                if (-1 == connect(fd, (struct sockaddr *)&address, sizeof(address))) {
                    perror("connect failed: ");
                    //return false;
                }
            }
        }
    } else {
        printf("socket fd is invalid\n");
        return false;
    }
    return true;
}

#ifdef __cplusplus
}
#endif
