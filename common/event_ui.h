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

#ifndef __EVENT_H__
#define __EVENT_H__

#include <stdio.h>
#include <progress_ipc.h>
#if CONFIG_LVGL_APP==1
#include <lvgl/src/core/lv_obj.h>
#endif

typedef struct
{
    int fd;
    struct progress_msg msg;

#if CONFIG_LVGL_APP==1
    lv_obj_t *label;
    lv_obj_t img;
    /* bar */
    lv_obj_t *bar;
    lv_style_t style_bg;
    void (* p_bar_refresh)(lv_obj_t *, int32_t);
#endif

#if CONFIG_DIRECTFB_APP==1
#endif
    /* data */
}lv_refresh_event_t;

bool get_finish_flag(void);

void set_finish_flag(bool flag);

void progress_handle(void *data);
//void event_handle(void *data);

#endif

#ifdef __cplusplus
}
#endif
