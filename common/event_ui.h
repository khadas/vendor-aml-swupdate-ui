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

#ifndef __EVENT_UI_H__
#define __EVENT_UI_H__

#include <stdio.h>
#include <progress_ipc.h>
#include <swupdate_status.h>

typedef struct
{
    int fd;
    struct progress_msg msg;
    RECOVERY_STATUS ui_status;

    void *screen;
    void (*p_bar_refresh)(void *, int);
    void (*p_show_status)(void *, const char *);
    /* data */
}lv_refresh_event_t;


void progress_handle(void *data);

#endif

#ifdef __cplusplus
}
#endif
