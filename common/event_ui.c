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

int cur_percent = 0;

bool is_swupdateui_finished(void)
{
    if (100 == cur_percent)
        return true;
    else
        return false;
}

void progress_handle(void *data)
{
    int ret = -1;

    lv_refresh_event_t *pdata = (lv_refresh_event_t *)data;
    ret = progress_ipc_receive(&pdata->fd, &pdata->msg);
    if (-1 == ret) {
        printf("Get the progress value failed!\n");
        pdata->fd = progress_ipc_connect(true);
        return ;
    }

    if ((cur_percent < 0) || (cur_percent > 100)) {
        printf("Get the error progress, current percent is %d \n", cur_percent);
        return ;
    }

    if (cur_percent == pdata->msg.cur_percent) {
        return;
    }

    cur_percent = pdata->msg.cur_percent;

    if (NULL != pdata->p_bar_refresh) {
        pdata->p_bar_refresh(pdata->screen, cur_percent);
    }

}

#ifdef __cplusplus
}
#endif
