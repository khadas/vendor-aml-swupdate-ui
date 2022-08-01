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

#include "event_ui.h"
#include "swupdate_ipc.h"

static bool update_is_finish = false;

bool get_finish_flag(void)
{
    return update_is_finish;
}

void set_finish_flag(bool flag)
{
    update_is_finish = flag;
}

void progress_handle(void *data)
{
    bool ret = false;
    static int cur_percent = 0;

    lv_refresh_event_t *pdata = (lv_refresh_event_t *)data;
    ret = sock_recv_prog(pdata->fd, (char *)&pdata->msg, sizeof(pdata->msg));
    if (false == ret)
        return ;

    if (cur_percent == pdata->msg.cur_percent) {
        return;
    }

    cur_percent = pdata->msg.cur_percent;

    if ((cur_percent < 0) || (cur_percent > 100)) {
        printf("Get the error progress, current percent is %d \n", cur_percent);
        return ;
    } else if (100 == cur_percent) {
       set_finish_flag(true);
    }

#if CONFIG_LVGL_APP==1
    if (NULL != pdata->p_bar_refresh) {
        pdata->p_bar_refresh(pdata->bar, cur_percent);
    }
#endif

}

#if 0
/* TODO XXX */
void event_handle(void *data)
{
    progress_handle(data);
}
#endif

#ifdef __cplusplus
}
#endif
